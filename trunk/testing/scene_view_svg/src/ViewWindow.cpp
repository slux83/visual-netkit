#include "ViewWindow.h"

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QLinkedList>
#include <QList>
#include <QListIterator>

ViewWindow::ViewWindow(Scene *s) : QWidget()
{
    setupUi(this);
	view->setScene(s);	//set the Scene to QGraphicsView
	scene = s;
	
	/* Connects */
	connect(addMachineButton, SIGNAL(clicked()), this, SLOT(addVM()));
	connect(minusButton, SIGNAL(clicked()), this, SLOT(zoomMinus()));
	connect(plusButton, SIGNAL(clicked()), this, SLOT(zoomPlus()));
	connect(groupButton, SIGNAL(clicked()), this, SLOT(makeGroup()));
	connect(ungroupButton, SIGNAL(clicked()), this, SLOT(destroyGroup()));
	connect(areaButton, SIGNAL(clicked()), this, SLOT(makeArea()));

}

ViewWindow::~ViewWindow()
{
	delete scene;
}

void ViewWindow::zoomMinus()
{
	scaleView(1 / 1.25);
}

void ViewWindow::zoomPlus()
{
	scaleView(1.25);
}

void ViewWindow::scaleView(qreal scaleFactor)
{
	qreal factor = view->matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	
	
	if(scene->selectedItems().size() != 0)
		view->centerOn(scene->selectedItems().first()); //ensure that the view show the first selected item
	
	/* min 20% max 500% */
	if (factor * 100 < 20 || factor * 100 > 500)
		return;
	
	zoomLabel->setText("Zoom: " + QByteArray::number((uint)(factor * 100)) + "%");

	view->scale(scaleFactor, scaleFactor);
}

void ViewWindow::makeGroup()
{
	if(scene->selectedItems().size() == 0 || scene->selectedItems().size() == 1)
	{
		//Show warning
		qDebug() << "no items selected / only one item selected";
		return;
	}
	
	QGraphicsItemGroup *newGroup = scene->createItemGroup(scene->selectedItems());
	newGroup->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	
	groupList.append(newGroup);
	
	newGroup->setSelected(true);	
}

/*
void ViewWindow::makeGroupFromItems(QLinkedList<QGraphicsItem *> *items)
{
	if(items->size() == 0 ||items->size() == 1)
	{
		//Show warning
		qDebug() << "no items selected / only one item selected";
		return;
	}
	
	QGraphicsItemGroup *newGroup = scene->createItemGroup(items);
	newGroup->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	
	groupList.append(newGroup);
	
	newGroup->setSelected(true);
}
*/

void ViewWindow::destroyGroup()
{
	QListIterator<QGraphicsItemGroup *> i(groupList);
	while(i.hasNext())
	{
		QGraphicsItemGroup * item = i.next();
		if(scene->selectedItems().contains(item))
		{
			int indexItem = groupList.indexOf(item);			
			
			if(indexItem == -1)
				continue;
			
			QGraphicsItemGroup *group = groupList.takeAt(indexItem);
			
			/* Force unselected all sub-items */
			qDebug() << "children:" << group->children().size();
			
			/* get children for the group and destroy it */
			QListIterator<QGraphicsItem *> it(group->children());
			scene->destroyItemGroup(group);
			
			while(it.hasNext())
			{
				it.next()->setSelected(false);
			}			
		}
	}	
}

void ViewWindow::addVM()
{
	SvgItemNode *vm = new SvgItemNode();
	vm->moveBy(190, 190);
	vm->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	scene->addItem(vm);
}

void ViewWindow::makeArea()
{
	if(areaButton->isChecked())
	{
		qDebug() << "ready to add the area";
		scene->setMode(InsertArea);
	}	
	else
	{
		scene->setMode(InsertItem);
	}
	
	
		
	
}