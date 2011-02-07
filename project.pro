system(./configure)
include(shared.pri)

TEMPLATE = subdirs

addSubDirs(src)
addSubDirs(mkspecs)
addSubDirs(themes)
addSubDirs(translations)
addSubDirs(demos)
addSubDirs(tests)
CONFIG(docs) {
    addSubDirs(doc)
}


QMAKE_CLEAN += configure-stamp build-stamp localconfig.pri

contains(BUILD_FEATURES,coverage) {
	QMAKE_EXTRA_TARGETS += coverage
	coverage.depends = src/Makefile
	coverage.commands = \
		cd tests && make coverage && cd .. \
				&& genhtml --no-branch-coverage --legend -o coverage/ \
		   -t \"SystemUI Coverage Report\" \
		tests/ut_*/selected.cov
}

