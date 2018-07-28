#include <csignal>
#include <iostream>

#include <boost/filesystem.hpp>

#include <Core.hpp>
#include <Macros.hpp>

volatile bool DEBUG_RUN = true;

void signal_handler(int signal)
{
    DEBUG_RUN = false;
}

int main(int argc, char* argv[])
{
    std::cout << "Command line: ";
    for (int i = 0; i < argc; i++)
        std::cout << argv[i] << ", ";
    std::cout << std::endl
              << "Working directory: " << boost::filesystem::initial_path() << std::endl;
    std::signal(SIGINT, signal_handler);

    try
    {
        CoreApi core(CoreOptions::make_default());

        try
        {
            while (DEBUG_RUN)
                core.run();
        }
        catch (const std::runtime_error& e)
        {
            std::cout << "Core running fatal error: " << e.what() << std::endl;
        }

        core.dump_all_memory();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Core init/exit/internal fatal error: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Core init/exit/internal fatal error" << std::endl;
    }

    std::cout << "Exiting" << std::endl;
    return 0;
}