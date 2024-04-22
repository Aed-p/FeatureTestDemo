#include <common/watch_helper.h>
#include <mutex>
#include <atomic>
#include <shared_mutex>

int main()
{
    std::shared_ptr<int> test_ptr = std::make_shared<int>(1);
    std::atomic<int> test_int(0);
    TestDemo::WatchHelper watch;
    watch.startWatch();
    for (size_t i = 0; i < 100000000; ++i)
    {
        auto a = test_int.load();
        // auto a = std::atomic_load(&test_ptr);
    }
    watch.stop();

    watch.startWatch();
    for (size_t i = 0; i < 100000000; ++i)
    {
        auto & a = test_ptr;
    }
    watch.stop();

    watch.startWatch();
    std::shared_mutex shared_lock;
    for (size_t i = 0; i < 100000000; ++i)
    {
        std::shared_lock<std::shared_mutex> lock(shared_lock);
        auto a = 1;
    }
    watch.stop();

    return 0;
}