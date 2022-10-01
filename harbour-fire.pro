PREFIX = harbour
NAME = fire

TARGET = $${PREFIX}-$${NAME}
CONFIG += sailfishapp link_pkgconfig
PKGCONFIG += sailfishapp
QT += qml quick
LIBS += -ldl

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-unused-parameter

TARGET_DATA_DIR = /usr/share/$${TARGET}

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
}

OTHER_FILES += \
    LICENSE \
    README.md \
    rpm/*.spec \
    *.desktop \
    qml/*.js \
    qml/*.qml \
    qml/images/*.svg \
    icons/*.svg \
    translations/*.ts

# App

HEADERS += \
    src/FireDebug.h \
    src/FireItem.h

SOURCES += \
    src/FireItem.cpp \
    src/main.cpp

# Icons

ICON_SIZES = 86 108 128 172 256
for(s, ICON_SIZES) {
    icon_target = icon_$${s}
    icon_dir = icons/$${s}x$${s}
    $${icon_target}.files = $${icon_dir}/$${TARGET}.png
    $${icon_target}.path = /usr/share/icons/hicolor/$${s}x$${s}/apps
    INSTALLS += $${icon_target}
}
