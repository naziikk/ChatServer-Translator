#ifndef CHATSERVER_TRANSLATE_H
#define CHATSERVER_TRANSLATE_H
#include <string>
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <fstream>

class Translate {
public:
    Translate();

    std::string HttpRequestYandexApi(const std::string& text, const std::string& language, const std::string& api_key);
};

#endif //CHATSERVER_TRANSLATE_H
