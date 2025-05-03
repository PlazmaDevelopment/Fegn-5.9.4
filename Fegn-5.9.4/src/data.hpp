#ifndef FEGN_DATA_HPP
#define FEGN_DATA_HPP

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <functional>
#include <future>

namespace Fegn {
    // Veri öğesi
    class DataItem {
    public:
        std::map<std::string, std::string> data;
        std::chrono::steady_clock::time_point timestamp;
        std::string id;
        
        DataItem(const std::map<std::string, std::string>& data)
            : data(data), timestamp(std::chrono::steady_clock::now()), id(generateId()) {}
        
    private:
        std::string generateId() {
            // ID oluşturma
            return "";
        }
    };
    
    // Veri yöneticisi
    class DataManager {
    private:
        std::map<std::string, std::vector<std::shared_ptr<DataItem>>> stores;
        std::map<std::string, std::function<void(const DataItem&)>> callbacks;
        std::map<std::string, std::function<void(const std::string&)>> errorCallbacks;
        
    public:
        // Veri ekle
        void add(const std::string& storeName, const std::map<std::string, std::string>& data) {
            auto item = std::make_shared<DataItem>(data);
            stores[storeName].push_back(item);
            
            // Callback tetikle
            if (callbacks.find(storeName) != callbacks.end()) {
                callbacks[storeName](*item);
            }
        }
        
        // Veri al
        std::vector<std::shared_ptr<DataItem>> get(const std::string& storeName, const std::map<std::string, std::string>& filters) {
            std::vector<std::shared_ptr<DataItem>> results;
            
            if (stores.find(storeName) != stores.end()) {
                for (auto& item : stores[storeName]) {
                    bool matches = true;
                    for (auto& filter : filters) {
                        if (item->data[filter.first] != filter.second) {
                            matches = false;
                            break;
                        }
                    }
                    if (matches) {
                        results.push_back(item);
                    }
                }
            }
            
            return results;
        }
        
        // Veri sil
        void remove(const std::string& storeName, const std::map<std::string, std::string>& filters) {
            if (stores.find(storeName) != stores.end()) {
                auto& items = stores[storeName];
                items.erase(
                    std::remove_if(
                        items.begin(),
                        items.end(),
                        [&filters](const std::shared_ptr<DataItem>& item) {
                            for (auto& filter : filters) {
                                if (item->data[filter.first] != filter.second) {
                                    return false;
                                }
                            }
                            return true;
                        }
                    ),
                    items.end()
                );
            }
        }
        
        // Veri akışı
        void pipe(const std::string& storeName, const std::function<void(const DataItem&)>& callback) {
            callbacks[storeName] = callback;
            
            // Mevcut verileri gönder
            if (stores.find(storeName) != stores.end()) {
                for (auto& item : stores[storeName]) {
                    callback(*item);
                }
            }
        }
        
        // Hata yönetimi
        void onError(const std::string& storeName, const std::function<void(const std::string&)>& callback) {
            errorCallbacks[storeName] = callback;
        }
    };
    
    // Veri istemcisi
    class DataClient {
    private:
        std::shared_ptr<DataManager> manager;
        
    public:
        DataClient(std::shared_ptr<DataManager> manager)
            : manager(manager) {}
        
        void add(const std::string& storeName, const std::map<std::string, std::string>& data) {
            manager->add(storeName, data);
        }
        
        std::vector<std::shared_ptr<DataItem>> get(const std::string& storeName, const std::map<std::string, std::string>& filters) {
            return manager->get(storeName, filters);
        }
        
        void remove(const std::string& storeName, const std::map<std::string, std::string>& filters) {
            manager->remove(storeName, filters);
        }
        
        void pipe(const std::string& storeName, const std::function<void(const DataItem&)>& callback) {
            manager->pipe(storeName, callback);
        }
        
        void onError(const std::string& storeName, const std::function<void(const std::string&)>& callback) {
            manager->onError(storeName, callback);
        }
    };
}

#endif // FEGN_DATA_HPP
