#include <QtGui>
#include <QPointF>
#include <QGraphicsItem>

#include "Scene.h"
#include "AreaItem.h"

Scene::Scene() : QGraphicsScene(0, 0, 1000, 1000)
{
	myMode = InsertItem;
	line = NULL;
	resizeItem = NULL;
}

Scene::~Scene()
{
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    //QPointF *end = new QPointF(mouseEvent->scenePos().x(), mouseEvent->scenePos().y());
    
    QGraphicsItem *item = itemAt(mouseEvent->scenePos());
    
    //if the user click the resize ball
    if(readyToResize(item, mouseEvent->scenePos(), true))
    {
    	resizeItem = item;
    	resizing = true;
    }
    
    //SvgItemNode *item;
    switch (myMode)
    {
        case InsertItem:
            //item = new SvgItemNode();
            //addItem(item);
            //item->setPos(mouseEvent->scenePos());
            //emit itemInserted(item);
            break;
        
        case InsertArea:
        	qDebug() << "adding an area";
        	addItem(new AreaItem());
        	//qDebug() << "Mouse:  start--> " << mouseEvent->scenePos() << "  end --> " << mouseEvent->scenePos();
            //line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), *end));
            //line->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            //addItem(line);
            break;

        /*
        case InsertText:
            textItem = new DiagramTextItem();
            textItem->setFont(myFont);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(1000.0);
            connect(textItem, SIGNAL(lostFocus(DiagramTextItem *)),
                    this, SLOT(editorLostFocus(DiagramTextItem *)));
            connect(textItem, SIGNAL(selectedChange(QGraphicsItem *)),
                    this, SIGNAL(itemSelected(QGraphicsItem *)));
            addItem(textItem);
            textItem->setDefaultTextColor(myTextColor);
            textItem->setPos(mouseEvent->scenePos());
            emit textInserted(textItem);
        */
        
        default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsItem *item = itemAt(mouseEvent->scenePos());
	readyToResize(item, mouseEvent->scenePos(), true);
	if(mouseEvent->button() != Qt::LeftButton && resizing)
	{
		AreaItem *aItem;
		if(!(aItem = static_cast<AreaItem*>(resizeItem)))
			return;
		
		//resize the area
		qreal width = aItem->rect().width() + (mouseEvent->scenePos().x() - (aItem->scenePos().x() + aItem->rect().width()));
		qreal height = aItem->rect().height() + (mouseEvent->scenePos().y() - (aItem->scenePos().y() + aItem->rect().height()));
		
		/* Size control */
		if(width < 20)
			width = aItem->rect().width();
		if(height < 20)
			height = aItem->rect().height();
		
		aItem->setRect(0, 0, width , height);
		
	}
	else
		QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsItem *item = itemAt(mouseEvent->scenePos());
	readyToResize(item, mouseEvent->scenePos(), true);
	if(resizing)
	{
		resizing = false;
		QApplication::restoreOverrideCursor();
		resizeItem = NULL;
	}
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool Scene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}

bool Scene::readyToResize(QGraphicsItem *item, QPointF mousePoint, bool overrideCursor)
{
	AreaItem *aItem = static_cast<AreaItem*>(item);
	if(!aItem)
	{
		QApplication::restoreOverrideCursor();
		return false;
	}
	
	QPointF mousePosOnArea = item->mapFromScene(mousePoint);
	QRectF resizeRegion;
	QRectF itemBrect = item->boundingRect();
	resizeRegion.setX(itemBrect.width() - 10);
	resizeRegion.setY(itemBrect.height() - 10);
	resizeRegion.setWidth(10);
	resizeRegion.setHeight(10);
	
	
	if(overrideCursor && resizeRegion.contains(mousePosOnArea) && item->isSelected())
	{
		QApplication::setOverrideCursor(Qt::SizeFDiagCursor);
	}
	else
	{
		QApplication::restoreOverrideCursor();
	}
	
	return resizeRegion.contains(mousePosOnArea);
}
