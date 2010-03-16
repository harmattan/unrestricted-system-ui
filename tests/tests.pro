TEMPLATE = subdirs
SUBDIRS = \
    ut_unlockslider \
    ut_batterybusinesslogic \
    ut_lowbatterynotifier

QMAKE_STRIP = echo

QMAKE_CLEAN += **/*.log.xml ./coverage.log.xml

tests_xml.path = /usr/share/system-ui-tests
tests_xml.files = tests.xml

INSTALLS += tests_xml
