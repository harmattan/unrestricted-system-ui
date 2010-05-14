include(../mconfig.pri)

MOC_DIR = .moc
MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj
QMAKE_RPATH = -Wl

TEMPLATE = app
TARGET = sysuid
target.path = /usr/bin
CONFIG += meegotouch \
          link_pkgconfig

QT += dbus gui

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
    sysuid.h                            \
    lockscreenbusinesslogic.h           \
    lockscreenui.h                      \
    shutdownui.h                        \
    shutdownbusinesslogic.h             \
    batterybusinesslogicadaptor.h       \
    systemuigconf.h                     \
    lockscreenbusinesslogicadaptor.h    \
    usbui.h                             \
    contextframeworkcontext.h           \
    applicationcontext.h                \
    unlockwidgets.h

SOURCES +=                              \
    debug.cpp                           \
    main.cpp                            \
    sysuidrequest.cpp                   \
    batterybusinesslogic.cpp            \
    sysuid.cpp                          \
    lockscreenbusinesslogic.cpp         \
    lockscreenui.cpp                    \
    shutdownui.cpp                      \
    shutdownbusinesslogic.cpp           \
    batterybusinesslogicadaptor.cpp     \
    systemuigconf.cpp                   \
    lockscreenbusinesslogicadaptor.cpp  \
    usbui.cpp                           \
    contextframeworkcontext.cpp         \
    unlockwidgets.cpp

include(statusindicatormenu/statusindicatormenu.pri)
include(statusarea/statusarea.pri)
include(notifications/notifications.pri)

PKGCONFIG += \
    contextsubscriber-1.0 \
    qmsystem \
    maemosec \
    dbus-1 \
    libngf0

dbus_policy.files += systemui.conf
dbus_policy.path = $$(DEBIAN_DESTDIR)/etc/dbus-1/system.d

INSTALLS += target \
            dbus_policy

