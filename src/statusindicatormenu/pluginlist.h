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
/*
 * This file is part of system ui
 *
 * Copyright (C) 2009 Nokia Corporation. All rights reserved.
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved.
 * Copying, including reproducing, storing, adapting or translating,
 * any or all of this material requires the prior written consent of
 * Nokia Corporation. This material also contains confidential
 * information which may not be disclosed to others without the prior
 * written consent of Nokia.
 */

#ifndef PLUGINLIST_H_
#define PLUGINLIST_H_

#include <DuiWidget>
#include "duistatusindicatormenuplugininterface.h"

class DuiWindow;
class DuiApplicationPage;
class QGraphicsLinearLayout;
class NotificationArea;

/*!
 * The plugin list displays a list of plugins instantiated from shared
 * libraries.
 */
class PluginList : public DuiWidget, public DuiStatusIndicatorMenuInterface {
    Q_OBJECT

public:
    /*!
     * Creates a new plugin list.
     *
     * \param applicationWindow the application window to minimize if the application should be hidden
     * \param applicationPage the application page to show if the plugin returns from a subpage
     * \param parent the parent item
     */
    PluginList(DuiWindow *applicationWindow, DuiApplicationPage *applicationPage, QGraphicsItem *parent = NULL);

    /*!
     * Destroys the plugin list.
     */
    virtual ~PluginList();

    //! \reimp
    virtual void showStatusIndicatorMenu();
    virtual void hideStatusIndicatorMenu();
    //! \reimp_end

private slots:
    /*!
     * \brief Slot for getting information about settings button clicks
     */
    void settingsButtonClicked();

    /*!
     * \brief Sets the visibility of the notification area based on the notification count
     * \param notificationCount the number of notifications visible
     */
    void setNotificationCount(int notificationCount);

private:
    /*!
     * Adds a plugin to the list.
     *
     * \param path the path of the plugin to load
     */
    void addPlugin(const QString &path);

    /*!
     * Adds a settings button to the list.
     */
    void addSettingsButton();

    //! The layout for the list
    QGraphicsLinearLayout *mainLayout;

    //! The application page to minimize if the application should be hidden
    DuiWindow *applicationWindow;

    //! The application page to show if the plugin returns from a subpage
    DuiApplicationPage *applicationPage;

    //! The notification area widget
    NotificationArea *notificationArea;

    //! The name of the control panel service
    const static QString CONTROL_PANEL_SERVICE_NAME;
};

#endif /* PLUGINLIST_H_ */
