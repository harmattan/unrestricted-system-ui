#ifndef OFFLINE_H
#define OFFLINE_H

#include <DuiWidget>

class DuiStatusIndicatorMenuInterface;
class QGraphicsItem;
class DuiLabel;
class DuiButton;

class Offline : public DuiWidget
{
    Q_OBJECT

public:
    Offline(DuiStatusIndicatorMenuInterface& statusIndicatorMenu, QGraphicsItem* parent = NULL);
    virtual ~Offline();

private slots:

private:
    DuiStatusIndicatorMenuInterface& statusIndicatorMenu;

    DuiLabel* label;
    DuiButton* button;
};

#endif // OFFLINE_H
