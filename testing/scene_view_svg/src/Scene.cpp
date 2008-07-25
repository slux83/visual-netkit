#include <QtGui>
#include <QPointF>
#include <QGraphicsItem>

#include "Scene.h"
#include "AreaItem.h"

Scene::Scene() : QGraphicsScene(0, 0, 1000, 1000)
{
	myMode = InsertItem;
	line = NULL;
}

Scene::~Scene()
{
}


/*
void Scene::dumpToPDF() {
	QPrinter *printer = new QPrinter(QPrinter::HighResolution);
	printer->setOutputFormat(QPrinter::PdfFormat);
	QString fileName = QFileDialog::getSaveFileName(this, tr("Export to PDF"), "~/untitled.pdf", tr("File (*.pdf, *.ps)"));
	printer->setOutputFileName(fileName);
	QPainter *pdfPainter = new QPainter(printer);
	scene()->render(pdfPainter);
	pdfPainter->end();
}

void Scene::dumpToSVG() {
	QSvgGenerator *gen = new QSvgGenerator();
	gen->setFileName("~/out.svg");
	QPainter *svgPainter = new QPainter(gen);
	scene()->render(svgPainter);
	svgPainter->end();
}
*/

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    //QPointF *end = new QPointF(mouseEvent->scenePos().x(), mouseEvent->scenePos().y());
    
    QGraphicsItem *item = itemAt(mouseEvent->scenePos());
    readyToResize(item, mouseEvent->scenePos());
    
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
	QGraphicsScene::mouseMoveEvent(mouseEvent);
	
	QGraphicsItem *item = itemAt(mouseEvent->scenePos());
	readyToResize(item, mouseEvent->scenePos(), true);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
 
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
