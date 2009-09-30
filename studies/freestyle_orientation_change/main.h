#ifndef MAIN_H
#define MAIN_H

#include <QObject>
#include <QPointer>
#include <QTimer>
#include <DuiApplicationPage>

class DuiButton;

/**
 * TestPage
 */
class  TestPage : public DuiApplicationPage
{
    Q_OBJECT

public:
    TestPage();
    virtual ~ TestPage();

    virtual void createContent();
    DuiButton* getButton();

public slots:
    void orientationChanged(const Dui::Orientation &orientation);

private:
    void createGridLayout();
    void createFreestyleLayout();

private:
    DuiButton *btn;
};

/**
 * TestObj
 */
class TestObj : public QObject
{
    Q_OBJECT
public:
    TestObj();
    virtual ~TestObj();

public slots:
    void handlePage();
    void doRotation();
    void close();

private:
    void createPage();
    void pageAppear();

private:
    QPointer<TestPage> p;

    int time;
    QTimer* timer;
};

#endif // MAIN_H
