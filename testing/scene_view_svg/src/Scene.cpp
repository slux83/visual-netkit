#include "Scene.h"

Scene::Scene() : QGraphicsScene(0, 0, 1000, 1000)
{
}

Scene::~Scene()
{
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mousePressEvent(mouseEvent);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	
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

/*
void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        centerNode->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        centerNode->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        centerNode->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        centerNode->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        scaleView(1.2);
        break;
    case Qt::Key_Minus:
        scaleView(1 / 1.2);
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        foreach (QGraphicsItem *item, scene()->items()) {
            if (qgraphicsitem_cast<Node *>(item))
                item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
        }
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}
*/