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

#ifndef STATUSINDICATORMENUVERTICALVIEW_H_
#define STATUSINDICATORMENUVERTICALVIEW_H_

#include <mscenewindowview.h>
#include "statusindicatormenustyle.h"

class QGraphicsLinearLayout;
class MApplicationExtensionArea;
class MApplicationExtensionInterface;
class StatusIndicatorMenuWindow;
class StatusIndicatorMenu;
class MBasicListItem;
class MWidgetController;

/*!
 * A view class for the status indicator menu with the extension area and the
 * settings button in a vertical layout.
*/
class StatusIndicatorMenuVerticalView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MSceneWindowModel, StatusIndicatorMenuStyle)

public slots:

    //! Sets the tile layout position of the extension's widget to vertical center
    void setExtensionLayoutPosition(MApplicationExtensionInterface *extension);

public:
    /*!
     * Constructs a view for a notification area.
     *
     * \param controller the controller of this StatusIndicatorMenuVerticalView
     */
    StatusIndicatorMenuVerticalView(StatusIndicatorMenu *controller);

    /*!
     * Destroys the notification area view.
     */
    virtual ~StatusIndicatorMenuVerticalView();

private:
    //! Controller for the view
    StatusIndicatorMenu *controller;

    //! Container for the main content
    MWidgetController *containerWidget;

    //! Item for accessing the full settings
    MBasicListItem *settingsItem;

#ifdef UNIT_TEST
    friend class Ut_StatusIndicatorMenuVerticalView;
#endif
};

#endif /* STATUSINDICATORMENUVERTICALVIEW_H_ */
