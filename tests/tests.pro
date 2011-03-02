TEMPLATE = subdirs
include(../shared.pri)

QT += meegographicssystemhelper

#
# Please note that there are several ways to temprorarily disable one or more
# tests. Using the new XML generator script it is possible to put a 'disabled'
# or a 'deleted' file into the test directory, so the test will still be
# compiled but left out from the xml (or the insignificant property will be set
# using the 'disabled' file).
#
contains(BUILD_FEATURES,coverage) {
    #
    # Sometimes if is faster like this, when you are working on a single unit
    # test for a while.
    #
    subdirs =  $$system(./unit_tests.sh)
    for(suitename, subdirs):{
        addSubDirs($${suitename})
    }
} else {
	subdirs=$$system(ls -1d ut_*/*.pro ft_*/*.pro 2>/dev/null | sed 's!/.*!!')
	for(suitename, subdirs):{
		addSubDirs($${suitename})
	}
}

contains(BUILD_FEATURES,nocheck) {
	SUBDIRS =
}

QMAKE_STRIP = echo

check.target = check
check.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check

check-xml.target = check-xml
check-xml.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check-xml

QMAKE_CLEAN += **/*.log.xml

contains(BUILD_FEATURES,coverage) {
  QMAKE_EXTRA_TARGETS += coverage
  coverage.target = coverage
  coverage.CONFIG = recursive
}

support_files.commands += $$PWD/gen-tests-xml.sh > $$OUT_PWD/tests.xml
support_files.target = support_files
support_files.files += $$OUT_PWD/tests.xml
support_files.path = /usr/share/system-ui-tests
support_files.CONFIG += no_check_exist

dummy_file.commands += touch $$OUT_PWD/keep_this_dir
dummy_file.target = dummy_file
dummy_file.files += $$OUT_PWD/keep_this_dir
dummy_file.path = /usr/lib/system-ui-tests

INSTALLS += \
    support_files \
    dummy_file

