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

# need to install theme file when ready
# pkg-config CellularQt --libs
LIBS += -lcellular-qt \
    -lQtDBus \
    -lQtXml \
    -lQtCore

# pkg-config CellularQt --cflags
QMAKE_CXXFLAGS += -DQT_SHARED \
    -I/usr/include/cellular-qt \
    -I/usr/include/qt4 \
    -I/usr/include/qt4/QtCore \
    -I/usr/include/qt4/QtDBus \
    -I/usr/include/qt4/QtXml

style_sheet.path = /usr/share/themes/dui/pinquery
style_sheet.files = pinquery.css
style_svg.path = /usr/share/themes/dui/pinquery
style_svg.files = *.svg

INSTALLS += target style_sheet style_svg
