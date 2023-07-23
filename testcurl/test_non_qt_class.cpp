#include "Curl.h"
#include <iostream>
#include <string.h>

namespace { //=================================================================

namespace _1 {

void test()
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

} //_1 --------------------------------------------------------------

namespace _2 {

void test()
{
    Curl curl;
    const char* url = "https://reqbin.com/echo/post/json";

    std::vector<std::string> headers;
    headers.push_back("Accept: application/json");
    headers.push_back("Content-Type: application/json");
    headers.push_back("Authorization: Basic e3VzZXJuYW1lfTp7cGFzc3dvcmR9");

    std::string body = "{\"employee\":{ \"name\":\"Emma\", \"age\":28, \"city\":\"Boston\" }}";

    std::string contentsLength = "Content-Length: ";
    contentsLength += std::to_string(body.size());

    headers.push_back(contentsLength);

    curl.setDebugOn(true);

    auto ret = curl.post(url, headers, body);
    std::cout << "curl.post(" << url << "): " << ret << "\n";
    if (0 == ret) {
        curl.printResponse();
        //curl.saveResponse("response.html");
    } else {
        curl.printError(ret);
    }
    std::cout << "\ncurl.post(" << url << ")\n";
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test()
{
    Curl curl;

    auto ret = curl.get("https://@github.com");
    curl.setPassword(":");
    if (ret == 0) {
        //curl.printResponse();
        curl.saveResponse("response.html");
    } else {
        curl.printError(ret);
    }
}

} //_3 --------------------------------------------------------------

} //===========================================================================

void test_non_qt_class()
{
#if (0) //done
    _1::test();
    _2::test();
#endif

    _3::test();
}
