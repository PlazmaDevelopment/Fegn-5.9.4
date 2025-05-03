#ifndef FEGN_EVENT_SOURCE_HPP
#define FEGN_EVENT_SOURCE_HPP

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <chrono>

namespace Fegn {
    // Event
    class Event {
    public:
        std::string type;
        std::map<std::string, std::string> data;
        std::chrono::steady_clock::time_point timestamp;
        std::string id;
        
        Event(const std::string& type, const std::map<std::string, std::string>& data)
            : type(type), data(data), timestamp(std::chrono::steady_clock::now()), id(generateId()) {}
        
    private:
        std::string generateId() {
            // ID oluşturma
            return "";
        }
    };
    
    // Event yöneticisi
    class EventSourceManager {
    private:
        std::vector<std::shared_ptr<Event>> events;
        std::map<std::string, std::vector<std::function<void(const Event&)>>> listeners;
        
    public:
        // Event oluşturma
        std::shared_ptr<Event> createEvent(const std::string& type, const std::map<std::string, std::string>& data) {
            return std::make_shared<Event>(type, data);
        }
        
        // Event kaydetme
        void storeEvent(const std::shared_ptr<Event>& event) {
            events.push_back(event);
            
            // Event dinleyicileri
            if (listeners.find(event->type) != listeners.end()) {
                for (auto& listener : listeners[event->type]) {
                    listener(*event);
                }
            }
        }
        
        // Event dinleyici ekleme
        void on(const std::string& type, const std::function<void(const Event&)>& callback) {
            listeners[type].push_back(callback);
        }
        
        // Event akışı
        void replay(const std::function<void(const Event&)>& callback) {
            for (auto& event : events) {
                callback(*event);
            }
        }
    };
    
    // Event istemcisi
    class EventSourceClient {
    private:
        std::shared_ptr<EventSourceManager> manager;
        
    public:
        EventSourceClient(std::shared_ptr<EventSourceManager> manager)
            : manager(manager) {}
        
        std::shared_ptr<Event> createEvent(const std::string& type, const std::map<std::string, std::string>& data) {
            return manager->createEvent(type, data);
        }
        
        void storeEvent(const std::shared_ptr<Event>& event) {
            manager->storeEvent(event);
        }
        
        void on(const std::string& type, const std::function<void(const Event&)>& callback) {
            manager->on(type, callback);
        }
        
        void replay(const std::function<void(const Event&)>& callback) {
            manager->replay(callback);
        }
    };
}

#endif // FEGN_EVENT_SOURCE_HPP
