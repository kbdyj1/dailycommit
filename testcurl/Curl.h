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

    static size_t invokeWriteData(void* data, size_t size, size_t memBytes, void* inst) {
        return ((Curl*)inst)->writeCallback(data, size, memBytes);
    }

    int get(const char* url, const std::vector<std::string>& list = std::vector<std::string>());
    int post(const char* url, const std::vector<std::string>& headers, const std::string& body);

    size_t writeCallback(void* data, size_t size, size_t memBytes);

    void printVersionInfo();
    void printResponse();
    void printError(int error);
    void saveResponse(const char* filename);

    void setDebugOn(bool);
    void setPassword(const std::string& user = std::string(), const std::string& password = std::string());

private:
    CurlPrivate *d = nullptr;
};

#endif // CURL_H
