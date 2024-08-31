#ifndef CHATSERVER_TRANSLATE_H
#define CHATSERVER_TRANSLATE_H
#include <string>

class Translate {
public:
    Translate();
    std::string HttpRequestGoogleApi(const std::string& text, const std::string& language);
};

#endif //CHATSERVER_TRANSLATE_H
