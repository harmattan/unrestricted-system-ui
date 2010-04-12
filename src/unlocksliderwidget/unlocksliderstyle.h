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

#include <mwidgetstyle.h>
#include <MScalableImage>

class UnlockSliderStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(UnlockSliderStyle)

    M_STYLE_PTR_ATTRIBUTE(MScalableImage*, handleImage,             HandleImage)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage*, handleHilightedImage,    HandleHilightedImage)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage*, backgroundTileImage,     BackgroundTileImage)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage*, iconImage,               IconImage)
    M_STYLE_ATTRIBUTE(qreal,             thickness,               Thickness)
    M_STYLE_ATTRIBUTE(int,               blinkTime,               BlinkTime)
    M_STYLE_ATTRIBUTE(int,               blinkCount,              BlinkCount)
};

class UnlockSliderStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(UnlockSliderStyle)
};

#endif // UNLOCKSLIDERSTYLE_H
