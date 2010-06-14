#
# We just need an extra target so that the recursive make will pass, we don't
# need to do anything else.
#
contains(BUILD_FEATURES,coverage) {
  QMAKE_EXTRA_TARGETS += coverage
  coverage.commands = $$system(true)
}

