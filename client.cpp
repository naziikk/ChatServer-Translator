#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

#define PORT 8080 // Порт, который будет использоваться для соединения с сервером

int main() {
    int sock = 0; // Дескриптор сокета
    struct sockaddr_in serv_addr; // Структура для хранения адреса сервера
    char buffer[1024] = {0}; // Буфер для приема данных от сервера

    // Создание сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // SOCK_STREAM используется для TCP соединения
        std::cout << "Socket creation error" << '\n'; // Ошибка при создании сокета
        return -1;
    }

    serv_addr.sin_family = AF_INET; // Указываем, что используем IPv4
    serv_addr.sin_port = htons(PORT); // Устанавливаем порт для подключения к серверу

    // Преобразуем IP-адрес из строки в бинарный формат
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address/ Address not supported" << '\n'; // Ошибка при преобразовании IP-адреса
        return -1;
    }

    // Устанавливаем соединение с сервером
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed" << '\n'; // Ошибка при попытке подключения к серверу
        return -1;
    }

    std::string message; // Сообщение, которое будем отправлять на сервер
    while (true) {
        std::cout << "Enter message to send to server (or 'exit' to quit): ";
        std::getline(std::cin, message); // Ввод сообщения от пользователя

        // Отправляем сообщение на сервер
        send(sock, message.c_str(), message.size(), 0);

        // Если сообщение "exit", завершаем соединение
        if (message == "exit") {
            std::cout << "Connection closed by client." << '\n';
            break;
        }

        // Получаем запрос от сервера на выбор языка перевода
        int valread = read(sock, buffer, 1024); // Читаем ответ от сервера
        buffer[valread] = '\0'; // Завершаем строку
        std::cout << "Server: " << buffer;

        std::string language;
        std::getline(std::cin, language); // Ввод языка от пользователя
        send(sock, language.c_str(), language.size(), 0); // Отправляем выбранный язык на сервер

        // Получаем переведенное сообщение от сервера
        valread = read(sock, buffer, 1024); // Читаем переведенное сообщение от сервера
        buffer[valread] = '\0'; // Завершаем строку
        std::cout << "Translated message from server: " << buffer << '\n';
    }

    close(sock); // Закрываем сокет
    return 0;
}