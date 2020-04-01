#include "menu/StateMenu.hpp"

#include <algorithm>
#include <iterator>

#include "app/AssetManager.hpp"
#include "app/Utils.hpp"

#include "menu/LobbyPanel.hpp"
#include "menu/ProfilePanel.hpp"
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

void StateMenu::setActivePanel(PanelType t) {
    print::info("{}: {}", __func__, t);
    (*activePanel_)->deactivate();
    activePanel_ = panels_.begin() + t;
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

    makePanel<LobbyPanel>(PanelType::Lobby);
    makePanel<SettingsPanel>(PanelType::Settings);
    makePanel<ProfilePanel>(PanelType::Profile);

    activePanel_ = panels_.begin();
    setActivePanel(PanelType::Lobby);

    app_.gui.get("Lobby")->connect("pressed", [this] () {
        setActivePanel(PanelType::Lobby);
    });

    app_.gui.get("Settings")->connect("pressed", [this] () {
        setActivePanel(PanelType::Settings);
    });

    app_.gui.get("QuitGame")->connect("pressed", [this] () {
        app_.quit();
    });
}

template <typename T>
void StateMenu::makePanel(PanelType p) {
    auto guiPanel = makeGuiPanel();
    app_.gui.add(guiPanel);
    panels_[p] = std::make_unique<T>(app_, *this, guiPanel);
}

tgui::Panel::Ptr StateMenu::makeGuiPanel() {
    const auto renderer = app_.getWidget<tgui::Panel>("Navigation")->getRenderer();
    auto ret = tgui::Panel::create({"&.height/4*3", "90%"});
    ret->setPosition("(&.width - width) / 2", "0");
    ret->setRenderer(renderer->getData());
    ret->setEnabled(false);
    ret->setVisible(false);
    return ret;
}
