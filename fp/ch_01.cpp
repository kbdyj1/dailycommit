#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

namespace { //=================================================================

namespace _1 {

int count_lines(const std::string& filename)
{
    std::ifstream in(filename);

    return std::count(std::istreambuf_iterator<char>(in),
                      std::istreambuf_iterator<char>(),
                      '\n');
}

std::vector<int> count_lines_in_files(const std::vector<std::string>& files)
{
    std::vector<int> results(files.size());
#if (0)
    for (const auto& file : files) {
        results.push_back(count_lines(file));
    }
#else
    std::transform(files.begin(), files.end(), results.begin(), count_lines);
#endif

    return results;
}

void test()
{
    std::vector<std::string> files{ "main.cpp", "ch_01.cpp"};

    auto results = count_lines_in_files(files);

    for (std::size_t i = 0; i < files.size(); i++) {
        std::cout << files.at(i) << ": " << results.at(i) << '\n';
    }
}

} //_1 --------------------------------------------------------------

} //===========================================================================

void test_ch_01()
{
    _1::test();
}
