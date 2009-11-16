#include "networklist.h"

#include <DuiLabel>
#include <DuiList>
#include <DuiWidgetFactory>
#include <DuiWidgetListModel>
#include <QGraphicsWidget>
#include <QModelIndex>
#include <QDebug>


// Class list cannot inherit DuiList because the signals are not inherited along

NetworkList::NetworkList(QGraphicsItem *parent) :
        DuiList(parent),
        parent(parent),
        listModel(NULL),
        selected(false)
{
    listModel = new DuiWidgetListModel(this);
    setObjectName("availableNetworksList");
    enableItemSelection(true);
    setItemModel(listModel);
    connect(this, SIGNAL(itemClicked(const QModelIndex &)), this, SLOT(availableNetworkClicked(const QModelIndex &)));
}

NetworkList::~NetworkList()
{
}

bool NetworkList::insertOperators(int selected, const QStringList &operators)
{
    if(operators.size() <= 0)
        return false;

    for(int i=0; i<operators.size(); ++i)
        listModel->appendWidget(new DuiLabel(operators.at(i), parent)); // is parent necessary?

    if(selected != -1) {
        selectItem(listModel->index(selected));
        this->selected = true;
    }

    return true;
}

void NetworkList::availableOperatorClicked(const QModelIndex &index)
{
    selected = true;
    emit availableOperatorSelected(index.row());
}

bool NetworkList::itemSelected()
{
    return selected;
}
