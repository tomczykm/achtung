#pragma once

#include <functional>
#include <string_view>
#include <SFML/Graphics.hpp>

class Button {
public:
    using OnClick = std::function<void()>;

    Button(sf::Vector2f pos, std::string_view caption, const sf::Font&, OnClick);

    sf::FloatRect getClickableArea() const { return shape_.getGlobalBounds(); }
    void draw(sf::RenderTarget&) const;
    void action() const { onClick_(); }

private:
    OnClick onClick_;

    sf::Text caption_;
    sf::RectangleShape shape_;
};