#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <pthread.h>
#include <vector>
#include <functional>
using namespace std;

const int MAX_THREADS = 1024;
const int MAX_QUEUE = 65535;

typedef unsigned short index_t;
typedef unsigned char type_t;
typedef unsigned char status_t;
typedef enum {
    immediate = 1, 
    graceful = 2
} threadpool_shutdown_type_t;
typedef enum {
    THREADPOOL_STATUS_INITIALIZER = 0,
    THREADPOOL_INVALID = -1,
    THREADPOOL_LOCK_FAILURE = -2,
    THREADPOOL_QUEUE_FULL = -3,
    THREADPOOL_SHUTDOWN = -4,
    THREADPOOL_THREAD_FAILURE = -5,
    THREADPOOL_GRACEFULL = 1
} threadpool_err_t;

struct ThreadPoolTask {
    function<void(void*)> func;
    void *args;
};

class ThreadPool {
    static pthread_mutex_t _lock;
    static pthread_cond_t notify;
    static vector<pthread_t> threads;
    static vector<ThreadPoolTask> task_queue;

    static size_t _thread_cnt;
    static size_t _task_cnt;
    static size_t _queue_size;
    static size_t _running_cnt;

    // [head, tail)
    static index_t _head;
    static index_t _tail;

    static type_t _shutdown;

public:
    static ThreadPool& InitialThreadPool(size_t pool_size, size_t queue_size);
    static status_t Add(function<void(void*)> func, void *args);
    static status_t Destroy();
    static status_t Free();
    static void *Thread(void *args);
    static bool empty();

private:
    ThreadPool();
    ThreadPool(ThreadPool const&);
    void operator=(ThreadPool const&);
};

#endif