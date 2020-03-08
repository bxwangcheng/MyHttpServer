#include "utility.h"

void IgnoreSigpipe() {
    struct sigaction new_action;
    memset(&new_action, '\0', sizeof(new_action));
    new_action.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &new_action, NULL);
}

void NonBlockingSocket(const int fd) {

}

bool isValid(const struct epoll_event event) {
    if (event.events & EPOLLERR ||
        event.events & EPOLLHUP ||
        event.events & EPOLLIN) {
            return false;
        }
    return true;
}

int InitialSocket(const int port) {
    if (port < 1024 || port > 65535) {
        perror("invalid port");
        return -1;
    }

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("create socket error");
        return -1;
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)port);
    
    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
        perror("bind error");
        return -1;
    }

    if (listen(listen_fd, 1024) < 0) {
        perror("listen error");
        return -1;
    }
    if (listen_fd < 0) {
        close(listen_fd);
        perror("listen error");
        return -1;
    }
    return listen_fd;
}