include(../../mconfig.pri)

TEMPLATE = subdirs

images.files = *.png
images.path = $$SYSTEMUI_THEMES_DIR/images

INSTALLS += images

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
