#pragma once

#include "Common/Constants.hpp"
#include "Controller/CController.hpp"
#include "Resources/RResources.hpp"
#include "Resources/Ee/Gif/GifRegisters.hpp"
#include "Resources/Ee/Vpu/Vif/VifUnits.hpp"
#include "Resources/Ee/Vpu/Vif/VifcodeInstruction.hpp"

class Core;

/// An implementation of the VIF channel system.
/// The VIF is responsible for processing and transfering data to the VU units, or optionally the GIF in the case of VIF1.
/// For each cycle @ BUSCLK, the VIF operates on 4 x 32-bits at a time, totaling the size of 1 DMA transfer unit of 128-bits.
class CVif : public CController
{
public:
    CVif(Core* core);

    void handle_event(const ControllerEvent& event) override;

    /// Converts a time duration into the number of ticks that would have occurred.
    int time_to_ticks(const double time_us);

    /// Steps through the VIF core state:
    /// - Check the FIFO queue and process data if available.
    int time_step(const int ticks_available);

    /// VIFcode handler functions.
    /// See EE Users Manual page 87 onwards.
    void INSTRUCTION_UNSUPPORTED(VifUnit_Base* unit, const VifcodeInstruction inst);
    void NOP(VifUnit_Base* unit, const VifcodeInstruction inst);
    void STCYCL(VifUnit_Base* unit, const VifcodeInstruction inst);
    void OFFSET(VifUnit_Base* unit, const VifcodeInstruction inst);
    void BASE(VifUnit_Base* unit, const VifcodeInstruction inst);
    void ITOP(VifUnit_Base* unit, const VifcodeInstruction inst);
    void STMOD(VifUnit_Base* unit, const VifcodeInstruction inst);
    void MSKPATH3(VifUnit_Base* unit, const VifcodeInstruction inst);
    void MARK(VifUnit_Base* unit, const VifcodeInstruction inst);
    void FLUSHE(VifUnit_Base* unit, const VifcodeInstruction inst);
    void FLUSH(VifUnit_Base* unit, const VifcodeInstruction inst);
    void FLUSHA(VifUnit_Base* unit, const VifcodeInstruction inst);
    void MSCAL(VifUnit_Base* unit, const VifcodeInstruction inst);
    void MSCNT(VifUnit_Base* unit, const VifcodeInstruction inst);
    void MSCALF(VifUnit_Base* unit, const VifcodeInstruction inst);
    void STMASK(VifUnit_Base* unit, const VifcodeInstruction inst);
    void STROW(VifUnit_Base* unit, const VifcodeInstruction inst);
    void STCOL(VifUnit_Base* unit, const VifcodeInstruction inst);
    void MPG(VifUnit_Base* unit, const VifcodeInstruction inst);
    void DIRECT(VifUnit_Base* unit, const VifcodeInstruction inst);
    void DIRECTHL(VifUnit_Base* unit, const VifcodeInstruction inst);
    void UNPACK_S_32(VifUnit_Base* unit, const VifcodeInstruction inst);
    void UNPACK_S_16(VifUnit_Base* unit, const VifcodeInstruction inst);
    void UNPACK_S_8(VifUnit_Base* unit, const VifcodeInstruction inst);
    void UNPACK_V2_32(VifUnit_Base* unit, const VifcodeInstruction inst);
    void UNPACK_V2_16(VifUnit_Base* unit, const VifcodeInstruction inst);
    void UNPACK_V2_8(VifUnit_Base* unit, const VifcodeInstruction inst);
    void UNPACK_V3_32(VifUnit_Base* unit, const VifcodeInstruction inst);
    void UNPACK_V3_16(VifUnit_Base* unit, const VifcodeInstruction inst);
    void UNPACK_V3_8(VifUnit_Base* unit, const VifcodeInstruction inst);
    void UNPACK_V4_32(VifUnit_Base* unit, const VifcodeInstruction inst);
    void UNPACK_V4_16(VifUnit_Base* unit, const VifcodeInstruction inst);
    void UNPACK_V4_8(VifUnit_Base* unit, const VifcodeInstruction inst);
    void UNPACK_V4_5(VifUnit_Base* unit, const VifcodeInstruction inst);

    /// Static arrays needed to call the appropriate VIFcode handler function.
    /// In total there are 34 unique instructions, based on the VIFcodeInstructionTable unique index.
    void (CVif::*INSTRUCTION_TABLE[Constants::EE::VPU::VIF::NUMBER_INSTRUCTIONS])(VifUnit_Base* unit, const VifcodeInstruction inst) =
        {
            &CVif::INSTRUCTION_UNSUPPORTED,
            &CVif::NOP,
            &CVif::STCYCL,
            &CVif::OFFSET,
            &CVif::BASE,
            &CVif::ITOP,
            &CVif::STMOD,
            &CVif::MSKPATH3,
            &CVif::MARK,
            &CVif::FLUSHE,
            &CVif::FLUSH,
            &CVif::FLUSHA,
            &CVif::MSCAL,
            &CVif::MSCALF,
            &CVif::MSCNT,
            &CVif::STMASK,
            &CVif::STROW,
            &CVif::STCOL,
            &CVif::MPG,
            &CVif::DIRECT,
            &CVif::DIRECTHL,
            &CVif::UNPACK_S_32,
            &CVif::UNPACK_S_16,
            &CVif::UNPACK_S_8,
            &CVif::UNPACK_V2_32,
            &CVif::UNPACK_V2_16,
            &CVif::UNPACK_V2_8,
            &CVif::UNPACK_V3_32,
            &CVif::UNPACK_V3_16,
            &CVif::UNPACK_V3_8,
            &CVif::UNPACK_V4_32,
            &CVif::UNPACK_V4_16,
            &CVif::UNPACK_V4_8,
            &CVif::UNPACK_V4_5
        };

private:
    /// Obtains the amount of words (a packet holds 4 subpackets, each subpacket is a
    /// word long) required for the instruction. 
    int obtain_required_words(const VifUnit_Base& unit, const VifcodeInstruction inst) const;
};
