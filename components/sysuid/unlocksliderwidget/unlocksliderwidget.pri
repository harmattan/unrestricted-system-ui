DUIGEN_OUTDIR = unlocksliderwidget
SOURCES += unlocksliderwidget/unlockslider.cpp \
    unlocksliderwidget/unlockslidermodel.cpp \
    unlocksliderwidget/unlocksliderview.cpp
HEADERS += unlocksliderwidget/unlockslider.h \
    unlocksliderwidget/unlocksliderstyle.h \
    unlocksliderwidget/unlockslidermodel.h \
    unlocksliderwidget/unlocksliderview.h
MODEL_HEADERS += unlocksliderwidget/unlockslidermodel.h
STYLE_HEADERS += unlocksliderwidget/unlocksliderstyle.h

style_sheet.files += unlocksliderwidget/unlocksliderstyle.css
style_svg.files += unlocksliderwidget/*.svg

