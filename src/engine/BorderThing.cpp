#include "engine/BorderThing.hpp"

constexpr int THICKNESS = 4;

BorderThing::BorderThing(float offset, float sideLength) {
    // top
    shapes_[0].setPosition({offset, offset});
    shapes_[0].setSize({sideLength, THICKNESS});

    // bottom
    shapes_[1].setPosition({offset, offset+sideLength-THICKNESS});
    shapes_[1].setSize({sideLength, THICKNESS});

    // left
    shapes_[2].setPosition({offset, offset});
    shapes_[2].setSize({THICKNESS, sideLength});

    // right
    shapes_[3].setPosition({offset+sideLength-THICKNESS, offset});
    shapes_[3].setSize({THICKNESS, sideLength});

    for (auto& shape: shapes_) {
        shape.setFillColor(sf::Color::Yellow);
    }
}

void BorderThing::setAlpha(std::uint8_t alpha) {
    const auto newColor = sf::Color{0xff, 0xff, 0x00, alpha};
    for (auto& s: shapes_) {
        s.setFillColor(newColor);
    }
}
