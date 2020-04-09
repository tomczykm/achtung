#include "app/Utils.hpp"

#include <ctime>
#include <cctype>
#include <cmath>

using namespace std::literals;

namespace xor_rand {
namespace {
    // PRNG state
    Type a, b, c, d;
}

void seed(Type seedA, Type seedB, Type seedC, Type seedD) {
    a = seedA;
    b = seedB;
    c = seedC;
    d = seedD;
    discard(5000);
}

// Marsaglia's xorshift
Type next(Type min, Type max) {
    if (min >= max) return max;

    auto t = d;
	auto s = a;

	d = c;
	c = b;
	b = s;

	t ^= t << 11;
	t ^= t >> 8;
	a = t ^ s ^ (s >> 19);

    return min + (a % (max - min + 1));
}

void discard(std::size_t n) {
    for(std::size_t i = 0u; i < n; ++i) next(0, 1);
}

}  // namespace random

std::string_view stripLeadingDigits(std::string_view s) {
    auto it = s.begin();
    while (std::isdigit(*it)) it++;
    s.remove_prefix(it - s.begin());
    return s;
}

std::string_view keycodeToStr(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::Key::Unknown: return ".."sv;
        case sf::Keyboard::Key::A: return "A"sv;
        case sf::Keyboard::Key::B: return "B"sv;
        case sf::Keyboard::Key::C: return "C"sv;
        case sf::Keyboard::Key::D: return "D"sv;
        case sf::Keyboard::Key::E: return "E"sv;
        case sf::Keyboard::Key::F: return "F"sv;
        case sf::Keyboard::Key::G: return "G"sv;
        case sf::Keyboard::Key::H: return "H"sv;
        case sf::Keyboard::Key::I: return "I"sv;
        case sf::Keyboard::Key::J: return "J"sv;
        case sf::Keyboard::Key::K: return "K"sv;
        case sf::Keyboard::Key::L: return "L"sv;
        case sf::Keyboard::Key::M: return "M"sv;
        case sf::Keyboard::Key::N: return "N"sv;
        case sf::Keyboard::Key::O: return "O"sv;
        case sf::Keyboard::Key::P: return "P"sv;
        case sf::Keyboard::Key::Q: return "Q"sv;
        case sf::Keyboard::Key::R: return "R"sv;
        case sf::Keyboard::Key::S: return "S"sv;
        case sf::Keyboard::Key::T: return "T"sv;
        case sf::Keyboard::Key::U: return "U"sv;
        case sf::Keyboard::Key::V: return "V"sv;
        case sf::Keyboard::Key::W: return "W"sv;
        case sf::Keyboard::Key::X: return "X"sv;
        case sf::Keyboard::Key::Y: return "Y"sv;
        case sf::Keyboard::Key::Z: return "Z"sv;
        case sf::Keyboard::Key::Num0: return "0"sv;
        case sf::Keyboard::Key::Num1: return "1"sv;
        case sf::Keyboard::Key::Num2: return "2"sv;
        case sf::Keyboard::Key::Num3: return "3"sv;
        case sf::Keyboard::Key::Num4: return "4"sv;
        case sf::Keyboard::Key::Num5: return "5"sv;
        case sf::Keyboard::Key::Num6: return "6"sv;
        case sf::Keyboard::Key::Num7: return "7"sv;
        case sf::Keyboard::Key::Num8: return "8"sv;
        case sf::Keyboard::Key::Num9: return "9"sv;
        case sf::Keyboard::Key::Escape: return "Esc"sv;
        case sf::Keyboard::Key::LControl: return "LCtrl"sv;
        case sf::Keyboard::Key::LShift: return "LShift"sv;
        case sf::Keyboard::Key::LAlt: return "LAlt"sv;
        case sf::Keyboard::Key::LSystem: return "LSys"sv;
        case sf::Keyboard::Key::RControl: return "RCtrl"sv;
        case sf::Keyboard::Key::RShift: return "RShift"sv;
        case sf::Keyboard::Key::RAlt: return "RAlt"sv;
        case sf::Keyboard::Key::RSystem: return "RSys"sv;
        case sf::Keyboard::Key::Menu: return "Menu"sv;
        case sf::Keyboard::Key::LBracket: return "["sv;
        case sf::Keyboard::Key::RBracket: return "]"sv;
        case sf::Keyboard::Key::Semicolon: return ";"sv;
        case sf::Keyboard::Key::Comma: return ","sv;
        case sf::Keyboard::Key::Period: return "."sv;
        case sf::Keyboard::Key::Quote: return "'"sv;
        case sf::Keyboard::Key::Slash: return "/"sv;
        case sf::Keyboard::Key::Backslash: return "\\"sv;
        case sf::Keyboard::Key::Tilde: return "`"sv;
        case sf::Keyboard::Key::Equal: return "="sv;
        case sf::Keyboard::Key::Hyphen: return "-"sv;
        case sf::Keyboard::Key::Space: return "Space"sv;
        case sf::Keyboard::Key::Enter: return "Enter"sv;
        case sf::Keyboard::Key::Backspace: return "Bksp"sv;
        case sf::Keyboard::Key::Tab: return "Tab"sv;
        case sf::Keyboard::Key::PageUp: return "PgUp"sv;
        case sf::Keyboard::Key::PageDown: return "PgDown"sv;
        case sf::Keyboard::Key::End: return "End"sv;
        case sf::Keyboard::Key::Home: return "Home"sv;
        case sf::Keyboard::Key::Insert: return "Ins"sv;
        case sf::Keyboard::Key::Delete: return "Del"sv;
        case sf::Keyboard::Key::Add: return "+"sv;
        case sf::Keyboard::Key::Subtract: return "-"sv;
        case sf::Keyboard::Key::Multiply: return "*"sv;
        case sf::Keyboard::Key::Divide: return "/"sv;
        case sf::Keyboard::Key::Left: return "Left"sv;
        case sf::Keyboard::Key::Right: return "Right"sv;
        case sf::Keyboard::Key::Up: return "Up"sv;
        case sf::Keyboard::Key::Down: return "Down"sv;
        case sf::Keyboard::Key::Numpad0: return "Num0"sv;
        case sf::Keyboard::Key::Numpad1: return "Num"sv;
        case sf::Keyboard::Key::Numpad2: return "Num2"sv;
        case sf::Keyboard::Key::Numpad3: return "Num3"sv;
        case sf::Keyboard::Key::Numpad4: return "Num4"sv;
        case sf::Keyboard::Key::Numpad5: return "Num5"sv;
        case sf::Keyboard::Key::Numpad6: return "Num6"sv;
        case sf::Keyboard::Key::Numpad7: return "Num7"sv;
        case sf::Keyboard::Key::Numpad8: return "Num8"sv;
        case sf::Keyboard::Key::Numpad9: return "Num9"sv;
        case sf::Keyboard::Key::F1: return "F1"sv;
        case sf::Keyboard::Key::F2: return "F2"sv;
        case sf::Keyboard::Key::F3: return "F3"sv;
        case sf::Keyboard::Key::F4: return "F4"sv;
        case sf::Keyboard::Key::F5: return "F5"sv;
        case sf::Keyboard::Key::F6: return "F6"sv;
        case sf::Keyboard::Key::F7: return "F7"sv;
        case sf::Keyboard::Key::F8: return "F8"sv;
        case sf::Keyboard::Key::F9: return "F9"sv;
        case sf::Keyboard::Key::F10: return "F10"sv;
        case sf::Keyboard::Key::F11: return "F11"sv;
        case sf::Keyboard::Key::F12: return "F12"sv;
        case sf::Keyboard::Key::F13: return "F13"sv;
        case sf::Keyboard::Key::F14: return "F14"sv;
        case sf::Keyboard::Key::F15: return "F15"sv;
        case sf::Keyboard::Key::Pause: return "Pause"sv;
        default: return "unhandled"sv;
    }
}

float distance(sf::Vector2f p1, sf::Vector2f p2) {
    auto xs = (p2.x - p1.x);
    auto ys = (p2.y - p2.y);
    return std::sqrt(xs*xs+ys*ys);
}
