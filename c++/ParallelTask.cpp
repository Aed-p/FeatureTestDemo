#include <iostream>
#include <common/thread_pool.h>
#include <common/stack_trace.h>

template<typename T>
T sum(const T & a, const T & b)
{
    return a + b;
}

int main()
{
    TestDemo::registerSignalHandler();
    TestDemo::ThreadPool pools(4);
    std::vector<TestDemo::TaskPtr> tasks;
    for (size_t i = 0; i < 8; ++i)
    {
        auto async_task = std::make_shared<TestDemo::AsyncTask>([=]() -> TestDemo::AnyPtr { return std::make_unique<TestDemo::AnyWrapper<decltype(i)>>(sum(i, i + 1)); });
        pools.addTask(async_task);
        tasks.emplace_back(std::move(async_task));
    }

    for (auto & task : tasks)
    {
        auto res = task->moveResult();
        size_t * result = static_cast<size_t *>(res->getData());
        std::cout << "this res is: " << *result << std::endl;
    }
}