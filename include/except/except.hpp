#pragma once

#include <exception>

#ifdef _MSC_VER
    #define noexcept
#endif

namespace except
{
    void register_for_os_exceptions();

    struct null_pointer_exception : public std::exception
    {
        const char* what() const noexcept
        {
            return "OS exception: null pointer!";
        }
    };

    struct division_by_zero_exception : public std::exception
    {
        const char* what() const noexcept
        {
            return "OS exception: division by zero!";
        }
    };
}

#ifdef _MSC_VER
    #undef noexcept
#endif
