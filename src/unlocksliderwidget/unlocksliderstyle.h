/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
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
    DUI_STYLE_ATTRIBUTE(qreal,             thickness,               Thickness)
    DUI_STYLE_ATTRIBUTE(int,               blinkTime,               BlinkTime)
    DUI_STYLE_ATTRIBUTE(int,               blinkCount,              BlinkCount)
};

class UnlockSliderStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER(UnlockSliderStyle)
};

#endif // UNLOCKSLIDERSTYLE_H
