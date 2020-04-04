#include "engine/BorderThing.hpp"

constexpr auto borderThickness = 4;
constexpr auto blackBarThickness = 6;

BorderThing::BorderThing(float offset, float sideLength) {
    // top
    shapes_[0].setPosition({offset, offset});
    shapes_[0].setSize({sideLength, borderThickness});
    blackBars_[0].setPosition({offset, offset-blackBarThickness});
    blackBars_[0].setSize({sideLength, blackBarThickness+borderThickness});

    // bottom
    shapes_[1].setPosition({offset, offset+sideLength-borderThickness});
    shapes_[1].setSize({sideLength, borderThickness});
    blackBars_[1].setPosition({offset, offset+sideLength-borderThickness});
    blackBars_[1].setSize({sideLength, blackBarThickness+borderThickness});

    // left
    shapes_[2].setPosition({offset, offset+borderThickness});
    shapes_[2].setSize({borderThickness, sideLength-2*borderThickness});
    blackBars_[2].setPosition({offset-blackBarThickness, offset});
    blackBars_[2].setSize({blackBarThickness+borderThickness, sideLength});

    // right
    shapes_[3].setPosition({offset+sideLength-borderThickness, offset+borderThickness});
    shapes_[3].setSize({borderThickness, sideLength-2*borderThickness});
    blackBars_[3].setPosition({offset+sideLength-borderThickness, offset});
    blackBars_[3].setSize({blackBarThickness+borderThickness, sideLength});

    std::size_t i = 0;
    for (auto& shape: blackBars_) {
        shape.setFillColor(sf::Color::Black);
        drawables_[i++] = &shape;
    }
    for (auto& shape: shapes_) {
        shape.setFillColor(sf::Color::Yellow);
        drawables_[i++] = &shape;
    }
}

void BorderThing::setAlpha(std::uint8_t alpha) {
    const auto newColor = sf::Color{0xff, 0xff, 0x00, alpha};
    for (auto& s: shapes_) {
        s.setFillColor(newColor);
    }
}
