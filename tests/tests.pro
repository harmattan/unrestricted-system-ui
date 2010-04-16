TEMPLATE = subdirs
SUBDIRS = $$system(ls -1d ut_*/*.pro ft_*/*.pro 2>/dev/null | sed 's!/.*!!')

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
