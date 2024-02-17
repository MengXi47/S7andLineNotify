#define CURL_STATICLIB
#include <iostream>
#include <string>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void Send_LineNotify(std::string message) {
    CURL* curl;
    CURLcode res;
    std::string postFields = "message=" + message;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://notify-api.line.me/api/notify");
        curl_easy_setopt(curl, CURLOPT_POST, 1L); struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Authorization: Bearer 1kyvxxb2BdECBFQoLBwSC3X47yRHqjc5HfSpXHsXFSa");
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        else {
            std::cout << "LineNotify Sent Message Successfully!" << std::endl;
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    else {
        std::cerr << "Curl initialization failed!" << std::endl;
    }
}
