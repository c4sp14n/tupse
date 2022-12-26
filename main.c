#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <fcntl.h>

// ToDo: Add threading
#include <pthread.h>


int PORT = 0;

void checkPort(char *address) {
    fd_set fd_set;
    struct timeval timeValue;

    int sock = 0;
    struct sockaddr_in serv_addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Not clear
    fcntl(sock, F_SETFL, O_NONBLOCK);

    if (sock < 0) {
        printf("Unable to create socket\n");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    int is_valid = inet_pton(AF_INET, address, &serv_addr.sin_addr);

    if (is_valid <= 0) {
        printf("Invalid address\n");
        exit(0);
    }

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // Not clear
    FD_ZERO(&fd_set);
    FD_SET(sock, &fd_set);

    timeValue.tv_sec = 1;
    timeValue.tv_usec = 0;

    // Not clear
    int result = select(sock + 1, NULL, &fd_set, NULL, &timeValue);

    if (result == 1) {
        int so_error;
        socklen_t len = sizeof so_error;
        getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);
        if (so_error == 0) {
            printf("Open port: %d\n", PORT);
        }
    }

    shutdown(sock, 0);
}

int main(int argc, char const* argv[])
{
    if (argc != 2) {
        printf("Usage: %s x.x.x.x\n", argv[0]);
        return 0;
    }

    char *address = argv[1];

    for (; PORT < 100; PORT++) {
        checkPort(address);
    }

    return 0;
}