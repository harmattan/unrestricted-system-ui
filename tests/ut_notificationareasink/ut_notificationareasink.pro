include(../coverage.pri)
include(../common_top.pri)
TARGET = ut_notificationareasink
INCLUDEPATH +=$$NOTIFICATIONSRCDIR $$SRCDIR $$LIBNOTIFICATIONSRCDIR
INCLUDEPATH += /usr/include/contextsubscriber
DEFINES += NOTIFICATIONS_EVENT_TYPES=\'$$quote(\"$$M_NOTIFICATIONS_EVENT_TYPES_DIR\")\'

# unit test and unit classes
SOURCES += ut_notificationareasink.cpp \
   $$NOTIFICATIONSRCDIR/notificationareasink.cpp \
   $$NOTIFICATIONSRCDIR/widgetnotificationsink.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationsink.cpp \
   $$LIBNOTIFICATIONSRCDIR/notification.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationgroup.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationparameter.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationparameters.cpp \
   $$NOTIFICATIONSRCDIR/dbusinterfacenotificationsource.cpp \
   $$NOTIFICATIONSRCDIR/dbusinterfacenotificationsourceadaptor.cpp \
    $$SRCDIR/contextframeworkcontext.cpp \
   $$LIBNOTIFICATIONSRCDIR/notificationsource.cpp

# helper classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += ut_notificationareasink.h \
   $$NOTIFICATIONSRCDIR/notificationareasink.h \
   $$NOTIFICATIONSRCDIR/widgetnotificationsink.h \
   $$LIBNOTIFICATIONSRCDIR/notificationsink.h \
   $$LIBNOTIFICATIONSRCDIR/notification.h \
   $$LIBNOTIFICATIONSRCDIR/notificationgroup.h \
   $$LIBNOTIFICATIONSRCDIR/notificationparameter.h \
   $$LIBNOTIFICATIONSRCDIR/notificationparameters.h \
   $$NOTIFICATIONSRCDIR/dbusinterfacenotificationsource.h \
   $$NOTIFICATIONSRCDIR/dbusinterfacenotificationsourceadaptor.h \
   $$NOTIFICATIONSRCDIR/eventtypestore.h \
   $$SRCDIR/contextframeworkcontext.h \
   $$SRCDIR/applicationcontext.h \
   $$LIBNOTIFICATIONSRCDIR/notificationsource.h \
   $$NOTIFICATIONSRCDIR/notificationmanager.h \
   $$SRCDIR/sysuid.h \


include(../common_bot.pri)
