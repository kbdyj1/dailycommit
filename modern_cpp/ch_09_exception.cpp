#include <iostream>
#include <exception>
#include <string>
#include <sstream>

#include "util.h"

namespace { //=================================================================

using namespace std::string_literals;

// logic_error
//  invalid_argument, out_of_range, length_error

// runtime_error
//  overflow_error, underflow_error, system_error

// bad
//  bad_alloc, bad_cast, bad_function_call

class simple_error : public std::exception {
public:
    virtual const char* what() const noexcept override {
        return "simple exception";
    }
};

class test_logic_error : public std::logic_error {
public:
    test_logic_error() : std::logic_error("test logic error")
    {}
};

class advanced_error : public std::runtime_error {
    int errorCode;
    std::string makeErrorString(const int e) {
        auto ss = std::stringstream{};
        ss << "advanced error(errorCode: "s << e << ")"s;
        return ss.str();
    }

public:
    advanced_error(const int e) : std::runtime_error(makeErrorString(e))
    {}

    int error() const noexcept {
        return errorCode;
    }
};

void throwFunc() throw() {
#if (0)
    throw std::system_error(std::make_error_code(std::errc::timed_out));
#else
    throw advanced_error(1);
#endif
}

void testException() {
    auto ePrint = [](const std::exception& e) {
        std::cout << e.what() << std::endl;
    };
    try {
        throwFunc();
    } catch (const std::system_error& e) {
        ePrint(e);
    } catch (const std::runtime_error& e) {
        ePrint(e);
    } catch (const std::exception& e) {
        ePrint(e);
    } catch (...) {
        std::cout << "Unknown exception." << std::endl;
    }
}

template <typename T>
T func0() noexcept(std::is_nothrow_constructible<T>::value)
{
    return T{};
}

template <typename T>
T func1() noexcept(noexcept(T{}))
{
    return T{};
}

template <typename F, typename Arg>
auto func2(F&& func, Arg&& arg) noexcept
{
    static_assert(!noexcept (func(arg)), "F is throwing");
    return func(arg);
}

void funcNoThrow()
{}

void test_no_except()
{
    PRINT_FUNC(noexcept(funcNoThrow));
    PRINT_FUNC(noexcept(throwFunc));
}

} //namespace =================================================================

void test_ch_09_exception()
{
    std::cout << std::boolalpha;

#if (0) // done
    testException();
#endif

    test_no_except();
}
