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

// serialize & deserialize ------------------------------------------

class A {
    int iValue;
    char cValue;
    std::string strValue;

public:
    A(const int i, const char c, const std::string& s) : iValue(i), cValue(c), strValue(s)
    {}
    A(const A&) = default;
    A& operator=(const A&) = default;

    bool operator==(const A& other) const {
        return iValue == other.iValue &&
               cValue == other.cValue &&
               strValue == other.strValue;
    }
    bool operator!=(const A& other) const {
        return !(*this != other);
    }
    friend bool serialize(std::ofstream&, const A&);
    friend bool deserialize(std::ifstream&, A&);
    friend std::ostream& operator<<(std::ostream& os, const A&);
};

bool serialize(std::ofstream& ofile, const A& obj)
{
    ofile.write(reinterpret_cast<const char*>(&obj.iValue), sizeof(int));
    ofile.write(reinterpret_cast<const char*>(&obj.cValue), sizeof(char));
    size_t len = obj.strValue.length();
    ofile.write(reinterpret_cast<const char*>(&len), sizeof(len));
    ofile.write(obj.strValue.c_str(), len);

    return !ofile.fail();
}

bool deserialize(std::ifstream& ifile, A& obj)
{
    ifile.read(reinterpret_cast<char*>(&obj.iValue), sizeof(int));
    ifile.read(reinterpret_cast<char*>(&obj.cValue), sizeof(char));
    size_t len = 0;
    ifile.read(reinterpret_cast<char*>(&len), sizeof(len));
    obj.strValue.resize(len);
    ifile.read(reinterpret_cast<char*>(&obj.strValue.front()), len);

    return !ifile.fail();
}

std::ostream& operator<<(std::ostream& os, const A& obj)
{
    os << "A{ iValue: " << obj.iValue << ", cValue: " << obj.cValue << ", strValue: " << obj.strValue << "}";
    return os;
}

struct POD {
    bool bValue;
    char cValue;
    int iArray[2];
};
bool operator==(const POD& l, const POD& r) {
    return l.bValue == r.bValue &&
           l.cValue == r.cValue &&
           l.iArray[0] == r.iArray[0] &&
           l.iArray[1] == r.iArray[1];
}
std::ostream& operator<<(std::ostream& os, const POD& pod)
{
    os << "POD { bValue: " << pod.bValue
       << ", cValue: " << pod.cValue
       << ", [" << pod.iArray[0] << ", " << pod.iArray[1] << "]"
       << " }";
}

void serialize(const std::string& filename, const std::vector<POD>& data)
{
    auto ofile = std::ofstream{filename, std::ios::binary};
    if (ofile.is_open()) {
        for (const auto& value : data) {
            ofile.write(reinterpret_cast<const char*>(&value), sizeof(value));
        }
        ofile.close();
    }
}

void deserialize(const std::string& filename, std::vector<POD>& data)
{
    auto ifile = std::ifstream{filename, std::ios::binary};
    if (ifile.is_open()) {
        while (true) {
            POD pod;
            ifile.read(reinterpret_cast<char*>(&pod), sizeof(POD));
            if (ifile.fail() || ifile.eof())
                break;
            data.push_back(pod);
        }
        ifile.close();
    }
}

void test_serialize()
{
    auto filename = "serialize.bin";
    auto v0 = std::vector<POD>{
        {true, '1', {1, 2}},
        {true, '2', {2, 4}},
        {false, '3', {3, 6}},
    };
    std::cout << "source\n";
    print(v0);
    serialize(filename, v0);

    std::cout << "\n\n";
    std::cout << "deserialize\n";
    auto v1 = std::vector<POD>{};
    deserialize(filename, v1);
    print(v1);
}

void test_non_pod_serialize()
{
    auto filename = "non_pod.bin";
    auto v0 = std::vector<A>{
        { 1, '1', "one"s },
        { 2, '2', "two"s },
        { 3, '3', "three"s }
    };
    std::cout << "source:\n";
    print(v0);
    auto ofile = std::ofstream{filename, std::ios::binary};
    if (ofile.is_open()) {
        for (const auto& item : v0) {
            serialize(ofile, item);
        }
        ofile.close();
    }

    std::cout << "\n\n";
    std::cout << "deserialize\n";
    auto v1 = std::vector<A>{};
    auto ifile = std::ifstream{filename, std::ios::binary};
    if (ifile.is_open()) {
        while (true) {
            auto a = A{0, '0', "zero"s};
            auto res = deserialize(ifile, a);
            if (res)
                v1.push_back(a);
            else
                break;

            if (ifile.eof())
                break;
        }
        ifile.close();
    }
    print(v1);
}

} //namespace =================================================================

void test_ch_07_file()
{
#if (0) // done
    test_fstream();
    test_serialize();
#endif

    test_non_pod_serialize();
}
