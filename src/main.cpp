#include "threadPool.h"

int main()
{
  static threadPool test(4);
  for (int i = 0; i < 15; i++)
  {
    auto task = ([i]()
                 {
      std::cout<<"task :"<<i<<" is running"<<std::endl;
     // std::this_thread::sleep_for(std::chrono::seconds(1));
      std::cout<<"task :"<<i<<" is done"<<std::endl; });
    test.enqueue(task);
  }
  system("pause");
  return 0;
}