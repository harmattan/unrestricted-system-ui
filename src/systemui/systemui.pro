include(../coverage.pri)
include(../../mconfig.pri)

MOC_DIR = .moc
M_MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj
QMAKE_LFLAGS_RPATH = -Wl
QMAKE_LFLAGS = -Wl,--as-needed

TEMPLATE = app
TARGET = sysuid
target.path = /usr/bin
CONFIG += meegotouch \
          link_pkgconfig

QT += dbus gui meegographicssystemhelper

INCLUDEPATH += ../libnotificationsystem
QMAKE_LIBDIR += ../../lib
LIBS += -lnotificationsystem

SYSTEMUI_SOURCE_DIR = .
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$M_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

contains(cov, true) {
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}

STYLE_HEADERS += shutdownwindowstyle.h

HEADERS +=                              \
    closeeventeater.h                   \
    batterybusinesslogic.h              \
    sysuid.h                            \
    shutdownui.h                        \
    shutdownwindowstyle.h               \
    shutdownbusinesslogic.h             \
    usbui.h                             \
    contextframeworkcontext.h           \
    applicationcontext.h                \
    x11wrapper.h                        \
    xeventlistener.h                    \
    diskspacenotifier.h

SOURCES +=                              \
    main.cpp                            \
    closeeventeater.cpp                 \
    batterybusinesslogic.cpp            \
    sysuid.cpp                          \
    shutdownui.cpp                      \
    shutdownbusinesslogic.cpp           \
    usbui.cpp                           \
    contextframeworkcontext.cpp         \
    x11wrapper.cpp                      \
    xeventlistener.cpp                  \
    diskspacenotifier.cpp

include(statusindicatormenu/statusindicatormenu.pri)
include(statusarea/statusarea.pri)
include(notifications/notifications.pri)
include(screenlock/screenlock.pri)
include(../../localconfig.pri)

contains(DEFINES, HAVE_CONTEXTSUBSCRIBER) {
    PKGCONFIG += contextsubscriber-1.0
}

contains(DEFINES, HAVE_QMSYSTEM) {
    PKGCONFIG += qmsystem2
}

contains(DEFINES, HAVE_AEGIS_CRYPTO) {
    PKGCONFIG += aegis-crypto
}

contains(DEFINES, HAVE_LIBNGF) {
    PKGCONFIG += libngf0
}

PKGCONFIG += \
    dbus-1 \
    dbus-glib-1 \
    xcomposite

dbus_policy.files += systemui.conf
dbus_policy.path = $$(DEBIAN_DESTDIR)/etc/dbus-1/system.d

headers.path = /usr/include/system-ui
headers.files += \
    volumeextensioninterface.h \
    VolumeExtensionInterface

INSTALLS += target \
            dbus_policy \
            headers
