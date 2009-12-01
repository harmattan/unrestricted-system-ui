#ifndef MAIN_H
#define MAIN_H
#include <qmsystem/qmlocks.h>
#include <QObject>


class TestObj : public QObject
{
    Q_OBJECT
public:
    TestObj();
    virtual ~TestObj();

public slots:
    void locksChanged(Maemo::QmLocks::Lock l, Maemo::QmLocks::State s);
};

#endif // MAIN_H
