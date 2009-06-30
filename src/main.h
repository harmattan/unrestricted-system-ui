#ifndef MAIN_H
#define MAIN_H

#include <QObject>
#include <QString>

class TestObj : public QObject
{
    Q_OBJECT
public:
    TestObj();
    virtual ~TestObj();

public slots:
    void doRotation();
};




#endif // MAIN_H
