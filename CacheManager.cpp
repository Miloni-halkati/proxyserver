#include "CacheManager.h"

bool CacheManager::isCached(const std::string& url) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    return cache.find(url) != cache.end();
}

std::string CacheManager::get(const std::string& url) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    return cache[url];
}

void CacheManager::put(const std::string& url, const std::string& content) {
    std::lock_guard<std::mutex> lock(cacheMutex);
    cache[url] = content;
}
