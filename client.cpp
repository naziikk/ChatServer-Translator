#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }

    std::string message;
    while (true) {
        std::cout << "Enter message to send to server (or 'exit' to quit): ";
        std::getline(std::cin, message);

        send(sock, message.c_str(), message.size(), 0);

        if (message == "exit") {
            std::cout << "Connection closed by client." << std::endl;
            break;
        }

        // Получаем запрос от сервера на выбор языка перевода
        int valread = read(sock, buffer, 1024);
        buffer[valread] = '\0';  // Завершаем строку
        std::cout << "Server: " << buffer;

        // Вводим язык перевода
        std::string language;
        std::getline(std::cin, language);
        send(sock, language.c_str(), language.size(), 0);

        // Получаем переведенное сообщение от сервера
        valread = read(sock, buffer, 1024);
        buffer[valread] = '\0';  // Завершаем строку
        std::cout << "Translated message from server: " << buffer << std::endl;
    }

    close(sock);
    return 0;
}