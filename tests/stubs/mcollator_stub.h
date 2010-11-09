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

#ifndef MCOLLATOR_STUB
#define MCOLLATOR_STUB

#include "mcollator.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MCollatorStub : public StubBase
{
public:
    virtual void MCollatorConstructor();
    virtual void MCollatorConstructor(const MLocale &locale);
    virtual void MCollatorConstructor(const MCollator &other);
    virtual void MCollatorDestructor();
    virtual bool operator()(const QString &s1, const QString &s2) const;
    virtual MLocale::Comparison compare(const QString &first, const QString &second);
    virtual MLocale::Comparison compare(MLocale &locale, const QString &first, const QString &second);
};

// 2. IMPLEMENT STUB
void MCollatorStub::MCollatorConstructor()
{

}
void MCollatorStub::MCollatorConstructor(const MLocale &locale)
{
    Q_UNUSED(locale);

}
void MCollatorStub::MCollatorConstructor(const MCollator &other)
{
    Q_UNUSED(other);

}
void MCollatorStub::MCollatorDestructor()
{

}
bool MCollatorStub::operator()(const QString &s1, const QString &s2) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(s1));
    params.append(new Parameter<const QString & >(s2));
    stubMethodEntered("operator()", params);
    return stubReturnValue<bool>("operator()");
}

MLocale::Comparison MCollatorStub::compare(const QString &first, const QString &second)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(first));
    params.append(new Parameter<const QString & >(second));
    stubMethodEntered("compare", params);
    return stubReturnValue<MLocale::Comparison>("compare");
}

MLocale::Comparison MCollatorStub::compare(MLocale &locale, const QString &first, const QString &second)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MLocale & >(locale));
    params.append(new Parameter<const QString & >(first));
    params.append(new Parameter<const QString & >(second));
    stubMethodEntered("compare", params);
    return stubReturnValue<MLocale::Comparison>("compare");
}



// 3. CREATE A STUB INSTANCE
MCollatorStub gDefaultMCollatorStub;
MCollatorStub *gMCollatorStub = &gDefaultMCollatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MCollator::MCollator() :
    d_ptr(0)
{
    gMCollatorStub->MCollatorConstructor();
}

MCollator::MCollator(const MLocale &locale) :
    d_ptr(0)
{
    gMCollatorStub->MCollatorConstructor(locale);
}

MCollator::MCollator(const MCollator &other) :
    d_ptr(0)
{
    gMCollatorStub->MCollatorConstructor(other);
}

MCollator::~MCollator()
{
    gMCollatorStub->MCollatorDestructor();
}

bool MCollator::operator()(const QString &s1, const QString &s2) const
{
    return gMCollatorStub->operator()(s1, s2);
}

MLocale::Comparison MCollator::compare(const QString &first, const QString &second)
{
    return gMCollatorStub->compare(first, second);
}

MLocale::Comparison MCollator::compare(MLocale &locale, const QString &first, const QString &second)
{
    return gMCollatorStub->compare(locale, first, second);
}


#endif
