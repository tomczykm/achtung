#include "game/StateSandbox.hpp"

#include "menu/StateMenu.hpp"

namespace {

constexpr static auto tickrate = 140;
const auto stepTime = sf::milliseconds(1000/tickrate).asMilliseconds() / 1000.f;

AssetManager::TextureSet gameTextures = {
        AssetManager::Texture::SelfHaste,
        AssetManager::Texture::OpponentHaste,
        AssetManager::Texture::SelfSlow,
        AssetManager::Texture::OpponentSlow,
        AssetManager::Texture::SelfRightAngle,
        AssetManager::Texture::OpponentRightAngle,
        AssetManager::Texture::ClearTrails,
        AssetManager::Texture::ControlSwap,
        AssetManager::Texture::MassPowerups,
        AssetManager::Texture::RandomPickMeUp
};

auto resetState = [] (PlayerTestable& player, Engine::Pickmeups&, Engine::Trails& t) {
    player.newRoundSetup(100, 900, t);
    player.setDirection(270);
    t.clear();
    for (int y = 850; y < 950; y+=2)
        t.emplace_back(150, y, 0, player.getRadius(), sf::Color::Red);
};

auto printPosition = [] (PlayerTestable& player, Engine::Pickmeups&, Engine::Trails&) {
    auto pos = player.getPosition();
    print::info("Position: {}x{}, dead={}", pos.x, pos.y, player.isDead());
};

}

StateSandbox::StateSandbox(Application::Interface& a):
    app_{a},
    engine_{
        app_.assets,
        tickrate,
        0,
        0.9 * app_.config.get<int>(Setting::ResHeight)
    }
{
    app_.assets.loadTextures(gameTextures);
    engine_.enablePowerups(false);
    engine_.createPickMeUp(PickUpType::ClearTrails, 100, 100);

    for (auto i = 0u; i < 10; ++i) {
        print::info("Run {}", i+1);
        engine_.accessState(resetState);
        for (auto tick = 0u; tick < 140; ++tick) {
            engine_.step(stepTime);
        }
        engine_.accessState(printPosition);
    }
}

StateSandbox::~StateSandbox() {
    print::info("destroy {}", __func__);
    app_.assets.releaseTextures(gameTextures);
}

void StateSandbox::input(const sf::Event& event) {
    if (event.key.code == sf::Keyboard::Escape) {
        app_.enterState<StateMenu>();
    }
}

void StateSandbox::tick(double) {

}

void StateSandbox::render() {
    for (auto d: engine_.getDrawables()) {
        app_.window.draw(*d);
    }
}

