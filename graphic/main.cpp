#include <QApplication>
#include <QQmlApplicationEngine>
#include "painter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<Painter>("wm.qml.Controls", 1, 0, "APaintedItem");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
