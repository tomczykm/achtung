#include "menu/SettingsPanel.hpp"

using namespace std::literals;

SettingsPanel::SettingsPanel(Application::Interface& i, StateMenu& gs, tgui::Panel::Ptr p):
    Panel{i, gs, p}
{
    loadGui();
}

void SettingsPanel::input(const sf::Event&) {

}

void SettingsPanel::loadGui() {
    constexpr auto resName = "ui/settingsPanel"sv;
    try {
        panel_->loadWidgetsFromStream(AssetManager::openResource(resName));
    }
    catch (const tgui::Exception& e) {
        const auto msg = fmt::format("Failed to create GUI from {}. {}", resName, e.what());
        print::error(msg);
        throw e;
    }
}
