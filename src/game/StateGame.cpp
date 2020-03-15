#include "game/StateGame.hpp"

#include <algorithm>

#include "menu/StateMenu.hpp"
#include "app/ResourceManager.hpp"
#include "app/Utils.hpp"

namespace
{
// todo: pick a real values for these
constexpr uint32_t PLAY_AREA_POS_MIN = 100;
constexpr uint32_t PLAY_AREA_POS_MAX = 600;
}

StateGame::StateGame(const Application::Interface& ctx, const std::vector<PlayerInfo>& infos):
    app_{ctx},
    scoreVictoryGoal_{(infos.size()-1)*10},
    border_{0.05 * app_.config.get<int>(Setting::ResHeight), 0.9 * app_.config.get<int>(Setting::ResHeight)}
{
    app_.window.setMouseCursorVisible(false);

    loadGui();
    initializePlayers(infos);

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
    for (const auto& p: pickmeups_) {
        app_.window.draw(p.getShape());
    }

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

void StateGame::initializePlayers(const std::vector<PlayerInfo>& infos) {
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

        players_.emplace_back(info, scoreLabel);

        ++i;
    }
}

void StateGame::loadGui() {
    constexpr auto resName = "game.ui";
    try {
        app_.gui.loadWidgetsFromStream(ResourceManager::openResource(resName));
    }
    catch (const std::invalid_argument&) {
        print::error("Failed to open resource {}", resName);
        exit(-1);
    }
    catch (const tgui::Exception& e) {
        print::error("Failed to create GUI from {}. {}", resName, e.what());
        exit(-1);
    }

    auto goalLabel = app_.getWidget<tgui::Label>("ScoreGoal");
    goalLabel->setText(fmt::format("Goal: {}", scoreVictoryGoal_));
}

void StateGame::sortScoreList() {
    // todo
}

bool StateGame::checkCollisions(PlayerThing& player) {
    for (const auto& shape: border_.getShapes()) {
        if (!player.isGap() && player.checkCollision(shape)) {
            player.kill();
            return true;
        }
    }

    // following code is fairly shoddy, better come up with a better idea for collisions later:
    // the reason we skip the newest bunch of trail segments is that
    // we don't want to check if we collide with segments we just created
    // (we obviously do, they're right underneath and we don't want to die immediately)

    auto toSkip = 15u;
    if (trails_.size() > toSkip) {
        for (const auto& t: trails_) {
            if (t.getShape().getFillColor() == player.getColor() && toSkip > 0) {
                --toSkip;
                continue;
            }
            if (!player.isGap() && player.checkCollision(t.getShape())) {
                player.kill();
                return true;
            }
        }
    }

    auto pickmeup = pickmeups_.begin();
    while (pickmeup != pickmeups_.end()) {
        if (player.checkCollision(pickmeup->getShape())) {
            print::info("{} got a pickmeup", player.name());
            pickmeup->onPickUp(player.name());
            pickmeup = pickmeups_.erase(pickmeup);
        } else {
            ++pickmeup;
        }
    }

    return false;
}

void StateGame::awardPoints() {
    lastAlive_ = std::partition(players_.begin(), players_.end(),
        [] (auto& p) { return !p.isDead(); });

    for (auto player = players_.begin() ; player != lastAlive_ ; ++player) {
        player->addPoint();
    }
    sortScoreList();

    if (lastAlive_ - players_.begin() == 1) {
        if (victoryGoalAchieved()) {
            changeState<GameEnd>();
        } else {
            changeState<RoundEnd>();
        }
    }
}

bool StateGame::victoryGoalAchieved() {
    std::vector<uint32_t> scores;
    std::transform(players_.cbegin(), players_.cend(), std::back_inserter(scores), [] (const auto& p) {
        return p.getScore();
    });
    std::sort(scores.begin(), scores.end());

    return scores.front() >= scoreVictoryGoal_
        && scores.front() - *(scores.begin()+1) >= 2; // first place needs to be at least two points ahead
}

StateGame::PlayerIt StateGame::getPlayer(std::string_view name) {
    return std::find_if(players_.begin(), players_.end(), [name] (const auto& p) {
        return p.name() == name;
    });
}

PickMeUp::OnPickUp StateGame::getRandomPickMeUpEffect() {
    auto type = xor_rand::next(1, static_cast<int>(PickUpType::Count)-1);

    switch(static_cast<PickUpType>(type)) {
    case PickUpType::SelfHaste:
        return makeSelfHaste();
    case PickUpType::OpponentHaste:
        return makeOpponentHaste();
    case PickUpType::SelfSlow:
        return makeSelfSlow();
    case PickUpType::OpponentSlow:
        return makeOpponentSlow();
    default:
        print::error("bad pickup type {}", type);
        throw std::runtime_error{fmt::format("bad pickup type {}", type)};
    }
}

void StateGame::resetPickmeupSpawnTimer() {
    pickmeupSpawnTimer_.restart();
    timeUntilNextPickmeupSpawn_ = sf::milliseconds(xor_rand::next(7500, 11500));
}


void StateGame::RoundBegin::onEnter() {
    gs.trails_.clear();
    gs.pickmeups_.clear();
    for (auto& player : gs.players_) {
        player.newRoundSetup(
            xor_rand::next(PLAY_AREA_POS_MIN, PLAY_AREA_POS_MAX),
            xor_rand::next(PLAY_AREA_POS_MIN, PLAY_AREA_POS_MAX),
            gs.trails_
        );
    }
    gs.lastAlive_ = gs.players_.end();
}

void StateGame::RoundBegin::onSpacebar() {
    gs.changeState<Running>();
}

void StateGame::RoundBegin::onEscape() {
    gs.app_.enterState<StateMenu>();
}


void StateGame::Running::onEnter() {
    gs.moveTimer_.restart();
    gs.resetPickmeupSpawnTimer();
}

void StateGame::Running::onSpacebar() {
    gs.changeState<Pause>();
}

void StateGame::Running::onTick() {
    bool playerDied = false;
    for (auto player = gs.players_.begin() ; player != gs.lastAlive_ ; ++player) {
        player->createTrail(gs.trails_);
        player->move(gs.moveTimer_.getElapsedTime().asMilliseconds() / 1000.f);

        if (gs.checkCollisions(*player)) {
            playerDied = true;
        }
    }

    if (gs.pickmeupSpawnTimer_.getElapsedTime() > gs.timeUntilNextPickmeupSpawn_) {
        gs.resetPickmeupSpawnTimer();
        gs.pickmeups_.emplace_back(
            xor_rand::next(PLAY_AREA_POS_MIN, PLAY_AREA_POS_MAX),
            xor_rand::next(PLAY_AREA_POS_MIN, PLAY_AREA_POS_MAX),
            gs.getRandomPickMeUpEffect()
        );
    }

    if (playerDied) {
        gs.awardPoints();
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


void StateGame::GameEnd::onEnter() {
    // todo: show end of game splash screen
}

void StateGame::GameEnd::onSpacebar() {
    gs.app_.enterState<StateMenu>();
}

void StateGame::GameEnd::onEscape() {
    gs.app_.enterState<StateMenu>();
}
