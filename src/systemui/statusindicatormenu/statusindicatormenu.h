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

#ifndef STATUSINDICATORMENU_H_
#define STATUSINDICATORMENU_H_

#include <MApplicationMenu>
#include "mstatusindicatormenuextensioninterface.h"

class StatusIndicatorMenuWindow;

/*!
 * The StatusIndicatorMenu shows the status indicator menu extensions
 */
class StatusIndicatorMenu : public MApplicationMenu, public MStatusIndicatorMenuInterface
{
    Q_OBJECT

public:
    /*!
     * Constructs a StatusIndicatorMenu widget.
     *
     */
    StatusIndicatorMenu(QGraphicsItem *parent = NULL);

    /*!
     * Destroys the StatusIndicatorMenu.
     */
    virtual ~StatusIndicatorMenu();

    //! \reimp
    virtual void showStatusIndicatorMenu();
    virtual void hideStatusIndicatorMenu();
    //! \reimp_end

signals:

    /*!
     * Emitted when the status indicator menu should be shown
     */
    void showRequested();

    /*!
     * Emitted when the status indicator menu should be hidden
     */
    void hideRequested();

private slots:

    /*!
     * \brief Sets the status indicator menu interface for the application extensions
     */
    void setStatusIndicatorMenuInterface(MApplicationExtensionInterface *extension);

    /*!
     * \brief Slot for getting information about settings button clicks
     */
    void launchControlPanelAndHide();

private: // methods

    //! The name of the control panel service
    const static QString CONTROL_PANEL_SERVICE_NAME;

#ifdef UNIT_TEST
    friend class Ut_StatusIndicatorMenu;
#endif
};

#endif /* STATUSINDICATORMENU_H_ */
