#include "menu/ProfilePanel.hpp"

ProfilePanel::ProfilePanel(Application::Interface& i, StateMenu& gs, tgui::Panel::Ptr p):
    Panel{i, gs, p}
{
    loadGui();
}

void ProfilePanel::input(const sf::Event&) {

}

void ProfilePanel::loadGui() {
    constexpr auto resName = "ui/profilePanel";
    try {
        panel_->loadWidgetsFromStream(AssetManager::openResource(resName));
    }
    catch (const tgui::Exception& e) {
        const auto msg = fmt::format("Failed to create GUI from {}. {}", resName, e.what());
        print::error(msg);
        throw e;
    }
}

void ProfilePanel::loadProfile(std::optional<ProfileId> id) {
    auto titleLabel = std::static_pointer_cast<tgui::Label>(panel_->get("Title"));
    if (id) {
        const auto& profile = app_.profiles.profiles()[*id];
        titleLabel->setText(profile.name);
        titleLabel->getRenderer()->setTextColor(profile.color);
    } else {
        titleLabel->setText("New profile");
        titleLabel->getRenderer()->setTextColor(sf::Color::Yellow);
    }
}
