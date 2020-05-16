#include <QWidget>
#include <QApplication>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QDebug>
#include "Shared.h"

/**
 * @brief Shared::resize
 * @param widget
 * @param w - szerokość w procentach ekranu
 * @param h - wysokość w procentach ekranu
 */
void Shared::resize(QWidget* widget, const int w, const int h, const int screenIndex) {
    if (const auto screen = getScreen(widget, screenIndex); screen) {
        const auto screen_size = screen->size();
        const auto width_precent = static_cast<double>(w) / 100.0;
        const auto height_precent = static_cast<double>(h) / 100.0;
        const auto width = static_cast<int>(screen_size.width() * width_precent);
        const auto height = static_cast<int>(screen_size.height() * height_precent);
        widget->resize(width, height);
    }
}

void Shared::moveToCenter(QWidget* widget, const int screenIndex) {
    if (const auto screen = getScreen(widget, screenIndex); screen) {
        const auto screenSize = screen->size();
        const auto widgetSize = widget->size();
        const int x = (screenSize.width() - widgetSize.width()) >> 1;
        const int y = (screenSize.height() - widgetSize.height()) >> 1;
        widget->move(screen->geometry().x() + x, screen->geometry().y() + y);
    }
}



QScreen* Shared::getScreen(QWidget* widget, const int screenIndex) {
    auto screen = QGuiApplication::screenAt(widget->pos());
    if (screenIndex != -1) {
        if (const auto screens = QGuiApplication::screens(); !screens.isEmpty()) {
            if (screenIndex >= 0 && screenIndex < screens.size()) {
                screen = screens[screenIndex];
            }
        }
    }
    return screen;
}

int Shared::currentScreenIndex(QWidget* widget) {
    return QApplication::desktop()->screenNumber(widget);
}
