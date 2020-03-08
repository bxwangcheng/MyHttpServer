#include "ThreadPool.h"

pthread_mutex_t ThreadPool::_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ThreadPool::notify = PTHREAD_COND_INITIALIZER;

vector<pthread_t> ThreadPool::threads;
vector<ThreadPoolTask> ThreadPool::task_queue;

size_t ThreadPool::_thread_cnt = 0;
size_t ThreadPool::_task_cnt = 0;
size_t ThreadPool::_queue_size = 0;
size_t ThreadPool::_running_cnt = 0;

index_t ThreadPool::_head = 0;
index_t ThreadPool::_tail = 0;

type_t ThreadPool::_shutdown = 0;

ThreadPool& ThreadPool::InitialThreadPool(size_t pool_size, size_t queue_size) {
    if (pool_size > MAX_THREADS)
        pool_size = MAX_THREADS;
    if (queue_size > MAX_QUEUE)
        queue_size = MAX_QUEUE;
    if (pool_size < 1)
        pool_size = 1;
    if (queue_size < 1)
        queue_size = 1;
    static ThreadPool m_instance;
    _thread_cnt = 0;
    _task_cnt = 0;
    _running_cnt = 0;
    _queue_size = queue_size;
    _head = 0;
    _tail = 0;
    threads.resize(pool_size);
    task_queue.resize(queue_size);

    for (int i = 0; i < pool_size; i++) {
        if (pthread_create(&threads[i], NULL, Thread, (void*)(0)) < 0) {
            break;
        }
        _thread_cnt++;
        _running_cnt++;
    }
    return m_instance;
}

status_t ThreadPool::Add(function<void(void*)> func, void *args) {
    index_t next;
    threadpool_err_t err = THREADPOOL_STATUS_INITIALIZER;
    if (pthread_mutex_lock(&_lock) < 0)
        return THREADPOOL_LOCK_FAILURE;
    // 进入临界区
    do {
        next = (_tail + 1) % _queue_size;
        if (_task_cnt == _queue_size) {
            err = THREADPOOL_QUEUE_FULL;
        }

    } while(false);
    // 退出临界区
    return err;
}

status_t ThreadPool::Destroy() {
    
}

status_t ThreadPool::Free() {

}

void *ThreadPool::Thread(void *args) {
    while (true) {
        ThreadPoolTask new_task;
        pthread_mutex_lock(&_lock);
        // 进临界区
        while (ThreadPool::empty() && (!_shutdown)) {
            pthread_cond_wait(&notify, &_lock);
        }
        if (ThreadPool::empty() && _shutdown)
            break;
        new_task.func = task_queue[_head].func;
        new_task.args = task_queue[_head].args;
        task_queue[_head].func = NULL;
        task_queue[_head].args = NULL;
        _head = (_head + 1) % _queue_size;
        _thread_cnt--;
        pthread_mutex_unlock(&_lock);
        // 出临界区
        (new_task.func)(new_task.args);
    }
    _running_cnt--;
    pthread_exit(NULL);
    return (void*)(0);
}

bool ThreadPool::empty() {
    return _thread_cnt == 0;
}