include(../../../../mconfig.pri)
include(../../../../shared.pri)

TEMPLATE = subdirs
addSubDirs(style)

INSTALLS += conf

#
# We just need an extra target so that the recursive make will pass, we don't
# need to do anything else.
#
contains(BUILD_FEATURES,coverage) {
  QMAKE_EXTRA_TARGETS += coverage
  coverage.commands = $$system(true)
}

conf.files = *.conf
conf.path = /usr/share/themes/base/meegotouch/libsysuid-screenlock

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
