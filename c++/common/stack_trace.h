#pragma once

#include <iostream>
#include <sstream>
#include <dlfcn.h>
#include <unistd.h>
#include <cstdlib>
#include <cxxabi.h>
#include <csignal>
#include <assert.h>

extern int unw_backtrace(void ** buffer, int size);

namespace TestDemo
{

inline std::string CurrentStackTrace() 
{
    std::stringstream ss("");
    ss << "*** Begin stack trace ***" << std::endl;

    // Get the mangled stack trace.
    int buffer_size = 128;
    void* trace[128];
    buffer_size = unw_backtrace(trace, buffer_size);

      for (int i = 0; i < buffer_size; ++i) {
        const char* symbol = "";
        Dl_info info;
        if (dladdr(trace[i], &info)) {
          if (info.dli_sname != nullptr) {
            symbol = info.dli_sname;
          }
        }
        ss << "\t" << symbol << std::endl;
      }

    ss << "*** End stack trace ***" << std::endl;
    return ss.str();
}

void singalHandler(int sigunm)
{
    std::cout << CurrentStackTrace() << std::endl;

    exit(sigunm);
}

void registerSignalHandler()
{
    for (int sig = 1; sig < NSIG; ++sig)
        signal(sig, singalHandler);
}
}