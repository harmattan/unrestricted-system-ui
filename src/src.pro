include(../mconfig.pri)

MOC_DIR = .moc
MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj
QMAKE_LFLAGS_RPATH = -Wl

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
    unlocknotifications.h               \
    unlocknotificationsink.h            \
    unlockmissedevents.h                \
    unlockwidgets.h                     \
    x11wrapper.h

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
    unlocknotifications.cpp             \
    unlocknotificationsink.cpp          \
    unlockmissedevents.cpp              \
    unlockwidgets.cpp                   \
    x11wrapper.cpp

include(statusindicatormenu/statusindicatormenu.pri)
include(statusarea/statusarea.pri)
include(notifications/notifications.pri)
include(volumecontrol/volumecontrol.pri)
include(../localconfig.pri)

contains(DEFINES, HAVE_CONTEXTSUBSCRIBER) {
	PKGCONFIG += contextsubscriber-1.0
}

contains(DEFINES, HAVE_QMSYSTEM) {
	PKGCONFIG += qmsystem
}

contains(DEFINES, HAVE_MAEMOSEC) {
	PKGCONFIG += maemosec
}

contains(DEFINES, HAVE_LIBRESOURCEQT) {
	PKGCONFIG += libresourceqt1
}

contains(DEFINES, HAVE_LIBNGF) {
	PKGCONFIG += libngf0
}

PKGCONFIG += \
    dbus-1 \
    xcomposite

# TODO: remove these very quickly when obexd is okay
usb_scripts.files += pcsuite-enable.sh
usb_scripts.path = $$(DEBIAN_DESTDIR)/usr/bin

usb_sudoers.files += usb.sudoers
usb_sudoers.path = $$(DEBIAN_DESTDIR)/etc/sudoers.d

dbus_policy.files += systemui.conf
dbus_policy.path = $$(DEBIAN_DESTDIR)/etc/dbus-1/system.d

INSTALLS += target \
            dbus_policy \
            usb_scripts \
            usb_sudoers

