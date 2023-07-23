#ifndef CURL_H
#define CURL_H

#include <memory>
#include <vector>
#include <string>

class CurlPrivate;

class Curl
{
public:
    Curl();
    ~Curl();

    int get(const char* url, const std::vector<std::string>& list);

    void printVersionInfo();

private:
    CurlPrivate *d = nullptr;
};

#endif // CURL_H
