#include "Curl.h"
#include <curl/curl.h>
#include <iostream>

class CurlPrivate
{
public:
    CurlPrivate() {
        curl_global_init(CURL_GLOBAL_SSL);

        curl = curl_easy_init();
    }
    ~CurlPrivate() {
        curl_easy_cleanup(curl);
    }

    void checkVersion() {
        auto* vi = curl_version_info(CURLVERSION_NOW);
        auto version = std::string(vi->version);
        auto host = std::string(vi->host);
        auto features = vi->features;
        auto sslVersion = std::string(vi->ssl_version);
        auto libzVersion = std::string(vi->libz_version);
        const auto* const* protocols = vi->protocols;

        std::cout << "libcurl information -----------------------------------\n";
        std::cout << "\tversion: " << version << "\n";
        std::cout << "\thost: " << host << "\n";
        std::cout << "\tssl version: " << sslVersion << "\n";
        std::cout << "\tlibz version: " << libzVersion << "\n";
        std::cout << "\tprotocols\n";
        while (*protocols != 0) {
            auto protocol = std::string(*protocols);
            std::cout << "\t\t" << protocol << "\n";
            protocols++;
        }
#define SUPPORT_FEATURE(a, b)\
        if (a & b) {\
            std::cout << "\t\t" << #b << ": yes\n";\
        } else {\
            std::cout << "\t\t" << #b << ": no\n";\
        }

        std::cout << "\tfeatures\n";
        SUPPORT_FEATURE(features, CURL_VERSION_ALTSVC);
        SUPPORT_FEATURE(features, CURL_VERSION_ASYNCHDNS);
        SUPPORT_FEATURE(features, CURL_VERSION_BROTLI);
        SUPPORT_FEATURE(features, CURL_VERSION_DEBUG);
        SUPPORT_FEATURE(features, CURL_VERSION_GSSAPI);
        SUPPORT_FEATURE(features, CURL_VERSION_HTTP2);
        SUPPORT_FEATURE(features, CURL_VERSION_HTTP3);
        SUPPORT_FEATURE(features, CURL_VERSION_HTTPS_PROXY);
        SUPPORT_FEATURE(features, CURL_VERSION_IDN);
        SUPPORT_FEATURE(features, CURL_VERSION_IPV6);
        SUPPORT_FEATURE(features, CURL_VERSION_KERBEROS5);
        SUPPORT_FEATURE(features, CURL_VERSION_LARGEFILE);
        SUPPORT_FEATURE(features, CURL_VERSION_LIBZ);
        SUPPORT_FEATURE(features, CURL_VERSION_MULTI_SSL);
        SUPPORT_FEATURE(features, CURL_VERSION_NTLM);
        SUPPORT_FEATURE(features, CURL_VERSION_NTLM_WB);
        SUPPORT_FEATURE(features, CURL_VERSION_PSL);
        SUPPORT_FEATURE(features, CURL_VERSION_SSL);
        SUPPORT_FEATURE(features, CURL_VERSION_SSPI);
        SUPPORT_FEATURE(features, CURL_VERSION_TLSAUTH_SRP);
        SUPPORT_FEATURE(features, CURL_VERSION_CURLDEBUG);
        SUPPORT_FEATURE(features, CURL_VERSION_UNIX_SOCKETS);
        SUPPORT_FEATURE(features, CURL_VERSION_CONV);
        SUPPORT_FEATURE(features, CURL_VERSION_GSSNEGOTIATE);
        SUPPORT_FEATURE(features, CURL_VERSION_KERBEROS4);

#if CURLVERSION_SECOND <= CURLVERSION_NOW
        if (vi->ares) {
            auto ares = std::string(vi->ares);
            std::cout << "\tares: " << ares << "\n";
        }
#endif

#if CURLVERSION_THIRD <= CURLVERSION_NOW
        if (vi->libidn) {
            auto libidn = std::string(vi->libidn);
            std::cout << "\tlibidn: " << libidn << "\n";
        }
#endif

#if CURLVERSION_FOURTH <= CURLVERSION_NOW
        if (vi->libssh_version) {
            auto libsshVersion = std::string(vi->libssh_version);
            std::cout << "\tlibssh: " << libsshVersion << "\n";
        }
#endif

#if CURLVERSION_FIFTH <= CURLVERSION_NOW
        if (vi->brotli_version) {
            auto brotliVersion = std::string(vi->brotli_version);
            std::cout << "\tbrotli version: " << brotliVersion << "\n";
        }
#endif

#if CURLVERSION_SIXTH <= CURLVERSION_NOW
        if (vi->nghttp2_version) {
            auto nghttp2Version = std::string(vi->nghttp2_version);
            std::cout << "\tnghttp2 version: " << nghttp2Version << "\n";
        }
        if (vi->quic_version) {
            auto quicVersion = std::string(vi->quic_version);
            std::cout << "\tquic version: " << quicVersion << "\n";
        }
#endif

        std::cout << "libcurl information -----------------------------------\n\n";
    }

    CURL* curl;
    std::string response;
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
    curl_easy_reset(d->curl);
    curl_easy_setopt(d->curl, CURLOPT_URL, url);

    curl_slist* slist = NULL;

    for (const auto& s : header) {
        slist = curl_slist_append(slist, s.c_str());
    }

    curl_easy_setopt(d->curl, CURLOPT_HTTPHEADER, slist);

    CURLcode res = curl_easy_perform(d->curl);

    return (int)res;
}

int Curl::post(const char *url, const std::vector<std::string> &headers, const std::string &body)
{
    curl_easy_reset(d->curl);

    // URL
    curl_easy_setopt(d->curl, CURLOPT_URL, url);

    // HEADER
    if (0 < headers.size()) {
        curl_slist* slist = NULL;
        for (auto header : headers) {
            slist = curl_slist_append(slist, header.c_str());
        }

        curl_easy_setopt(d->curl, CURLOPT_HTTPHEADER, slist);
    }

    // METHOD
    curl_easy_setopt(d->curl, CURLOPT_POST, 1L);

    // SSL
    curl_easy_setopt(d->curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(d->curl, CURLOPT_SSL_VERIFYHOST, 1L);

    // DATA
    if (0 < body.length()) {
        curl_easy_setopt(d->curl, CURLOPT_POSTFIELDS, body.c_str());
    }

    curl_easy_setopt(d->curl, CURLOPT_WRITEFUNCTION, &Curl::invokeWriteData);
    curl_easy_setopt(d->curl, CURLOPT_WRITEDATA, this);

    CURLcode res = curl_easy_perform(d->curl);

    return (int)res;
}

size_t Curl::writeCallback(void *data, size_t size, size_t memBytes)
{
    std::cout << "writeCallback(" << data << ", " << size << ", " << memBytes << ")\n\n";
    size_t totalSize = size*memBytes;
    d->response = std::string((char*)data, totalSize);

    FILE* fp = fopen("response.txt", "wt");
    if (fp) {
        fwrite(data, 1, totalSize, fp);
        fclose(fp);
    }
    return totalSize;
}

void Curl::printVersionInfo()
{
    d->checkVersion();
}

void Curl::printResponse()
{
    std::cout << d->response;
}

void Curl::printError(int error)
{
    const char* errorString = curl_easy_strerror((CURLcode)error);
    std::cout << "error(" << error << "): " << errorString << "\n";
}
