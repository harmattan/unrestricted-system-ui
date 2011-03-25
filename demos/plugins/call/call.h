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

#ifndef CALL_H
#define CALL_H

#include <MWidget>

class MBasicListItem;
class CallPlugin;

/*!
 * The Call widget makes it possible to select the currently
 * active call.
 */
class Call : public MWidget
{
    Q_OBJECT

public:
    Call(CallPlugin *callPlugin, QGraphicsItem *parent = NULL);
    virtual ~Call();
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

private slots:
    void hideItem();
    void showItem();

private:
    //! The call plugin for accessing the status indicator menu
    CallPlugin *plugin;
    MBasicListItem *item;
};

#endif // CALL_H
