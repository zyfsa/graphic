TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    painter.cpp

RESOURCES += qml.qrc

QML_IMPORT_PATH =

include(deployment.pri)

OTHER_FILES += \
    ColorPicker.qml

HEADERS += \
    painter.h
