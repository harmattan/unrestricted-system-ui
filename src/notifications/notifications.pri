# Input
include(eventtypes/eventtypes.pri)
SYSTEMUI_NOTIFICATIONS_SRC_DIR = $$SYSTEMUI_SOURCE_DIR/notifications
INCLUDEPATH += $$SYSTEMUI_SOURCE_DIR/notifications
HEADERS += $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationmanagerinterface.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationmanager.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationparameter.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationparameters.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notification.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationgroup.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationsource.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsource.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsourceadaptor.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationsink.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationareasink.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/widgetnotificationsink.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/mcompositornotificationsink.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/ngfnotificationsink.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/ngfadapter.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notifiernotificationsink.h \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/eventtypestore.h \

SOURCES += $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationmanager.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationparameter.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationparameters.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notification.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationgroup.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationsource.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsource.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsourceadaptor.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationsink.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notificationareasink.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/widgetnotificationsink.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/mcompositornotificationsink.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/ngfnotificationsink.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/ngfadapter.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/notifiernotificationsink.cpp \
    $$SYSTEMUI_NOTIFICATIONS_SRC_DIR/eventtypestore.cpp

