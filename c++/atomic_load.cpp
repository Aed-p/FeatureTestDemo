#include <common/watch_helper.h>
#include <common/thread_pool.h>
#include <gflags/gflags.h>
#include <algorithm>
#include <mutex>
#include <atomic>
#include <shared_mutex>

class DecoupleInfo
{
public:
    DecoupleInfo()
    {
        for (size_t i = 0; i < 4; ++i)
            a.emplace_back(std::make_shared<int>(i));
    }
    ~DecoupleInfo() = default;
    using DecoupleInfos = std::vector<std::shared_ptr<int>>;
    DecoupleInfos gitDecoupleInfo()
    {
        std::lock_guard lock(decouple_info_lock);
        return a;
    }

private:
    mutable std::mutex decouple_info_lock;
    DecoupleInfos a;
};

DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");
DEFINE_string(languages, "english,french,german",
                "comma-separated list of languages to offer in the 'lang' menu");
DEFINE_bool(verbose, false, "Display program name before message");
DEFINE_string(message, "Hello world!", "Message to print");
DEFINE_uint64(nums, 10000000, "The number of atomic load");
DEFINE_uint64(threads, 32, "The number of threads");

static bool IsNonEmptyMessage(const char *flagname, const std::string &value)
{
  return value[0] != '\0';
}
DEFINE_validator(message, &IsNonEmptyMessage);

 void initGflags(int argc, char *argv[])
{
    gflags::SetUsageMessage("Atomic load usage message");
    // gflags::SetVersionString("1.0.0");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    // if (FLAGS_verbose) std::cout << gflags::ProgramInvocationShortName() << ": ";
    // std::cout << FLAGS_message << std::endl;
    // gflags::ShutDownCommandLineFlags();
}

int main(int argc, char *argv[])
{
    initGflags(argc, argv);
    std::shared_ptr<int> test_ptr = std::make_shared<int>(1);
    std::atomic<int> test_int(0);
    size_t nums = FLAGS_nums;
    size_t threads = FLAGS_threads;
    DecoupleInfo test_lock;

    TestDemo::WatchHelper watch;
    {
        TestDemo::ThreadPool thread_pool(threads);
        watch.startWatch();
        size_t batch = nums / threads;
        for (size_t i = 0; i < threads; ++i)
        {
            auto task = std::make_unique<TestDemo::Task>([&]() 
            {
                // for (size_t a = 0; a < batch; ++a)
                //     test_int.fetch_add(1, std::memory_order_acq_rel);
                for (size_t a = 0; a < batch; ++a)
                    // test_lock.gitDecoupleInfo();
                    auto x = test_int.load();
            });
            thread_pool.addTask(std::move(task));
        }
    }
    watch.stop();
    std::cout << "Current i: " << test_int.load() << std::endl;
    // watch.startWatch();
    // std::shared_mutex shared_lock;
    // for (size_t i = 0; i < nums; ++i)
    // {
    //     // std::shared_lock<std::shared_mutex> lock(shared_lock);
    //     auto a = 1;
    // }
    // watch.stop();

    return 0;
}