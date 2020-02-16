#include "Utils.hpp"

#include <ctime>
#include <cctype>

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
    return std::string_view{it, static_cast<size_t>(s.end()-it)};
}

std::string_view keycodeToStr(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::Key::Unknown: return "..";
        case sf::Keyboard::Key::A: return "A";
        case sf::Keyboard::Key::B: return "B";
        case sf::Keyboard::Key::C: return "C";
        case sf::Keyboard::Key::D: return "D";
        case sf::Keyboard::Key::E: return "E";
        case sf::Keyboard::Key::F: return "F";
        case sf::Keyboard::Key::G: return "G";
        case sf::Keyboard::Key::H: return "H";
        case sf::Keyboard::Key::I: return "I";
        case sf::Keyboard::Key::J: return "J";
        case sf::Keyboard::Key::K: return "K";
        case sf::Keyboard::Key::L: return "L";
        case sf::Keyboard::Key::M: return "M";
        case sf::Keyboard::Key::N: return "N";
        case sf::Keyboard::Key::O: return "O";
        case sf::Keyboard::Key::P: return "P";
        case sf::Keyboard::Key::Q: return "Q";
        case sf::Keyboard::Key::R: return "R";
        case sf::Keyboard::Key::S: return "S";
        case sf::Keyboard::Key::T: return "T";
        case sf::Keyboard::Key::U: return "U";
        case sf::Keyboard::Key::V: return "V";
        case sf::Keyboard::Key::W: return "W";
        case sf::Keyboard::Key::X: return "X";
        case sf::Keyboard::Key::Y: return "Y";
        case sf::Keyboard::Key::Z: return "Z";
        case sf::Keyboard::Key::Num0: return "Num0";
        case sf::Keyboard::Key::Num1: return "Num1";
        case sf::Keyboard::Key::Num2: return "Num2";
        case sf::Keyboard::Key::Num3: return "Num3";
        case sf::Keyboard::Key::Num4: return "Num4";
        case sf::Keyboard::Key::Num5: return "Num5";
        case sf::Keyboard::Key::Num6: return "Num6";
        case sf::Keyboard::Key::Num7: return "Num7";
        case sf::Keyboard::Key::Num8: return "Num8";
        case sf::Keyboard::Key::Num9: return "Num9";
        case sf::Keyboard::Key::Escape: return "Esc";
        case sf::Keyboard::Key::LControl: return "LCtrl";
        case sf::Keyboard::Key::LShift: return "LShift";
        case sf::Keyboard::Key::LAlt: return "LAlt";
        case sf::Keyboard::Key::LSystem: return "LSys";
        case sf::Keyboard::Key::RControl: return "RCtrl";
        case sf::Keyboard::Key::RShift: return "RShift";
        case sf::Keyboard::Key::RAlt: return "RAlt";
        case sf::Keyboard::Key::RSystem: return "RSys";
        case sf::Keyboard::Key::Menu: return "Menu";
        case sf::Keyboard::Key::LBracket: return "[";
        case sf::Keyboard::Key::RBracket: return "]";
        case sf::Keyboard::Key::Semicolon: return ";";
        case sf::Keyboard::Key::Comma: return ",";
        case sf::Keyboard::Key::Period: return ".";
        case sf::Keyboard::Key::Quote: return "'";
        case sf::Keyboard::Key::Slash: return "/";
        case sf::Keyboard::Key::Backslash: return "\\";
        case sf::Keyboard::Key::Tilde: return "`";
        case sf::Keyboard::Key::Equal: return "=";
        case sf::Keyboard::Key::Hyphen: return "-";
        case sf::Keyboard::Key::Space: return "Space";
        case sf::Keyboard::Key::Enter: return "Enter";
        case sf::Keyboard::Key::Backspace: return "Bksp";
        case sf::Keyboard::Key::Tab: return "Tab";
        case sf::Keyboard::Key::PageUp: return "PgUp";
        case sf::Keyboard::Key::PageDown: return "PgDown";
        case sf::Keyboard::Key::End: return "End";
        case sf::Keyboard::Key::Home: return "Home";
        case sf::Keyboard::Key::Insert: return "Ins";
        case sf::Keyboard::Key::Delete: return "Del";
        case sf::Keyboard::Key::Add: return "+";
        case sf::Keyboard::Key::Subtract: return "-";
        case sf::Keyboard::Key::Multiply: return "*";
        case sf::Keyboard::Key::Divide: return "/";
        case sf::Keyboard::Key::Left: return "Left";
        case sf::Keyboard::Key::Right: return "Right";
        case sf::Keyboard::Key::Up: return "Up";
        case sf::Keyboard::Key::Down: return "Down";
        case sf::Keyboard::Key::Numpad0: return "Num0";
        case sf::Keyboard::Key::Numpad1: return "Num";
        case sf::Keyboard::Key::Numpad2: return "Num2";
        case sf::Keyboard::Key::Numpad3: return "Num3";
        case sf::Keyboard::Key::Numpad4: return "Num4";
        case sf::Keyboard::Key::Numpad5: return "Num5";
        case sf::Keyboard::Key::Numpad6: return "Num6";
        case sf::Keyboard::Key::Numpad7: return "Num7";
        case sf::Keyboard::Key::Numpad8: return "Num8";
        case sf::Keyboard::Key::Numpad9: return "Num9";
        case sf::Keyboard::Key::F1: return "F1";
        case sf::Keyboard::Key::F2: return "F2";
        case sf::Keyboard::Key::F3: return "F3";
        case sf::Keyboard::Key::F4: return "F4";
        case sf::Keyboard::Key::F5: return "F5";
        case sf::Keyboard::Key::F6: return "F6";
        case sf::Keyboard::Key::F7: return "F7";
        case sf::Keyboard::Key::F8: return "F8";
        case sf::Keyboard::Key::F9: return "F9";
        case sf::Keyboard::Key::F10: return "F10";
        case sf::Keyboard::Key::F11: return "F11";
        case sf::Keyboard::Key::F12: return "F12";
        case sf::Keyboard::Key::F13: return "F13";
        case sf::Keyboard::Key::F14: return "F14";
        case sf::Keyboard::Key::F15: return "F15";
        case sf::Keyboard::Key::Pause: return "Pause";
        default: return "unhandled";
    }
    #undef CASE_ITEM
}