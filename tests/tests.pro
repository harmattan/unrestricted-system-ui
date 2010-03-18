TEMPLATE = subdirs
SUBDIRS = $$system(ls -1d ut_*/ ft_*/)

QMAKE_STRIP = echo

QMAKE_CLEAN += **/*.log.xml ./coverage.log.xml

tests_xml.path = /usr/share/system-ui-tests
tests_xml.files = tests.xml

INSTALLS += tests_xml
