#include "ViewWindow.h"

ViewWindow::ViewWindow() : QGraphicsView()
{
	scene = new Scene();
	setScene(scene);	//set the Scene to 'this' QGraphicsView
	
	//resize the scene (scale)
	scale(0.7, 0.7);
}

ViewWindow::~ViewWindow()
{
	delete scene;
}

void ViewWindow::notifySelection()
{
	qDebug() << "Selected" << scene->selectedItems().size() << "items";
}
