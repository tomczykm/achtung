#include "game/StateGame.hpp"

#include <algorithm>

#include "Utils.hpp"

StateGame::StateGame(const Application::Interface& ctx, const std::vector<PlayerInfo>& infos):
    app_{ctx},
    border_{0.05 * app_.settings.getResH(), 0.9 * app_.settings.getResH()}
{
    app_.window.setMouseCursorVisible(false);

    loadGui();
    players_ = initializePlayers(infos);
    lastAlive_ = players_.end();

    print::info("StateGame ready");
}

StateGame::~StateGame() {
    app_.window.setMouseCursorVisible(true);
}

void StateGame::input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch(event.key.code) {
        case sf::Keyboard::Space:
            // space (hehe) for shitty debugging
            break;
        default: break;
        }
    }
}

void StateGame::logic() {
    bool playerDied = false;

    for (auto player = players_.begin() ; player != lastAlive_ ; player++) {
        player->createTrail(trails_);
        player->move(moveTimer_.getElapsedTime().asMilliseconds() / 1000.f);

        for (const auto& shape: border_.getShapes()) {
            if (!player->isGap() && player->checkCollision(shape)) {
                player->kill();
                playerDied = true;
            }
        }

        // this is fairly shoddy, better come up with a better idea on skipping later

        // the reason we skip the newest bunch of trail segments is that
        // we don't want to check if we collide with segments we just created
        // (we obviously do, they're right underneath and we don't want to die immediately)
        constexpr auto SKIP_TRAILS = 15u;  // TODO: fix, hardcoding this value will result in instakill with many players
        if (trails_.size() > SKIP_TRAILS) {
            for (auto t = trails_.begin() + SKIP_TRAILS; t != trails_.end(); t++) {
                if (!player->isGap() && player->checkCollision(t->getShape())) {
                    player->kill();
                    playerDied = true;
                }
            }
        }
    }

    if (playerDied) {
        lastAlive_ = std::partition(players_.begin(), players_.end(),
            [] (auto& p) { return !p.isDead(); });

        for (auto player = players_.begin() ; player != lastAlive_ ; ++player) {
            player->addPoint();
        }
        sortScoreList();
    }

    moveTimer_.restart();
}

void StateGame::render() {
    for (const auto& t: trails_) {
        app_.window.draw(t.getShape());
    }

    for (const auto& shape: border_.getShapes()) {
        app_.window.draw(shape);
    }

    for (const auto& p: players_) {
        app_.window.draw(p.getShape());
    }
}

std::vector<PlayerThing> StateGame::initializePlayers(const std::vector<PlayerInfo>& infos) {
    std::vector<PlayerThing> players;
    auto scoresPanel = app_.getWidget<tgui::Panel>("Scores");
    auto i = 0u;
    for (const auto& info : infos) {
        constexpr auto HEIGHT = 30u;
        constexpr auto TEXT_SIZE = 24u;

        auto nameLabel = tgui::Label::create(info.name);
        nameLabel->getRenderer()->setTextColor(info.color);
        nameLabel->setSize("150%", HEIGHT);
        nameLabel->setPosition(0, i*HEIGHT);
        nameLabel->setTextSize(TEXT_SIZE);
        scoresPanel->add(nameLabel);

        auto scoreLabel = tgui::Label::create("0");
        scoreLabel->getRenderer()->setTextColor(info.color);
        scoreLabel->setSize("150%", HEIGHT);
        scoreLabel->setPosition(0, i*HEIGHT);
        scoreLabel->setTextSize(TEXT_SIZE);
        scoreLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
        scoresPanel->add(scoreLabel);

        players.emplace_back(info, scoreLabel);
        players.back().newRoundSetup(100, 600, 100, 600, trails_);

        ++i;
    }
    return players;
}

void StateGame::loadGui() {
    constexpr auto filename = "../game.ui";
    try {
        app_.gui.loadWidgetsFromFile(filename);
    }
    catch (const tgui::Exception& e) {
        print::error("Failed to load GUI layout from {}. {}", filename, e.what());
        exit(-1);
    }
}

void StateGame::sortScoreList() {
    // todo
}