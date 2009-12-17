#ifndef NETWORKLIST_H
#define NETWORKLIST_H

#include <DuiList>

class QStringListModel;
class QModelIndex;

class NetworkList : public DuiList
{
    Q_OBJECT

public:
    NetworkList(QGraphicsItem *parent = 0);
    virtual ~NetworkList();

    void insertOperators(int selected, const QStringList &operators);
    void removeOperators();
    void removeSelection();

signals:
    void availableOperatorSelected(int);

private slots:
    void availableOperatorClicked(const QModelIndex &index);

private:
    QStringListModel* listModel;
    bool selected;
};

#endif // NETWORKLIST_H
