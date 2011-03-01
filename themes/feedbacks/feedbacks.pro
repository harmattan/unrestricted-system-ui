include(../../mconfig.pri)

TEMPLATE = subdirs

# Touch-screen-unlock UI feedbacks

startdnd.files = \
    start-dragndrop/*

startdnd.path = $$SYSTEMUI_THEMES_DIR/feedbacks/start-dragndrop

enterdnddrop.files = \
    enter-dragndrop-dropzone/*

enterdnddrop.path = $$SYSTEMUI_THEMES_DIR/feedbacks/enter-dragndrop-dropzone

exitdnddrop.files = \
    exit-dragndrop-dropzone/*

exitdnddrop.path = $$SYSTEMUI_THEMES_DIR/feedbacks/exit-dragndrop-dropzone

releasein.files = \
    release-inside-dragndrop-dropzone/*

releasein.path = $$SYSTEMUI_THEMES_DIR/feedbacks/release-inside-dragndrop-dropzone

releaseout.files = \
    release-outside-dragndrop-dropzone/*

releaseout.path = $$SYSTEMUI_THEMES_DIR/feedbacks/release-outside-dragndrop-dropzone

# Power-off feedback

poweroff.files = \
    priority1_power_off/*

poweroff.path = $$SYSTEMUI_THEMES_DIR/feedbacks/priority1_power_off

INSTALLS += \
    startdnd \
    enterdnddrop \
    exitdnddrop \
    releasein \
    releaseout \
    poweroff

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
