#include "game/StateSandbox.hpp"

#include "menu/StateMenu.hpp"

namespace {

constexpr static auto tickrate = 140;
const auto stepTime = sf::milliseconds(1000/tickrate).asMilliseconds() / 1000.f;

constexpr Ticks ticksToRun = 700;

IAssetManager::TextureSet gameTextures = {
        TextureType::SelfHaste,
        TextureType::OpponentHaste,
        TextureType::SelfSlow,
        TextureType::OpponentSlow,
        TextureType::SelfRightAngle,
        TextureType::OpponentRightAngle,
        TextureType::ClearTrails,
        TextureType::ControlSwap,
        TextureType::MassPowerups,
        TextureType::RandomPickMeUp
};

auto resetState = [] (PlayerTestable& player, Engine::Pickmeups& pickups, Engine::Trails& t) {
    player.setDead(false);
    player.setPosition(100, 900);
    player.setDirection(270);
    t.clear();
    pickups.clear();
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

    setupState();
}

StateSandbox::~StateSandbox() {
    print::info("destroy {}", __func__);
    app_.assets.releaseTextures(gameTextures);
}

void StateSandbox::setupState() {
    engine_.enablePowerups(false);
    engine_.accessState(resetState);

    engine_.createPickMeUp(PickUpType::SelfHaste, 150, 900);
    // engine_.createPickMeUp(PickUpType::SelfHaste, 550, 900);
}

void StateSandbox::onFinished() {
    engine_.accessState(printPosition);
}

void StateSandbox::input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            app_.enterState<StateMenu>();
        } else if (event.key.code == sf::Keyboard::Space) {
            started_ = true;
        }
    }
}

void StateSandbox::tick(double timeStep) {
    if (!started_) return;
    if (currentTick_ < ticksToRun) {
        engine_.step(timeStep);
        ++currentTick_;
    }

    if (currentTick_ == ticksToRun) {
        onFinished();
        ++currentTick_;
    }
}

void StateSandbox::render() {
    for (auto d: engine_.getDrawables()) {
        app_.window.draw(*d);
    }
}

