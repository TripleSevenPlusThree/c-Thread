#ifndef _threadPool_
#define _threadPool_

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <vector>

class threadPool
{

public:
  threadPool() {}
  threadPool(int numThreads);
  ~threadPool();

  template <class F, class... Args>   // class...Args 表示任意个参数，用于函数中
  void enqueue(F &&f, Args &&...args) // && 双引用符既可以用于左值引用，也能是右值引用，即可以传递临时变量，// 如 func（10），而左值引用时必须传递一个变量 如，int a=10；func（a）
  {                                   // 类中类模版成员函数的实现必须与类在同一个文件中，因为模版函数是在调用时编译的，不是提前编译，会导致未定义问题
    std::function<void()> task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    /*
    bind：将一个可调用对象（这里为f）与一个或多个
    参数绑定在一起，形成一个新的可调用参数而不改变
    原有的函数类型。简单理解为形成一个可传递任意个
    参数的函数.
    forward:将参数转发到另一个函数，保持参数类型不变
    因为bind将参数绑定后，所有参数都具有数据类型，如果
    传递一个临时变量（右值），则会改变其数据类型，
    会与开发者的意愿相违背
    */
    {
      std::unique_lock<std::mutex> lock(mtx);
      tasks.emplace(std::move(task)); // move()函数直接将task的资源移动到任务队列中进行构造，避免了复制，提高了效率
    }
    condition.notify_one();
  }

private:
  threadPool(const threadPool &) = delete;
  threadPool &operator=(threadPool &) = delete;

  std::vector<std::thread> threads;
  std::queue<std::function<void()>> tasks; // function<void()> 表示一种数据类型（函数的数据类型），这里void（）函数类型，即无参，无返回值的函数类型
  std::mutex mtx;
  std::condition_variable condition;

  bool stop;
};

#endif