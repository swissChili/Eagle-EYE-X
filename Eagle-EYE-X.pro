QT += quick websockets

CONFIG += c++11 qmltypes

QML_IMPORT_NAME = com.eagleeyex.EagleEYEX
QML_IMPORT_MAJOR_VERSION = 1
QML_IMPORT_MINOR_VERSION = 0

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Aimnet.cpp \
        Overlay.cpp \
        main.cpp

RESOURCES += qml.qrc res/

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Aimnet.h \
    Overlay.h

LIBS += -luser32 -lkernel32
OTHER_FILES += AiMNET/*

DISTFILES += \
    AiMNET/actuator.py
