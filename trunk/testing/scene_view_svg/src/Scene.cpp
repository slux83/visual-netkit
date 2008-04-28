#include <QtGui>
#include <QPointF>

#include "Scene.h"

Scene::Scene() : QGraphicsScene(0, 0, 1000, 1000)
{
	myMode = InsertLine;
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

    QPointF *end = new QPointF(mouseEvent->scenePos().x(), mouseEvent->scenePos().y());
    
    
    //SvgItemNode *item;
    switch (myMode)
    {
        case InsertItem:
            //item = new SvgItemNode();
            //addItem(item);
            //item->setPos(mouseEvent->scenePos());
            //emit itemInserted(item);
            break;
        
        case InsertLine:
        	//qDebug() << "Mouse:  start--> " << mouseEvent->scenePos() << "  end --> " << mouseEvent->scenePos();
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), *end));
            line->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            addItem(line);
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
    if (myMode == InsertLine && line != NULL) {
    	QLineF newLine(line->line().p1(), mouseEvent->scenePos());
    	line->setLine(newLine);
        
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0 && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == SvgItemNode::Type &&
            endItems.first()->type() == SvgItemNode::Type &&
            startItems.first() != endItems.first()) {
        	SvgItemNode *startItem = qgraphicsitem_cast<SvgItemNode *>(startItems.first());
            SvgItemNode *endItem = qgraphicsitem_cast<SvgItemNode *>(endItems.first());
            SvgItemLink *arrow = new SvgItemLink(startItem, endItem);
            arrow->setColor(myLineColor);
            startItem->addLink(arrow);
            endItem->addLink(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
        }
    }
    line = 0;
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