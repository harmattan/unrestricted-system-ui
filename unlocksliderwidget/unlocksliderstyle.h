#ifndef UNLOCKSLIDERSTYLE_H
#define UNLOCKSLIDERSTYLE_H

#include <duiwidgetstyle.h>
#include <DuiScalableImage>

class UnlockSliderStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE(UnlockSliderStyle)

    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage*, handleImage,           HandleImage)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage*, handleHilightedImage,  HandleHilightedImage)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage*, backgroundTileImage,   BackgroundTileImage)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage*, lockIconImage,         LockIconImage)
    DUI_STYLE_ATTRIBUTE(    QSize,             handleSize,            HandleSize)
    DUI_STYLE_ATTRIBUTE(    QSize,             backgroundTileSize,    BackgroundTileSize)
    DUI_STYLE_ATTRIBUTE(    QSize,             lockIconSize,          LockIconSize)
};

class UnlockSliderStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER(UnlockSliderStyle)
};

#endif // UNLOCKSLIDERSTYLE_H
