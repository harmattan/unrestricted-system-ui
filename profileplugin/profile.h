/*
 * profile.h
 *
 * This file is part of duistatusindicatormenu
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

#ifndef PROFILE_H
#define PROFILE_H

#include <DuiWidget>

class DuiApplicationPage;
class DuiButton;
class DuiStatusIndicatorMenuInterface;
class ProfileDataInterface;

/*!
 * The Profile widget makes it possible to select the currently
 * active profile.
 */
class Profile : public DuiWidget
{
    Q_OBJECT

public:
    Profile(DuiStatusIndicatorMenuInterface &statusIndicatorMenu, QGraphicsItem *parent = NULL);
    virtual ~Profile();

private slots:
    /*!
     * \brief A slot for showing the profile duicontrolpanel plugin
     */
    void showProfileModificationPage();

    /*!
     * \brief A slot for receiving information about profile button clicks.
     */
    void buttonClicked(int index);

private:
    //! Interface for controlling the status indicator menu
    DuiStatusIndicatorMenuInterface &statusIndicatorMenu;

    //! Profile data If to receive information about possible profiles
    ProfileDataInterface* dataIf;

    QStringList profileNames;

};

#endif // PROFILE_H
