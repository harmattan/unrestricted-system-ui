include(duiconfig.pri)
TEMPLATE = app
TARGET = sysuid
target.path = /usr/bin
CONFIG += dui \ 
    cellular-qt
LIBS += -lsystem
SOURCES += main.cpp \
    notifier.cpp \
    batterybusinesslogic.cpp \
    pincodequeryui.cpp \
    pincodequerybusinesslogic.cpp \
    sysuid.cpp
HEADERS += main.h \
    notifier.h \
    batterybusinesslogic.h \
    pincodequeryui.h \
    pincodequerybusinesslogic.h \
    sysuid.h
style_sheet.path = /usr/share/themes/dui/pinquery
style_sheet.files = pinquery.css
style_svg.path = /usr/share/themes/dui/pinquery
style_svg.files = *.svg
INSTALLS += target \
    style_sheet \
    style_svg
