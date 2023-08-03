#include <exception>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define PORT 8080

using namespace std;

struct Socket {
    int server_fd, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    string buff;
    string hello;
    const char *mensagem = hello.c_str();
    vector<int> sockets;

    Socket() {
        //Criando file descriptor do socket
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            throw string("Erro em server_fd");
        }

        //Attaching socket à porta definida 
        if (setsockopt(server_fd, SOL_SOCKET,
                    SO_REUSEADDR | SO_REUSEPORT, &opt,
                    sizeof(opt))) {
            throw string("Erro em setsockopt");
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        if (bind(server_fd, (struct sockaddr*)&address,
                    sizeof(address))
                < 0) {
            throw string("Erro em bind");
        }

        if (listen(server_fd, 3) < 0) {
            throw string("Erro em Listen");
        }
    };

    ~Socket() {
        // Fecha os sockets conectados 
        for (auto socket : sockets)
            close(socket);

        // Fecha todos os sockets escutando
        shutdown(server_fd, SHUT_RDWR);
    }

    void run() {

        connect();

        bool running{true};
        while (running) {

            for (auto socket : sockets) {
                memset(buffer, 0, 1024);
                valread = read(socket, buffer, 1024);
                if (!(strcmp(buffer, "exit")))
                    running = false;
                cout << buffer << endl;
                for (auto sock :sockets) {
                    //if (sock == socket) continue;
                    send(sock, buffer, sizeof(buffer), 0);
                }
            }
        }
    }

    void connect() {
        sockets.push_back(accept(server_fd, (struct sockaddr*)&address,
                        (socklen_t*)&addrlen));
    }

};

int main(void) {
    try {
        Socket sock;
        sock.run();
    } catch (string err) {
        cout << err << endl;
    }


    return 0;
}
