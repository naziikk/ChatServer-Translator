#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <cstring>
#include "translate.h"

const int port = 8080;
std::string getAuthToken(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::string token;
    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.find("AUTH_TOKEN") != std::string::npos) {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    token = line.substr(pos + 1);
                    break;
                }
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open the configuration file." << std::endl;
    }
    return token;
}

int main() {
    Translate translate;
    int server_fd;
    int new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    std::string api_key = getAuthToken("config.ini");
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { // создание сокета
        std::cerr << "socket failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) { // установка параметров для повторного использования адреса
        std::cerr << "setsockopt failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // любой адрес на этом устройстве
    address.sin_port = htons(port); // порт для прослушивания

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) { // привязка сокета к IP и порту
        std::cerr << "bind failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) { // включение сервера на прослушивание
        std::cerr << "listen failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Server is running on port " << port << std::endl;
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) { // принятие нового соединения
        std::cerr << "accept failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string text;
    std::string language;
    while (true) {
        int valread = read(new_socket, buffer, 1024); // чтение сообщения клиента
        if (valread <= 0) {
            std::cout << "Client disconnected or error occurred" << std::endl;
            break;
        }
        buffer[valread] = '\0';
        text = std::string(buffer);
        std::cout << "Received from client: " << text << '\n';

        if (text == "exit") {
            std::cout << "Connection closed by client." << '\n';
            break;
        }
        std::string s = "Enter language: \n";
        send(new_socket, s.c_str(), s.size(), 0);
        valread = read(new_socket, buffer, 1024);
        buffer[valread] = '\0';
        language = std::string(buffer);
        std::cout << "Target language chosen by client: " << language << '\n';
        std::string translated_text = translate.HttpRequestYandexApi(text, language, api_key);
        send(new_socket, translated_text.c_str(), translated_text.size(), 0); // отправка переведенного текста
        std::cout << "Translated message sent to client.\n";
    }
    close(new_socket);
    close(server_fd);

    return 0;
}