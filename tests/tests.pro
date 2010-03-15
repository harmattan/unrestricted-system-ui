TEMPLATE = subdirs
SUBDIRS = \
    ut_template \
    ut_pluginlist \
    ut_clock \
    ut_clockview \
    ut_notifier \
    ut_statusarea \
    ut_statusareaview \
    ut_statusindicator \
    ut_statusindicatorimageview \
    ut_statusindicatorlabelview \
    ut_notificationarea

# These tests needs X server:
#    ut_unlockslider \
#    ut_batterybusinesslogic \
#    ut_lowbatterynotifier

QMAKE_STRIP = echo
#include(shell.pri)
#include(runtests.pri)

QMAKE_CLEAN += **/*.log.xml ./coverage.log.xml

tests_xml.path = /usr/share/system-ui-tests
tests_xml.files = tests.xml

INSTALLS += tests_xml
