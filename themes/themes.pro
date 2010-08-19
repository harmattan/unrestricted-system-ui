include(../mconfig.pri)

TEMPLATE = subdirs
SUBDIRS = \
    feedbacks \
    style \
    svg

# images not used anymore...

INSTALLS += \
    conf

#
# We just need an extra target so that the recursive make will pass, we don't
# need to do anything else.
#
contains(BUILD_FEATURES,coverage) {
  QMAKE_EXTRA_TARGETS += coverage
  coverage.commands = $$system(true)
}

conf.files = \
    sysuid.conf
conf.path = $$SYSTEMUI_THEMES_DIR

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
