#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "util.h"

using namespace std::string_literals;

namespace { //=================================================================

template <typename C>
void test_write_bin(const std::string& filename, const C& c)
{
    auto of = std::ofstream{filename, std::iostream::binary};
    if (of.is_open()) {
        of.write(reinterpret_cast<const char*>(c.data()), c.size());
        of.close();
    }
}

template <typename C>
void test_read_bin(const std::string& filename, C& c)
{
    auto ifile = std::ifstream{filename, std::iostream::ate | std::iostream::binary};
    if (ifile.is_open()) {
#if (0) //[1]
        //ifile.seekg(0, std::ios_base::end);
        auto len = ifile.tellg();
        ifile.seekg(0, std::ios_base::beg);
        c.resize(static_cast<size_t>(len));
        ifile.read(reinterpret_cast<char*>(c.data()), len);
        auto success = !ifile.fail() && len == ifile.gcount();
        std::cout << "test_read_bin(" << filename << "): " << success << "(" << len << " bytes.)\n";
#else   //[1]
        auto len = ifile.tellg();
        ifile.seekg(0, std::ios_base::beg);

#if (0) //[2]
        c.resize(len);
        c.assign(std::istreambuf_iterator<char>(ifile), std::istreambuf_iterator<char>());
#else   //[2]
        std::copy(std::istreambuf_iterator<char>(ifile), std::istreambuf_iterator<char>(), std::back_inserter(c));
#endif  //[2]

        auto success = !ifile.fail() && len == ifile.gcount();
        std::cout << "read: " << success << ", c.size(): " << c.size() << "\n";
        std::cout << "len: " << len << ", ifile.gcount(): " << ifile.gcount() << "\n";
#endif  //[1]

        ifile.close();
    }
}

void test_fstream()
{
    std::cout << std::boolalpha;

    auto out = std::vector<unsigned char>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto filename = "sample.bin"s;
    test_write_bin(filename, out);

    auto in = std::vector<unsigned char>{};
    test_read_bin(filename, in);

    for (auto uc : in) {
        std::cout << static_cast<int>(uc) << " ";
    }
    std::cout << "\n";
}

} //namespace =================================================================

void test_ch_07_file()
{
    test_fstream();
}
