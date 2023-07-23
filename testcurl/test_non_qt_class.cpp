#include "Curl.h"
#include <iostream>

void test_non_qt_class()
{
    Curl curl;

    const char* url = "https://google.com";

    std::vector<std::string> header;
    header.push_back("Content-Type: application/json");

    auto ret = curl.get(url, header);
    if (ret != 0) {
        std::cerr << "get(" << url << ") : " << ret << std::endl;
    }
    ret = curl.get("https://www.clien.net/service/", header);
    if (ret != 0) {
        std::cerr << "get(" << url << ") : " << ret << std::endl;
    }
}
