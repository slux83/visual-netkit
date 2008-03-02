#include "ViewWindow.h"

ViewWindow::ViewWindow() : QGraphicsView()
{
	scene = new Scene();
	setScene(scene);	//set the Scene to 'this' QGraphicsView
	
	//ensure to view at (0, 0)
	ensureVisible(0, 0, 100, 100);
}

ViewWindow::~ViewWindow()
{
	delete scene;
}

/* key + and - to resize the scene */
void ViewWindow::keyPressEvent(QKeyEvent *event)
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

void ViewWindow::scaleView(qreal scaleFactor)
{
	qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
  
	QString zoom;
	zoom.setNum((qint16)(factor * 100));
	
	qDebug() << "selected" << scene->selectedItems();
	
	if(scene->selectedItems().size() != 0)
		centerOn(scene->selectedItems().first()); //ensure that the view show the first selected item
	
	setWindowTitle("Current Zoom: " + zoom + "%");
	
	/* min 20% max 500% */
	if (factor * 100 < 20 || factor * 100 > 500)
		return;

	scale(scaleFactor, scaleFactor);
}
