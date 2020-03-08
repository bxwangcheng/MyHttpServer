#ifndef __EPOLL_H__
#define __EPOLL_H__

#include <sys/epoll.h>
#include <stdio.h>

// 创建epoll文件描述符并返回
int EpollInit();

// 注册新文件描述符到epoll
int EpollAdd(const int epoll_fd, const int fd, const __uint32_t EVENTS, void *request);

// 从epoll中删除文件描述符
int EpollRemove(const int epoll_fd, const int fd, const __uint32_t EVENTS, void *request);

// 修改epoll中注册的文件描述符
int EpollModify(const int epoll_fd, const int fd, const __uint32_t EVENTS, void *request);

// 返回准备就绪的epoll事件数量，所有就绪的epoll事件存放在epoll_event数组events里
int EpollWait(const int epoll_fd, const int max_events, const int timeout, struct epoll_event* events);

#endif