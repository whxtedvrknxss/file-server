// ThreadPool.hpp
#pragma once

#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  ThreadPool(size_t num_threads = std::thread::hardware_concurrency());
  ~ThreadPool();

  [[nodiscard]] std::size_t Size() const { return workers_.size(); }
  void Enqueue(std::function<void()> Task);

 private:
  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;
  std::mutex queue_mutex_;
  std::condition_variable cv_;
  bool stop_;
};
