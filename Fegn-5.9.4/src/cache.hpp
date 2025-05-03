#ifndef FEGN_CACHE_HPP
#define FEGN_CACHE_HPP

#include <string>
#include <memory>
#include <map>
#include <chrono>
#include <functional>

namespace Fegn {
    // Cache öğesi
    class CacheItem {
    public:
        std::string value;
        std::chrono::steady_clock::time_point expiresAt;
        std::string id;
        
        CacheItem(const std::string& value, std::chrono::steady_clock::time_point expiresAt)
            : value(value), expiresAt(expiresAt), id(generateId()) {}
        
        bool isExpired() const {
            return std::chrono::steady_clock::now() > expiresAt;
        }
        
    private:
        std::string generateId() {
            // ID oluşturma
            return "";
        }
    };
    
    // Cache yöneticisi
    class CacheManager {
    private:
        std::map<std::string, std::shared_ptr<CacheItem>> items;
        std::map<std::string, std::function<void(const std::string&)>> callbacks;
        
    public:
        // Öğe ekleme
        void set(const std::string& key, const std::string& value, std::chrono::seconds duration) {
            auto expiresAt = std::chrono::steady_clock::now() + duration;
            items[key] = std::make_shared<CacheItem>(value, expiresAt);
            
            // Callback tetikle
            if (callbacks.find(key) != callbacks.end()) {
                callbacks[key](value);
            }
        }
        
        // Öğe alma
        std::string get(const std::string& key) {
            if (items.find(key) != items.end()) {
                auto item = items[key];
                if (!item->isExpired()) {
                    return item->value;
                }
                items.erase(key);
            }
            return "";
        }
        
        // Öğe silme
        void remove(const std::string& key) {
            items.erase(key);
        }
        
        // Tüm önbelleği temizle
        void clear() {
            items.clear();
        }
        
        // Öğe değişikliği callback'i
        void onChange(const std::string& key, const std::function<void(const std::string&)>& callback) {
            callbacks[key] = callback;
        }
    };
    
    // Cache istemcisi
    class CacheClient {
    private:
        std::shared_ptr<CacheManager> manager;
        
    public:
        CacheClient(std::shared_ptr<CacheManager> manager)
            : manager(manager) {}
        
        void set(const std::string& key, const std::string& value, std::chrono::seconds duration) {
            manager->set(key, value, duration);
        }
        
        std::string get(const std::string& key) {
            return manager->get(key);
        }
        
        void remove(const std::string& key) {
            manager->remove(key);
        }
        
        void clear() {
            manager->clear();
        }
        
        void onChange(const std::string& key, const std::function<void(const std::string&)>& callback) {
            manager->onChange(key, callback);
        }
    };
}

#endif // FEGN_CACHE_HPP
