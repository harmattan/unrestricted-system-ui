TEMPLATE = subdirs
SUBDIRS = \
    ut_pluginlist \
    ut_clock \
    ut_clockview \
    ut_notifier \
    ut_statusarea \
    ut_statusareaview \
    ut_statusindicator \
    ut_statusindicatorimageview \
    ut_statusindicatorlabelview \
    ut_notificationarea \
    ut_notification \
    ut_notificationgroup \
    ut_notificationmanager \
    ut_notificationparameter \
    ut_notificationparameters \
    ut_notifiernotificationsink \
    ut_dbusinterfacenotificationsource \
    ut_duicompositornotificationsink \
    ut_duifeedbacknotificationsink \
    ut_eventtypestore \
    ut_widgetnotificationsink

# These tests needs X server:
#    ut_unlockslider \
#    ut_batterybusinesslogic \
#    ut_lowbatterynotifier

QMAKE_STRIP = echo

check.target = check
check.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check

check-xml.target = check-xml
check-xml.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check-xml

QMAKE_CLEAN += **/*.log.xml ./coverage.log.xml

tests_xml.path = /usr/share/system-ui-tests
tests_xml.files = tests.xml

INSTALLS += tests_xml
