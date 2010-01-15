#ifndef SHUTDOWNUI_H
#define SHUTDOWNUI_H

#include <DuiApplicationPage>

class DuiLabel;

class ShutdownUI : public DuiApplicationPage
{
    Q_OBJECT

public:
    ShutdownUI();
    virtual ~ShutdownUI();
    virtual void createContent();

    static void showWindow();

protected:
    virtual void resizeEvent(QGraphicsSceneResizeEvent* event);
    void alignText();

private:
    DuiLabel* text;
};

#endif // SHUTDOWNUI_H
