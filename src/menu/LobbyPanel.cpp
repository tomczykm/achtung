#include "menu/LobbyPanel.hpp"

#include "app/Log.hpp"
#include "app/Utils.hpp"
#include "game/StateGame.hpp"

namespace {
constexpr auto PLAYER_LIST_ENTRY_HEIGHT = 60u;
}

LobbyPanel::LobbyPanel(Application::Interface& i, tgui::Panel::Ptr p):
    Panel{i, p}
{
    loadGui();
}

void LobbyPanel::loadGui() {
    constexpr auto resName = "lobby.ui";
    try {
        panel_->loadWidgetsFromStream(AssetManager::openResource(resName));
    }
    catch (const std::invalid_argument&) {
        const auto msg = fmt::format("Failed to open resource {}", resName);
        print::error(msg);
        throw std::runtime_error{msg};
    }
    catch (const tgui::Exception& e) {
        const auto msg = fmt::format("Failed to create GUI from {}. {}", resName, e.what());
        print::error(msg);
        throw e;
    }

    panel_->get("AddPlayer")->connect("pressed", [this] () {
        addPlayer();
    });

    panel_->get("StartGame")->connect("pressed", [this] () {
        startGame();
    });
}

void LobbyPanel::input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (setKeysMode_) {
            setKey(*setKeysMode_, event.key.code);
        }
    }
}

void LobbyPanel::addPlayer() {
    print::info(__func__);
    auto playerListPanel = app_.getWidget<tgui::Panel>("PlayerListInnerPanel");

    auto newEntryPanel = tgui::Panel::create({"100%", "60"});
    newEntryPanel->setRenderer(playerListPanel->getSharedRenderer()->getData());
    newEntryPanel->setPosition("0", std::to_string(getCurrentNumPlayers()*PLAYER_LIST_ENTRY_HEIGHT));

    const auto playerId = PlayerId{playerInfos_.size() == 0 ? 0 : playerInfos_.rbegin()->first + 1};

    const auto widgetNamePrefix = fmt::format("Player{}", playerId);

    const auto color = sf::Color{xor_rand::next(0,255), xor_rand::next(0,255), xor_rand::next(0,255)};
    const auto info = PlayerInfo{"", sf::Keyboard::Unknown, sf::Keyboard::Unknown, color};

    auto removeButton = tgui::Button::create("Rem");
    removeButton->setPosition("100% - 10 - width", "15%");
    removeButton->setSize("30", "70%");
    removeButton->connect("pressed", [=] () {
        if (!setKeysMode_ || (setKeysMode_ && *setKeysMode_ != playerId))
            removePlayer(playerId, newEntryPanel);
    });
    newEntryPanel->add(removeButton, fmt::format("{}Remove", widgetNamePrefix));

    auto keysLabel = tgui::Label::create("_   _");
    keysLabel->setPosition(widgetNamePrefix + "Remove.left - 5 - width", "50%-height/2");
    keysLabel->setTextSize(17);
    keysLabel->setSize(60, "100%");  // todo: hardcoded width bad, should be automatic
    keysLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    keysLabel->getRenderer()->setTextColor(info.color);
    keysLabel->connect("clicked", [=] () {
        enterSetKeysMode(playerId);
    });
    newEntryPanel->add(keysLabel, fmt::format("{}Keys", widgetNamePrefix));

    auto nameEdit = tgui::EditBox::create();
    nameEdit->setTextSize(17);
    nameEdit->setSize(widgetNamePrefix + "Keys.left - 20", "80%");
    nameEdit->setText(info.name);
    nameEdit->getRenderer()->setDefaultTextColor(info.color);
    nameEdit->setPosition("10", "50%-height/2");
    nameEdit->getRenderer()->setTextColor(info.color);
    nameEdit->connect("TextChanged", [=] () {
        playerInfos_[playerId].name = nameEdit->getText();
    });
    newEntryPanel->add(nameEdit, fmt::format("{}Label", widgetNamePrefix));

    playerListPanel->add(newEntryPanel, fmt::format("{}Panel", widgetNamePrefix));
    playerInfos_.emplace(playerId, info);
}

std::vector<PlayerInfo> LobbyPanel::preparePlayerInfos() {
    std::vector<PlayerInfo> infos(playerInfos_.size());
    std::transform(playerInfos_.cbegin(), playerInfos_.cend(), infos.begin(), [] (const auto& kvPair) {
        return kvPair.second;
    });
    return infos;
}

bool LobbyPanel::canStartGame() {
    auto playersHaveSetValues = std::all_of(playerInfos_.cbegin(), playerInfos_.cend(), [] (const auto& pair) {
        const auto& p = pair.second;
        return p.left != sf::Keyboard::Unknown && p.right != sf::Keyboard::Unknown && p.name != "";
    });

    std::set<std::string> names;
    std::transform(playerInfos_.begin(), playerInfos_.end(), std::inserter(names, names.begin()), [] (auto& i) {
        return i.second.name;
    });
    auto playersHaveUniqueNames = names.size() == playerInfos_.size();

    return playerInfos_.size() > 1 && playersHaveSetValues && playersHaveUniqueNames;
}

std::size_t LobbyPanel::getCurrentNumPlayers() {
    return std::static_pointer_cast<tgui::Panel>(app_.gui.get("PlayerListInnerPanel"))->
        getWidgets().size();
}

void LobbyPanel::removePlayer(PlayerId player, tgui::Panel::Ptr panel) {
    print::info("Remove player {}", player);
    app_.getWidget<tgui::Panel>("PlayerListInnerPanel")->remove(panel);
    recalculatePlayerListPositions();

    playerInfos_.erase(player);
}

void LobbyPanel::enterSetKeysMode(PlayerId player) {
    if (setKeysMode_) return;
    print::info("set keys of {}", player);

    setKeysMode_ = player;

    playerInfos_[player].left = sf::Keyboard::Unknown;
    playerInfos_[player].right = sf::Keyboard::Unknown;
    updateKeysLabel(player);
}

void LobbyPanel::setKey(PlayerId player, sf::Keyboard::Key key) {
    static std::set<sf::Keyboard::Key> forbidden = {
        sf::Keyboard::Enter, sf::Keyboard::Escape, sf::Keyboard::Space
    };
    if (forbidden.find(key) != forbidden.end()) return;

    if (playerInfos_[player].left ==  sf::Keyboard::Unknown) {
        playerInfos_[player].left = key;
    } else {
        playerInfos_[player].right = key;
        setKeysMode_ = std::nullopt;
    }
    updateKeysLabel(player);
}

void LobbyPanel::updateKeysLabel(PlayerId player) {
    app_.getWidget<tgui::Label>(fmt::format("Player{}Keys", player))->setText(fmt::format("{}   {}",
        keycodeToStr(playerInfos_[player].left), keycodeToStr(playerInfos_[player].right)));
}

void LobbyPanel::recalculatePlayerListPositions() {
    auto playerListPanel = std::static_pointer_cast<tgui::Panel>(app_.gui.get("PlayerListInnerPanel"));
    auto playerPanels = playerListPanel->getWidgets();

    std::size_t i = 0;
    for (auto& panel : playerPanels) {
        panel->setPosition({0, PLAYER_LIST_ENTRY_HEIGHT*(i++)});
    }
}

void LobbyPanel::startGame() {
    if (canStartGame()) {
        app_.enterState<StateGame>(preparePlayerInfos());
    }
}
