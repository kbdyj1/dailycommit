#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <utility>
#include <iterator>

namespace { //=================================================================

namespace _1 {

std::string file_reader(const char* filename)
{
    std::ifstream fs;
    fs.open(filename);
    if (!fs) {
        std::cerr << "can't open " << filename << std::endl;
        return "";
    }
    std::stringstream ss;
    std::copy(std::istreambuf_iterator<char>(fs.rdbuf()),
              std::istreambuf_iterator<char>(),
              std::ostreambuf_iterator<char>(ss));

    return ss.str();
}

void file_reader(std::istream& in, std::string& out)
{
    std::stringstream ss;
    std::copy(std::istreambuf_iterator<char>(in.rdbuf()),
              std::istreambuf_iterator<char>(),
              std::ostreambuf_iterator<char>(ss));

    ss.str().swap(out);
}

void test()
{
    std::string s;
    std::ifstream fs("err.txt");
    if (fs) {
        file_reader(fs, s);
        std::cout << s << std::endl;
    }
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_fileio()
{
    _1::test();
}
