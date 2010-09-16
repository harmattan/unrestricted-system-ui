include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_sysuid
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$SRCDIR/statusarea $$LIBNOTIFICATIONSRCDIR
INCLUDEPATH += $$SRCDIR/statusindicatormenu $$SRCDIR/volumecontrol
INCLUDEPATH += /usr/include/contextsubscriber
INCLUDEPATH += /usr/include/resource/qt4

PKGCONFIG += dbus-1

# unit test and unit classes
SOURCES += \
    $$SRCDIR/debug.cpp \
    $$SRCDIR/shutdownui.cpp \
    $$SRCDIR/usbui.cpp \
    $$SRCDIR/lockscreen.cpp \
    $$SRCDIR/eventeater.cpp \
    $$SRCDIR/lockscreenwindow.cpp \
    $$SRCDIR/shutdownbusinesslogic.cpp \
    $$SRCDIR/batterybusinesslogic.cpp \
    $$SRCDIR/lockscreenbusinesslogic.cpp \
    $$SRCDIR/lockscreenbusinesslogicadaptor.cpp \
    $$SRCDIR/sysuid.cpp \
    ut_sysuid.cpp \

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \
    $$STUBSDIR/systemstatestub.cpp \
    $$STUBSDIR/locksstub.cpp \
    $$STUBSDIR/testcontextitem.cpp \

# unit test and unit classes
HEADERS += \
    $$SRCDIR/debug.h \
    $$STUBSDIR/volumecontrolui_stub.h \
    $$SRCDIR/volumecontrol/volumecontrolui.h \
    $$SRCDIR/shutdownui.h \
    $$SRCDIR/usbui.h \
    $$SRCDIR/lockscreen.h \
    $$SRCDIR/lockscreenwindow.h \
    $$SRCDIR/eventeater.h \
    $$SRCDIR/shutdownbusinesslogic.h \
    $$SRCDIR/batterybusinesslogic.h \
    $$SRCDIR/lockscreenbusinesslogic.h \
    $$SRCDIR/lockscreenbusinesslogicadaptor.h \
    $$SRCDIR/sysuidrequest.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/contextframeworkcontext.h \
    ut_sysuid.h \
   $$NOTIFICATIONSRCDIR/eventtypestore.h \
   $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
   $$NOTIFICATIONSRCDIR/widgetnotificationsink.h \
   $$NOTIFICATIONSRCDIR/mcompositornotificationsink.h \
   $$NOTIFICATIONSRCDIR/ngfnotificationsink.h \
   $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$SRCDIR/unlockwidgets.h \
    $$SRCDIR/unlocknotifications.h \
    $$SRCDIR/unlocknotificationsink.h \
    $$SRCDIR/unlockmissedevents.h \
    $$SRCDIR/statusarea/statusarearenderer.h \
    $$SRCDIR/applicationcontext.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenuwindow.h \
    $$SRCDIR/x11wrapper.h

# service classes
HEADERS += \
    $$STUBSDIR/stubbase.h \
    $$STUBSDIR/systemstatestub.h \
    $$STUBSDIR/locksstub.h \
    $$STUBSDIR/testcontextitem.h \
    $$STUBSDIR/widgetnotificationsink_stub.h \
    $$STUBSDIR/eventtypestore_stub.h \
    $$STUBSDIR/notificationmanager_stub.h \
    $$STUBSDIR/statusarearenderer_stub.h \
    $$STUBSDIR/unlocknotifications_stub.h \
    $$STUBSDIR/unlockmissedevents_stub.h \
    $$STUBSDIR/unlockwidgets_stub.h

include(../common_bot.pri)
