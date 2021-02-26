
#include "MainWindow.h"


#include <iostream>       // std::cerr
#include <exception>      // std::set_terminate
#include <cstdlib>        // std::abort
#include <csignal>


[[noreturn]] void signal_handler(int sig)
{
    std::fprintf(stderr, "Error: signal %d\n", sig);
    std::abort();
}

[[noreturn]] void terminate_handler()
{

    std::exception_ptr exptr = std::current_exception();
    // the only useful feature of std::exception_ptr is that it can be rethrown...
    try
    {
        std::rethrow_exception(exptr);
    }
    catch (std::exception &ex)
    {
        std::fprintf(stderr, "Terminated due to exception: %s\n", ex.what());
    }
    catch (...)
    {
        std::fprintf(stderr, "Terminated due to unknown exception\n");
    }


    std::abort();
}


// Main code
int main(int, char**)
{
    std::set_terminate(terminate_handler);

    auto previous_handler = std::signal(SIGABRT, signal_handler);
    if (previous_handler == SIG_ERR)
    {
        std::cerr << "Setup failed\n";
        return EXIT_FAILURE;
    }

    MainWindow w;
    w.Initialize();

    w.Loop();

    return 0;
}
