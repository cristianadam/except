#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <map>
#include <functional>

#include <except/except.hpp>

struct Message
{
    std::string message;
    Message(const std::string& aMessage) : message(aMessage)
    {
        std::cout << "Message: " << message << std::endl;
    }
    
    ~Message()
    {
        std::cout << "~Message: " << message << std::endl;
    }
};

void readNullPointer()
{
    try
    {
       Message msg("read from nullptr");
       int* p = nullptr;
       std::cout << *p << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void writeNullPointer()
{
   try
   {
      Message msg("write to nullptr");
      int* p = nullptr;
      *p = 42;
      std::cout << *p << std::endl;
   }
   catch (const std::exception& ex)
   {
       std::cout << ex.what() << std::endl;
   }
}

void divisionByZero()
{
   try
   {
      Message msg("division by zero");
      int a = 42;
      volatile int b = 0;
      std::cout << a / b << std::endl;
   }
   catch (const std::exception& ex)
   {
       std::cout << ex.what() << std::endl;
   }
}

void outOfBoundsVector()
{
    try
    {
        Message("out of bounds vector");
        std::vector<int> v;
        v[0] = 42;
        std::cout << v[0] << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void nullSharedPointer()
{
    try
    {
        Message("reading empty shared_ptr");
        std::shared_ptr<int> sp = std::make_shared<int>(42);
        std::shared_ptr<int> sp2;
        sp.swap(sp2);
        
        std::cout << *sp << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

std::vector<std::function<void()>> processArguments(int argc, char* argv[])
{
    std::vector<std::string> arguments(argv, argv + argc);

    std::map<std::string, std::function<void()>> functions
    {
        { "readNullPointer", readNullPointer },
        { "writeNullPointer", writeNullPointer },
        { "nullSharePointer", nullSharedPointer },
        { "outOfBoundsVector", outOfBoundsVector },
        { "divisionByZero", divisionByZero }
    };

    std::vector<std::function<void()>> callList;

    if (arguments.size() == 1)
    {
        std::ostringstream os;
        for (auto pair : functions)
        {
            if (os.str().size())
            {
                os << "|";
            }
            os << pair.first;
        }
        std::cout << "Usage: " << arguments[0] << " [all][" << os.str() << "]" << std::endl;
    }
    else if (arguments.size() == 2 && arguments[1] == "all")
    {
        for (auto pair : functions)
        {
            callList.push_back(pair.second);
        }
    }
    else
    {
        for (auto arg : arguments)
        {
            auto it = functions.find(arg);
            if (it != functions.end())
            {
                callList.push_back(it->second);
            }
        }
    }

    return callList;
}

void terminateHandler()
{
    if (std::current_exception())
    {
        try
        {
            throw;
        }
        catch (const std::exception& ex)
        {
            std::cout << "terminateHandler: " << ex.what() << std::endl;
        }
        catch (...)
        {
            std::cout << "terminateHandler: Unknown exception!" << std::endl;
        }
    }
    else
    {
        std::cout  << "terminateHandler: called without an exception." << std::endl;
    }
    std::abort();
}

int main(int argc, char* argv[])
{
    except::register_for_os_exceptions();
    std::set_terminate(terminateHandler);
   
    auto callList = processArguments(argc, argv);
    for (int i = 0; i < 10 && callList.size(); ++i)
    {
        std::cout << i << "------------------------------------" << std::endl;
        for (auto func : callList)
        {
            func();
        }
        std::cout << "------------------------------------" << i << std::endl;
    }
}
