include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_notifiernotificationsink
INCLUDEPATH +=$$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR

# unit test and unit
SOURCES += \
    ut_notifiernotificationsink.cpp \
    $$NOTIFICATIONSRCDIR/notifiernotificationsink.cpp

# base classes
SOURCES += \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.cpp \
    $$LIBNOTIFICATIONSRCDIR/notification.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp

# service classes
SOURCES += $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_notifiernotificationsink.h \
    $$NOTIFICATIONSRCDIR/notifiernotificationsink.h

# base classes
HEADERS += \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notification.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.h \
    $$NOTIFICATIONSRCDIR/ngfadapter.h

# service classes
HEADERS += $$STUBSDIR/stubbase.h

include(../common_bot.pri)
