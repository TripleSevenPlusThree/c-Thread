#include "threadPool.h"

threadPool::threadPool(int numThreads) : stop(false) // 表示在函数体执行之前就将stop进行初始化
{
  for (int i = 0; i < numThreads; i++)
  {
    threads.emplace_back([this] // 表示将调用构造函数的对象捕获
                         {
      while(true){
        std::function<void()>task;
        {std::unique_lock<std::mutex>lock(mtx);
        condition.wait(lock,[this]
        {
          return !tasks.empty()||stop;
        });
        //如果线程池终止则不再执行任务，直接退出子线程  
        if(stop&&tasks.empty()){
          return;
        }
        task=std::move(tasks.front());
        tasks.pop();
        }
        task();
      } });
  }
}

threadPool ::~threadPool() // 确保线程池安全地关闭
{
  {
    std::unique_lock<std::mutex> lock(mtx);
    stop = true; // 线程池的终止标志，
  }
  condition.notify_all(); // 通知所用等待的子线程，检查stop条件，并退出线程
  for (auto &t : threads) // 基于范围的for循环，类似python中的for循环，
  {
    t.join(); // 等待子线程全部退出，主线程再关闭
  }
}
