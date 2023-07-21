#include "Curl.h"
#include <curl/curl.h>

class CurlPrivate
{
public:
    CurlPrivate() {
        curl = curl_easy_init();
    }
    ~CurlPrivate() {
        curl_easy_cleanup(curl);
    }
    CURL* curl;
};

Curl::Curl() : d(new CurlPrivate)
{
}

Curl::~Curl()
{
    delete d;
}

int Curl::get(const char *url, const std::vector<std::string>& header)
{
    curl_easy_setopt(d->curl, CURLOPT_URL, url);

    curl_slist* slist = NULL;

    for (const auto& s : header) {
        slist = curl_slist_append(slist, s.c_str());
    }

    curl_easy_setopt(d->curl, CURLOPT_HTTPHEADER, slist);

    CURLcode res = curl_easy_perform(d->curl);

    return (int)res;
}
