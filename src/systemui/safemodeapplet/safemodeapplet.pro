TEMPLATE      = lib

CONFIG       += plugin meegotouch system-ui

INCLUDEPATH  += /usr/include/meegotouch
INCLUDEPATH  += /usr/include/system-ui
INCLUDEPATH  += ../statusindicatormenu
TARGET        = statusindicatormenu-safemode

OTHER_FILES += \
    statusindicatormenu-safemode.desktop

target.path = /usr/lib/meegotouch/applicationextensions

desktop.files = statusindicatormenu-safemode.desktop
desktop.path = /usr/share/meegotouch/applicationextensions

INSTALLS += target desktop

HEADERS += \
    safemodeplugin.h \
    safemodecontroller.h

SOURCES += \
    safemodeplugin.cpp \
    safemodecontroller.cpp
