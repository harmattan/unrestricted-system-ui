include(duiconfig.pri)
TEMPLATE = app
TARGET = sysuid
target.path = /usr/bin
CONFIG += dui \
    cellular-qt \
    qmsystem
QT += dbus
LIBS += -lX11 \
    -lcallui0 \
    -lhal
INCLUDEPATH += /usr/include/alarmd \
    /usr/include/dbus-1.0 \
    /usr/lib/dbus-1.0/include
contains(cov, true) { 
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}
SOURCES += main.cpp \
    notifier.cpp \
    #batterybusinesslogic.cpp \
    pincodequeryui.cpp \
    pincodequerybusinesslogic.cpp \
    sysuid.cpp \
    cancellablenotification.cpp \
    lockscreenbusinesslogic.cpp \
    lockscreenui.cpp \
    eventhandler.cpp \
    eventeater.cpp \
    usbhal.cpp \
    shutdownui.cpp \
    shutdownbusinesslogic.cpp \
    #batterybusinesslogicadaptor.cpp \
    displaybusinesslogic.cpp \
    systemuigconf.cpp \
    displaybusinesslogicadaptor.cpp \
    notifierdbusadaptor.cpp \
    phonenetworkbusinesslogic.cpp \
    phonenetworkbusinesslogicadaptor.cpp \
    pincodequerydbusadaptor.cpp \
    lockscreenbusinesslogicadaptor.cpp
HEADERS += main.h \
    notifier.h \
    #batterybusinesslogic.h \
    pincodequeryui.h \
    pincodequerybusinesslogic.h \
    sysuid.h \
    cancellablenotification.h \
    lockscreenbusinesslogic.h \
    lockscreenui.h \
    eventhandler.h \
    eventeater.h \
    usbhal.h \
    shutdownui.h \
    shutdownbusinesslogic.h \
    #batterybusinesslogicadaptor.h \
    displaybusinesslogic.h \
    systemuigconf.h \
    displaybusinesslogicadaptor.h \
    notifierdbusadaptor.h \
    phonenetworkbusinesslogic.h \
    phonenetworkbusinesslogicadaptor.h \
    pincodequerydbusadaptor.h \
    lockscreenbusinesslogicadaptor.h
style_sheet.files += sysuid.css
style_svg.files += *.svg \
    ../data/*.svg
include(unlocksliderwidget/unlocksliderwidget.pri)
style_sheet.path = $$(DEBIAN_DESTDIR)/usr/share/sysuid/themes/style
style_svg.path = $$(DEBIAN_DESTDIR)/usr/share/sysuid/themes/svg
style_images.path = $$(DEBIAN_DESTDIR)/usr/share/sysuid/themes/images
INSTALLS += target \
    style_sheet \
    style_svg \
    style_images
