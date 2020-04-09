#include "menu/ProfilePanel.hpp"

#include "menu/StateMenu.hpp"

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

    auto saveButton = panel_->get("Save");
    saveButton->connect("pressed", [=] () {
        saveProfile();
    });

    auto deleteButton = panel_->get("Delete");
    deleteButton->connect("pressed", [=] () {
        deleteProfile();
    });

    auto backButton = panel_->get("Back");
    backButton->connect("pressed", [=] () {
        gs_.setActivePanel(PanelType::Lobby);
    });
}

void ProfilePanel::saveProfile() {
    const auto& name = static_cast<tgui::EditBox*>(panel_->get("NameEdit").get())->getText();
    const auto& color = decodeColor(static_cast<tgui::EditBox*>(panel_->get("ColorEdit").get())->getText());

    app_.profiles.updateProfile(loadedProfile_, {name, color});
    print::info("saved profile {}, \"{}\"", *loadedProfile_, name.toAnsiString());

    auto titleLabel = static_cast<tgui::Label*>(panel_->get("Title").get());
    titleLabel->setText(name);
    titleLabel->getRenderer()->setTextColor(color);

    auto deleteButton = panel_->get("Delete");
    deleteButton->setEnabled(true);
    deleteButton->setVisible(true);
}

void ProfilePanel::deleteProfile() {
    // todo: ask player for confimation
    // seriously, DO NOT add tracking stats before doing that
    // profiles are going to get accidentaly deleted
    app_.profiles.deleteProfile(*loadedProfile_);
    gs_.setActivePanel(PanelType::Lobby);

}

void ProfilePanel::loadProfile(std::optional<ProfileId> id) {
    auto titleLabel = static_cast<tgui::Label*>(panel_->get("Title").get());
    if (id) {
        const auto& profile = app_.profiles[*id];
        titleLabel->setText(profile.name);
        titleLabel->getRenderer()->setTextColor(profile.color);

        static_cast<tgui::EditBox*>(panel_->get("NameEdit").get())->setText(profile.name);
        static_cast<tgui::EditBox*>(panel_->get("ColorEdit").get())->setText(encodeColor(profile.color));

        auto deleteButton = panel_->get("Delete");
        deleteButton->setEnabled(true);
        deleteButton->setVisible(true);
    } else {
        titleLabel->setText("New profile");
        titleLabel->getRenderer()->setTextColor(sf::Color::Yellow);

        static_cast<tgui::EditBox*>(panel_->get("NameEdit").get())->setText("");
        static_cast<tgui::EditBox*>(panel_->get("ColorEdit").get())->setText("");

        auto deleteButton = panel_->get("Delete");
        deleteButton->setEnabled(false);
        deleteButton->setVisible(false);
    }
    loadedProfile_ = id;
}

// example: converts "ffffff" to sf::Color::White
sf::Color ProfilePanel::decodeColor(const std::string& str) {
    return {
        static_cast<uint8_t>(std::stoi(str.substr(0, 2), 0, 16)),
        static_cast<uint8_t>(std::stoi(str.substr(2, 2), 0, 16)),
        static_cast<uint8_t>(std::stoi(str.substr(4, 2), 0, 16))
    };
}

std::string ProfilePanel::encodeColor(sf::Color c) {
    return fmt::format("{:02x}{:02x}{:02x}", c.r, c.g, c.b);
}
