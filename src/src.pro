include(duiconfig.pri)
TEMPLATE = app
TARGET = sysuid
target.path = /usr/bin
CONFIG += dui \
    cellular-qt
LIBS += -lsystem \
    -lX11 \
    -lcallui0
SOURCES += main.cpp \
    notifier.cpp \
    batterybusinesslogic.cpp \
    pincodequeryui.cpp \
    pincodequerybusinesslogic.cpp \
    sysuid.cpp \
    cancellablenotification.cpp \
    lockscreenbusinesslogic.cpp \
    lockscreenui.cpp \
    unlockslider.cpp \
    eventhandler.cpp \
    eventeater.cpp \
    ../batteryapplet/batterygconf.cpp
HEADERS += main.h \
    notifier.h \
    batterybusinesslogic.h \
    pincodequeryui.h \
    pincodequerybusinesslogic.h \
    sysuid.h \
    cancellablenotification.h \
    lockscreenbusinesslogic.h \
    lockscreenui.h \
    unlockslider.h \
    eventhandler.h \
    eventeater.h \
    ../batteryapplet/batterygconf.h
style_sheet.path = /usr/share/themes/dui/pinquery
style_sheet.files = pinquery.css
style_svg.path = /usr/share/themes/dui/pinquery
style_svg.files = *.svg
INSTALLS += target \
    style_sheet \
    style_svg
