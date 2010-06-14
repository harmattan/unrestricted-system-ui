contains(BUILD_FEATURES,coverage) {
	QMAKE_CXXFLAGS += --coverage 
	QMAKE_LFLAGS += --coverage
	QMAKE_EXTRA_TARGETS += coverage
	coverage.commands    = \
  		lcov --directory . --zerocounters \
                && make check \
                && lcov --base-directory `pwd` --directory . --capture --output-file all.cov \
                && lcov --remove all.cov \"*debug.cpp\" --output-file tmp.cov \
                && lcov --extract tmp.cov \"*src*.cpp\" --output-file selected.cov
}

QMAKE_CLEAN += .tmp*gcov ./.tmp/*.gcda ./.tmp/*.gcno *.cov

