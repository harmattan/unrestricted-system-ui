system(./configure)

TEMPLATE = subdirs

SUBDIRS = src \
	  translations \
          themes \
	  demos \
          tests

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

include(doc/doc.pri)
