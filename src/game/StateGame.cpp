#include "game/StateGame.hpp"

#include <algorithm>
#include <functional>

#include "menu/StateMenu.hpp"
#include "app/AssetManager.hpp"
#include "app/Utils.hpp"

namespace {

constexpr auto playerToGameAreaSizeRatio = 253.334f;
constexpr auto pickMeUpToGameAreaSizeRatio = 32.2f;
constexpr auto playerSpeedToGameAreaSizeRatio = 7.6f;

constexpr auto hasteTurnAngleChange = 9;
const auto selfHasteDuration = sf::milliseconds(3500);
const auto oppHasteDuration = sf::milliseconds(2500);

constexpr auto selfSlowTurnAngleChange = 3;
constexpr auto oppSlowTurnAngleChange = 0;
const auto selfSlowDuration = sf::milliseconds(5000);
const auto oppSlowDuration = sf::milliseconds(4000);

const auto selfRightAngleMovementDuration = sf::milliseconds(8500);
const auto oppRightAngleMovementDuration = sf::milliseconds(5500);

const auto controlSwapDuration = sf::milliseconds(6500);

constexpr auto scoreListEntryHeight = 30u;

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

}  // namespace

StateGame::StateGame(const Application::Interface& ctx, const std::vector<PlayerInfo>& infos):
    app_{ctx},
    playAreaCornerOffset_{0.05 * app_.config.get<int>(Setting::ResHeight)},
    playAreaSideLength_{0.9 * app_.config.get<int>(Setting::ResHeight)},
    pickMeUpRadius_{playAreaSideLength_ / pickMeUpToGameAreaSizeRatio},
    scoreVictoryGoal_{(infos.size()-1)*10},
    border_{playAreaCornerOffset_, playAreaSideLength_}
{
    app_.window.setMouseCursorVisible(false);

    loadGui();
    initializePlayers(infos);

    app_.assets.loadTextures(gameTextures);

    state_ = std::make_unique<RoundBegin>(*this);

    print::info("StateGame ready");
}

StateGame::~StateGame() {
    app_.assets.releaseTextures(gameTextures);
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

        for (auto p = players_.begin(); p != lastAlive_; ++p) {
            p->input(event);
        }
    }
}

void StateGame::tick(double deltaTime) {
    state_->onTick(deltaTime);
}

void StateGame::render() {
    for (const auto& p: pickmeups_) {
        app_.window.draw(p.getSprite());
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
    const auto radius = playAreaSideLength_ / playerToGameAreaSizeRatio;
    const auto velocity = playAreaSideLength_ / playerSpeedToGameAreaSizeRatio;
    auto scoresPanel = app_.getWidget<tgui::Panel>("Scores");
    auto i = 0u;
    for (const auto& info : infos) {
        constexpr auto textSize = 24u;

        auto nameLabel = tgui::Label::create(info.name);
        nameLabel->getRenderer()->setTextColor(info.color);
        nameLabel->setSize("100%", scoreListEntryHeight);
        nameLabel->setPosition(0, i*scoreListEntryHeight);
        nameLabel->setTextSize(textSize);
        scoresPanel->add(nameLabel);

        auto scoreLabel = tgui::Label::create("0");
        scoreLabel->getRenderer()->setTextColor(info.color);
        scoreLabel->setSize("100%", scoreListEntryHeight);
        scoreLabel->setPosition(0, i*scoreListEntryHeight);
        scoreLabel->setTextSize(textSize);
        scoreLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
        scoresPanel->add(scoreLabel);

        players_.emplace_back(info, std::make_pair(nameLabel, scoreLabel), radius, velocity);

        ++i;
    }
}

void StateGame::loadGui() {
    constexpr auto resName = "game.ui";
    try {
        app_.gui.loadWidgetsFromStream(AssetManager::openResource(resName));
    }
    catch (const std::invalid_argument&) {
        const auto msg = fmt::format("Failed to open resource {}", resName);
        print::error(msg);
        throw std::runtime_error{msg};
    }
    catch (const tgui::Exception& e) {
        const auto msg = fmt::format("Failed to create GUI from {}. {}", resName, e.what());
        print::error(msg);
        throw std::runtime_error{msg};
    }

    auto goalLabel = app_.getWidget<tgui::Label>("ScoreGoal");
    goalLabel->setText(fmt::format("Goal: {}\n2 points diff", scoreVictoryGoal_));
}

void StateGame::sortScoreList() {
    std::multimap<PlayerThing::Score, PlayerThing::Labels> labelsByScore;
    std::transform(players_.cbegin(), players_.cend(), std::inserter(labelsByScore, labelsByScore.begin()),
        [] (const auto& player) {
            return std::make_pair(player.getScore(), player.getLabels());
        });

    auto i = 0u;
    for (auto it = labelsByScore.rbegin(); it != labelsByScore.rend(); ++it, ++i) {
        it->second.first->setPosition(0, i*scoreListEntryHeight);
        it->second.second->setPosition(0, i*scoreListEntryHeight);
    }
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
    std::sort(scores.begin(), scores.end(), std::greater<uint32_t>{});

    return scores.front() >= scoreVictoryGoal_
        && scores.front() - *(scores.begin()+1) >= 2; // first place needs to be at least two points ahead
}

StateGame::PlayerIt StateGame::getPlayer(std::string_view name) {
    return std::find_if(players_.begin(), players_.end(), [name] (const auto& p) {
        return p.name() == name;
    });
}

void StateGame::createRandomPickMeUp() {
    resetPickmeupSpawnTimer();
    auto [onPickMeUp, texture] = getRandomPickMeUpEffect();
    if (xor_rand::next(1, static_cast<int>(PickUpType::Count)) == 1) texture = AssetManager::Texture::RandomPickMeUp;
    pickmeups_.emplace_back(
        xor_rand::next(playAreaCornerOffset_ + pickMeUpRadius_, playAreaCornerOffset_ + playAreaSideLength_ - pickMeUpRadius_),
        xor_rand::next(playAreaCornerOffset_ + pickMeUpRadius_, playAreaCornerOffset_ + playAreaSideLength_ - pickMeUpRadius_),
        pickMeUpRadius_,
        app_.assets.getTexture(texture),
        onPickMeUp
    );
}

std::pair<PickMeUp::OnPickUp, AssetManager::Texture> StateGame::getRandomPickMeUpEffect() {
    const auto type = xor_rand::next(1, static_cast<int>(PickUpType::Count)-1);

    switch(static_cast<PickUpType>(type)) {
    case PickUpType::SelfHaste:
        return std::make_pair(makeSelfEffect([this] (auto player) {
            const auto velChange = playAreaSideLength_ / playerSpeedToGameAreaSizeRatio;
            addVelocityChange(player, velChange, hasteTurnAngleChange, selfHasteDuration);
        }), AssetManager::Texture::SelfHaste);
    case PickUpType::OpponentHaste:
        return std::make_pair(makeOpponentEffect([this] (auto player) {
            const auto velChange = playAreaSideLength_ / playerSpeedToGameAreaSizeRatio;
            addVelocityChange(player, velChange, hasteTurnAngleChange, oppHasteDuration);
        }), AssetManager::Texture::OpponentHaste);
    case PickUpType::SelfSlow:
        return std::make_pair(makeSelfEffect([this] (auto player) {
            const auto velChange = -(player->getVelocity() / 2);
            addVelocityChange(player, velChange, selfSlowTurnAngleChange, selfSlowDuration);
        }), AssetManager::Texture::SelfSlow);
    case PickUpType::OpponentSlow:
        return std::make_pair(makeOpponentEffect([this] (auto player) {
            const auto velChange = -(player->getVelocity() / 2);
            addVelocityChange(player, velChange, oppSlowTurnAngleChange, oppSlowDuration);
        }), AssetManager::Texture::OpponentSlow);
    case PickUpType::ClearTrails:
        return std::make_pair(makeSelfEffect([this] (auto) {
            trails_.clear();
        }), AssetManager::Texture::ClearTrails);
    case PickUpType::SelfRightAngle:
        return std::make_pair(makeSelfEffect([this] (auto player) {
            addRightAngleMovement(player, selfRightAngleMovementDuration);
        }), AssetManager::Texture::SelfRightAngle);
    case PickUpType::OpponentRightAngle:
        return std::make_pair(makeOpponentEffect([this] (auto player) {
            addRightAngleMovement(player, oppRightAngleMovementDuration);
        }), AssetManager::Texture::OpponentRightAngle);
    case PickUpType::ControlSwap:
        return std::make_pair(makeOpponentEffect([this] (auto player) {
            addControlSwap(player, controlSwapDuration);
        }), AssetManager::Texture::ControlSwap);
    case PickUpType::MassPowerups:
        return std::make_pair(makeSelfEffect([this] (auto) {
            addMassPowerups();
        }), AssetManager::Texture::MassPowerups);
    default:
        const auto msg = fmt::format("bad pickup type {}", type);
        print::error(msg);
        throw std::runtime_error{msg};
    }
}

template <typename OnPickUp>
PickMeUp::OnPickUp StateGame::makeSelfEffect(OnPickUp onPickUp) {
    return [this, onPickUp] (auto name) {
        onPickUp(getPlayer(name));
    };
}

template <typename OnPickUp>
PickMeUp::OnPickUp StateGame::makeOpponentEffect(OnPickUp onPickUp) {
    return [this, onPickUp] (auto name) {
        auto pickedBy = getPlayer(name);
        for (auto player = players_.begin(); player != players_.end(); ++player) {
            if (player == pickedBy) continue;
            onPickUp(player);
        }
    };
}

void StateGame::addVelocityChange(PlayerIt player, int velChange, int turnAngleChange, sf::Time duration) {
    player->changeVelocity(velChange);
    player->changeTurn(turnAngleChange);
    player->addTimedEffect(duration, [=, name=player->name()] () {
        auto player = getPlayer(name);
        player->changeVelocity(-velChange);
        player->changeTurn(-turnAngleChange);
    });
}

void StateGame::addRightAngleMovement(PlayerIt player, sf::Time duration) {
    player->setRightAngleMovement(true);
    player->addTimedEffect(duration, [this, name=player->name()] {
        auto player = getPlayer(name);
        player->setRightAngleMovement(false);
    });
}

void StateGame::addControlSwap(PlayerIt player, sf::Time duration) {
    player->swapControls();
    player->addTimedEffect(duration, [this, name=player->name()] {
        getPlayer(name)->swapControls();
    });
}

void StateGame::addMassPowerups() {
    if (massPowerups_) {
        massPowerups_->extendBy(sf::milliseconds(4000));
    } else {
        massPowerups_.emplace(sf::milliseconds(6000), [] () {});
    }
}

void StateGame::resetPickmeupSpawnTimer() {
    pickmeupSpawnTimer_.restart();
    timeUntilNextPickmeupSpawn_ =
        massPowerups_ ?
        sf::milliseconds(xor_rand::next(500, 4000)) :
        sf::milliseconds(xor_rand::next(4500, 9000));
}


void StateGame::RoundBegin::onEnterState() {
    gs.trails_.clear();
    gs.pickmeups_.clear();
    for (auto& player : gs.players_) {
        player.newRoundSetup(
            xor_rand::next(gs.playAreaCornerOffset_ + 0.15 * gs.playAreaSideLength_,
                gs.playAreaCornerOffset_ + 0.85 * gs.playAreaSideLength_),
            xor_rand::next(gs.playAreaCornerOffset_ + 0.15 * gs.playAreaSideLength_,
                gs.playAreaCornerOffset_ + 0.85 * gs.playAreaSideLength_),
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


void StateGame::Running::onEnterState() {
    gs.resetPickmeupSpawnTimer();
}

void StateGame::Running::onSpacebar() {
    gs.changeState<Pause>();
}

void StateGame::Running::onTick(double deltaTime) {
    bool playerDied = false;
    for (auto player = gs.players_.begin() ; player != gs.lastAlive_ ; ++player) {
        player->createTrail(gs.trails_);
        player->move(deltaTime);

        if (gs.checkCollisions(*player)) {
            playerDied = true;
        }
    }

    if (gs.pickmeupSpawnTimer_.getElapsedTime() > gs.timeUntilNextPickmeupSpawn_) {
        gs.createRandomPickMeUp();
    }

    if (playerDied) {
        gs.awardPoints();
    }

    if (gs.massPowerups_ && gs.massPowerups_->isExpired()) {
        gs.massPowerups_.reset();
    }
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


void StateGame::GameEnd::onEnterState() {
    // todo: show end of game splash screen
}

void StateGame::GameEnd::onSpacebar() {
    gs.app_.enterState<StateMenu>();
}

void StateGame::GameEnd::onEscape() {
    gs.app_.enterState<StateMenu>();
}
