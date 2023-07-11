#include <iostream>
#include <curl/curl.h>
#include <memory.h>

namespace { //=================================================================

const char* url_post = "https://www.example.com";
const char* url_get = "https://google.com";

struct Chunk {
    char* memory;
    size_t size;
};

#define ENABLE_CALLBACK

size_t write_to_file_callback(void* buffer, size_t size, size_t nmemb, void* user)
{
    size_t realSize = size * nmemb;
    Chunk* mem = (Chunk*)user;
    char* p = (char*)realloc(mem->memory, mem->size + realSize + 1);
    if (!p) {
        std::cerr << "realloc() failed.\n";
        return 0;
    }
    mem->memory = p;
    memcpy(&(mem->memory[mem->size]), buffer, realSize);
    mem->size += realSize;
    mem->memory[mem->size] = 0;

    return realSize;
}

void get(CURL* curl)
{
    curl_easy_setopt(curl, CURLOPT_URL, url_get);

    curl_slist* list = NULL;

    list = curl_slist_append(list, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

    CURLcode res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
}

void post(CURL* curl)
{
    curl_easy_setopt(curl, CURLOPT_URL, url_post);

    curl_slist* list = NULL;

    // METHOD
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    // HEADER
    list = curl_slist_append(list, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

    // SSL
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);

    // DATA
    std::string data = "{\"snippet\": {\"title\": \"this is title\", \"scheduledStartTime\": \"2017-05-15\"},\"status\": {\"privacyStatus\": \"private\"}}";
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

#if defined(ENABLE_CALLBACK)
    // CALLBACK

    Chunk chunk;
    chunk.memory = (char*)malloc(1);
    chunk.size = 0;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_file_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
#endif

    // EXEC
    CURLcode res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);

#if defined (ENABLE_CALLBACK)
    if (0 < chunk.size) {
        //std::cout << "result:\n" << chunk.memory << "\n";

        FILE* fp = fopen("result.html", "wt");
        if (fp) {
            size_t size = fwrite(chunk.memory, 1, chunk.size, fp);
            if (size != chunk.size) {
                std::cerr << "fwrite(" << chunk.size << ") -> " << size << " failed.\n";
            }
            fclose(fp);
        }
    }
    free(chunk.memory);
#endif
}


void test()
{
    CURL* curl = curl_easy_init();
    if (curl) {
#if (0)
        get(curl);
#else
        post(curl);
#endif
    }
}

} //===========================================================================

void test_non_qt()
{
    test();
}
