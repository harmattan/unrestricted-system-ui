#include "gconfstub.h"

SystemUIGConf::SystemUIGConf(QObject* parent) :
        QObject(parent)
{
}

SystemUIGConf::~SystemUIGConf()
{
}

int SystemUIGConf::keyCount(SystemUIGConf::GConfKeyGroup keyGroup)
{
    Q_UNUSED(keyGroup);
    return 0;
}

void SystemUIGConf::setValue(SystemUIGConf::GConfKey key, QVariant value)
{
    GConfValues.insert(key, value);
}

QVariant SystemUIGConf::value(SystemUIGConf::GConfKey key, QVariant def)
{
    if(GConfValues.contains(key))
        return GConfValues.value(key);
    else
        return def;   
}
