include(duiconfig.pri)
TEMPLATE = app
TARGET = sysuid
target.path = /usr/bin
CONFIG += dui \
    cellular-qt \
    qmsystem
QT += dbus
LIBS += -lX11 \
    -lcallui0
INCLUDEPATH += /usr/include/alarmd

contains(cov, true) { 
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}
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
    shutdowndialog.cpp \
    shutdowndialogbusinesslogic.cpp \
    batterybusinesslogicadaptor.cpp \
    displaybusinesslogic.cpp \
    systemuigconf.cpp \
    displaybusinesslogicadaptor.cpp
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
    shutdowndialog.h \
    shutdowndialogbusinesslogic.h \
    batterybusinesslogicadaptor.h \
    displaybusinesslogic.h \
    systemuigconf.h \
    displaybusinesslogicadaptor.h
style_sheet.path = $$(DEBIAN_DESTDIR)/usr/share/themes/dui/pinquery
style_sheet.files = pinquery.css
style_svg.path = $$(DEBIAN_DESTDIR)/usr/share/themes/dui/pinquery
style_svg.files = *.svg
INSTALLS += target \
    style_sheet \
    style_svg
