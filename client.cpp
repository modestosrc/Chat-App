// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

using namespace std;

int main(int argc, char const* argv[])
{
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    string hello;
    char buffer[1024] = { 0 };
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
            <= 0) {
        printf(
                "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status
                = connect(client_fd, (struct sockaddr*)&serv_addr,
                    sizeof(serv_addr)))
            < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    while (hello != "exit") {
        cin >> hello;
        const char *mensagem = hello.c_str();

        send(client_fd, mensagem, strlen(mensagem), 0);
        valread = read(client_fd, buffer, 1024);
        printf("%s\n", buffer);

    }

    // closing the connected socket
    close(client_fd);
    return 0;
}
