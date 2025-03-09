PREFIX = meego
NAME = fire
CONFIG += meegotouch qdeclarative-boostable
QT += declarative dbus

TARGET = $${PREFIX}-$${NAME}

DEFINES += HARMATTAN HARMATTAN_BOOSTER

QMAKE_CXXFLAGS *= -Wno-unused-parameter -Wno-psabi

INSTALL_PREFIX = /opt/$${TARGET}

CONFIG(debug, debug|release) {
    DEFINES += DEBUG HARBOUR_DEBUG
}

OTHER_FILES += \
    LICENSE \
    README.md \
    meego/README.md \
    meego/*.png \
    meego/*.desktop \
    meego/qml/*.qml \
    qtc_packaging/debian_harmattan/*

# Directories

HARBOUR_LIB_DIR = $$_PRO_FILE_PWD_/harbour-lib

# harbour-lib

HARBOUR_LIB_INCLUDE = $${HARBOUR_LIB_DIR}/include
HARBOUR_LIB_SRC = $${HARBOUR_LIB_DIR}/src

INCLUDEPATH += \
    $${HARBOUR_LIB_INCLUDE}

HEADERS += \
    $${HARBOUR_LIB_INCLUDE}/HarbourDebug.h \
    $${HARBOUR_LIB_INCLUDE}/HarbourDisplayBlanking.h \
    $${HARBOUR_LIB_INCLUDE}/HarbourSystemState.h \
    $${HARBOUR_LIB_SRC}/HarbourMce.h

SOURCES += \
    $${HARBOUR_LIB_SRC}/HarbourDisplayBlanking.cpp \
    $${HARBOUR_LIB_SRC}/HarbourMce.cpp \
    $${HARBOUR_LIB_SRC}/HarbourSystemState.cpp

# App

HEADERS += \
    src/FireItem.h

SOURCES += \
    src/FireItem.cpp \
    src/main.cpp

INCLUDEPATH += \
  meego/src

HEADERS += \
  meego/src/MeegoFire.h

SOURCES += \
  meego/src/MeegoFire.cpp

# Deployment

qml_meego.files = meego/qml
qml_meego.path = $$INSTALL_PREFIX
INSTALLS += qml_meego

desktopfile.files = meego/$${TARGET}.desktop
desktopfile.path = /usr/share/applications
INSTALLS += desktopfile

icon.files = meego/$${TARGET}.png
icon.path = /usr/share/icons/hicolor/80x80/apps
INSTALLS += icon

splash.files = meego/splash-*.png
splash.path = $$INSTALL_PREFIX
INSTALLS += splash

target.path = $$INSTALL_PREFIX/bin
INSTALLS += target
