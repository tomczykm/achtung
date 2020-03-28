#include "menu/LobbyPanel.hpp"

#include "app/Log.hpp"
#include "app/Utils.hpp"
#include "game/StateGame.hpp"
#include "menu/StateMenu.hpp"
#include "menu/ProfilePanel.hpp"

namespace {
constexpr auto PLAYER_LIST_ENTRY_HEIGHT = 60u;
}

LobbyPanel::LobbyPanel(Application::Interface& i, StateMenu& gs, tgui::Panel::Ptr p):
    Panel{i, gs, p}
{
    loadGui();
    loadProfiles();
}

void LobbyPanel::input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (setKeysMode_) {
            setKey(*setKeysMode_, event.key.code);
        }
    }
}

void LobbyPanel::loadGui() {
    constexpr auto resName = "ui/lobby";
    try {
        panel_->loadWidgetsFromStream(AssetManager::openResource(resName));
    }
    catch (const tgui::Exception& e) {
        const auto msg = fmt::format("Failed to create GUI from {}. {}", resName, e.what());
        print::error(msg);
        throw e;
    }

    panel_->get("StartGame")->connect("pressed", [this] () {
        startGame();
    });

    panel_->get("CreateProfile")->connect("pressed", [this] () {
        auto& profilePanel = gs_.setActivePanel<ProfilePanel>(PanelType::Profile);
        profilePanel.loadProfile({});
    });
}

void LobbyPanel::loadProfiles() {
    print::info(__func__);

    for (const auto& info: app_.profiles.profiles()) {
        addProfileEntry(info.first);
    }
    recalculateListPositions();
}

void LobbyPanel::addProfileEntry(ProfileId id) {
    const auto& profile = app_.profiles.profiles()[id];
    auto profileListPanel = app_.getWidget<tgui::Panel>("ProfileListInnerPanel");

    auto newEntryPanel = tgui::Panel::create({"100%", PLAYER_LIST_ENTRY_HEIGHT});
    newEntryPanel->loadWidgetsFromStream(AssetManager::openResource("ui/profileEntry"));
    newEntryPanel->setRenderer(profileListPanel->getSharedRenderer()->getData());

    auto nameLabel = std::static_pointer_cast<tgui::Label>(newEntryPanel->get("Name"));
    nameLabel->getRenderer()->setTextColor(profile.color);
    nameLabel->setText(profile.name);

    auto addButton = newEntryPanel->get("Add");
    addButton->connect("pressed", [=] () {
        addPlayerEntry(id);
        profileListPanel->remove(newEntryPanel);
        recalculateListPositions();
    });

    auto editButton = newEntryPanel->get("Edit");
    editButton->connect("pressed", [=] () {
        auto& profilePanel = gs_.setActivePanel<ProfilePanel>(PanelType::Profile);
        profilePanel.loadProfile(id);
    });

    profileListPanel->add(newEntryPanel);
}

void LobbyPanel::addPlayerEntry(ProfileId id) {
    print::info("{}: id {}", __func__, id);
    const auto& profile = app_.profiles.profiles()[id];
    const auto info = PlayerInfo{profile.name, sf::Keyboard::Unknown, sf::Keyboard::Unknown, profile.color};

    auto playerListPanel = app_.getWidget<tgui::Panel>("PlayerListInnerPanel");

    auto newEntryPanel = tgui::Panel::create({"100%", PLAYER_LIST_ENTRY_HEIGHT});
    newEntryPanel->loadWidgetsFromStream(AssetManager::openResource("ui/playerEntry"));
    newEntryPanel->setRenderer(playerListPanel->getSharedRenderer()->getData());

    auto removeButton = newEntryPanel->get("Remove");
    removeButton->connect("pressed", [=] () {
        if (!setKeysMode_ || *setKeysMode_ != id)
            removePlayerEntry(id, newEntryPanel);
    });

    auto keysLabel = std::static_pointer_cast<tgui::Label>(newEntryPanel->get("Keys"));
    keysLabel->getRenderer()->setTextColor(info.color);
    keysLabel->connect("clicked", [=] () {
        enterSetKeysMode(id);
    });
    keysLabel->setWidgetName(fmt::format("Player{}Keys", id));

    auto nameLabel = std::static_pointer_cast<tgui::Label>(newEntryPanel->get("Name"));
    nameLabel->setText(info.name);
    nameLabel->getRenderer()->setTextColor(info.color);

    playerListPanel->add(newEntryPanel);
    playerInfos_.emplace(id, info);
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

void LobbyPanel::removePlayerEntry(ProfileId id, tgui::Panel::Ptr panel) {
    print::info("Remove player {}", id);
    app_.getWidget<tgui::Panel>("PlayerListInnerPanel")->remove(panel);
    addProfileEntry(id);
    recalculateListPositions();

    playerInfos_.erase(id);
}

void LobbyPanel::enterSetKeysMode(ProfileId player) {
    if (setKeysMode_) return;
    print::info("set keys of {}", player);

    setKeysMode_ = player;

    playerInfos_[player].left = sf::Keyboard::Unknown;
    playerInfos_[player].right = sf::Keyboard::Unknown;
    updateKeysLabel(player);
}

void LobbyPanel::setKey(ProfileId player, sf::Keyboard::Key key) {
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

void LobbyPanel::updateKeysLabel(ProfileId player) {
    app_.getWidget<tgui::Label>(fmt::format("Player{}Keys", player))->setText(fmt::format("{}   {}",
        keycodeToStr(playerInfos_[player].left), keycodeToStr(playerInfos_[player].right)));
}

void LobbyPanel::recalculateListPositions() {
    auto playerPanels = app_.getWidget<tgui::Panel>("PlayerListInnerPanel")->getWidgets();
    std::size_t i = 0;
    for (auto& panel : playerPanels) {
        panel->setPosition({0, PLAYER_LIST_ENTRY_HEIGHT*(i++)});
    }

    auto profilePanels = app_.getWidget<tgui::Panel>("ProfileListInnerPanel")->getWidgets();
    i = 0;
    for (auto& panel : profilePanels) {
        panel->setPosition({0, PLAYER_LIST_ENTRY_HEIGHT*(i++)});
    }
}

void LobbyPanel::startGame() {
    if (canStartGame()) {
        app_.enterState<StateGame>(playerInfos_);
    }
}
