//=============================================================================
//  A tour of C++
//  Ch.10  Input & Output
//
//  22/09/17
//=============================================================================

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <filesystem>

namespace { //=================================================================

using namespace std::string_literals;
using namespace std::filesystem;

template <typename To=std::string, typename From=std::string>
To to(From source)
{
    auto ss = std::stringstream{};
    auto result = To{};

    if (!(ss << source) ||
        !(ss >> result) ||
        !(ss >> std::ws).eof())
        throw std::runtime_error{"to<>() failed."};

    return result;
}

void test_to()
{
    auto x0 = to<std::string, double>(1.2);
    std::cout << typeid(x0).name() << ": " << x0 << std::endl;

    auto x1 = to<std::string>(1.2);
    std::cout << typeid(x1).name() << ": " << x1 << std::endl;

    auto x2 = to<>(1.2);
    std::cout << typeid(x2).name() << ": " << x2 << std::endl;

    auto x3 = to(1.2);
    x3 += " <- string type"s;
    std::cout << typeid(x3).name() << ": " << x3 << std::endl;
}

void test_print_directory(const path& p)
{
    auto dir = p;
    if (!is_directory(dir)) {
        std::cout << "#0 - " << dir << "\n";
        dir = dir.parent_path();
        std::cout << "#1 - " << dir << "\n";
    }

    try {
        for (const directory_entry& entry : directory_iterator(dir)) {
            std::cout << " " << entry.path() << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
    }
}

} //namespace =================================================================

void test_ch_10(const char* path)
{
    std::ios_base::sync_with_stdio(false);  // do not use stdio

#if (0) // done
    test_to();
#endif

    test_print_directory(path);
}
