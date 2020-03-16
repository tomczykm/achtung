#pragma once

#include <string>
#include <string_view>
#include <deque>

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "game/TrailThing.hpp"
#include "game/Effect.hpp"

struct PlayerInfo {
    std::string name;
    sf::Keyboard::Key left, right;
    sf::Color color;
};

class PlayerThing {
public:
    using Score = uint32_t;
    using Labels = std::pair<tgui::Label::Ptr, tgui::Label::Ptr>;

    PlayerThing(const PlayerInfo&, Labels, float radius, int vel);

    void move(double timeStep);
    void input(const sf::Event&);

    void newRoundSetup(uint32_t xPos, uint32_t yPos, std::deque<TrailThing>&);
    void createTrail(std::deque<TrailThing>&) const;

    const sf::Shape& getShape() const;
    sf::Color getColor() const { return info_.color; }
    int getVelocity() const { return vel_; }

    bool checkCollision(const sf::Shape&) const;

    void addPoint();
    Score getScore() const { return score_; }
    Labels getLabels() const { return {nameLabel_, scoreLabel_}; }

    void kill();
    bool isDead() const { return dead_; }
    bool isGap() const { return gap_; }

    const std::string& name() const { return info_.name; }

    void changeVelocity(int d) { vel_ += d; }
    void changeTurn(int d) { turnDegrees_ += d; }
    void setRightAngleMovement(bool v);

    template <typename... Ts>
    void addTimedEffect(Ts&&... args) {
        effects_.emplace_back(std::forward<Ts>(args)...);
    }

private:
    void gapSwitch();
    void endExpiredEffects();

    void setPosition(float x, float y);

    PlayerInfo info_;

    sf::CircleShape shape_;
    sf::RectangleShape recShape_; // for right angle movement

    double direction_ = 0.0; // in degrees
    int vel_; // pixels per seconds
    double turnDegrees_ = 130;
    bool rightAngleMovement_ = false;

    Score score_ = 0;

    tgui::Label::Ptr nameLabel_;
    tgui::Label::Ptr scoreLabel_;

    bool dead_ = false;

    // gap logic
    bool gap_ = false;
    sf::Clock gapSwitchTimer_;
    sf::Time gapSwitchDuration_;

    std::vector<TimedEffect> effects_;
};
