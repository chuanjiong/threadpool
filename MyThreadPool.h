/*
 * MyThreadPool.h
 */

#ifndef _MYTHREADPOOL_H_
#define _MYTHREADPOOL_H_

#include <pthread.h>
#include <functional>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>

class MyThread {
public:
    MyThread(const std::function<void()> &f) : stop(false), func(f) {
        if (pthread_create(&thread, nullptr,
                [](void *arg){
                    ((MyThread *)arg)->func();
                    return (void *)nullptr;
                }, this) != 0) {
            stop = true;
        }
    }

    ~MyThread() {
        join();
    }

    void join() {
        if (!stop) {
            stop = true;
            pthread_join(thread, nullptr);
        }
    }

private:
    pthread_t thread;
    std::function<void()> func;
    bool stop;
};

class MyThreadPool {
public:
    MyThreadPool(int num) : stop(false) {
        while (num-- > 0) {
            auto p = std::make_shared<MyThread>([this](){
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lk(this->mtx);
                        this->cond.wait(lk, [this](){
                            return this->stop || !this->tasks.empty();
                        });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = this->tasks.front();
                        this->tasks.pop();
                    }
                    task();
                }
            });
            threads.push_back(p);
        }
    }

    ~MyThreadPool() {
        {
            std::unique_lock<std::mutex> lk(mtx);
            stop = true;
        }
        cond.notify_all();
        for (auto &thread : threads) {
            thread->join();
        }
    }

    void enqueue(const std::function<void()> &f) {
        {
            std::unique_lock<std::mutex> lk(mtx);
            if (stop)
                return;
            tasks.push(f);
        }
        cond.notify_one();
    }

private:
    std::vector<std::shared_ptr<MyThread>> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cond;
    bool stop;
};

#endif //_MYTHREADPOOL_H_

