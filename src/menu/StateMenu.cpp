#include "menu/StateMenu.hpp"

#include <algorithm>
#include <iterator>

#include "app/AssetManager.hpp"
#include "app/Utils.hpp"

#include "menu/LobbyPanel.hpp"
#include "menu/SettingsPanel.hpp"

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
    (*activePanel_)->input(event);
}

void StateMenu::setActivePanel(int num) {
    (*activePanel_)->deactivate();
    activePanel_ = panels_.begin() + num;
    (*activePanel_)->activate();
}

void StateMenu::loadGui() {
    constexpr auto resName = "ui/menu";
    try {
        app_.gui.loadWidgetsFromStream(AssetManager::openResource(resName));
    }
    catch (const tgui::Exception& e) {
        const auto msg = fmt::format("Failed to create GUI from {}. {}", resName, e.what());
        print::error(msg);
        throw e;
    }

    const auto panelRenderer = app_.getWidget<tgui::Panel>("Navigation")->getRenderer();

    auto guiPanel = makePanel(panelRenderer);
    app_.gui.add(guiPanel);
    panels_[0] = std::make_unique<LobbyPanel>(app_, guiPanel);

    guiPanel = makePanel(panelRenderer);
    app_.gui.add(guiPanel);
    panels_[1] = std::make_unique<SettingsPanel>(app_, guiPanel);

    activePanel_ = panels_.begin();
    setActivePanel(0);

    app_.gui.get("Lobby")->connect("pressed", [this] () {
        setActivePanel(0);
    });

    app_.gui.get("Settings")->connect("pressed", [this] () {
        setActivePanel(1);
    });

    app_.gui.get("QuitGame")->connect("pressed", [this] () {
        app_.quit();
    });
}

tgui::Panel::Ptr StateMenu::makePanel(tgui::PanelRenderer* renderer) {
    auto ret = tgui::Panel::create({"&.height/4*3", "90%"});
    ret->setPosition("(&.width - width) / 2", "0");
    ret->setRenderer(renderer->getData());
    ret->setEnabled(false);
    ret->setVisible(false);
    return ret;
}
