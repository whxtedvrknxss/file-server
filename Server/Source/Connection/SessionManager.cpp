#include "SessionManager.hpp"

void SessionManager::Start(std::shared_ptr<Session> session) {
  sessions_.push_back(std::move(session));
}

void SessionManager::Stop(std::shared_ptr<Session> session) {}

void SessionManager::StopAll() {}
