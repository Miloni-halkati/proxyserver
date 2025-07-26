#include "LoadBalancer.h"

LoadBalancer::LoadBalancer(const std::vector<std::string>& servers)
    : backendServers(servers), currentIndex(0) {}

std::string LoadBalancer::getNextServer() {
    std::lock_guard<std::mutex> lock(lbMutex);
    if (backendServers.empty()) return "";
    std::string server = backendServers[currentIndex];
    currentIndex = (currentIndex + 1) % backendServers.size();
    return server;
}
