#include <config.h>

#if defined(USE_POSIX_SIGNAL) || defined(USE_POSIX_SIGACTION)
    #include <signal.h>
#endif

#if defined(USE_WINDOWS_SE)
    #include <eh.h>
    #include <windows.h>
#endif

#include <sstream>
#include <exception>
#include <stdexcept>

#include <except/except.hpp>

namespace except
{
#if defined(USE_POSIX_SIGNAL) || defined(USE_POSIX_SIGACTION)
    const char* signalDescription(int sgn)
    {
        switch(sgn)
        {
            case SIGABRT: return "SIGABRT";
            case SIGFPE:  return "SIGFPE";
            case SIGILL:  return "SIGILL";
            case SIGINT:  return "SIGINT";
            case SIGSEGV: return "SIGSEGV";
            case SIGTERM: return "SIGTERM";
            default:      return "UNKNOWN";
        }
    }
#endif

#ifdef USE_POSIX_SIGACTION    
    void signalHandler(int sgn, siginfo_t *info, void *)
    {
        if (sgn == SIGSEGV && info->si_addr == 0)
        {
            throw null_pointer_exception();
        }

        if (sgn == SIGFPE && (info->si_code == FPE_INTDIV || info->si_code == FPE_FLTDIV))
        {
            throw division_by_zero_exception();
        }

        std::ostringstream os;
        os << "Signal caught: " << signalDescription(sgn) << "(" << sgn << ")";

        throw std::runtime_error(os.str().c_str());
    }

    void register_for_os_exceptions()
    {
        struct sigaction act;

        act.sa_sigaction = signalHandler;
        sigemptyset(&act.sa_mask);
        act.sa_flags = SA_SIGINFO | SA_NODEFER;

        sigaction(SIGABRT, &act, NULL);
        sigaction(SIGFPE, &act, NULL);
        sigaction(SIGILL, &act, NULL);
        sigaction(SIGINT, &act, NULL);
        sigaction(SIGSEGV, &act, NULL);
        sigaction(SIGTERM, &act, NULL);
    }
#endif

#ifdef USE_POSIX_SIGNAL
    void
    signalHandler(int sgn)
    {
        std::ostringstream os;
        os << "Signal caught: " << signalDescription(sgn) << "(" << sgn << ")";

        signal(sgn, signalHandler);

        throw std::runtime_error(os.str().c_str());
    }

    void register_for_os_exceptions()
    {
        signal(SIGABRT, signalHandler);
        signal(SIGFPE, signalHandler);
        signal(SIGILL, signalHandler);
        signal(SIGINT, signalHandler);
        signal(SIGSEGV, signalHandler);
        signal(SIGTERM, signalHandler);
    }
#endif

#ifdef USE_WINDOWS_SE
    const char* seDescription(const unsigned int& code)
    {
        switch (code)
        {
            case EXCEPTION_ACCESS_VIOLATION:         return "EXCEPTION_ACCESS_VIOLATION";
            case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
            case EXCEPTION_BREAKPOINT:               return "EXCEPTION_BREAKPOINT";
            case EXCEPTION_DATATYPE_MISALIGNMENT:    return "EXCEPTION_DATATYPE_MISALIGNMENT";
            case EXCEPTION_FLT_DENORMAL_OPERAND:     return "EXCEPTION_FLT_DENORMAL_OPERAND";
            case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
            case EXCEPTION_FLT_INEXACT_RESULT:       return "EXCEPTION_FLT_INEXACT_RESULT";
            case EXCEPTION_FLT_INVALID_OPERATION:    return "EXCEPTION_FLT_INVALID_OPERATION";
            case EXCEPTION_FLT_OVERFLOW:             return "EXCEPTION_FLT_OVERFLOW";
            case EXCEPTION_FLT_STACK_CHECK:          return "EXCEPTION_FLT_STACK_CHECK";
            case EXCEPTION_FLT_UNDERFLOW:            return "EXCEPTION_FLT_UNDERFLOW";
            case EXCEPTION_ILLEGAL_INSTRUCTION:      return "EXCEPTION_ILLEGAL_INSTRUCTION";
            case EXCEPTION_IN_PAGE_ERROR:            return "EXCEPTION_IN_PAGE_ERROR";
            case EXCEPTION_INT_DIVIDE_BY_ZERO:       return "EXCEPTION_INT_DIVIDE_BY_ZERO";
            case EXCEPTION_INT_OVERFLOW:             return "EXCEPTION_INT_OVERFLOW";
            case EXCEPTION_INVALID_DISPOSITION:      return "EXCEPTION_INVALID_DISPOSITION";
            case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
            case EXCEPTION_PRIV_INSTRUCTION:         return "EXCEPTION_PRIV_INSTRUCTION";
            case EXCEPTION_SINGLE_STEP:              return "EXCEPTION_SINGLE_STEP";
            case EXCEPTION_STACK_OVERFLOW:           return "EXCEPTION_STACK_OVERFLOW";
            default:                                 return "UNKNOWN EXCEPTION";
        }
    }
    
    void seTranslator(unsigned int code, struct _EXCEPTION_POINTERS* ep)
    {
        if (code == EXCEPTION_ACCESS_VIOLATION || code == EXCEPTION_IN_PAGE_ERROR)
        {
            if (ep->ExceptionRecord->ExceptionInformation[1] == 0)
            {
                throw null_pointer_exception();
            }
        }
        else if (code == EXCEPTION_FLT_DIVIDE_BY_ZERO ||
                 code == EXCEPTION_INT_DIVIDE_BY_ZERO)
        {
            throw division_by_zero_exception();
        }

        std::ostringstream os;
        os << "Structured exception caught: " << seDescription(code);

        throw std::runtime_error(os.str().c_str());
    }

    void register_for_os_exceptions()
    {
        _set_se_translator(seTranslator);
    }
#endif
}
