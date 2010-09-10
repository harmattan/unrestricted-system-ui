include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_notificationgroup
INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR

# unit test and unit
SOURCES += \
    ut_notificationgroup.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationgroup.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp

# unit test and unit
HEADERS += \
    ut_notificationgroup.h \
   $$LIBNOTIFICATIONSRCDIR/notificationgroup.h \
   $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
   $$LIBNOTIFICATIONSRCDIR/notificationparameters.h

include(../common_bot.pri)
