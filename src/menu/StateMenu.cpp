#include "menu/StateMenu.hpp"
#include "game/StateGame.hpp"
#include "game/StateSandbox.hpp"

namespace
{

constexpr auto PLAYER_LIST_ENTRY_HEIGHT = 60u;

}

StateMenu::StateMenu(const Application::Interface& app):
    app_{app}
{
    loadGui();
    print::info("StateMenu ready");
}

void StateMenu::input(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            app_.quit();
        }
    }
}

void StateMenu::addPlayer() {
    print::info(__func__);
    auto playerListPanel = std::static_pointer_cast<tgui::Panel>(app_.gui.get("PlayerListInnerPanel"));

    auto newEntryPanel = tgui::Panel::create({"100%", "60"});
    newEntryPanel->setRenderer(playerListPanel->getSharedRenderer()->getData());
    newEntryPanel->setPosition("0", std::to_string(getCurrentNumPlayers()*PLAYER_LIST_ENTRY_HEIGHT));

    auto widgetNamePrefix = "Player" + std::to_string(latestPlayerId_++);

    auto nameLabel = tgui::Label::create(widgetNamePrefix + "Name");
    nameLabel->setTextSize(20);
    nameLabel->setPosition("10", "50%-height/2");
    nameLabel->getRenderer()->setTextColor("red");
    nameLabel->getRenderer()->setBackgroundColor("transparent");
    newEntryPanel->add(nameLabel, widgetNamePrefix + "Label");

    auto removeButton = tgui::Button::create("Rem");
    removeButton->setPosition("100% - 10 - width", "15%");
    removeButton->setSize("30", "70%");
    removeButton->connect("pressed", [=] () {
        playerListPanel->remove(newEntryPanel);
        recalculatePlayerListPositions();
    });
    newEntryPanel->add(removeButton, widgetNamePrefix + "Remove");

    auto rkeyButton = tgui::Button::create("R");
    rkeyButton->setPosition(widgetNamePrefix + "Remove.left - 6% - width", "15%");
    rkeyButton->setSize("30", "70%");
    newEntryPanel->add(rkeyButton, widgetNamePrefix + "Right");

    auto lkeyButton = tgui::Button::create("L");
    lkeyButton->setPosition(widgetNamePrefix + "Right.left - 5 - width", "15%");
    lkeyButton->setSize("30", "70%");
    newEntryPanel->add(lkeyButton, widgetNamePrefix + "Left");

    playerListPanel->add(newEntryPanel, widgetNamePrefix + "Panel");
}

std::vector<PlayerInfo> StateMenu::preparePlayerInfos() {
    auto playerListPanel = std::static_pointer_cast<tgui::Panel>(app_.gui.get("PlayerListInnerPanel"));
    auto playerPanels = playerListPanel->getWidgets();

    print::info("Preparing infos for {} players", playerPanels.size());

    return {{"player", sf::Keyboard::Q, sf::Keyboard::W, sf::Color::Red}};
}

void StateMenu::loadGui() {
    constexpr auto filename = "../menu.gui";
    try {
        app_.gui.loadWidgetsFromFile(filename);
    }
    catch (const tgui::Exception& e) {
        print::error("Failed to load GUI layout from {}. {}", filename, e.what());
        exit(-1);
    }

    app_.gui.get("AddPlayer")->connect("pressed", [this] () {
        addPlayer();
    });

    app_.gui.get("StartGame")->connect("pressed", [this] () {
        app_.enterState<StateGame>(preparePlayerInfos());
    });

    app_.gui.get("QuitGame")->connect("pressed", [this] () {
        app_.quit();
    });
}

std::size_t StateMenu::getCurrentNumPlayers() {
    return std::static_pointer_cast<tgui::Panel>(app_.gui.get("PlayerListInnerPanel"))->
        getWidgets().size();
}

void StateMenu::recalculatePlayerListPositions() {
    auto playerListPanel = std::static_pointer_cast<tgui::Panel>(app_.gui.get("PlayerListInnerPanel"));
    auto playerPanels = playerListPanel->getWidgets();

    std::size_t i = 0;
    for (auto& panel : playerPanels) {
        panel->setPosition({0, PLAYER_LIST_ENTRY_HEIGHT*(i++)});
    }
}