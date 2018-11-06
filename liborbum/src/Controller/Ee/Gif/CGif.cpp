#include <algorithm>

#include "Controller/Ee/Gif/CGif.hpp"

#include "Core.hpp"
#include "Resources/RResources.hpp"

CGif::CGif(Core* core) :
    CController(core)
{
}

void CGif::handle_event(const ControllerEvent& event)
{
    switch (event.type)
    {
    case ControllerEvent::Type::Time:
    {
        int ticks_remaining = time_to_ticks(event.data.time_us);
        while (ticks_remaining > 0)
            ticks_remaining -= time_step(ticks_remaining);
        break;
    }
    default:
    {
        throw std::runtime_error("CGif event handler not implemented - please fix!");
    }
    }
}

int CGif::time_to_ticks(const double time_us)
{
    int ticks = static_cast<int>(time_us / 1.0e6 * Constants::EE::EEBUS_CLK_SPEED * core->get_options().system_bias_gif);

    if (ticks < 10)
    {
        static bool warned = false;
        if (!warned)
        {
            BOOST_LOG(Core::get_logger()) << "Gif ticks too low - increase time delta";
            warned = true;
        }
    }

    return ticks;
}

int CGif::time_step(const int ticks_available)
{
    auto& r = core->get_resources();
    auto& ctrl = r.ee.gif.ctrl;
    auto& mode = r.ee.gif.mode;
    auto& stat = r.ee.gif.stat;
    auto& count = r.ee.gif.cnt;

    auto& fifo_gif_path1 = r.fifo_gif_path1;
    auto& fifo_gif_path2 = r.fifo_gif_path2;
    auto& fifo_gif_path3 = r.fifo_gif_path3;

    int cycles_consumed = 0;

    // Prioritise paths by 1 -> 2 -> 3.
    // See EE Users Manual page 149. 
    DmaFifoQueue<>*const paths[3] = {&fifo_gif_path1, &fifo_gif_path2, &fifo_gif_path3};

    // Copies bit 0 and 2 of MODE to STAT
    stat.write_uword(stat.read_uword() | (mode.read_uword() & 0b101));

    // Obtain the active path. If the GIF is idling (APATH == 0), then we start at
    // PATH1 (which is paths[0] because zero-indexing).
    int active_path = stat.extract_field(GifRegister_Stat::APATH);
    active_path -= (active_path != 0);

    // If PATH3 is masked and not the active path, no arbitration is performed for it
    const bool masked_path3 = stat.extract_field(GifRegister_Stat::M3P) || stat.extract_field(GifRegister_Stat::M3R);
    const int max_path = 2 + (active_path == 2 || !masked_path3);

    // Stop transfer if PSE is set to 1.
    stat.insert_field(GifRegister_Stat::PSE, ctrl.extract_field(GifRegister_Ctrl::PSE));
    if (ctrl.extract_field(GifRegister_Ctrl::PSE)) return 1;

    for (int path_index = active_path; path_index < max_path; path_index++)
    {
        auto& fifo = paths[path_index];

        // A GIFtag is always read before processing data.
        if (!count.extract_field(GifRegister_Cnt::LOOPCNT))
        {
            if (!fifo->has_read_available(NUMBER_BYTES_IN_QWORD))
                continue;

            // P3TAG holding something means that PATH3 was previously interrupted, and needs to recontinue
            if (r.ee.gif.p3tag.read_uword())
            {
                // Note: check endianness?
                uqword p3data;
                p3data.uw[0] = r.ee.gif.p3tag.read_uword();

                cycles_consumed = handle_tag(Giftag(p3data));

                // Clear P3TAG after it is written to TAG
                r.ee.gif.p3tag.write_uword(0);
                stat.insert_field(GifRegister_Stat::IP3, 0);
            }

            uqword data;
            fifo->read(reinterpret_cast<ubyte*>(&data), NUMBER_BYTES_IN_QWORD);

            stat.insert_field(GifRegister_Stat::APATH, path_index + 1);

            cycles_consumed = handle_tag(Giftag(data));

            // Set OPH to 0 when idle
            stat.insert_field(GifRegister_Stat::OPH, 0);
        }
        else
        {
            // Set OPH to 1 when transferring starts
            stat.insert_field(GifRegister_Stat::OPH, 1);

            switch (ctrl.tag.flg())
            {
            case Giftag::DataFormat::Packed:
            {
                if (!fifo->has_read_available(NUMBER_BYTES_IN_QWORD))
                    continue;

                uqword data;
                fifo->read(reinterpret_cast<ubyte*>(&data), NUMBER_BYTES_IN_QWORD);

                cycles_consumed = handle_data_packed(data);

                break;
            }
            case Giftag::DataFormat::Reglist:
            {
                // TODO: maybe a weird situation can occur where there is only 
                // 1 dword available which should be processed first before 
                // attempting processing of other paths... Probably should make
                // this proper at some stage. Manual doesn't really say what to do...
                if (!fifo->has_read_available(NUMBER_BYTES_IN_QWORD))
                    continue;

                uqword data;
                fifo->read(reinterpret_cast<ubyte*>(&data), NUMBER_BYTES_IN_QWORD);

                cycles_consumed = handle_data_reglist(data);

                break;
            }
            case Giftag::DataFormat::Image:
            case Giftag::DataFormat::Disabled:
            {
                if (!fifo->has_read_available(NUMBER_BYTES_IN_QWORD))
                    continue;

                uqword data;
                fifo->read(reinterpret_cast<ubyte*>(&data), NUMBER_BYTES_IN_QWORD);

                // In intermittent mode, the GIF checks for other requests from PATH1 & PATH2,
                // and priotises them over PATH3 if there are.
                // The check is done every 8 qwords (or 1 slice).
                if (stat.extract_field(GifRegister_Stat::APATH) == 3 && mode.extract_field(GifRegister_Mode::IMT))
                {
                    // The first 32 bits of PATH3 tag is written to P3TAG register in IMAGE mode
                    r.ee.gif.p3tag.write_uword(ctrl.tag.tag.uw[0]);

                    if (r.ee.gif.path3_qword_count < 8)
                    {
                        r.ee.gif.path3_qword_count += 1;
                    }
                    else
                    {
                        // If PATH3 is interuptted and there are other requests pending, 
                        // save its LOOPCNT, and reset APATH & LOOPCNT to 0.
                        if (stat.extract_field(GifRegister_Stat::P1Q) || stat.extract_field(GifRegister_Stat::P2Q))
                        {
                            stat.insert_field(GifRegister_Stat::IP3, 1);
                            r.ee.gif.p3cnt.insert_field(GifRegister_P3cnt::P3CNT, count.extract_field(GifRegister_Cnt::LOOPCNT));
                            count.insert_field(GifRegister_Cnt::LOOPCNT, 0);
                            stat.insert_field(GifRegister_Stat::APATH, 0);
                        }

                        r.ee.gif.path3_qword_count = 0;
                    }
                }

                cycles_consumed = handle_data_image(data);

                break;
            }
            default:
            {
                throw std::runtime_error("Unknown GIF data processing method");
            }
            }
        }

        // TODO: Confirm behaviour.
        if (ctrl.tag.eop())
        {
            if (count.extract_field(GifRegister_Cnt::LOOPCNT))
            {
                BOOST_LOG(Core::get_logger()) << "GIF: EOP reached, but LOOPCNT > 0";   
            }
            else
            {
                BOOST_LOG(Core::get_logger()) << "GIF: EOP reached, and LOOPCNT == 0; stopping transfer";
                stat.insert_field(GifRegister_Stat::APATH, 0);
                break;
            }
        }

        // TODO: remove this hack.
        // Hack: if there are active transfers, force FQC to 16, otherwise 0.
        // This is because we do not have a reliable way of tracking the FIFO usage
        if (fifo->has_read_available(NUMBER_BYTES_IN_QWORD))
        {
            BOOST_LOG(Core::get_logger()) << "GIF: FQC set to 16";
            stat.insert_field(GifRegister_Stat::FQC, 16);
        }
        else
        {
            stat.insert_field(GifRegister_Stat::FQC, 0);
        }

        // Do not process other paths if at least one path was successfully processed.
        if (cycles_consumed)
            break;
    }

    // If there are requests in queue for the PATHs, set the P*Q registers to 1.
    stat.insert_field(GifRegister_Stat::P1Q, r.fifo_gif_path1.has_read_available(NUMBER_BYTES_IN_QWORD));
    stat.insert_field(GifRegister_Stat::P2Q, r.fifo_gif_path2.has_read_available(NUMBER_BYTES_IN_QWORD));
    stat.insert_field(GifRegister_Stat::P3Q, r.fifo_gif_path3.has_read_available(NUMBER_BYTES_IN_QWORD));

    // At least 1 cycle is consumed always if no paths had data available for processing.
    return std::max(cycles_consumed, 1);
}

int CGif::handle_tag(const Giftag tag)
{
    auto& r = core->get_resources();
    auto& ctrl = r.ee.gif.ctrl;
    auto& count = r.ee.gif.cnt;

    ctrl.tag = tag;

    // The tag is split into 4 parts and copied to the respective registers
    r.ee.gif.tag0.write_uword(tag.tag.uw[0]);
    r.ee.gif.tag1.write_uword(tag.tag.uw[1]);
    r.ee.gif.tag2.write_uword(tag.tag.uw[2]);
    r.ee.gif.tag3.write_uword(tag.tag.uw[3]);

    // Initialise the RGBAQ.Q value on every tag read.
    // See EE Users manual page 153.
    ctrl.rgbaq_q = 1.0f;

    // Some special processing when the NLOOP field is non-zero 
    // "Values other than the EOP field are disregarded."
    // See EE Users Manual page 151.
    if (tag.nloop())
    {
        if (tag.flg() == Giftag::DataFormat::Packed)
        {
            // Output the packed PRIM data to GS if the PRE bit is set.
            if (tag.pre())
            {
                //throw std::runtime_error("Sending GIFtag packed PRIM data not implemented yet");
                //uhword prim_value = tag.prim();
                //auto& prim = r.gs.prim;
                //prim.write_udword(prim_value);
            }
        }
    }

    count.insert_field(GifRegister_Cnt::LOOPCNT, static_cast<uword>(tag.nloop()));
    count.insert_field(GifRegister_Cnt::REGCNT, static_cast<uword>(tag.nreg()));

    // 1 cycle consumed for the tag, and 1 cycle for the handling of the PRIM data
    // (always done even if it's not used). See the descriptions of the transfer
    // modes in the EE Users Manual page 153, 159, 160.
    return 2;
}

int CGif::handle_data_packed(const uqword data)
{
    auto& r = core->get_resources();
    auto& ctrl = r.ee.gif.ctrl;
    auto& stat = r.ee.gif.stat;
    auto& count = r.ee.gif.cnt;

    // Get the register descriptor.
    ubyte loop_count = count.extract_field(GifRegister_Cnt::LOOPCNT);
    ubyte reg_count = (count.extract_field(GifRegister_Cnt::REGCNT) - 1) & 0xF;
    size_t register_descriptor = ctrl.tag.regs(reg_count);

    // Process the data given, and write to the GS register.
    // See EE Users Manual page 152 onwards for processing details.
    switch (register_descriptor)
    {
    case 0x0:
    {
        // PRIM
        constexpr Bitfield PRIM = Bitfield(0, 11);

        udword prim_value = PRIM.extract_from<udword>(data); 

        //auto& prim = r.gs.prim;
        //prim.write_udword(prim_value);
        break;
    }
    case 0x1:
    {
        // RGBAQ
        constexpr Bitfield R = Bitfield(0, 8);
        constexpr Bitfield G = Bitfield(32, 8);
        constexpr Bitfield B = Bitfield(64, 8);
        constexpr Bitfield A = Bitfield(96, 8);

        udword r_value = R.extract_from<udword>(data); 
        udword g_value = G.extract_from<udword>(data); 
        udword b_value = B.extract_from<udword>(data); 
        udword a_value = A.extract_from<udword>(data);
        udword q_value = *reinterpret_cast<udword*>(&ctrl.rgbaq_q);

        //auto& rgbaq = r.gs.rgbaq;
        //rgbaq.write_field(GsRegister_Rgbaq::R, r_value);
        //rgbaq.write_field(GsRegister_Rgbaq::G, g_value);
        //rgbaq.write_field(GsRegister_Rgbaq::B, b_value);
        //rgbaq.write_field(GsRegister_Rgbaq::A, a_value);
        //rgbaq.write_field(GsRegister_Rgbaq::Q, q_value);
        break;
    }
    case 0x2:
    {
        // ST
        constexpr Bitfield S = Bitfield(0, 32);
        constexpr Bitfield T = Bitfield(32, 32);
        constexpr Bitfield Q = Bitfield(64, 32);

        // Store the Q value internally, used when handling RGBAQ register descriptor (see above).
        uword q_value = Q.extract_from<uword>(data);
        ctrl.rgbaq_q = *reinterpret_cast<f32*>(&q_value);

        udword s_value = S.extract_from<udword>(data);
        udword t_value = T.extract_from<udword>(data);

        //auto& st = r.gs.st;
        //st.write_field(GsRegister_St::S, s_value);
        //st.write_field(GsRegister_St::T, t_value);
        break;
    }
    case 0x3:
    {
        // UV
        constexpr Bitfield U = Bitfield(0, 14);
        constexpr Bitfield V = Bitfield(32, 14);

        udword u_value = U.extract_from<udword>(data);
        udword v_value = V.extract_from<udword>(data);

        //auto& uv = r.gs.uv;
        //uv.write_field(GsRegister_Uv::U, u_value);
        //uv.write_field(GsRegister_Uv::V, v_value);
        break;
    }
    case 0x4:
    {
        // XYZF2/3
        constexpr Bitfield X = Bitfield(0, 16);
        constexpr Bitfield Y = Bitfield(32, 16);
        constexpr Bitfield Z = Bitfield(68, 24);
        constexpr Bitfield F = Bitfield(100, 8);
        constexpr Bitfield ADC = Bitfield(111, 1);

        udword x_value = X.extract_from<udword>(data);
        udword y_value = Y.extract_from<udword>(data);
        udword z_value = Z.extract_from<udword>(data);
        udword f_value = F.extract_from<udword>(data);
        udword adc_value = ADC.extract_from<udword>(data);

        //GsRegister_Xyzf * xyzf = adc_value ? &r.gs.xyzf2 : &r.gs.xyzf3;
        //xyzf->write_field(GsRegister_Xyzf::X, x_value);
        //xyzf->write_field(GsRegister_Xyzf::Y, y_value);
        //xyzf->write_field(GsRegister_Xyzf::Z, z_value);
        //xyzf->write_field(GsRegister_Xyzf::F, f_value);
        break;
    }
    case 0x5:
    {
        // XYZ2/3
        constexpr Bitfield X = Bitfield(0, 16);
        constexpr Bitfield Y = Bitfield(32, 16);
        constexpr Bitfield Z = Bitfield(64, 32);
        constexpr Bitfield ADC = Bitfield(111, 1);

        udword x_value = X.extract_from<udword>(data);
        udword y_value = Y.extract_from<udword>(data);
        udword z_value = Z.extract_from<udword>(data);
        udword adc_value = ADC.extract_from<udword>(data);

        //GsRegister_Xyzf * xyz = adc_value ? &r.gs.xyz2 : &r.gs.xyz3;
        //xyz->write_field(GsRegister_Xyzf::X, x_value);
        //xyz->write_field(GsRegister_Xyzf::Y, y_value);
        //xyz->write_field(GsRegister_Xyzf::Z, z_value);
        break;
    }
    case 0x6:
    {
        // TEX0_1
        //auto& tex0_1 = r.gs.tex0_1;
        //tex0_1.write_udword(data.lo);
        break;
    }
    case 0x7:
    {
        // TEX0_2
        //auto& tex0_2 = r.gs.tex0_2;
        //tex0_2.write_udword(data.lo);
        break;
    }
    case 0x8:
    {
        // CLAMP_1
        //auto& clamp_1 = r.gs.clamp_1;
        //clamp_1.write_udword(data.lo);
        break;
    }
    case 0x9:
    {
        // CLAMP_2
        //auto& clamp_2 = r.gs.clamp_2;
        //clamp_2.write_udword(data.lo);
        break;
    }
    case 0xA:
    {
        // FOG
        // TODO: Not sure if we need to extract the integral part? Wording a bit unclear.
        // See EE Users Manual page 155.
        constexpr Bitfield F = Bitfield(100, 8);

        udword f_value = F.extract_from<udword>(data);

        //auto& fog = r.gs.fog;
        //fog.write_field(GsRegister_Fog::F, f_value);
        break;
    }
    case 0xB:
    {
        // Reserved
        throw std::runtime_error("GIF: Packed - Reserved register descriptor read from GIFtag");
    }
    case 0xC:
    {
        // XYZF3
        //auto& xyzf3 = r.gs.xyzf3;
        //xyzf3.write_udword(data.lo);
        break;
    }
    case 0xD:
    {
        // XYZ3
        //auto& xyz3 = r.gs.xyz3;
        //xyz3.write_udword(data.lo);
        break;
    }
    case 0xE:
    {
        // A+D (packed address + data)
        constexpr Bitfield ADDR = Bitfield(64, 8);

        uptr addr_value = ADDR.extract_from<uptr>(data);
        udword data_value = data.lo;

        //r.gs.bus.write_udword(BusContext::Gif, addr_value, data_value);
        break;
    }
    case 0xF:
    {
        // NOP    
        break;
    }
    default:
    {
        throw std::runtime_error("GIF: Packed - Unknown register descriptor given");
    }
    }
    
    // +1 for incrementing, another +1 when inserting for restoring the index
    reg_count += 1;
    count.insert_field(GifRegister_Cnt::REGCNT, reg_count + 1);
    
    if (reg_count == ctrl.tag.nreg())
    {
        reg_count = 0;
        count.insert_field(GifRegister_Cnt::REGCNT, 1);
        count.insert_field(GifRegister_Cnt::LOOPCNT, --loop_count);
    }

    return 1;
}

int CGif::handle_data_reglist(const uqword data)
{
    auto& r = core->get_resources();
    auto& ctrl = r.ee.gif.ctrl;
    auto& stat = r.ee.gif.stat;
    auto& count = r.ee.gif.cnt;

    const udword datas[] = { data.lo, data.hi };
    for (auto& data : datas)
    {
        // Get the register descriptor.
        ubyte loop_count = count.extract_field(GifRegister_Cnt::LOOPCNT);
        ubyte reg_count = (count.extract_field(GifRegister_Cnt::REGCNT) - 1) & 0xF;
        size_t register_descriptor = ctrl.tag.regs(reg_count);

        // Write data directly to the GS register.
        // See EE Users Manual page 159 onwards for processing details.
        switch (register_descriptor)
        {
        case 0x0:
        {
            // PRIM
            //auto& prim = r.gs.prim;
            //prim.write_udword(data);
            break;
        }
        case 0x1:
        {
            // RGBAQ
            //auto& rgbaq = r.gs.rgbaq;
            //rgbaq.write_udword(data);
            break;
        }
        case 0x2:
        {
            // ST
            //auto& st = r.gs.st;
            //st.write_udword(data);
            break;
        }
        case 0x3:
        {
            // UV
            //auto& uv = r.gs.uv;
            //uv.write_udword(data);
            break;
        }
        case 0x4:
        {
            // XYZF2
            //auto& xyzf2 = r.gs.xyzf2;
            //xyzf2.write_udword(data);
            break;
        }
        case 0x5:
        {
            // XYZ2
            //auto& xyz2 = r.gs.xyz2;
            //xyz2.write_udword(data);
            break;
        }
        case 0x6:
        {
            // TEX0_1
            //auto& tex0_1 = r.gs.tex0_1;
            //tex0_1.write_udword(data);
            break;
        }
        case 0x7:
        {
            // TEX0_2
            //auto& tex0_2 = r.gs.tex0_2;
            //tex0_2.write_udword(data);
            break;
        }
        case 0x8:
        {
            // CLAMP_1
            //auto& clamp_1 = r.gs.clamp_1;
            //clamp_1.write_udword(data);
            break;
        }
        case 0x9:
        {
            // CLAMP_2
            //auto& clamp_2 = r.gs.clamp_2;
            //clamp_2.write_udword(data);
            break;
        }
        case 0xA:
        {
            // FOG
            //auto& fog = r.gs.fog;
            //fog.write_udword(data);
            break;
        }
        case 0xB:
        {
            // Reserved
            throw std::runtime_error("GIF: Reglist - Reserved register descriptor read from GIFtag");
        }
        case 0xC:
        {
            // XYZF3
            //auto& xyzf3 = r.gs.xyzf3;
            //xyzf3.write_udword(data);
            break;
        }
        case 0xD:
        {
            // XYZ3
            //auto& xyz3 = r.gs.xyz3;
            //xyz3.write_udword(data);
            break;
        }
        case 0xE:
        {
            // A+D (packed address + data)
            // No-op for REGLIST mode. See EE Users Manual page 159.
            break;
        }
        case 0xF:
        {
            // NOP    
            break;
        }
        default:
        {
            throw std::runtime_error("GIF: Reglist - Unknown register descriptor given");
        }
        }

        // +1 for incrementing, another +1 when inserting for restoring the index
        reg_count += 1;
        count.insert_field(GifRegister_Cnt::REGCNT, reg_count + 1);

        if (reg_count == ctrl.tag.nreg())
        {
            count.insert_field(GifRegister_Cnt::REGCNT, 1);
            count.insert_field(GifRegister_Cnt::LOOPCNT, --loop_count);
        }

        if (loop_count == 0)
        {
            break;
        }
    }
    
    // Although not stated, I'm gussing the GIF consumes an extra cycle even when
    // NREGS is odd (ie: upper 64-bits of last qword of data discarded).
    return 2;
}

int CGif::handle_data_image(const uqword data)
{
    auto& r = core->get_resources();
    auto& ctrl = r.ee.gif.ctrl;
    auto& stat = r.ee.gif.stat;
    auto& count = r.ee.gif.cnt;

    ubyte loop_count = count.extract_field(GifRegister_Cnt::LOOPCNT);

    //auto& hwreg = r.gs.hwreg;

    udword datas[] = { data.lo, data.hi };
    for (auto& data : datas)
    {
        // Data is output sequentially to the HWREG transfer register of the GS.
        // See EE Users manual page 160.
        //hwreg.write_udword(data);
    }

    count.insert_field(GifRegister_Cnt::LOOPCNT, --loop_count);

    return 1;
}
