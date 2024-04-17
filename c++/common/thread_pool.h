#pragma once

#include <thread>
#include <functional>
#include <future>
#include <exception>
#include <list>
#include <queue>
#include <vector>
#include <memory>
#include <mutex>
#include <iostream>

namespace TestDemo
{

class Any
{
public:
    virtual std::string typeString() = 0;
    virtual void * getData() = 0;
};

template <typename T>
class AnyWrapper : public Any
{
public:
    AnyWrapper(const T value) : data(std::move(value)) {}
    ~AnyWrapper() = default;
    std::string typeString() override { return typeid(data).name(); }
    void * getData() override { return &data; }

private:
    T data;
};

using AnyPtr = std::unique_ptr<Any>;

class TaskImpl
{
public:
    using Task = std::function<void()>;
    TaskImpl() = default;
    virtual ~TaskImpl() = default;
    virtual void runImpl() = 0;
    virtual AnyPtr moveResult() = 0;

public:
    void run()
    {
        if (!pause)
            runImpl();
    }
private:
    bool pause = false;
};

class AsyncTask : public TaskImpl
{
public:
    using AsyncResult = std::future<AnyPtr>;
    using PromiseAny = std::promise<AnyPtr>;
    using Job = std::function<AnyPtr()>;
    
    AsyncTask(Job task_job_) : task_job(task_job_), prom(PromiseAny()), fut(prom.get_future()) {}
    ~AsyncTask() = default;

    void runImpl() override
    {
        AnyPtr res = task_job();
        prom.set_value(std::move(res));
    }

    AnyPtr moveResult() override
    {
        return fut.get();
    }

private:
    PromiseAny prom;
    AsyncResult fut;
    Job task_job;
};

class Task : public TaskImpl
{
public:
    using Job = std::function<void()>;
    Task(Job task_job_) : task_job(task_job_) {};
    ~Task() = default;

    void runImpl() override
    {
        task_job();
    }
    AnyPtr moveResult() override
    {
        throw std::runtime_error("Not implemented!");
    }
private:
    Job task_job;
};

using TaskPtr = std::shared_ptr<TaskImpl>;

class ThreadPool
{
public:
    ThreadPool(const size_t max_thread_) : max_thread(max_thread_), max_task(max_thread_ * 2)
    {
        for (size_t i = 0; i < max_task; ++i)
        {
            std::thread t1([this] { worker(); });
            threads.emplace_back(std::move(t1));
        }
    }
    void worker()
    {
        try
        {
            while (!stop)
            {
                TaskPtr task;
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    auto pred = [this]() -> bool { return !tasks.empty() || stop; };
                    if (stop)
                        return;
                    task_finished.wait(lock, pred);
                    if (stop && tasks.empty())
                        return;
                    task = std::move(tasks.front());
                    tasks.pop();
                    task_finished.notify_one();
                }
                task->run();
            }
        }
        catch(...)
        {
            std::cout << "Occur some exception in worker, will stop." << std::endl;
        }
    }
    void addTask(TaskPtr task)
    {
        std::unique_lock<std::mutex> lock(mutex);
        auto pred = [this]() -> bool { return tasks.size() < max_task || stop; };
        if (stop)
            return;
        task_finished.wait(lock, pred);
        tasks.emplace(std::move(task));
        task_finished.notify_all();
    }
    ~ThreadPool()
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            stop = true;
        }
        task_finished.notify_all();
        for (auto & thread : threads)
            thread.join();
    }

private:
    const size_t max_thread;
    const size_t max_task;
    std::vector<std::thread> threads;

    mutable std::mutex mutex;
    bool stop = false;
    std::condition_variable task_finished;
    std::queue<TaskPtr> tasks;
};
}