#include "preview_test/StateViewer.hpp"
#include "EngineTestParams.hpp"

namespace {

constexpr static auto tickrate = 140;
const auto stepTime = sf::milliseconds(1000/tickrate).asMilliseconds() / 1000.f;

constexpr auto playAreaCorner = 50;

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
    TextureType::RandomPickMeUp,
    TextureType::Shrink,
    TextureType::Enlarge,
    TextureType::SelfWarp,
    TextureType::MapWarp,
    TextureType::NoTrails
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

StateViewer::StateViewer(const Application::Interface& a, int testcaseIndex):
    app_{a},
    engine_{
        app_.assets,
        inputs_,
        tickrate,
        playAreaCorner,
        0.9 * app_.config.get<uint32_t>(Setting::ResHeight)
    }
{
    app_.assets.loadTextures(gameTextures);

    setupAndTicks[testcaseIndex].first(engine_, inputs_);
    ticksToRun_ = setupAndTicks[testcaseIndex].second;
}

StateViewer::~StateViewer() {
    app_.assets.releaseTextures(gameTextures);
}

void StateViewer::setupState() {
    engine_.enablePowerups(false);
    engine_.accessState(resetState);

    engine_.createPickMeUp(PickUpType::SelfRightAngle, 200, 900);
    engine_.createPickMeUp(PickUpType::SelfRightAngle, 400, 900);
}

void StateViewer::onFinished() {
    engine_.accessState(printPosition);
}

void StateViewer::input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            app_.quit();
        } else if (event.key.code == sf::Keyboard::Space) {
            started_ = true;
        }
    }
}

void StateViewer::tick(double timeStep) {
    if (!started_) return;
    if (currentTick_ < ticksToRun_) {
        sf::Event event;
        while (inputs_.pollEvent(event)) {
            engine_.input(event);
        }
        engine_.step(timeStep);
        ++currentTick_;
        inputs_.advance();
    }

    if (currentTick_ == ticksToRun_) {
        onFinished();
        ++currentTick_;
    }
}

void StateViewer::render() {
    for (auto d: engine_.getDrawables()) {
        app_.window.draw(*d);
    }
}

