#ifndef MGCONFITEM_STUB_H
#define MGCONFITEM_STUB_H

#include <MGConfItem>
#include <stubbase.h>

// 1. DECLARE STUB
class MGConfItemStub: public StubBase {
public:
    virtual void MGConfItemConstructor(QString key);
    virtual void MGConfItemDestructor();
    virtual QString key() const;
    virtual QVariant value() const;
    virtual QVariant valueTest(const QVariant& def) const;
    virtual void set(const QVariant& val);
    virtual void unset();
    virtual QList<QString> listDirs() const;
    virtual QList<QString> listEntries() const;
private:
    QString gconfKey;
};

// 2. IMPLEMENT STUB
void MGConfItemStub::MGConfItemConstructor(QString key) {
    gconfKey = key;
}

void MGConfItemStub::MGConfItemDestructor() {
}

QString MGConfItemStub::key() const {
    stubMethodEntered("key");
    return gconfKey;
}

QVariant MGConfItemStub::value() const {
    stubMethodEntered("value");
    return stubReturnValue<QVariant>("value");
}

QVariant MGConfItemStub::valueTest(const QVariant& def) const {
    QList<ParameterBase*> params;
    params.append( new Parameter<const QVariant&>(def));
    stubMethodEntered("valueTest",params);
    return def;
}

void MGConfItemStub::set(const QVariant& val) {
    QList<ParameterBase*> params;
    params.append( new Parameter<const QVariant&>(val));
    stubMethodEntered("set",params);
}

void MGConfItemStub::unset() {
    stubMethodEntered("unset");
}

QList<QString> MGConfItemStub::listDirs() const {
    stubMethodEntered("listDirs");
    return stubReturnValue<QList<QString> >("listDirs");

}

QList<QString> MGConfItemStub::listEntries() const {
    stubMethodEntered("listEntries");
    return stubReturnValue<QList<QString> >("listEntries");

}

// 3. CREATE STUB INSTANCE
MGConfItemStub gDefaultMGConfItemStub;
MGConfItemStub* gMGConfItemStub = &gDefaultMGConfItemStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
MGConfItem::MGConfItem(const QString &key, QObject *parent) {
    Q_UNUSED(parent);
    gMGConfItemStub->MGConfItemConstructor(key);
}

MGConfItem::~MGConfItem() {
    gMGConfItemStub->MGConfItemDestructor();
}

QString MGConfItem::key() const {
    return gMGConfItemStub->key();
}

QVariant MGConfItem::value() const {
    return gMGConfItemStub->value();
}

QVariant MGConfItem::value(const QVariant &def) const {
    return gMGConfItemStub->valueTest(def);
}

void MGConfItem::set(const QVariant &val) {
    gMGConfItemStub->set(val);
}

void MGConfItem::unset() {
    gMGConfItemStub->unset();
}

QList<QString> MGConfItem::listDirs() const {
    return gMGConfItemStub->listDirs();
}

QList<QString> MGConfItem::listEntries() const {
    return gMGConfItemStub->listEntries();
}

#endif // MGCONFITEM_STUB_H

