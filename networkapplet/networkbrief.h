#ifndef NETWORKBRIEF_H
#define NETWORKBRIEF_H

#include "dcpbrief.h"

class NetworkDBusInterface;

class NetworkBrief: public DcpBrief
{
    Q_OBJECT

public:
    NetworkBrief();
    ~NetworkBrief();
    virtual QString valueText() const;
    virtual int widgetTypeID() const;
    virtual bool toggle() const;

private slots:
    void changeText(const QString &text);
    void changeToggle(bool toggle);
    void changeIcon(const QString &icon);

private:
    NetworkDBusInterface *networkIf;
    QString networkText;
    bool networkToggle;

};

#endif // NETWORKBRIEF
