TEMPLATE = subdirs
SUBDIRS = \
    ut_unlockslider \

QMAKE_STRIP = echo
#include(shell.pri)
#include(runtests.pri)

QMAKE_CLEAN += **/*.log.xml ./coverage.log.xml

tests_xml.path = /usr/share/system-ui-tests
tests_xml.files = tests.xml

INSTALLS += tests_xml
