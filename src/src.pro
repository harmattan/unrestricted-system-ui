include(duiconfig.pri)

TEMPLATE = app
TARGET = pinquery
target.path = /usr/bin
CONFIG += dui \
    cellular-qt
SOURCES += main.cpp \
    notifier.cpp \
    pincodequery.cpp \
    businesslogic.cpp
HEADERS += main.h \
    notifier.h \
    pincodequery.h \
    businesslogic.h

style_sheet.path = /usr/share/themes/dui/pinquery
style_sheet.files = pinquery.css
style_svg.path = /usr/share/themes/dui/pinquery
style_svg.files = *.svg

INSTALLS += target style_sheet style_svg
