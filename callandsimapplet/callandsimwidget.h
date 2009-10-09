#ifndef CALLANDSIMWIDGET_H
#define CALLANDSIMWIDGET_H

#include "dcpwidget.h"

class CallAndSimWidget : public DcpWidget
{
    Q_OBJECT

public:
    CallAndSimWidget(QGraphicsWidget* parent = 0);
    virtual ~CallAndSimWidget();

protected:
    void initWidget();
};

#endif // CALLANDSIMWIDGET_H
