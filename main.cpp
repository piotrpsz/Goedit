#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationName("Beesoft Software");
    QCoreApplication::setOrganizationDomain("beesoft.pl");
    QCoreApplication::setApplicationName("Goedit");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
