#include "menu/Button.hpp"

constexpr unsigned FONT_SIZE = 22;

Button::Button(sf::Vector2f pos, std::string_view caption, const sf::Font& font, OnClick click):
    onClick_(click),
    caption_(caption.data(), font),
    shape_({static_cast<float>(FONT_SIZE*0.66*caption.size()), 1.75*FONT_SIZE})
{
    shape_.setFillColor({11, 25, 54});
    caption_.setCharacterSize(FONT_SIZE);
    caption_.setFillColor({222, 222, 222});

    shape_.setPosition(pos);
    caption_.setPosition(pos);
}


void Button::draw(sf::RenderTarget& t) const {
    t.draw(shape_);
    t.draw(caption_);
}