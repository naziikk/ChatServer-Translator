#include "translate.h"
#include <curl/curl.h>
// indef - ajee8c3bma76mj9l86sb
// key - AQVNzkMGuyjmtNNUG4tx_7HH7Vs8ra3G8tZSiBOt
Translate::Translate() {}

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string Translate::HttpRequestYandexApi(const std::string& text, const std::string& language, const std::string& api_key) {
    // https://curl.se/libcurl/c/curl_easy_init.html - используя это
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        CURLcode res;
        std::string url = "https://translate.api.cloud.yandex.net/translate/v2/translate";
        std::string json_data = "{ \"texts\": [\"" + text + "\"], \"targetLanguageCode\": \"" + language + "\" }";

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        // https://curl.se/libcurl/c/curl_slist_append.html - используя это
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, ("Authorization: Api-Key " + api_key).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str()); // post request

        // https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html - используя это
//        size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << '\n';
        }
        // https://curl.se/libcurl/c/curl_easy_cleanup.html - используя это
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return response;
}

