include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_mnotificationproxy
INCLUDEPATH += $$NOTIFICATIONSRCDIR $$LIBNOTIFICATIONSRCDIR

# unit test and unit classes
SOURCES += ut_mnotificationproxy.cpp \
    $$NOTIFICATIONSRCDIR/mnotificationproxy.cpp \
    $$LIBNOTIFICATIONSRCDIR/notification.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.cpp

# unit test and unit classes
HEADERS += ut_mnotificationproxy.h \
    $$NOTIFICATIONSRCDIR/mnotificationproxy.h \
    $$LIBNOTIFICATIONSRCDIR/notification.h \
    $$LIBNOTIFICATIONSRCDIR/notificationgroup.h

