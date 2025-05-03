#ifndef FEGN_ASYNC_HPP
#define FEGN_ASYNC_HPP

#include <string>
#include <memory>
#include <future>
#include <functional>
#include <vector>

namespace Fegn {
    // Asenkron fonksiyon
    template<typename T>
    class AsyncFunction {
    private:
        std::future<T> future;
        
    public:
        AsyncFunction(std::future<T> future) : future(future) {}
        
        // Bekle ve değer al
        T get() {
            return future.get();
        }
        
        // Bekleme durumu
        bool isReady() {
            return future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
        }
    };
    
    // Asenkron yöneticisi
    class AsyncManager {
    private:
        std::vector<std::future<void>> tasks;
        
    public:
        // Görev ekle
        template<typename T>
        void addTask(std::function<T()> task) {
            auto future = std::async(std::launch::async, task);
            tasks.push_back(std::move(future));
        }
        
        // Tüm görevleri bekle
        void waitForAll() {
            for (auto& task : tasks) {
                task.wait();
            }
        }
        
        // Görevi bekle ve değer al
        template<typename T>
        T getTaskResult(std::future<T>& future) {
            return future.get();
        }
    };
    
    // Asenkron istemcisi
    class AsyncClient {
    private:
        std::shared_ptr<AsyncManager> manager;
        
    public:
        AsyncClient(std::shared_ptr<AsyncManager> manager)
            : manager(manager) {}
        
        template<typename T>
        void addTask(std::function<T()> task) {
            manager->addTask(task);
        }
        
        void waitForAll() {
            manager->waitForAll();
        }
        
        template<typename T>
        T getTaskResult(std::future<T>& future) {
            return manager->getTaskResult(future);
        }
    };
}

#endif // FEGN_ASYNC_HPP
