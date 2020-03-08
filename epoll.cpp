#include "epoll.h"

// 创建epoll文件描述符并返回
int EpollInit() {
    int epoll_fd = epoll_create(1);
    if (epoll_fd < 0) {
        return -1;
    }
    return epoll_fd;
}

// 注册新文件描述符到epoll
int EpollAdd(const int epoll_fd, const int fd, const __uint32_t EVENTS, void *request) {
    struct epoll_event new_event;
    new_event.data.fd = fd;
    new_event.data.ptr = request;
    new_event.events = EVENTS;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &new_event) < 0) {
        perror("Add event to epoll_fd error!");
        return -1;
    }
    return 0;
}

// 从epoll中删除文件描述符
int EpollRemove(const int epoll_fd, const int fd, const __uint32_t EVENTS, void *request) {
    // struct epoll_event remove_event;
    // remove_event.events = EVENTS;
    // remove_event.data.ptr = request;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) < 0) {
        perror("Remove event from epoll_fd error!");
        return -1;
    }
    return 0;
}

// 修改epoll中注册的文件描述符
int EpollModify(const int epoll_fd, const int fd, const __uint32_t EVENTS, void *request) {
    struct epoll_event modify_event;
    modify_event.events = EVENTS;
    modify_event.data.ptr = request;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &modify_event) < 0) {
        perror("Modify event in epoll_fd error!");
        return -1;
    }
    return 0;
}

// 返回准备就绪的epoll事件数量，所有就绪的epoll事件存放在epoll_event数组events里
int EpollWait(const int epoll_fd, const int max_events, const int timeout, struct epoll_event* events) {
    int ready_count = epoll_wait(epoll_fd, events, max_events, timeout);
    if (ready_count < 0) {
        perror("Wait epoll error");
        return -1;
    }
    return ready_count;
}