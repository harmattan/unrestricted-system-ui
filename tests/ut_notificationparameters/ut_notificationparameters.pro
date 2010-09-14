include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_notificationparameters
INCLUDEPATH += $$LIBNOTIFICATIONSRCDIR

# unit test and unit
SOURCES += \
   ut_notificationparameters.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp

# base classes
SOURCES += \


# unit test and unit
HEADERS += \
   ut_notificationparameters.h \
   $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
   $$LIBNOTIFICATIONSRCDIR/notificationparameter.h

# base classes
HEADERS += \


# service classes
HEADERS += \
    ../stubs/qdbusargument_fake.h


include(../common_bot.pri)
