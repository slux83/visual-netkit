#include "ViewWindow.h"

ViewWindow::ViewWindow(Scene *s) : QWidget()
{
    setupUi(this);
	view->setScene(s);	//set the Scene to QGraphicsView
	scene = s;
	
	/* Connects */
	connect(zoomBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(zoom(QString)));
}

ViewWindow::~ViewWindow()
{
	delete scene;
}

/* key + and - to resize the scene */
/*void ViewWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
		case Qt::Key_Plus:
			scaleView(1.25);
			break;
		case Qt::Key_Minus:
			scaleView(1 / 1.25);
			break;
		default:
			QGraphicsView::keyPressEvent(event);
	}
}
*/

void ViewWindow::zoom(QString z)
{
	qDebug() << z;
}

void ViewWindow::scaleView(qreal scaleFactor)
{
	qreal factor = view->matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	
	qDebug() << "selected" << scene->selectedItems();
	
	if(scene->selectedItems().size() != 0)
		view->centerOn(scene->selectedItems().first()); //ensure that the view show the first selected item
	
	/* min 20% max 500% */
	if (factor * 100 < 20 || factor * 100 > 500)
		return;

	view->scale(scaleFactor, scaleFactor);
}
