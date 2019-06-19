#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	//ignore global warning window
	vtkOutputWindow::GlobalWarningDisplayOff();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
