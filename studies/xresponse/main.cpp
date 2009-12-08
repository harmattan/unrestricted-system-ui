
#include <QDebug>
#include <QFile>
#include <QtTest/QtTest>

//#include "main.h"

const QString logfile = "/tmp/xrepsonse.log";

void iterate(const QList<int> &ints);
void checkLogfile();
void processLine(const QString &line, int &startTime, int &endTime);

void iterate(const QList<int> &ints)
{    
    int index = 0;
    while(index<ints.size()) {
        qDebug() << "Measuring...";
        int x=ints.at(index++);
        int y=ints.at(index++);
        int time=ints.at(index++);
        bool measure=(bool)ints.at(index++);        

        QString command = QString("xresponse -o %1 -c %2x%3 -w %4").arg(logfile).arg(x).arg(y).arg(time);
        system(command.toLatin1());        
        QTest::qSleep(time+500);
        checkLogfile();
    }
}

void checkLogfile()
{
    QFile file(logfile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    int startTime, endTime = 0;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        processLine(line, startTime, endTime);
    }
    if(startTime != 0 && endTime != 0)
        qDebug() << "time (ms): " << endTime - startTime;    
}

void processLine(const QString &line, int &startTime, int &endTime)
{
    endTime = line.section("ms", 0, 0).toInt();
    if(line.contains("Clicked"))
        startTime = line.section("ms", 0, 0).toInt();
}

int main(int argc, char** argv)
{    
    QList<int> ints;
    for(int i=1; i<argc; ++i)
        ints << atoi(argv[i]);

     if(ints.size() %4 != 0) {
        qDebug() << "Wrong amount of parameters";
        exit(1);
    }
    iterate(ints);
}

