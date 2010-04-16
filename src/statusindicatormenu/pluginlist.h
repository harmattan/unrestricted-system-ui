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

#ifndef PLUGINLIST_H_
#define PLUGINLIST_H_

#include <MWidget>
#include "mstatusindicatormenuplugininterface.h"

class MWindow;
class MApplicationPage;
class QGraphicsLinearLayout;
class NotificationArea;

/*!
 * The plugin list displays a list of plugins instantiated from shared
 * libraries.
 */
class PluginList : public MWidget, public MStatusIndicatorMenuInterface {
    Q_OBJECT

public:
    /*!
     * Creates a new plugin list.
     *
     * \param applicationWindow the application window to minimize if the application should be hidden
     * \param applicationPage the application page to show if the plugin returns from a subpage
     * \param parent the parent item
     */
    PluginList(MWindow *applicationWindow, MApplicationPage *applicationPage, QGraphicsItem *parent = NULL);

    /*!
     * Destroys the plugin list.
     */
    virtual ~PluginList();

    //! \reimp
    virtual void showStatusIndicatorMenu();
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

public slots:
    /*!
     * \brief Slot for hiding thestatus indicator menu.
     */
    virtual void hideStatusIndicatorMenu();

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
    MWindow *applicationWindow;

    //! The application page to show if the plugin returns from a subpage
    MApplicationPage *applicationPage;

    //! The notification area widget
    NotificationArea *notificationArea;

    //! The name of the control panel service
    const static QString CONTROL_PANEL_SERVICE_NAME;
};

#endif /* PLUGINLIST_H_ */
