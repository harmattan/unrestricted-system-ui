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

#ifndef CONTEXTFRAMEWORKCONTEXT_H
#define CONTEXTFRAMEWORKCONTEXT_H

#include "applicationcontext.h"

#ifdef HAVE_CONTEXTSUBSCRIBER
#include <contextproperty.h>
#endif

/*!
 * Concrete ApplicationContext that creates ContextFrameworkItems from the
 * given context framework key.
 */
class ContextFrameworkContext : public ApplicationContext
{
public:
    /*!
     * \copydoc ApplicationContext::createContextItem()
     */
    ContextItem *createContextItem(const QString &key);
};

/*!
 * Context item that monitors a given context framework item.
 */
class ContextFrameworkItem : public ContextItem
{
    Q_OBJECT

public:
    /*!
     * Default constructor. Creates a new ContextFrameworkItem to
     * track a given ContextProperty.
     * \param key The key of the ContextProperty of context framework to be tracked
     * by this ContextFrameworkItem.
     */
    ContextFrameworkItem(const QString &key);

    /*!
     * \copydoc ContextItem::value()
     */
    QVariant value() const;

    /*!
     * \copydoc ContextItem::subscribe()
     */
    void subscribe() const;

    /*!
     * \copydoc ContextItem::unsubscribe()
     */
    void unsubscribe() const;

private:
#ifdef HAVE_CONTEXTSUBSCRIBER
    //! The context property being tracked by this item
    ContextProperty property;
#endif
};

#endif // CONTEXTFRAMEWORKCONTEXT_H
