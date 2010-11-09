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

#ifndef MLOCALE_STUB
#define MLOCALE_STUB

#include "mlocale.h"
#include "mcollator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MLocaleStub : public StubBase
{
public:
    virtual MLocale *createSystemMLocale();
    virtual MLocale createCLocale();
    virtual void MLocaleConstructor(QObject *parent);
    virtual void MLocaleConstructor(const QString &localeName, QObject *parent);
    virtual void MLocaleConstructor(const MLocale &other, QObject *parent);
    virtual void MLocaleDestructor();
    virtual MLocale operatorEquals(const MLocale &other);
    virtual void setDefault(const MLocale &locale);
    virtual bool isValid() const;
    virtual void setCategoryLocale(MLocale::Category category, const QString &localeName);
    virtual void setCollation(MLocale::Collation collation);
    virtual MLocale::Collation collation() const;
    virtual void setCalendarType(MLocale::CalendarType calendarType);
    virtual MLocale::CalendarType calendarType() const;
    virtual MLocale::TimeFormat24h timeFormat24h() const;
    virtual MLocale::TimeFormat24h defaultTimeFormat24h() const;
    virtual MCollator collator() const;
    virtual QString languageEndonym() const;
    virtual QString countryEndonym() const;
    virtual QString language() const;
    virtual QString country() const;
    virtual QString script() const;
    virtual QString variant() const;
    virtual QString name() const;
    virtual Qt::LayoutDirection textDirection() const;
    virtual QString categoryLanguage(MLocale::Category category) const;
    virtual QString categoryCountry(MLocale::Category category) const;
    virtual QString categoryVariant(MLocale::Category category) const;
    virtual QString categoryName(MLocale::Category category) const;
    virtual QString formatNumber(qlonglong i) const;
    virtual QString formatNumber(short i) const;
    virtual QString formatNumber(int i) const;
    virtual QString formatNumber(double i, int precision) const;
    virtual QString formatNumber(float i) const;
    virtual QString formatPercent(double i, int decimals) const;
    virtual QString formatCurrency(double amount, const QString &currency) const;
    virtual QString formatDateTime(const QDateTime &dateTime, MLocale::DateType dateType, MLocale::TimeType timeType, MLocale::CalendarType calendarType) const;
    virtual QString formatDateTime(const QDateTime &dateTime, MLocale::CalendarType calendarType) const;
    virtual QString formatDateTime(const MCalendar &mCalendar, MLocale::DateType datetype, MLocale::TimeType timetype) const;
    virtual QString formatDateTimeICU(const QDateTime &dateTime, const QString &formatString) const;
    virtual QString formatDateTimeICU(const MCalendar &mCalendar, const QString &formatString) const;
    virtual QString formatDateTime(const QDateTime &dateTime, const QString &formatString) const;
    virtual QString formatDateTime(const MCalendar &mCalendar, const QString &formatString) const;
    virtual QString monthName(const MCalendar &mCalendar, int monthNumber) const;
    virtual QString monthName(const MCalendar &mCalendar, int monthNumber, MLocale::DateSymbolContext context, MLocale::DateSymbolLength symbolLength) const;
    virtual QString weekdayName(const MCalendar &mCalendar, int weekday) const;
    virtual QString weekdayName(const MCalendar &mCalendar, int weekday, MLocale::DateSymbolContext context, MLocale::DateSymbolLength symbolLength) const;
    virtual void installCategoryCatalog(MLocale::Category category, const QString &name);
    virtual void removeCategoryCatalog(MLocale::Category category, const QString &name);
    virtual void copyCatalogsFrom(const MLocale &other);
    virtual bool hasCategoryCatalog(MLocale::Category category, const QString &name);
    virtual void installTrCatalog(const QString &name);
    virtual QString translate(const char *context, const char *sourceText, const char *comment, int n);
    virtual void setDataPaths(const QStringList &dataPaths);
    virtual void setDataPath(const QString &dataPath);
    virtual QStringList dataPaths();
    virtual void setTranslationPaths(const QStringList &paths);
    virtual void addTranslationPath(const QString &path);
    virtual void removeTranslationPath(const QString &path);
    virtual QStringList translationPaths();
    virtual void connectSettings();
    virtual void disconnectSettings();
    virtual void insertTrToQCoreApp();
    virtual void removeTrFromQCoreApp();
    virtual MLocale getDefault();
    virtual bool operator==(const MLocale &other) const;
    virtual bool operator!=(const MLocale &other) const;
    virtual void refreshSettings();
};

// 2. IMPLEMENT STUB
MLocale *MLocaleStub::createSystemMLocale()
{
    stubMethodEntered("createSystemMLocale");
    return stubReturnValue<MLocale *>("createSystemMLocale");
}

MLocale MLocaleStub::createCLocale()
{
    stubMethodEntered("createCLocale");
    return stubReturnValue<MLocale>("createCLocale");
}

void MLocaleStub::MLocaleConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void MLocaleStub::MLocaleConstructor(const QString &localeName, QObject *parent)
{
    Q_UNUSED(localeName);
    Q_UNUSED(parent);

}
void MLocaleStub::MLocaleConstructor(const MLocale &other, QObject *parent)
{
    Q_UNUSED(other);
    Q_UNUSED(parent);

}
void MLocaleStub::MLocaleDestructor()
{

}
MLocale MLocaleStub::operatorEquals(const MLocale &other)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MLocale & >(other));
    stubMethodEntered("operator=", params);
    return stubReturnValue<MLocale >("operator=");
}

void MLocaleStub::setDefault(const MLocale &locale)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MLocale & >(locale));
    stubMethodEntered("setDefault", params);
}

bool MLocaleStub::isValid() const
{
    stubMethodEntered("isValid");
    return stubReturnValue<bool>("isValid");
}

void MLocaleStub::setCategoryLocale(MLocale::Category category, const QString &localeName)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MLocale::Category >(category));
    params.append(new Parameter<const QString & >(localeName));
    stubMethodEntered("setCategoryLocale", params);
}

void MLocaleStub::setCollation(MLocale::Collation collation)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MLocale::Collation >(collation));
    stubMethodEntered("setCollation", params);
}

MLocale::Collation MLocaleStub::collation() const
{
    stubMethodEntered("collation");
    return stubReturnValue<MLocale::Collation>("collation");
}

void MLocaleStub::setCalendarType(MLocale::CalendarType calendarType)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MLocale::CalendarType >(calendarType));
    stubMethodEntered("setCalendarType", params);
}

MLocale::CalendarType MLocaleStub::calendarType() const
{
    stubMethodEntered("calendarType");
    return stubReturnValue<MLocale::CalendarType>("calendarType");
}

MLocale::TimeFormat24h MLocaleStub::timeFormat24h() const
{
    stubMethodEntered("timeFormat24h");
    return stubReturnValue<MLocale::TimeFormat24h>("timeFormat24h");
}

MLocale::TimeFormat24h MLocaleStub::defaultTimeFormat24h() const
{
    stubMethodEntered("defaultTimeFormat24h");
    return stubReturnValue<MLocale::TimeFormat24h>("defaultTimeFormat24h");
}

MCollator MLocaleStub::collator() const
{
    stubMethodEntered("collator");
    return stubReturnValue<MCollator>("collator");
}

QString MLocaleStub::languageEndonym() const
{
    stubMethodEntered("languageEndonym");
    return stubReturnValue<QString>("languageEndonym");
}

QString MLocaleStub::countryEndonym() const
{
    stubMethodEntered("countryEndonym");
    return stubReturnValue<QString>("countryEndonym");
}

QString MLocaleStub::language() const
{
    stubMethodEntered("language");
    return stubReturnValue<QString>("language");
}

QString MLocaleStub::country() const
{
    stubMethodEntered("country");
    return stubReturnValue<QString>("country");
}

QString MLocaleStub::script() const
{
    stubMethodEntered("script");
    return stubReturnValue<QString>("script");
}

QString MLocaleStub::variant() const
{
    stubMethodEntered("variant");
    return stubReturnValue<QString>("variant");
}

QString MLocaleStub::name() const
{
    stubMethodEntered("name");
    return stubReturnValue<QString>("name");
}

Qt::LayoutDirection MLocaleStub::textDirection() const
{
    stubMethodEntered("textDirection");
    return stubReturnValue<Qt::LayoutDirection>("textDirection");
}

QString MLocaleStub::categoryLanguage(MLocale::Category category) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MLocale::Category >(category));
    stubMethodEntered("categoryLanguage", params);
    return stubReturnValue<QString>("categoryLanguage");
}

QString MLocaleStub::categoryCountry(MLocale::Category category) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MLocale::Category >(category));
    stubMethodEntered("categoryCountry", params);
    return stubReturnValue<QString>("categoryCountry");
}

QString MLocaleStub::categoryVariant(MLocale::Category category) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MLocale::Category >(category));
    stubMethodEntered("categoryVariant", params);
    return stubReturnValue<QString>("categoryVariant");
}

QString MLocaleStub::categoryName(MLocale::Category category) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MLocale::Category >(category));
    stubMethodEntered("categoryName", params);
    return stubReturnValue<QString>("categoryName");
}

QString MLocaleStub::formatNumber(qlonglong i) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<qlonglong >(i));
    stubMethodEntered("formatNumber", params);
    return stubReturnValue<QString>("formatNumber");
}

QString MLocaleStub::formatNumber(short i) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<short >(i));
    stubMethodEntered("formatNumber", params);
    return stubReturnValue<QString>("formatNumber");
}

QString MLocaleStub::formatNumber(int i) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(i));
    stubMethodEntered("formatNumber", params);
    return stubReturnValue<QString>("formatNumber");
}

QString MLocaleStub::formatNumber(double i, int precision) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<double >(i));
    params.append(new Parameter<int >(precision));
    stubMethodEntered("formatNumber", params);
    return stubReturnValue<QString>("formatNumber");
}

QString MLocaleStub::formatNumber(float i) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<float >(i));
    stubMethodEntered("formatNumber", params);
    return stubReturnValue<QString>("formatNumber");
}

QString MLocaleStub::formatPercent(double i, int decimals) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<double >(i));
    params.append(new Parameter<int >(decimals));
    stubMethodEntered("formatPercent", params);
    return stubReturnValue<QString>("formatPercent");
}

QString MLocaleStub::formatCurrency(double amount, const QString &currency) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<double >(amount));
    params.append(new Parameter<const QString & >(currency));
    stubMethodEntered("formatCurrency", params);
    return stubReturnValue<QString>("formatCurrency");
}

QString MLocaleStub::formatDateTime(const QDateTime &dateTime, MLocale::DateType dateType, MLocale::TimeType timeType, MLocale::CalendarType calendarType) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QDateTime & >(dateTime));
    params.append(new Parameter<MLocale::DateType >(dateType));
    params.append(new Parameter<MLocale::TimeType >(timeType));
    params.append(new Parameter<MLocale::CalendarType >(calendarType));
    stubMethodEntered("formatDateTime", params);
    return stubReturnValue<QString>("formatDateTime");
}

QString MLocaleStub::formatDateTime(const QDateTime &dateTime, MLocale::CalendarType calendarType) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QDateTime & >(dateTime));
    params.append(new Parameter<MLocale::CalendarType >(calendarType));
    stubMethodEntered("formatDateTime", params);
    return stubReturnValue<QString>("formatDateTime");
}

QString MLocaleStub::formatDateTime(const MCalendar &mCalendar, MLocale::DateType datetype, MLocale::TimeType timetype) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MCalendar & >(mCalendar));
    params.append(new Parameter<MLocale::DateType >(datetype));
    params.append(new Parameter<MLocale::TimeType >(timetype));
    stubMethodEntered("formatDateTime", params);
    return stubReturnValue<QString>("formatDateTime");
}

QString MLocaleStub::formatDateTimeICU(const QDateTime &dateTime, const QString &formatString) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QDateTime & >(dateTime));
    params.append(new Parameter<const QString & >(formatString));
    stubMethodEntered("formatDateTimeICU", params);
    return stubReturnValue<QString>("formatDateTimeICU");
}

QString MLocaleStub::formatDateTimeICU(const MCalendar &mCalendar, const QString &formatString) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MCalendar & >(mCalendar));
    params.append(new Parameter<const QString & >(formatString));
    stubMethodEntered("formatDateTimeICU", params);
    return stubReturnValue<QString>("formatDateTimeICU");
}

QString MLocaleStub::formatDateTime(const QDateTime &dateTime, const QString &formatString) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QDateTime>(dateTime));
    params.append(new Parameter<QString>(formatString));
    stubMethodEntered("formatDateTime", params);
    return stubReturnValue<QString>("formatDateTime");
}

QString MLocaleStub::formatDateTime(const MCalendar &mCalendar, const QString &formatString) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MCalendar & >(mCalendar));
    params.append(new Parameter<const QString & >(formatString));
    stubMethodEntered("formatDateTime", params);
    return stubReturnValue<QString>("formatDateTime");
}

QString MLocaleStub::monthName(const MCalendar &mCalendar, int monthNumber) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MCalendar & >(mCalendar));
    params.append(new Parameter<int >(monthNumber));
    stubMethodEntered("monthName", params);
    return stubReturnValue<QString>("monthName");
}

QString MLocaleStub::monthName(const MCalendar &mCalendar, int monthNumber, MLocale::DateSymbolContext context, MLocale::DateSymbolLength symbolLength) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MCalendar & >(mCalendar));
    params.append(new Parameter<int >(monthNumber));
    params.append(new Parameter<MLocale::DateSymbolContext >(context));
    params.append(new Parameter<MLocale::DateSymbolLength >(symbolLength));
    stubMethodEntered("monthName", params);
    return stubReturnValue<QString>("monthName");
}

QString MLocaleStub::weekdayName(const MCalendar &mCalendar, int weekday) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MCalendar & >(mCalendar));
    params.append(new Parameter<int >(weekday));
    stubMethodEntered("weekdayName", params);
    return stubReturnValue<QString>("weekdayName");
}

QString MLocaleStub::weekdayName(const MCalendar &mCalendar, int weekday, MLocale::DateSymbolContext context, MLocale::DateSymbolLength symbolLength) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MCalendar & >(mCalendar));
    params.append(new Parameter<int >(weekday));
    params.append(new Parameter<MLocale::DateSymbolContext >(context));
    params.append(new Parameter<MLocale::DateSymbolLength >(symbolLength));
    stubMethodEntered("weekdayName", params);
    return stubReturnValue<QString>("weekdayName");
}

void MLocaleStub::installCategoryCatalog(MLocale::Category category, const QString &name)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MLocale::Category >(category));
    params.append(new Parameter<const QString & >(name));
    stubMethodEntered("installCategoryCatalog", params);
}

void MLocaleStub::removeCategoryCatalog(MLocale::Category category, const QString &name)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MLocale::Category >(category));
    params.append(new Parameter<const QString & >(name));
    stubMethodEntered("removeCategoryCatalog", params);
}

void MLocaleStub::copyCatalogsFrom(const MLocale &other)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MLocale & >(other));
    stubMethodEntered("copyCatalogsFrom", params);
}

bool MLocaleStub::hasCategoryCatalog(MLocale::Category category, const QString &name)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MLocale::Category >(category));
    params.append(new Parameter<const QString & >(name));
    stubMethodEntered("hasCategoryCatalog", params);
    return stubReturnValue<bool>("hasCategoryCatalog");
}

void MLocaleStub::installTrCatalog(const QString &name)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString >(name));
    stubMethodEntered("installTrCatalog", params);
}

QString MLocaleStub::translate(const char *context, const char *sourceText, const char *comment, int n)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const char * >(context));
    params.append(new Parameter<const char * >(sourceText));
    params.append(new Parameter<const char * >(comment));
    params.append(new Parameter<int >(n));
    stubMethodEntered("translate", params);
    return stubReturnValue<QString>("translate");
}

void MLocaleStub::setDataPaths(const QStringList &dataPaths)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QStringList & >(dataPaths));
    stubMethodEntered("setDataPaths", params);
}

void MLocaleStub::setDataPath(const QString &dataPath)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(dataPath));
    stubMethodEntered("setDataPath", params);
}

QStringList MLocaleStub::dataPaths()
{
    stubMethodEntered("dataPaths");
    return stubReturnValue<QStringList>("dataPaths");
}

void MLocaleStub::setTranslationPaths(const QStringList &paths)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QStringList & >(paths));
    stubMethodEntered("setTranslationPaths", params);
}

void MLocaleStub::addTranslationPath(const QString &path)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(path));
    stubMethodEntered("addTranslationPath", params);
}

void MLocaleStub::removeTranslationPath(const QString &path)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(path));
    stubMethodEntered("removeTranslationPath", params);
}

QStringList MLocaleStub::translationPaths()
{
    stubMethodEntered("translationPaths");
    return stubReturnValue<QStringList>("translationPaths");
}

void MLocaleStub::connectSettings()
{
    stubMethodEntered("connectSettings");
}

void MLocaleStub::disconnectSettings()
{
    stubMethodEntered("disconnectSettings");
}

void MLocaleStub::insertTrToQCoreApp()
{
    stubMethodEntered("insertTrToQCoreApp");
}

void MLocaleStub::removeTrFromQCoreApp()
{
    stubMethodEntered("removeTrFromQCoreApp");
}

MLocale MLocaleStub::getDefault()
{
    stubMethodEntered("getDefault");
    return stubReturnValue<MLocale>("getDefault");
}

bool MLocaleStub::operator==(const MLocale &other) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MLocale & >(other));
    stubMethodEntered("operator==", params);
    return stubReturnValue<bool>("operator==");
}

bool MLocaleStub::operator!=(const MLocale &other) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MLocale & >(other));
    stubMethodEntered("operator!=", params);
    return stubReturnValue<bool>("operator!=");
}

void MLocaleStub::refreshSettings()
{
    stubMethodEntered("refreshSettings");
}



// 3. CREATE A STUB INSTANCE
MLocaleStub gDefaultMLocaleStub;
MLocaleStub *gMLocaleStub = &gDefaultMLocaleStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MLocale *MLocale::createSystemMLocale()
{
    return gMLocaleStub->createSystemMLocale();
}

MLocale MLocale::createCLocale()
{
    return gMLocaleStub->createCLocale();
}

MLocale::MLocale(QObject *parent) : QObject(parent), d_ptr(NULL)
{
    gMLocaleStub->MLocaleConstructor(parent);
}

MLocale::MLocale(const QString &localeName, QObject *parent) : QObject(parent), d_ptr(NULL)
{
    gMLocaleStub->MLocaleConstructor(localeName, parent);
}

MLocale::MLocale(const MLocale &other, QObject *parent) : QObject(parent), d_ptr(NULL)
{
    gMLocaleStub->MLocaleConstructor(other, parent);
}

MLocale::~MLocale()
{
    gMLocaleStub->MLocaleDestructor();
}

MLocale &MLocale::operator=(const MLocale &other)
{
    static MLocale equals = gMLocaleStub->operatorEquals(other);
    return equals;
}

void MLocale::setDefault(const MLocale &locale)
{
    gMLocaleStub->setDefault(locale);
}

bool MLocale::isValid() const
{
    return gMLocaleStub->isValid();
}

void MLocale::setCategoryLocale(MLocale::Category category, const QString &localeName)
{
    gMLocaleStub->setCategoryLocale(category, localeName);
}

void MLocale::setCollation(MLocale::Collation collation)
{
    gMLocaleStub->setCollation(collation);
}

MLocale::Collation MLocale::collation() const
{
    return gMLocaleStub->collation();
}

void MLocale::setCalendarType(MLocale::CalendarType calendarType)
{
    gMLocaleStub->setCalendarType(calendarType);
}

MLocale::CalendarType MLocale::calendarType() const
{
    return gMLocaleStub->calendarType();
}

MLocale::TimeFormat24h MLocale::timeFormat24h() const
{
    return gMLocaleStub->timeFormat24h();
}

MLocale::TimeFormat24h MLocale::defaultTimeFormat24h() const
{
    return gMLocaleStub->defaultTimeFormat24h();
}

MCollator MLocale::collator() const
{
    return gMLocaleStub->collator();
}

QString MLocale::languageEndonym() const
{
    return gMLocaleStub->languageEndonym();
}

QString MLocale::countryEndonym() const
{
    return gMLocaleStub->countryEndonym();
}

QString MLocale::language() const
{
    return gMLocaleStub->language();
}

QString MLocale::country() const
{
    return gMLocaleStub->country();
}

QString MLocale::script() const
{
    return gMLocaleStub->script();
}

QString MLocale::variant() const
{
    return gMLocaleStub->variant();
}

QString MLocale::name() const
{
    return gMLocaleStub->name();
}

Qt::LayoutDirection MLocale::textDirection() const
{
    return gMLocaleStub->textDirection();
}

QString MLocale::categoryLanguage(MLocale::Category category) const
{
    return gMLocaleStub->categoryLanguage(category);
}

QString MLocale::categoryCountry(MLocale::Category category) const
{
    return gMLocaleStub->categoryCountry(category);
}

QString MLocale::categoryVariant(MLocale::Category category) const
{
    return gMLocaleStub->categoryVariant(category);
}

QString MLocale::categoryName(MLocale::Category category) const
{
    return gMLocaleStub->categoryName(category);
}

QString MLocale::formatNumber(qlonglong i) const
{
    return gMLocaleStub->formatNumber(i);
}

QString MLocale::formatNumber(short i) const
{
    return gMLocaleStub->formatNumber(i);
}

QString MLocale::formatNumber(int i) const
{
    return gMLocaleStub->formatNumber(i);
}

QString MLocale::formatNumber(double i, int precision) const
{
    return gMLocaleStub->formatNumber(i, precision);
}

QString MLocale::formatNumber(float i) const
{
    return gMLocaleStub->formatNumber(i);
}

QString MLocale::formatPercent(double i, int decimals) const
{
    return gMLocaleStub->formatPercent(i, decimals);
}

QString MLocale::formatCurrency(double amount, const QString &currency) const
{
    return gMLocaleStub->formatCurrency(amount, currency);
}

QString MLocale::formatDateTime(const QDateTime &dateTime, MLocale::DateType dateType, MLocale::TimeType timeType, MLocale::CalendarType calendarType) const
{
    return gMLocaleStub->formatDateTime(dateTime, dateType, timeType, calendarType);
}

QString MLocale::formatDateTime(const QDateTime &dateTime, MLocale::CalendarType calendarType) const
{
    return gMLocaleStub->formatDateTime(dateTime, calendarType);
}

QString MLocale::formatDateTime(const MCalendar &mCalendar, MLocale::DateType datetype, MLocale::TimeType timetype) const
{
    return gMLocaleStub->formatDateTime(mCalendar, datetype, timetype);
}

QString MLocale::formatDateTimeICU(const QDateTime &dateTime, const QString &formatString) const
{
    return gMLocaleStub->formatDateTimeICU(dateTime, formatString);
}

QString MLocale::formatDateTimeICU(const MCalendar &mCalendar, const QString &formatString) const
{
    return gMLocaleStub->formatDateTimeICU(mCalendar, formatString);
}

QString MLocale::formatDateTime(const QDateTime &dateTime, const QString &formatString) const
{
    return gMLocaleStub->formatDateTime(dateTime, formatString);
}

QString MLocale::formatDateTime(const MCalendar &mCalendar, const QString &formatString) const
{
    return gMLocaleStub->formatDateTime(mCalendar, formatString);
}

QString MLocale::monthName(const MCalendar &mCalendar, int monthNumber) const
{
    return gMLocaleStub->monthName(mCalendar, monthNumber);
}

QString MLocale::monthName(const MCalendar &mCalendar, int monthNumber, MLocale::DateSymbolContext context, MLocale::DateSymbolLength symbolLength) const
{
    return gMLocaleStub->monthName(mCalendar, monthNumber, context, symbolLength);
}

QString MLocale::weekdayName(const MCalendar &mCalendar, int weekday) const
{
    return gMLocaleStub->weekdayName(mCalendar, weekday);
}

QString MLocale::weekdayName(const MCalendar &mCalendar, int weekday, MLocale::DateSymbolContext context, MLocale::DateSymbolLength symbolLength) const
{
    return gMLocaleStub->weekdayName(mCalendar, weekday, context, symbolLength);
}

void MLocale::copyCatalogsFrom(const MLocale &other)
{
    gMLocaleStub->copyCatalogsFrom(other);
}

void MLocale::installTrCatalog(const QString &name)
{
    gMLocaleStub->installTrCatalog(name);
}

QString MLocale::translate(const char *context, const char *sourceText, const char *comment, int n)
{
    return gMLocaleStub->translate(context, sourceText, comment, n);
}

void MLocale::setDataPaths(const QStringList &dataPaths)
{
    gMLocaleStub->setDataPaths(dataPaths);
}

void MLocale::setDataPath(const QString &dataPath)
{
    gMLocaleStub->setDataPath(dataPath);
}

void MLocale::setTranslationPaths(const QStringList &paths)
{
    gMLocaleStub->setTranslationPaths(paths);
}

void MLocale::addTranslationPath(const QString &path)
{
    gMLocaleStub->addTranslationPath(path);
}

void MLocale::removeTranslationPath(const QString &path)
{
    gMLocaleStub->removeTranslationPath(path);
}

QStringList MLocale::translationPaths()
{
    return gMLocaleStub->translationPaths();
}

void MLocale::connectSettings()
{
    gMLocaleStub->connectSettings();
}

void MLocale::disconnectSettings()
{
    gMLocaleStub->disconnectSettings();
}

MLocale &MLocale::getDefault()
{
    static MLocale def = gMLocaleStub->getDefault();
    return def;
}

bool MLocale::operator==(const MLocale &other) const
{
    return gMLocaleStub->operator==(other);
}

bool MLocale::operator!=(const MLocale &other) const
{
    return gMLocaleStub->operator!=(other);
}

void MLocale::refreshSettings()
{
    gMLocaleStub->refreshSettings();
}


#endif
