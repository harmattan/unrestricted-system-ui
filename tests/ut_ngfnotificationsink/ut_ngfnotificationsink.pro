include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_ngfnotificationsink
INCLUDEPATH +=$$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR

SOURCES += \
    ut_ngfnotificationsink.cpp \
    $$NOTIFICATIONSRCDIR/ngfnotificationsink.cpp \
    $$LIBNOTIFICATIONSRCDIR/notification.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += \
    ut_ngfnotificationsink.h \
    $$NOTIFICATIONSRCDIR/ngfnotificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
    $$LIBNOTIFICATIONSRCDIR/notification.h \
    $$NOTIFICATIONSRCDIR/ngfadapter.h

include(../common_bot.pri)
