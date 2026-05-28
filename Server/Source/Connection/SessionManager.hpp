#pragma once

#include "Session.hpp"

class session_manager
{
public:
  session_manager() = default;

  void Start(std::shared_ptr<session> Session);
  void Stop(std::shared_ptr<session> Session);

  void StopAll();

private:
  std::vector<std::shared_ptr<session>> Sessions;
};
