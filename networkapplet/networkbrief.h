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
    virtual QString toggleIconId() const;        

private slots:
    void updateOperatorLabel(const QString &text);
    void updateToggleIcon(const QString &icon);
    virtual void setToggle (bool toggle);

private:
    void updateToggle(bool toggle);

private:
    NetworkBusinessLogic *logic;
    QString networkText;
    bool networkToggle;
    QString networkToggleIcon;

};

#endif // NETWORKBRIEF
