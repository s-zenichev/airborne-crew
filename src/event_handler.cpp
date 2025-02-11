#include "event_handler.h"


namespace event_handler {

// Для статического поля обязательна предварительная инициализация
utils::log_handler::LogHandler* EventHandler::logger_ = nullptr;
sf::Texture* EventHandler::plane_texture_ = nullptr;

// Метод, отвечающий за кнопку Debug -> Show FPS
void EventHandler::showFPS(gui_wrapper::FrameRateLabel& fps, const std::vector<tgui::String>& menuItem) {
    if (menuItem.size() == 2 && menuItem[0] == "Debug" && menuItem[1] == "Show FPS") {
        // Пример логгирования
        // Слэши перед кавычками (\") нужны для правильного экранирования кавычек, т.к. это служебный символ
        logger_->LogTrivial(boost::log::trivial::severity_level::debug, "\"Show FPS\" button has been pressed");

        fps.ShowLabel();
    }
}

void EventHandler::showCoordinates(gui_wrapper::CoordsLabel& coords_label, const std::vector<tgui::String>& menuItem) {
    if (menuItem.size() == 2 && menuItem[0] == "Debug" && menuItem[1] == "Show coordinates") {
        logger_->LogTrivial(boost::log::trivial::severity_level::debug, "\"Show coordinates\" button has been pressed");

        coords_label.ShowLabel();
    }
}


// Метод, отвечающий за кнопку Info -> About
void EventHandler::showInfo(tgui::Gui& gui, const std::vector<tgui::String>& menuItem) {
    if (menuItem.size() == 2 && menuItem[0] == "Info" && menuItem[1] == "About") {
        logger_->LogTrivial(boost::log::trivial::severity_level::info, "\"About\" button has been pressed");

        auto messageBox = tgui::MessageBox::create();
        messageBox->setPosition("(&.size - size) / 2");
        messageBox->setText("This program was developed by airborne-crew team");
        messageBox->addButton("OK");
        messageBox->setButtonAlignment(tgui::MessageBox::Alignment::Center);
        messageBox->onButtonPress([msgBox = messageBox.get()] (const tgui::String& button) {
            msgBox->getParent()->remove(msgBox->shared_from_this());
        });

        gui.add(messageBox);
    }
}

// Метод, отвечающий за кнопку Program -> Start
void EventHandler::startProgram(objects::Plane& plane, const std::vector<tgui::String>& menuItem) {
    if (menuItem.size() == 2 && menuItem[0] == "Program" && menuItem[1] == "Start") {
        logger_->LogTrivial(boost::log::trivial::severity_level::info, "The plane object has been loaded");

        plane_texture_ = new sf::Texture;
        plane_texture_->loadFromFile("../meta/plane.png");

        sf::Sprite plane_sprite;
        plane_sprite.setTexture(*plane_texture_);
        sf::Vector2u texture_size = plane_texture_ ->getSize();
        sf::Vector2f plane_scale = plane_sprite.getScale();
        plane_sprite.setScale({ objects::PLANE_SIZE.x / texture_size.x, objects::PLANE_SIZE.y / texture_size.y });
        plane_sprite.setOrigin(texture_size.x/2,texture_size.y/2);
        plane_sprite.setPosition({ global_parameters::PLANE_INITIAL_POS_X, global_parameters::PLANE_INITIAL_POS_Y });
        plane.SetPrimitive(plane_sprite);
        plane.SetToDraw(true);
        plane.SetTargetPosition({ global_parameters::PLANE_INITIAL_POS_X, global_parameters::PLANE_INITIAL_POS_Y });
        plane.SetAngle(0);
    }
}

// Метод, отвечающий за кнопку Program -> Finish
void EventHandler::finishProgram(objects::Plane& plane, const std::vector<tgui::String>& menuItem) {
    if (menuItem.size() == 2 && menuItem[0] == "Program" && menuItem[1] == "Finish") {
        logger_->LogTrivial(boost::log::trivial::severity_level::info, "The plane object has been deleted");

        plane.SetToDraw(false);
        delete plane_texture_;
    }
}

// Метод, отвечающий за передвижение самолета
void EventHandler::movePlane(objects::Plane& plane, const sf::Vector2f& mousePosition) {
    if (plane.GetToDraw()) {
        logger_->LogTrivial(boost::log::trivial::severity_level::info, "Plane terminal point has been set to " + std::to_string(mousePosition.x) + ", " + std::to_string(mousePosition.y));

        plane.SetTargetPosition(mousePosition);
    }
}

void EventHandler::changeSliderValue(gui_wrapper::TextLabel& slider_label, objects::Plane& plane, bool change_linear, float value) {
    slider_label.SetLabelText(objects::Plane::FloatToStringWithPrecision(value));

    if (change_linear) {
        logger_->LogTrivial(boost::log::trivial::severity_level::info, "Plane linear speed has been set to " + std::to_string(value));
        plane.SetLinearSpeed(value);
        if (plane.GetSpeed() > 10*plane.GetAngleSpeed())gui_wrapper::InterfaceBuilder::GetAngleSlider().setSliderValue(plane.GetSpeed()/10.f);
    }
    else {
        logger_->LogTrivial(boost::log::trivial::severity_level::info, "Plane angle speed has been set to " + std::to_string(value));
        plane.SetAngleSpeed(value);
    }
}


// Системный метод для передачи логгера в EventHandler
void EventHandler::SetLogger(utils::log_handler::LogHandler* logger) {
    logger_ = logger;
}


EventHandler::~EventHandler() {
    delete plane_texture_;
}

} // namespace event_handler