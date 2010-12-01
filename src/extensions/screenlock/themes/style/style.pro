include(../../../../../mconfig.pri)

TEMPLATE = subdirs

INSTALLS += style

style.files = *.css
style.path = /usr/share/themes/base/meegotouch/libsysuid-screenlock/style

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
