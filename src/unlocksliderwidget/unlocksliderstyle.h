#ifndef UNLOCKSLIDERSTYLE_H
#define UNLOCKSLIDERSTYLE_H

#include <duiwidgetstyle.h>
#include <DuiScalableImage>

class UnlockSliderStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE(UnlockSliderStyle)

    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage*, handleImage,             HandleImage)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage*, handleHilightedImage,    HandleHilightedImage)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage*, backgroundTileImage,     BackgroundTileImage)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage*, iconImage,               IconImage)
    DUI_STYLE_ATTRIBUTE(    QSize,             handleSize,              HandleSize)
    DUI_STYLE_ATTRIBUTE(    QSize,             backgroundTileSize,      BackgroundTileSize)
    DUI_STYLE_ATTRIBUTE(    QSize,             iconSize,                IconSize)
    DUI_STYLE_ATTRIBUTE(    int,               blinkTime,               BlinkTime)
    DUI_STYLE_ATTRIBUTE(    int,               blinkCount,              BlinkCount)
};

class UnlockSliderStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER(UnlockSliderStyle)
};

#endif // UNLOCKSLIDERSTYLE_H
