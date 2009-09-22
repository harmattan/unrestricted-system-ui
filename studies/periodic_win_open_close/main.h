#ifndef MAIN_H
#define MAIN_H

#include <QObject>
#include <DuiApplicationWindow>

class TestObj : public QObject
{
    Q_OBJECT
public:
    TestObj(DuiApplicationWindow &win);
    virtual ~TestObj();

private:
    bool visible;
    DuiApplicationWindow *win;

public slots:
    void doStuff();
};

#endif // MAIN_H
