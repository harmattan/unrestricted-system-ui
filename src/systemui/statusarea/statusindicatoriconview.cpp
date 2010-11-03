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

#include "statusindicatoriconview.h"
#include "statusindicator.h"
#include <MViewCreator>

StatusIndicatorIconView::StatusIndicatorIconView(StatusIndicator *controller) :
    StatusIndicatorAnimationView(controller)
{
}

StatusIndicatorIconView::~StatusIndicatorIconView()
{
}

void StatusIndicatorIconView::updateData(const QList<const char *>& modifications)
{
    StatusIndicatorAnimationView::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == StatusIndicatorModel::Value ||
			member == StatusIndicatorModel::Animate) {
	    int frameNumber = model()->value().toDouble() * images.size();
            // Set the animation frame based on the model value
            setFirstAnimationFrame(frameNumber);
            setAnimationFrame(frameNumber);
        }
    }
}

void StatusIndicatorIconView::applyStyle()
{
    StatusIndicatorAnimationView::applyStyle();

    setupImageList(style()->imageList());

    // Update the animation frame after the image list changed
    QList<const char *> modifications;
    modifications << StatusIndicatorModel::Value;
    updateData(modifications);
}

M_REGISTER_VIEW_NEW(StatusIndicatorIconView, StatusIndicator)
