#include "game/StateGame.hpp"

#include <algorithm>

#include "menu/StateMenu.hpp"
#include "Utils.hpp"

StateGame::StateGame(const Application::Interface& ctx, const std::vector<PlayerInfo>& infos):
    app_{ctx},
    scoreVictoryGoal_{(infos.size()-1)*10},
    border_{0.05 * app_.settings.getResH(), 0.9 * app_.settings.getResH()}
{
    app_.window.setMouseCursorVisible(false);

    loadGui();
    players_ = initializePlayers(infos);
    lastAlive_ = players_.end();

    state_ = std::make_unique<RoundBegin>(*this);

    print::info("StateGame ready");
}

StateGame::~StateGame() {
    app_.window.setMouseCursorVisible(true);
}

void StateGame::input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch(event.key.code) {
        case sf::Keyboard::Space:
            state_->onSpacebar();
            break;
        case sf::Keyboard::Escape:
            state_->onEscape();
            break;
        default: break;
        }
    }
}

void StateGame::logic() {
    state_->onTick();
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
        nameLabel->setSize("100%", HEIGHT);
        nameLabel->setPosition(0, i*HEIGHT);
        nameLabel->setTextSize(TEXT_SIZE);
        scoresPanel->add(nameLabel);

        auto scoreLabel = tgui::Label::create("0");
        scoreLabel->getRenderer()->setTextColor(info.color);
        scoreLabel->setSize("100%", HEIGHT);
        scoreLabel->setPosition(0, i*HEIGHT);
        scoreLabel->setTextSize(TEXT_SIZE);
        scoreLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
        scoresPanel->add(scoreLabel);

        players.emplace_back(info, scoreLabel);

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

    auto goalLabel = app_.getWidget<tgui::Label>("ScoreGoal");
    goalLabel->setText(fmt::format("Goal: {}", scoreVictoryGoal_));
}

void StateGame::sortScoreList() {
    // todo
}

bool StateGame::victoryGoalAchieved() {
    for (const auto& p : players_) {
        if (p.getScore() == scoreVictoryGoal_) return true;
    }
    return false;
}


StateGame::RoundBegin::RoundBegin(StateGame& s):
    RoundState{s}
{
    print::info(PRINT, __func__);
    gs.trails_.clear();
    for (auto& player : gs.players_) {
        player.newRoundSetup(100, 600, 100, 600, gs.trails_);
    }
    gs.lastAlive_ = gs.players_.end();
}

void StateGame::RoundBegin::onSpacebar() {
    gs.changeState<Running>();
}

void StateGame::RoundBegin::onEscape() {
    gs.app_.enterState<StateMenu>();
}


StateGame::Running::Running(StateGame& s):
    RoundState{s}
{
    print::info(PRINT, __func__);
    gs.moveTimer_.restart();
}

void StateGame::Running::onSpacebar() {
    gs.changeState<Pause>();
}

void StateGame::Running::onTick() {
    bool playerDied = false;

    for (auto player = gs.players_.begin() ; player != gs.lastAlive_ ; player++) {
        player->createTrail(gs.trails_);
        player->move(gs.moveTimer_.getElapsedTime().asMilliseconds() / 1000.f);

        for (const auto& shape: gs.border_.getShapes()) {
            if (!player->isGap() && player->checkCollision(shape)) {
                player->kill();
                playerDied = true;
            }
        }

        // following code fairly shoddy, better come up with a better idea on skipping later:
        // the reason we skip the newest bunch of trail segments is that
        // we don't want to check if we collide with segments we just created
        // (we obviously do, they're right underneath and we don't want to die immediately)

        constexpr auto SKIP_TRAILS = 15u;  // todo: fix, hardcoding this value will result in instakill with several players in the game
        if (gs.trails_.size() > SKIP_TRAILS) {
            for (auto t = gs.trails_.begin() + SKIP_TRAILS; t != gs.trails_.end(); t++) {
                if (!player->isGap() && player->checkCollision(t->getShape())) {
                    player->kill();
                    playerDied = true;
                }
            }
        }
    }

    if (playerDied) {
        gs.lastAlive_ = std::partition(gs.players_.begin(), gs.players_.end(),
            [] (auto& p) { return !p.isDead(); });

        for (auto player = gs.players_.begin() ; player != gs.lastAlive_ ; ++player) {
            player->addPoint();
        }
        gs.sortScoreList();

        if (gs.lastAlive_ - gs.players_.begin() == 1) {
            if (gs.victoryGoalAchieved()) {
                gs.changeState<GameEnd>();
            } else {
                gs.changeState<RoundEnd>();
            }
        }
    }

    gs.moveTimer_.restart();
}


void StateGame::Pause::onSpacebar() {
    gs.changeState<Running>();
}

void StateGame::Pause::onEscape() {
    gs.app_.enterState<StateMenu>();
}


void StateGame::RoundEnd::onSpacebar() {
    gs.changeState<RoundBegin>();
}

void StateGame::RoundEnd::onEscape() {
    gs.app_.enterState<StateMenu>();
}


StateGame::GameEnd::GameEnd(StateGame& s):
    RoundState{s}
{
    print::info(PRINT, __func__);
    // todo: show end of game splash screen
}

void StateGame::GameEnd::onSpacebar() {
    gs.app_.enterState<StateMenu>();
}

void StateGame::GameEnd::onEscape() {
    gs.app_.enterState<StateMenu>();
}