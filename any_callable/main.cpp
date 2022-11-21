#include <iostream>

#include "any_callable.h"

void fun()
{
    std::cout << "hello from free function\n";
}

int main()
{
    {
        any_callable<void(), sizeof(void (*)())> a {};

        std::cout << "any_callable is safe to call without initialization\n";
        a();

        std::cout << "it works with non capturing lambdas\n";
        a = []() {
            std::cout << "Hello from non-capturing lambda\n";
        };
        a();

        std::cout << "and capturing lambdas\n";
        int i = 6;
        a = [&]() {
            std::cout << "Any callable captured " << i << "\n";
        };
        a();

        std::cout << "it works with free functions\n";
        a = fun;
        a();

        struct call_me
        {
            void operator()()
            {
                std::cout << "hello from callable class\n";
            }
        };

        std::cout << "it works with object with operator()\n";
        a = call_me {};
        a();
    }
}