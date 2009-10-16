#ifndef NETWORKLIST_H
#define NETWORKLIST_H

#include <DuiList>

class DuiWidgetListModel;

class NetworkList : public DuiList
{
    Q_OBJECT

public:
    NetworkList(QGraphicsItem *parent = 0);
    virtual ~NetworkList();

    bool insertNetworks(int selected, const QStringList &networks);

signals:
    void availableNetworkSelected(QString);

private slots:
    void availableNetworkSelected(const QModelIndex &index);

private:
    DuiWidgetListModel *listModel;
};

#endif // NETWORKLIST_H
