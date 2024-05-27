#include "ThreadHandler.h"

ThreadHandler::ThreadHandler(size_t num_threads)
{
    for (size_t i = 0; i < num_threads; ++i)
    {
        threads.emplace_back([this]
                             { 
                while (true) { 
                    function<void()> task; 
                    { 
                        unique_lock<mutex> lock(queue_mutex_); 
  
                        cv.wait(lock, [this] { return !tasks.empty() || stop; }); 

                        if (stop && tasks.empty()) { 
                            return; 
                        } 
  
                        task = move(tasks.front()); 
                        tasks.pop(); 
                    } 
  
                    task(); 
                } });
    }
}

ThreadHandler::~ThreadHandler()
{
    {
        unique_lock<mutex> lock(queue_mutex_);
        stop = true;
    }

    cv.notify_all();

    for (auto &thread : threads)
    {
        thread.join();
    }
}

void ThreadHandler::enqueue(function<void()> task)
{
    {
        unique_lock<std::mutex> lock(queue_mutex_);
        tasks.emplace(move(task));
    }
    cv.notify_one();
}