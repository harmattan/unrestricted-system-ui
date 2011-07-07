include(../common_top.pri)

TARGET = ut_notificationstatusindicatorsink
CONFIG += notificationsystem
INCLUDEPATH += $$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR

SOURCES += ut_notificationstatusindicatorsink.cpp \
    $$NOTIFICATIONSRCDIR/notificationstatusindicatorsink.cpp \
    $$LIBNOTIFICATIONSRCDIR/notification.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += ut_notificationstatusindicatorsink.h \
    $$NOTIFICATIONSRCDIR/notificationstatusindicatorsink.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
    $$LIBNOTIFICATIONSRCDIR/notification.h

