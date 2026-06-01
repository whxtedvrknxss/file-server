#pragma once

#include "Session.hpp"

class SessionManager {
 public:
  SessionManager() = default;

  void Start(std::shared_ptr<Session> session);
  void Stop(std::shared_ptr<Session> session);

  void StopAll();

 private:
  std::vector<std::shared_ptr<Session>> sessions_;
};
