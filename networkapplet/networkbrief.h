#ifndef NETWORKBRIEF_H
#define NETWORKBRIEF_H

#include "dcpbrief.h"

class NetworkBusinessLogic;

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
    void updateOperatorLabel(const QString &text);
    void updateToggle(bool toggle);
    void updateToggleIcon(const QString &icon);

private:
    NetworkBusinessLogic *logic;
    QString networkText;
    bool networkToggle;

};

#endif // NETWORKBRIEF
