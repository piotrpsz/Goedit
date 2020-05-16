#ifndef BEEVID_SHARED_H
#define BEEVID_SHARED_H

#include <QString>

class QWidget;
class QScreen;

class Shared {
public:
    Shared() = delete;
    ~Shared() = delete;
    Shared(const Shared&) = delete;
    Shared(const Shared&&) = delete;

    static void resize(QWidget*, const int, const int, const int = -1);
    static void moveToCenter(QWidget*, const int = -1);
    static int currentScreenIndex(QWidget*);
    static QScreen* getScreen(QWidget*, const int);
};

#endif // BEEVID_SHARED_H
