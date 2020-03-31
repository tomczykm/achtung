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

StateGame::StateGame(const Application::Interface& ctx, const PlayerInfos& infos):
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

        for (auto& [id, player]: players_) {
            if (player.isDead()) continue;
            player.input(event);
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

    for (const auto& [id, player]: players_) {
        app_.window.draw(player.getShape());
    }
}

void StateGame::initializePlayers(const PlayerInfos& infos) {
    const auto radius = playAreaSideLength_ / playerToGameAreaSizeRatio;
    const auto velocity = playAreaSideLength_ / playerSpeedToGameAreaSizeRatio;
    auto scoresPanel = app_.getWidget<tgui::Panel>("Scores");
    auto i = 0u;
    for (const auto& [id, info] : infos) {
        auto scoreListEntryPanel = tgui::Panel::create({"100%", scoreListEntryHeight});
        scoreListEntryPanel->loadWidgetsFromStream(AssetManager::openResource("ui/scoresEntry"));
        scoreListEntryPanel->setRenderer(scoresPanel->getSharedRenderer()->getData());

        auto nameLabel = std::static_pointer_cast<tgui::Label>(scoreListEntryPanel->get("Name"));
        nameLabel->getRenderer()->setTextColor(info.color);
        nameLabel->setText(info.name);

        auto scoreLabel = std::static_pointer_cast<tgui::Label>(scoreListEntryPanel->get("Score"));
        scoreLabel->getRenderer()->setTextColor(info.color);

        scoresPanel->add(scoreListEntryPanel);
        players_.emplace(id, PlayerThing{info, scoreListEntryPanel, radius, velocity});

        ++i;
    }
    sortScoreList();
}

void StateGame::loadGui() {
    constexpr auto resName = "ui/game";
    try {
        app_.gui.loadWidgetsFromStream(AssetManager::openResource(resName));
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
    std::multimap<PlayerThing::Score, tgui::Panel::Ptr, std::greater<PlayerThing::Score>> labelsByScore;
    for (const auto& [id, player]: players_) {
        labelsByScore.emplace(player.getScore(), player.getLabels());
    }

    auto i = 0u;
    for (auto& [score, labels]: labelsByScore) {
        labels->setPosition(0, (i++)*scoreListEntryHeight);
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

    auto toSkip = 45u;
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
            pickmeup->onPickUp(player);
            pickmeup = pickmeups_.erase(pickmeup);
        } else {
            ++pickmeup;
        }
    }

    return false;
}

void StateGame::awardPoints() {
    for (auto& [id, player]: players_) {
        if (player.isDead()) continue;
        player.addPoint();
    }
    sortScoreList();

    const auto aliveCount = std::count_if(players_.begin(), players_.end(), [] (auto& kv) {
            return not kv.second.isDead();
        });
    if (aliveCount <= 1) {
        if (victoryGoalAchieved()) {
            changeState<GameEnd>();
        } else {
            changeState<RoundEnd>();
        }
    }
}

bool StateGame::victoryGoalAchieved() {
    std::vector<PlayerThing::Score> scores;
    for (const auto&[id, player]: players_) {
        scores.push_back(player.getScore());
    }
    std::sort(scores.begin(), scores.end(), std::greater<PlayerThing::Score>{});

    return scores.front() >= scoreVictoryGoal_
        && scores.front() - *(scores.begin()+1) >= 2; // first place needs to be at least two points ahead
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
    const auto type = xor_rand::next(5, static_cast<int>(PickUpType::Count)-1);

    switch(static_cast<PickUpType>(type)) {
    case PickUpType::SelfHaste:
        return std::make_pair(makeSelfEffect([this] (auto& player) {
            const auto velChange = playAreaSideLength_ / playerSpeedToGameAreaSizeRatio;
            addVelocityChange(player, velChange, hasteTurnAngleChange, selfHasteDuration);
        }), AssetManager::Texture::SelfHaste);
    case PickUpType::OpponentHaste:
        return std::make_pair(makeOpponentEffect([this] (auto& player) {
            const auto velChange = playAreaSideLength_ / playerSpeedToGameAreaSizeRatio;
            addVelocityChange(player, velChange, hasteTurnAngleChange, oppHasteDuration);
        }), AssetManager::Texture::OpponentHaste);
    case PickUpType::SelfSlow:
        return std::make_pair(makeSelfEffect([this] (auto& player) {
            const auto velChange = -(player.getVelocity() / 2);
            addVelocityChange(player, velChange, selfSlowTurnAngleChange, selfSlowDuration);
        }), AssetManager::Texture::SelfSlow);
    case PickUpType::OpponentSlow:
        return std::make_pair(makeOpponentEffect([this] (auto& player) {
            const auto velChange = -(player.getVelocity() / 2);
            addVelocityChange(player, velChange, oppSlowTurnAngleChange, oppSlowDuration);
        }), AssetManager::Texture::OpponentSlow);
    case PickUpType::ClearTrails:
        return std::make_pair(makeSelfEffect([this] (auto&) {
            trails_.clear();
        }), AssetManager::Texture::ClearTrails);
    case PickUpType::SelfRightAngle:
        return std::make_pair(makeSelfEffect([this] (auto& player) {
            addRightAngleMovement(player, selfRightAngleMovementDuration);
        }), AssetManager::Texture::SelfRightAngle);
    case PickUpType::OpponentRightAngle:
        return std::make_pair(makeOpponentEffect([this] (auto& player) {
            addRightAngleMovement(player, oppRightAngleMovementDuration);
        }), AssetManager::Texture::OpponentRightAngle);
    case PickUpType::ControlSwap:
        return std::make_pair(makeOpponentEffect([this] (auto& player) {
            addControlSwap(player, controlSwapDuration);
        }), AssetManager::Texture::ControlSwap);
    case PickUpType::MassPowerups:
        return std::make_pair(makeSelfEffect([this] (auto&) {
            addMassPowerups();
        }), AssetManager::Texture::MassPowerups);
    default:
        const auto msg = fmt::format("bad pickup type {}", type);
        print::error(msg);
        throw std::runtime_error{msg};
    }
}

template <typename PlayerUnaryOp>
PickMeUp::OnPickUp StateGame::makeSelfEffect(PlayerUnaryOp effect) {
    return [this, effect] (PlayerThing& pickedBy) {
        effect(pickedBy);
    };
}

template <typename PlayerUnaryOp>
PickMeUp::OnPickUp StateGame::makeOpponentEffect(PlayerUnaryOp effect) {
    return [this, effect] (PlayerThing& pickedBy) {
        for (auto& [id, player]: players_) {
            if (player.name() == pickedBy.name()) continue;
            effect(player);
        }
    };
}

void StateGame::addVelocityChange(PlayerThing& player, int velChange, int turnAngleChange, sf::Time duration) {
    player.changeVelocity(velChange);
    player.changeTurn(turnAngleChange);
    player.addTimedEffect(duration, [&] () {
        player.changeVelocity(-velChange);
        player.changeTurn(-turnAngleChange);
    });
}

void StateGame::addRightAngleMovement(PlayerThing& player, sf::Time duration) {
    player.setRightAngleMovement(true);
    player.addTimedEffect(duration, [&] {
        player.setRightAngleMovement(false);
    });
}

void StateGame::addControlSwap(PlayerThing& player, sf::Time duration) {
    player.swapControls();
    player.addTimedEffect(duration, [&] {
        player.swapControls();
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
    for (auto& [id, player] : gs.players_) {
        player.newRoundSetup(
            xor_rand::next(gs.playAreaCornerOffset_ + 0.15 * gs.playAreaSideLength_,
                gs.playAreaCornerOffset_ + 0.85 * gs.playAreaSideLength_),
            xor_rand::next(gs.playAreaCornerOffset_ + 0.15 * gs.playAreaSideLength_,
                gs.playAreaCornerOffset_ + 0.85 * gs.playAreaSideLength_),
            gs.trails_
        );
    }
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
    for (auto& [id, player]: gs.players_) {
        if (player.isDead()) continue;
        player.tick(deltaTime, gs.trails_);

        if (gs.checkCollisions(player)) {
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
