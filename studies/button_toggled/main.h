#ifndef MAIN_H
#define MAIN_H

#include <QObject>


class TestObj : public QObject
{
    Q_OBJECT
public:
    TestObj();
    virtual ~TestObj();

public slots:
    void toggled(bool);
};

#endif // MAIN_H
