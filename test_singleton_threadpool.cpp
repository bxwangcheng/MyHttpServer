/*
 * Thread pool
 *
 * Compile with "g++ ThreadPool.cpp -o ThreadPool --std=c++11"
 */

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

using namespace std;

struct Task {
	function<void(void*)> func;
    void* args;
    Task() :
        func(nullptr),
        args(nullptr)
    {}

    Task(function<void(void*)> f, void* a) :
        func(f),
        args(a)
    {}
};

class ThreadPool {
private:
    static int num_threads;        /* fixed no of threads in pool */


    mutex task_queue_mutex;
    condition_variable conditionEmptyQueue;

    vector<thread> threads;
    queue<Task> task_queue;

    ThreadPool() {
        cout << "Creating " << num_threads << " in thread pool" << endl;
        for (int i = 0; i < num_threads; i++) {
            threads.push_back(thread(&ThreadPool::infinite_loop_fn, this));
        }
    }

    ~ThreadPool() {
        // for singleton class it only gets called at program termination.
    }

    void infinite_loop_fn();
public:
    static ThreadPool& getInstance() {
        static ThreadPool threadPool; /* part of singleton pattern   */
        return threadPool;
    }

    void addJob(Task task);
};

/* Initialize static class attributes */
int ThreadPool::num_threads = 8;

/*
 * Threads run with an infinite loop, constantly waiting for new tasks
 * to grab and run.
 */
void ThreadPool::infinite_loop_fn() {
	Task task;
    while (true) {
        {
            unique_lock<mutex> lock(task_queue_mutex);
            while (task_queue.empty()) {
                // lock will be released automatically.
                conditionEmptyQueue.wait(lock);
            }
            task = task_queue.front();
            task_queue.pop();
            // release the lock
        }
        (task.func)(task.args);
    }
}

void ThreadPool::addJob(Task task) {
    unique_lock<mutex> lock(task_queue_mutex);
    task_queue.push(task);
    conditionEmptyQueue.notify_one();
}

mutex display_mutex;

/* function that will be passed to job queue to be execute by thread pool */
void func(void* args) {
    int param = *(int*)args;
    display_mutex.lock();
    cout << "Executing task in thread " << this_thread::get_id() << endl;
    cout << "circle " << param << endl;
    display_mutex.unlock();
}


int main() {
    cout << "Thread pool implementation" << endl;
    ThreadPool& threadPool = ThreadPool::getInstance();

    // Add jobs to queue to be executed by thread pool
    for (int i = 0; i < 1000; i++) {
        Task new_task(func, (void*)(&i));
        threadPool.addJob(new_task);
    }
    while (1);
    return 0;
}

/*
  Suggestions:
  Instead of creating threads in the pool in constructor we can create
  the threads in main function dynamically and push it into threadpool.
 */