#ifndef THREAD_HANDLER
#define THREAD_HANDLER

#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <functional>

using namespace std;

class ThreadHandler
{
public:
    ThreadHandler(size_t);
    ~ThreadHandler();

    void enqueue(function<void()>);

private:
    vector<thread> threads;
    queue<function<void()>> tasks;
    mutex queue_mutex_;
    condition_variable cv;

    bool stop = false;
};

#endif