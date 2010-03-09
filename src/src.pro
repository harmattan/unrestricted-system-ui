TEMPLATE = app
TARGET = sysuid
target.path = /usr/bin
CONFIG += dui \
          qmsystem \
          silent \
          link_pkgconfig

QT += dbus

SYSTEMUI_SOURCE_DIR = .
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$DUI_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

contains(cov, true) {
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}

HEADERS +=                              \
    debug.h                             \
    sysuidrequest.h                     \
    batterybusinesslogic.h              \
    ledbusinesslogic.h                  \
    sysuid.h                            \
    lockscreenbusinesslogic.h           \
    lockscreenui.h                      \
    shutdownui.h                        \
    shutdownbusinesslogic.h             \
    batterybusinesslogicadaptor.h       \
    ledbusinesslogicadaptor.h           \
    systemuigconf.h                     \
    lockscreenbusinesslogicadaptor.h    \
    usbmodes.h                          \
    usbbusinesslogic.h                  \
    usbbusinesslogicadaptor.h           \
    usbui.h \
    x11wrapper.h \
    contextframeworkcontext.h \
    applicationcontext.h

SOURCES +=                              \
    debug.cpp                           \
    main.cpp                            \
    sysuidrequest.cpp                   \
    batterybusinesslogic.cpp            \
    ledbusinesslogic.cpp                \
    sysuid.cpp                          \
    lockscreenbusinesslogic.cpp         \
    lockscreenui.cpp                    \
    shutdownui.cpp                      \
    shutdownbusinesslogic.cpp           \
    batterybusinesslogicadaptor.cpp     \
    ledbusinesslogicadaptor.cpp         \
    systemuigconf.cpp                   \
    lockscreenbusinesslogicadaptor.cpp  \
    usbmodes.cpp                        \
    usbbusinesslogic.cpp                \
    usbbusinesslogicadaptor.cpp         \
    usbui.cpp \
    x11wrapper.cpp \
    contextframeworkcontext.cpp

include(unlocksliderwidget/unlocksliderwidget.pri)
include(statusindicatormenu/statusindicatormenu.pri)
include(statusarea/statusarea.pri)
include(notifications/notifications.pri)

PKGCONFIG += contextsubscriber-1.0 \
                          xcomposite \
                          maemosec


# TODO: remove these when ke-recv going to be integrated:
usb_scripts.files += pcsuite-enable.sh
usb_scripts.path = $$(DEBIAN_DESTDIR)/usr/bin

usb_sudoers.files += usb.sudoers
usb_sudoers.path = $$(DEBIAN_DESTDIR)/etc/sudoers.d

INSTALLS += target \
            usb_scripts \
            usb_sudoers

