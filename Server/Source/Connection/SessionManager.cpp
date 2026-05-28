#include "SessionManager.hpp"

void session_manager::Start(std::shared_ptr<session> Session)
{
  Sessions.push_back(std::move(Session));
}

void session_manager::Stop(std::shared_ptr<session> Session)
{
}

void session_manager::StopAll()
{
}
