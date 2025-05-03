#ifndef FEGN_WEBSOCKET_HPP
#define FEGN_WEBSOCKET_HPP

#include <string>
#include <memory>
#include <map>
#include <functional>
#include <future>

namespace Fegn {
    // WebSocket bağlantı yöneticisi
    class WebSocketManager {
    private:
        std::string url;
        bool isConnected;
        
        // Olay yöneticileri
        std::function<void(const std::string&)> onMessage;
        std::function<void(const std::string&)> onError;
        std::function<void()> onClose;
        
    public:
        WebSocketManager(const std::string& url)
            : url(url), isConnected(false) {}
        
        // Bağlantı yönetimi
        void connect() {
            // WebSocket bağlantısı kur
            isConnected = true;
        }
        
        void disconnect() {
            // Bağlantıyı kes
            isConnected = false;
            if (onClose) {
                onClose();
            }
        }
        
        // Olay yöneticileri
        void onMessage(const std::function<void(const std::string&)>& callback) {
            onMessage = callback;
        }
        
        void onError(const std::function<void(const std::string&)>& callback) {
            onError = callback;
        }
        
        void onClose(const std::function<void()>& callback) {
            onClose = callback;
        }
        
        // Mesaj gönderme
        void send(const std::string& message) {
            if (isConnected) {
                // Mesaj gönder
            }
        }
    };
    
    // WebSocket istemcisi
    class WebSocketClient {
    private:
        std::shared_ptr<WebSocketManager> manager;
        
    public:
        WebSocketClient(const std::string& url)
            : manager(std::make_shared<WebSocketManager>(url)) {}
        
        void connect() {
            manager->connect();
        }
        
        void disconnect() {
            manager->disconnect();
        }
        
        void onMessage(const std::function<void(const std::string&)>& callback) {
            manager->onMessage(callback);
        }
        
        void onError(const std::function<void(const std::string&)>& callback) {
            manager->onError(callback);
        }
        
        void onClose(const std::function<void()>& callback) {
            manager->onClose(callback);
        }
        
        void send(const std::string& message) {
            manager->send(message);
        }
    };
}

#endif // FEGN_WEBSOCKET_HPP
