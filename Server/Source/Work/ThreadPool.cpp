#include "ThreadPool.hpp"

thread_pool::thread_pool(size_t NumThreads)
{
  for (size_t I = 0; I < NumThreads; I++)
  {
    Workers.emplace_back(
        [this]
        {
          while (true)
          {
            std::function<void()> Task;
            {
              std::unique_lock<std::mutex> Lock(QueueMutex);
              ConditionVariable.wait(Lock, [this] { return !Tasks.empty() || Stop; });

              if (Stop && Tasks.empty())
              {
                return;
              }

              Task = std::move(Tasks.front());
              Tasks.pop();
              Task();
            }
          }
        });
  }
}

thread_pool::~thread_pool()
{
  {
    std::unique_lock<std::mutex> Lock(QueueMutex);
    Stop = true;
  }
  ConditionVariable.notify_all();

  for (auto &W : Workers)
  {
    W.join();
  }
}

void thread_pool::enqueue(std::function<void()> Task)
{
  {
    std::unique_lock<std::mutex> Lock(QueueMutex);
    Tasks.emplace(std::move(Task));
  }
  ConditionVariable.notify_one();
}
