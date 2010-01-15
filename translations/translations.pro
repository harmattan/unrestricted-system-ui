# empty, means to build only engineering English
LANGUAGES =
CATALOGNAME = duicontrolpanel-systemui
SOURCEDIR = $$PWD/../components/sysuid $$PWD/../components/displayapplet $$PWD/../components/batteryapplet
TRANSLATIONDIR = $$PWD
TRANSLATION_INSTALLDIR = /usr/share/l10n/dui
include(translations.pri)
