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

#ifndef NOTIFICATIONAREAMODEL_H_
#define NOTIFICATIONAREAMODEL_H_

#include <MWidgetModel>

class MBanner;

typedef QList<MBanner *> BannerList;

class NotificationAreaModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL(NotificationAreaModel)
    //! Notification MBanner's in statusarea
    M_MODEL_PROPERTY(BannerList, banners, Banners, true, QList<MBanner *>())
};

#endif /* NOTIFICATIONAREAMODEL_H_ */
