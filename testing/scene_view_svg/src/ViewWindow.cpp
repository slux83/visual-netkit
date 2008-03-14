#include "ViewWindow.h"

#include <QGraphicsItem>
#include <QListIterator>

ViewWindow::ViewWindow(Scene *s) : QWidget()
{
    setupUi(this);
	view->setScene(s);	//set the Scene to QGraphicsView
	scene = s;
	
	/* Connects */
	connect(minusButton, SIGNAL(clicked()), this, SLOT(zoomMinus()));
	connect(plusButton, SIGNAL(clicked()), this, SLOT(zoomPlus()));
	connect(groupButton, SIGNAL(clicked()), this, SLOT(makeGroup()));
	connect(ungroupButton, SIGNAL(clicked()), this, SLOT(destroyGroup()));

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
	
	
}

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
			
			scene->destroyItemGroup(groupList.takeAt(indexItem));
		}
	}
}
