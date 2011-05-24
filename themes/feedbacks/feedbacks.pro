include(../../mconfig.pri)

TEMPLATE = subdirs

# Power off feedback
poweroff.files = priority1_power_off/*
poweroff.path = $$SYSTEMUI_THEMES_DIR/feedbacks/priority1_power_off

INSTALLS += poweroff

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
