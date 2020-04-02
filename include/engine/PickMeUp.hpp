#pragma once

#include <functional>

#include <SFML/Graphics.hpp>

#include "app/ProfileManager.hpp"
#include "app/Log.hpp"
#include "engine/Player.hpp"

enum class PickUpType {
    Null = 0,

    SelfHaste,
    OpponentHaste,
    SelfSlow,
    OpponentSlow,
    SelfRightAngle,
    OpponentRightAngle,
    ClearTrails,
    ControlSwap,
    MassPowerups,
    Shrink,
    Enlarge,

    Count
};

class PickMeUp {
public:
    using OnPickUp = std::function<void(ProfileId, PlayerThing&)>;

    PickMeUp(uint32_t xPos, uint32_t yPos, float radius, const sf::Texture&, OnPickUp);

    const sf::CircleShape& getShape() const { return shape_; }
    const sf::Sprite& getSprite() const { return sprite_; }

    void onPickUp(ProfileId id, PlayerThing& player) {
        onPickUp_(id, player);
    }

private:
    sf::CircleShape shape_;
    sf::Sprite sprite_;

    OnPickUp onPickUp_;
};
