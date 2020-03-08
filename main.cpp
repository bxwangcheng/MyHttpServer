#include "utility.h"
#include "epoll.h"

const int PORT = 55888;
const int MAX_EVENTS = 1024;
const int THREADPOLL_SIZE = 4;
const int QUEUE_SIZE = 65535;

void AcceptConnection(const int epoll_fd, const int listen_fd);

int main() {

    IgnoreSigpipe();
    int listen_fd = InitialSocket(PORT);
    if (listen_fd < 0) {
        perror("Initial socket failed");
        return 1;
    }
    NonBlockingSocket(listen_fd);

    // 创建用于监听连接的套接字
    int epoll_fd = EpollInit();
    EpollAdd(epoll_fd, listen_fd, EPOLLIN, NULL);

    // 创建存放就绪事件的数组
    struct epoll_event *ready_events;
    ready_events = new epoll_event[MAX_EVENTS];

    // 创建线程池
    ThreadPool &thread_pool = ThreadPool(THREADPOLL_SIZE, QUEUE_SIZE);

    while (true) {
        int ready_count = EpollWait(epoll_fd, MAX_EVENTS, -1, ready_events);
        if (ready_count < 1) {
            continue;
        }
        for (int i = 0; i < ready_count; i++) {
            // 如果有新连接加入
            if (ready_events[i].data.fd == listen_fd) {
                AcceptConnection(epoll_fd, listen_fd);
            } else if (!isValid(ready_events[i])) {
                printf("invalid event\n");
                continue;
            } else {
                /*处理客户端请求*/
            }
        }
        // CleanExpiredEvents();
    }

    



    return 0;
}

