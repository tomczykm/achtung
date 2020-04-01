#include "testable/InputSequence.hpp"

bool InputSequence::pollEvent(sf::Event& event) const {
    const auto it = eventQueues_.find(currentTick_);
    if (it == eventQueues_.end()) return false;

    auto& q = it->second;
    if (q.size() == 0) return false;

    event = q.front();
    q.pop();
    return true;
}

bool InputSequence::isKeyPressed(sf::Keyboard::Key k) const {
    const auto& keyRanges = keyPresses_.find(k);
    if (keyRanges == keyPresses_.end()) return false;

    for(auto& range: keyRanges->second) {
        if (currentTick_ == std::clamp(currentTick_, range.first, range.second))
            return true;
    }
    return false;
}

void InputSequence::addKeyHold(sf::Keyboard::Key k, Ticks start, Ticks end) {
    keyPresses_[k].push_back(std::make_pair(start, end));
}


void InputSequence::queueEvent(Ticks t, const sf::Event& e) {
    eventQueues_[t].push(e);
}
