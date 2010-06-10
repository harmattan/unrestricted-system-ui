TEMPLATE = subdirs

#
# Please note that there are several ways to temprorarily disable one or more
# tests. Using the new XML generator script it is possible to put a 'disabled'
# or a 'deleted' file into the test directory, so the test will still be
# compiled but left out from the xml (or the insignificant property will be set
# using the 'disabled' file).
#
SUBDIRS = $$system(ls -1d ut_*/*.pro ft_*/*.pro 2>/dev/null | sed 's!/.*!!')

QMAKE_STRIP = echo

check.target = check
check.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check

check-xml.target = check-xml
check-xml.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check-xml

QMAKE_CLEAN += **/*.log.xml

support_files.commands += $$PWD/gen-tests-xml.sh > $$OUT_PWD/tests.xml
support_files.target = support_files
support_files.files += $$OUT_PWD/tests.xml
support_files.path = /usr/share/system-ui-tests
support_files.CONFIG += no_check_exist

INSTALLS += support_files
