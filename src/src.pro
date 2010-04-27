include(../mconfig.pri)

MOC_DIR = .moc
MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

TEMPLATE = app
TARGET = sysuid
target.path = /usr/bin
CONFIG += meegotouch \
          qmsystem \
          link_pkgconfig

QT += dbus

SYSTEMUI_SOURCE_DIR = .
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$M_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

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
    usbbusinesslogic.h                  \
    usbbusinesslogicadaptor.h           \
    usbui.h                             \
    contextframeworkcontext.h           \
    applicationcontext.h                \
    unlockwidgets.h

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
    usbbusinesslogic.cpp                \
    usbbusinesslogicadaptor.cpp         \
    usbui.cpp                           \
    contextframeworkcontext.cpp         \
    unlockwidgets.cpp

include(statusindicatormenu/statusindicatormenu.pri)
include(statusarea/statusarea.pri)
include(notifications/notifications.pri)

PKGCONFIG += \
    contextsubscriber-1.0 \
    maemosec \
    dbus-1 \
    usb_moded \
    libngf0

dbus_policy.files += systemui.conf
dbus_policy.path = $$(DEBIAN_DESTDIR)/etc/dbus-1/system.d

INSTALLS += target \
            dbus_policy

