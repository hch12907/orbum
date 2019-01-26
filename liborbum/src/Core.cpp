#include <chrono>
#include <stdexcept>
#include <thread>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

#include <Console.hpp>
#include <Datetime.hpp>
#include <Macros.hpp>

#include "Core.hpp"

#include "Controller/Cdvd/CCdvd.hpp"
#include "Controller/Ee/Core/Interpreter/CEeCoreInterpreter.hpp"
#include "Controller/Ee/Dmac/CEeDmac.hpp"
#include "Controller/Ee/Gif/CGif.hpp"
#include "Controller/Ee/Intc/CEeIntc.hpp"
#include "Controller/Ee/Ipu/CIpu.hpp"
#include "Controller/Ee/Timers/CEeTimers.hpp"
#include "Controller/Ee/Vpu/CVpu.hpp"
#include "Controller/Ee/Vpu/Vu/Interpreter/CVuInterpreter.hpp"
#include "Controller/Gs/Core/CGsCore.hpp"
#include "Controller/Gs/Crtc/CCrtc.hpp"
#include "Controller/Iop/Core/Interpreter/CIopCoreInterpreter.hpp"
#include "Controller/Iop/Dmac/CIopDmac.hpp"
#include "Controller/Iop/Intc/CIopIntc.hpp"
#include "Controller/Iop/Sio0/CSio0.hpp"
#include "Controller/Iop/Sio2/CSio2.hpp"
#include "Controller/Iop/Timers/CIopTimers.hpp"
#include "Controller/Spu2/CSpu2.hpp"
#include "Resources/RResources.hpp"

boost::log::sources::logger_mt Core::logger;

CoreOptions CoreOptions::make_default()
{
    return CoreOptions{
        "./logs/",
        "./bios/",
        "./dumps/",
        "./saves/",
        "scph10000.bin",
        "",
        "",
        "",
        5,
        4, //std::thread::hardware_concurrency() - 1,

        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0};
}

CoreApi::CoreApi(const CoreOptions& options)
{
    impl = new Core(options);
}

CoreApi::~CoreApi()
{
    delete impl;
}

void CoreApi::run()
{
    impl->run();
}

void CoreApi::dump_all_memory() const
{
    impl->dump_all_memory();
}

void CoreApi::save_state()
{
    impl->save_state();
}

Core::Core(const CoreOptions& options) :
    options(options)
{
    // Initialise logging.
    init_logging();

    BOOST_LOG(get_logger()) << "Core initialising... please wait";

    // Initialise resources.
    resources = std::make_unique<RResources>();
    initialise_resources(resources);

    // Initialise roms (boot_rom (required), rom1, rom2, erom).
    const std::string roms_dir_path = options.roms_dir_path;
    const std::string boot_rom_file_name = options.boot_rom_file_name;
    const std::string rom1_file_name = options.rom1_file_name;
    const std::string rom2_file_name = options.rom2_file_name;
    const std::string erom_file_name = options.erom_file_name;

    get_resources().boot_rom.read_from_file(roms_dir_path + boot_rom_file_name, Constants::EE::ROM::SIZE_BOOT_ROM);
    if (!rom1_file_name.empty())
        get_resources().rom1.read_from_file(roms_dir_path + rom1_file_name, Constants::EE::ROM::SIZE_ROM1);
    if (!rom2_file_name.empty())
        get_resources().rom2.read_from_file(roms_dir_path + rom2_file_name, Constants::EE::ROM::SIZE_ROM2);
    if (!erom_file_name.empty())
        get_resources().erom.read_from_file(roms_dir_path + erom_file_name, Constants::EE::ROM::SIZE_EROM);

    // Initialise controllers.
    controllers[ControllerType::Type::EeCore] = std::make_unique<CEeCoreInterpreter>(this);
    controllers[ControllerType::Type::EeDmac] = std::make_unique<CEeDmac>(this);
    controllers[ControllerType::Type::EeTimers] = std::make_unique<CEeTimers>(this);
    controllers[ControllerType::Type::EeIntc] = std::make_unique<CEeIntc>(this);
    controllers[ControllerType::Type::Gif] = std::make_unique<CGif>(this);
    controllers[ControllerType::Type::Ipu] = std::make_unique<CIpu>(this);
    controllers[ControllerType::Type::Vif] = std::make_unique<CVpu<CVuInterpreter>>(this, 0);
    controllers[ControllerType::Type::Vu] = std::make_unique<CVpu<CVuInterpreter>>(this, 1);
    controllers[ControllerType::Type::IopCore] = std::make_unique<CIopCoreInterpreter>(this);
    controllers[ControllerType::Type::IopDmac] = std::make_unique<CIopDmac>(this);
    controllers[ControllerType::Type::IopTimers] = std::make_unique<CIopTimers>(this);
    controllers[ControllerType::Type::IopIntc] = std::make_unique<CIopIntc>(this);
    controllers[ControllerType::Type::Cdvd] = std::make_unique<CCdvd>(this);
    controllers[ControllerType::Type::Spu2] = std::make_unique<CSpu2>(this);
    controllers[ControllerType::Type::GsCore] = std::make_unique<CGsCore>(this);
    controllers[ControllerType::Type::Crtc] = std::make_unique<CCrtc>(this);
    controllers[ControllerType::Type::Sio0] = std::make_unique<CSio0>(this);
    controllers[ControllerType::Type::Sio2] = std::make_unique<CSio2>(this);

    // Task executor.
    task_executor = std::make_unique<TaskExecutor>(options.number_workers);

    BOOST_LOG(get_logger()) << "Core initialised";
}

Core::~Core()
{
    BOOST_LOG(get_logger()) << "Core shutting down";
}

boost::log::sources::logger_mt& Core::get_logger()
{
    return logger;
}

void Core::run()
{
    try
    {
#if defined(BUILD_DEBUG)
        static double DEBUG_TIME_ELAPSED = 0.0;
        static double DEBUG_TIME_LOGGED = 0.0;
        static std::chrono::high_resolution_clock::time_point DEBUG_T1 = std::chrono::high_resolution_clock::now();
        if ((DEBUG_TIME_ELAPSED - DEBUG_TIME_LOGGED) > 0.01e6)
        {
            const std::chrono::high_resolution_clock::time_point DEBUG_T2 = std::chrono::high_resolution_clock::now();
            const std::chrono::duration<double, std::micro> duration = DEBUG_T2 - DEBUG_T1;

            const std::string info = str(boost::format("Emulation time elapsed: %.3f (%.4fx)")
                                         % (DEBUG_TIME_ELAPSED / 1e6)
                                         % ((DEBUG_TIME_ELAPSED - DEBUG_TIME_LOGGED) / duration.count()));

            BOOST_LOG(get_logger()) << info;
            //print_title(info);

            DEBUG_TIME_LOGGED = DEBUG_TIME_ELAPSED;
            DEBUG_T1 = DEBUG_T2;
        }
        DEBUG_TIME_ELAPSED += options.time_slice_per_run_us;
#endif

        // Enqueue time events (always done on each run).
        auto event = ControllerEvent{ControllerEvent::Type::Time, options.time_slice_per_run_us};
        for (int i = 0; i < static_cast<int>(ControllerType::Type::COUNT); i++) // TODO: find better syntax..
        {
            auto controller = static_cast<ControllerType::Type>(i);
            enqueue_controller_event(controller, event);
        }

        // Package events into tasks and send to workers.
        EventEntry entry;
        while (controller_event_queue.try_pop(entry))
        {
            auto task = [this, entry]() {
                if (controllers[entry.t])
                    controllers[entry.t]->handle_event_marshall_(entry.e);
            };

            task_executor->enqueue_task(task);
        }

        // Dispatch all tasks and wait for resynchronisation.
        task_executor->dispatch();
        task_executor->wait_for_idle();

#if defined(BUILD_DEBUG)
        if (!task_executor->task_sync.running_task_queue.is_empty() || task_executor->task_sync.thread_busy_counter.busy_counter)
            throw std::runtime_error("Task queue was not empty!");
#endif
    }
    catch (const std::runtime_error& e)
    {
        BOOST_LOG(get_logger()) << "Core running fatal error: " << e.what();
        throw;
    }
}

void Core::dump_all_memory() const
{
    const std::string dumps_dir_path = options.dumps_dir_path;
    boost::filesystem::create_directory(dumps_dir_path);
    get_resources().ee.main_memory.write_to_file(dumps_dir_path + "dump_ee_" + datetime_fmt(Core::DATETIME_FORMAT) + ".bin");
    get_resources().iop.main_memory.write_to_file(dumps_dir_path + "dump_iop_" + datetime_fmt(Core::DATETIME_FORMAT) + ".bin");
    get_resources().spu2.main_memory.write_to_file(dumps_dir_path + "dump_spu2_" + datetime_fmt(Core::DATETIME_FORMAT) + ".bin");
    get_resources().cdvd.nvram.memory.write_to_file(dumps_dir_path + "dump_cdvd_nvram_" + datetime_fmt(Core::DATETIME_FORMAT) + ".bin");
}

void Core::init_logging()
{
    const std::string logs_dir_path = options.logs_dir_path;
    boost::filesystem::create_directory(logs_dir_path);
    boost::log::add_common_attributes();

    boost::log::add_file_log(
        boost::log::keywords::file_name = logs_dir_path + "log_" + datetime_fmt(Core::DATETIME_FORMAT) + ".log",
        boost::log::keywords::format = "[%TimeStamp%]: %Message%");
    boost::log::add_console_log(
        std::cout,
        boost::log::keywords::format = "[%TimeStamp%]: %Message%");
}

void Core::save_state()
{
    const std::string save_states_dir_path = options.save_states_dir_path;
    boost::filesystem::create_directory(save_states_dir_path);

    std::ofstream fout(save_states_dir_path + "save_" + datetime_fmt(Core::DATETIME_FORMAT) + ".json", std::ios_base::out);
    if (!fout)
        throw std::runtime_error("Unable to write file");

    cereal::JSONOutputArchive oarchive(fout);
    oarchive(get_resources());
}
