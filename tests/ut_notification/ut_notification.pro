include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_notification
INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR

# unit test and unit
SOURCES += \
    ut_notification.cpp \
   $$LIBNOTIFICATIONSRCDIR/notification.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp

# unit test and unit
HEADERS += \
    ut_notification.h \
   $$LIBNOTIFICATIONSRCDIR/notification.h \
   $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
   $$LIBNOTIFICATIONSRCDIR/notificationparameters.h

HEADERS += \
    qdbusargument_fake.h

   
include(../common_bot.pri)
