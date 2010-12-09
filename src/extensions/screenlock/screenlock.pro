MOC_DIR = .moc
M_MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj
LOCALLIBSDIR = ../../../lib

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

QMAKE_LIBDIR += $$LOCALLIBSDIR
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
    lockscreenstyle.h \
    lockscreen.h \
    lockscreenheader.h \
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
    lockscreen.cpp \
    lockscreenheader.cpp \
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
           ../../systemui/notifications/notificationareasink.h \
           ../../systemui/notifications/widgetnotificationsink.h \
           
SOURCES += ../../systemui/contextframeworkcontext.cpp \
           ../../systemui/x11wrapper.cpp \
           ../../systemui/statusarea/statusindicator.cpp \
           ../../systemui/statusarea/clock.cpp \
           ../../systemui/statusarea/statusarea.cpp \
           ../../systemui/statusarea/notificationstatusindicator.cpp \
           ../../systemui/statusindicatormenu/notificationarea.cpp \
           ../../systemui/notifications/notificationareasink.cpp \
           ../../systemui/notifications/widgetnotificationsink.cpp \

MODEL_HEADERS += ../../systemui/statusarea/clockmodel.h \
                 ../../systemui/statusarea/statusindicatormodel.h \
                 ../../systemui/statusindicatormenu/notificationareamodel.h

TARGET        = $$qtLibraryTarget(sysuid-screenlock)
DESTDIR       = $$LOCALLIBSDIR

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
