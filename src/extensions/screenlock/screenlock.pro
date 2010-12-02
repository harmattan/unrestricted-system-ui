MOC_DIR = .moc
M_MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

include(../../../mconfig.pri)
include(../../../localconfig.pri)

TEMPLATE      = lib
CONFIG       += plugin gui meegotouch system-ui link_pkgconfig
INCLUDEPATH  += \
    ../../libnotificationsystem \
    ../../systemui/statusindicatormenu \
    ../../systemui/statusarea \
    ../../systemui/notifications \
    ../../systemui/screenlock \
    ../../systemui

QMAKE_LIBDIR += ../../lib
LIBS += -lnotificationsystem

STYLE_HEADERS += lockscreenheaderstyle.h \
    lockscreenheaderwithpadlockstyle.h \
    lockscreenstatusareastyle.h \
    lockscreenstyle.h \
    datestyle.h
    
HEADERS += \
    screenlockextension.h \
    unlocknotifications.h \
    unlocknotificationsink.h \
    unlockmissedevents.h \
    unlockarea.h \
    lockscreenview.h \
    lockscreenwithpadlockview.h \
    lockscreenwithoutpadlockview.h \
    lockscreenstyle.h \
    lockscreen.h \
    lockscreenheaderview.h \
    lockscreenheaderstyle.h \
    lockscreenheaderwithpadlockview.h \
    lockscreenheaderwithpadlockstyle.h \
    lockscreenstatusareaview.h \
    lockscreenstatusareastyle.h \
    date.h \
    datestyle.h

SOURCES += \
    screenlockextension.cpp \
    unlocknotifications.cpp \
    unlocknotificationsink.cpp \
    unlockmissedevents.cpp \
    unlockarea.cpp \
    lockscreenview.cpp \
    lockscreenwithpadlockview.cpp \
    lockscreenwithoutpadlockview.cpp \
    lockscreen.cpp \
    lockscreenheaderview.cpp \
    lockscreenheaderwithpadlockview.cpp \
    lockscreenstatusareaview.cpp \
    date.cpp

HEADERS += ../../systemui/contextframeworkcontext.h \
           ../../systemui/applicationcontext.h \ 
           ../../systemui/x11wrapper.h \
           ../../systemui/statusarea/statusindicator.h \
           ../../systemui/statusarea/statusindicatormodel.h \
           ../../systemui/statusarea/clock.h \
           ../../systemui/statusarea/clockmodel.h \
           ../../systemui/statusarea/statusarea.h \
           ../../systemui/statusarea/notificationstatusindicator.h \
           ../../systemui/statusindicatormenu/notificationarea.h \
           ../../systemui/statusindicatormenu/notificationareamodel.h \
           ../../systemui/statusindicatormenu/notificationareastyle.h \
           ../../systemui/statusindicatormenu/notificationareaview.h \
           ../../systemui/notifications/notificationareasink.h \
           ../../systemui/notifications/widgetnotificationsink.h \
           
SOURCES += ../../systemui/contextframeworkcontext.cpp \
           ../../systemui/x11wrapper.cpp \
           ../../systemui/statusarea/statusindicator.cpp \
           ../../systemui/statusarea/clock.cpp \
           ../../systemui/statusarea/statusarea.cpp \
           ../../systemui/statusarea/notificationstatusindicator.cpp \
           ../../systemui/statusindicatormenu/notificationarea.cpp \
           ../../systemui/statusindicatormenu/notificationareaview.cpp \
           ../../systemui/notifications/notificationareasink.cpp \
           ../../systemui/notifications/widgetnotificationsink.cpp \

STYLE_HEADERS += ../../systemui/statusindicatormenu/notificationareastyle.h

MODEL_HEADERS += ../../systemui/statusarea/clockmodel.h \
                 ../../systemui/statusarea/statusindicatormodel.h \
                 ../../systemui/statusindicatormenu/notificationareamodel.h

TARGET        = $$qtLibraryTarget(sysuid-screenlock)
DESTDIR       = ../../../lib

contains(DEFINES, HAVE_CONTEXTSUBSCRIBER) {
    PKGCONFIG += contextsubscriber-1.0
}

contains(DEFINES, HAVE_QMSYSTEM) {
    PKGCONFIG += qmsystem2
}

PKGCONFIG += xcomposite

target.path += $$M_APPLICATION_EXTENSION_DIR

desktop_entry.path = $$M_APPLICATION_EXTENSION_DATA_DIR
desktop_entry.files = *.desktop

INSTALLS += \
    target \
    desktop_entry
