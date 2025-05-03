#ifndef FEGN_STREAM_HPP
#define FEGN_STREAM_HPP

#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <future>

namespace Fegn {
    // Stream öğesi
    class StreamItem {
    public:
        std::string data;
        std::chrono::steady_clock::time_point timestamp;
        std::string id;
        
        StreamItem(const std::string& data)
            : data(data), timestamp(std::chrono::steady_clock::now()), id(generateId()) {}
        
    private:
        std::string generateId() {
            // ID oluşturma
            return "";
        }
    };
    
    // Stream yöneticisi
    class StreamManager {
    private:
        std::vector<std::shared_ptr<StreamItem>> items;
        std::map<std::string, std::function<void(const StreamItem&)>> listeners;
        
    public:
        // Öğe ekle
        void push(const std::string& data) {
            auto item = std::make_shared<StreamItem>(data);
            items.push_back(item);
            
            // Dinleyicileri tetikle
            for (auto& listener : listeners) {
                listener.second(*item);
            }
        }
        
        // Dinleyici ekle
        void on(const std::string& name, const std::function<void(const StreamItem&)>& callback) {
            listeners[name] = callback;
        }
        
        // Akış durumu
        bool isEmpty() const {
            return items.empty();
        }
        
        size_t size() const {
            return items.size();
        }
    };
    
    // Stream istemcisi
    class StreamClient {
    private:
        std::shared_ptr<StreamManager> manager;
        
    public:
        StreamClient(std::shared_ptr<StreamManager> manager)
            : manager(manager) {}
        
        void push(const std::string& data) {
            manager->push(data);
        }
        
        void on(const std::string& name, const std::function<void(const StreamItem&)>& callback) {
            manager->on(name, callback);
        }
        
        bool isEmpty() const {
            return manager->isEmpty();
        }
        
        size_t size() const {
            return manager->size();
        }
    };
}

#endif // FEGN_STREAM_HPP
