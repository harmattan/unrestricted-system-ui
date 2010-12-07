# Input
include(eventtypes/eventtypes.pri)

system(qdbusxml2cpp ../../libnotificationsystem/notificationsink.xml -p dbusinterfacenotificationsinkproxy -c DBusInterfaceNotificationSinkProxy -i metatypedeclarations.h)
system(qdbusxml2cpp dbusinterfacenotificationsink.xml -a dbusinterfacenotificationsinkadaptor -c DBusInterfaceNotificationSinkAdaptor -l DBusInterfaceNotificationSink -i dbusinterfacenotificationsink.h -i metatypedeclarations.h)
system(qdbusxml2cpp notificationmanager.xml -a dbusinterfacenotificationsourceadaptor -c DBusInterfaceNotificationSourceAdaptor -l DBusInterfaceNotificationSource -i dbusinterfacenotificationsource.h -i metatypedeclarations.h)

SYSTEMUI_NOTIFICATIONS_SRC_DIR = $$SYSTEMUI_SOURCE_DIR/notifications
INCLUDEPATH += $$SYSTEMUI_SOURCE_DIR/notifications $$SYSTEMUI_SOURCE_DIR/libnotificationsystem
HEADERS +=  \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsource.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsourceadaptor.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationareasink.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/widgetnotificationsink.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/mcompositornotificationsink.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/ngfnotificationsink.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/ngfadapter.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notifiernotificationsink.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/eventtypestore.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationmanager.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationsource.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/mnotificationproxy.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsink.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsinkadaptor.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsinkproxy.h

SOURCES +=  \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsource.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsourceadaptor.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationareasink.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/widgetnotificationsink.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/mcompositornotificationsink.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/ngfnotificationsink.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/ngfadapter.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notifiernotificationsink.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/eventtypestore.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationmanager.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationsource.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/mnotificationproxy.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsink.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsinkadaptor.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsinkproxy.cpp
