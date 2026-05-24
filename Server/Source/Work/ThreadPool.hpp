// ThreadPool.hpp
#pragma once

#include <functional>
#include <mutex>
#include <queue>
#include <vector>
#include <thread>

class thread_pool
{
public:
  thread_pool(size_t NumThreads = std::thread::hardware_concurrency());
  ~thread_pool();

  void enqueue(std::function<void()> Task);

private:
  std::vector<std::thread> Workers;
  std::queue<std::function<void()>> Tasks;
  std::mutex QueueMutex;
  std::condition_variable ConditionVariable;
  bool Stop;
};
