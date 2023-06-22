#include <iostream>
#include <thread>
#include <vector>

int main()
{

    std::vector<thread> threads;

    cout << "最大スレッド数: " << thread::hardware_concurrency() << endl;

    for (int i = 0; i < 100; ++i)
    {
        threads.emplace_back(
            // 何かしらの処理をスレッドに投げる
            function);
    }

    /// スレッドの同期待ち
    for (auto &t : threads)
    {
        t.join();
    }

    return 0;
}