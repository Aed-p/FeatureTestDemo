#include <iostream>
#include <common/thread_pool.h>
#include <common/stack_trace.h>
#include <unordered_map>

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
    std::unordered_map<int, int> map;
    std::cout << "add insert task" << std::endl;
    for (size_t i = 0; i < 8; ++i)
    {
        auto cur_id = i;
        auto async_task = std::make_shared<TestDemo::AsyncTask>([&]() -> TestDemo::Any { return map.insert({cur_id, cur_id + 1}); });
        pools.addTask(async_task);
        tasks.emplace_back(std::move(async_task));
    }
    std::cout << "add erase task" << std::endl;
    for (size_t i = 8; i > 0; --i)
    {
        std::cout << "add erase task1" << std::endl;
        auto cur_id = i;
        auto async_task = std::make_shared<TestDemo::AsyncTask>([&]() -> TestDemo::Any { return map.erase(cur_id); });
        std::cout << "add erase task2" << std::endl;
        pools.addTask(async_task);
        tasks.emplace_back(std::move(async_task));
        std::cout << "add erase task3" << std::endl;
    }
    std::cout << "get task res" << std::endl;
    for (auto & task : tasks)
    {
        std::cout << "get task res" << std::endl;
        auto res = task->moveResult();
        // size_t result = std::any_cast<size_t>(res);
        // std::cout << "this res is: " << result << std::endl;
    }
}