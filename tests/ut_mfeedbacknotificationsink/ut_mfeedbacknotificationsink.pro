include(../common_top.pri)
TARGET = ut_mfeedbacknotificationsink
INCLUDEPATH += $$SRCDIR/notifications
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$M_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

# unit test and unit
SOURCES += \
    ut_mfeedbacknotificationsink.cpp \
    $$SRCDIR/notifications/mfeedbacknotificationsink.cpp \
    $$SRCDIR/notifications/notification.cpp \
    $$SRCDIR/notifications/notificationgroup.cpp \
    $$SRCDIR/notifications/notificationparameters.cpp \
    $$SRCDIR/notifications/notificationparameter.cpp

# base classes
SOURCES += \
    $$SRCDIR/notifications/notificationsink.cpp \
    ../stubs/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_mfeedbacknotificationsink.h \
    $$SRCDIR/notifications/mfeedbacknotificationsink.h \

# base classes
HEADERS += \
    $$SRCDIR/notifications/notificationsink.h \

# service classes
HEADERS += \
    $$SRCDIR/notifications/notification.h \
    $$SRCDIR/notifications/notificationparameters.h \
    $$SRCDIR/notifications/notificationparameter.h \
    $$SRCDIR/notifications/notification.h \
    $$SRCDIR/notifications/notificationgroup.h \
    $$SRCDIR/notifications/notificationmanager.h \
    $$SRCDIR/notifications/eventtypestore.h \
    $$SRCDIR/sysuid.h

include(../common_bot.pri)
