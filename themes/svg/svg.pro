include(../../mconfig.pri)

TEMPLATE = subdirs

svg.files =*.svg
svg.path = $$SYSTEMUI_THEMES_DIR/svg

INSTALLS += svg

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
