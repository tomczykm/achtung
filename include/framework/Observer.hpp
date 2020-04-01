#pragma once

#include <memory>
#include <vector>
#include <functional>

namespace framework {

struct IEvent {
    virtual ~IEvent() = default;
};

using Observer = std::function<void(const IEvent&)>;

class Observable {
public:
    void attach(Observer o) {
        observers_.push_back(o);
    }

protected:
    void notifyAll(const IEvent& event) {
        for (const auto& o: observers_) {
            o(event);
        }
    }

    std::vector<Observer> observers_;
};

}  // namespace framework
