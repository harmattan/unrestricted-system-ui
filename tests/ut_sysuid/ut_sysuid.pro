include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_sysuid
INCLUDEPATH += $$SRCDIR/statusarea $$LIBNOTIFICATIONSRCDIR $$NOTIFICATIONSRCDIR $$SRCDIR/statusindicatormenu $$SRCDIR/volumecontrol $$SRCDIR/lockscreen
INCLUDEPATH += /usr/include/contextsubscriber /usr/include/resource/qt4

PKGCONFIG += dbus-1

STYLE_HEADERS += $$SRCDIR/lockscreen/lockscreenwindowstyle.h

# unit test and unit classes
SOURCES += \
    ut_sysuid.cpp \
    $$SRCDIR/shutdownui.cpp \
    $$SRCDIR/usbui.cpp \
    $$SRCDIR/batterybusinesslogic.cpp \
    $$SRCDIR/lockscreen/lockscreenbusinesslogic.cpp \
    $$SRCDIR/lockscreen/lockscreenbusinesslogicadaptor.cpp \
    $$SRCDIR/sysuid.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \
    $$STUBSDIR/systemstatestub.cpp \
    $$STUBSDIR/locksstub.cpp \
    $$STUBSDIR/testcontextitem.cpp \

# unit test and unit classes
HEADERS += \
    ut_sysuid.h \
    $$STUBSDIR/volumebarwindow_stub.h \
    $$SRCDIR/volumecontrol/volumebarwindow.h \
    $$SRCDIR/shutdownui.h \
    $$SRCDIR/usbui.h \
    $$SRCDIR/lockscreen/lockscreen.h \
    $$SRCDIR/lockscreen/lockscreenwindow.h \
    $$SRCDIR/lockscreen/lockscreenwindowstyle.h \
    $$SRCDIR/lockscreen/eventeater.h \
    $$SRCDIR/shutdownbusinesslogic.h \
    $$SRCDIR/batterybusinesslogic.h \
    $$SRCDIR/lockscreen/lockscreenbusinesslogic.h \
    $$SRCDIR/lockscreen/lockscreenbusinesslogicadaptor.h \
    $$SRCDIR/sysuidrequest.h \
    $$SRCDIR/sysuid.h \
    $$SRCDIR/contextframeworkcontext.h \
    $$NOTIFICATIONSRCDIR/eventtypestore.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$NOTIFICATIONSRCDIR/widgetnotificationsink.h \
    $$NOTIFICATIONSRCDIR/mcompositornotificationsink.h \
    $$NOTIFICATIONSRCDIR/ngfnotificationsink.h \
    $$NOTIFICATIONSRCDIR/notifiernotificationsink.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$SRCDIR/lockscreen/unlockarea.h \
    $$SRCDIR/lockscreen/unlocknotifications.h \
    $$SRCDIR/lockscreen/unlocknotificationsink.h \
    $$SRCDIR/lockscreen/unlockmissedevents.h \
    $$SRCDIR/statusarea/statusarearenderer.h \
    $$SRCDIR/statusarea/statusarearendereradaptor.h \
    $$SRCDIR/applicationcontext.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenuwindow.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenuadaptor.h \
    $$SRCDIR/x11wrapper.h \
    $$SRCDIR/closeeventeater.h

# service classes
HEADERS += \
    $$STUBSDIR/stubbase.h \
    $$STUBSDIR/systemstatestub.h \
    $$STUBSDIR/locksstub.h \
    $$STUBSDIR/testcontextitem.h

include(../common_bot.pri)
