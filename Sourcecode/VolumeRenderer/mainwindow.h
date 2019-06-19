#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>

#include <iostream>
#include <memory>
#include <cstdlib>

#ifdef VTK_OPENGL2
#include <vtk_glew.h>
#include <QVTKWidget2.h>
#else
#include <vtkgl.h>
#include <QVTKWidget.h>
#endif

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkMetaImageReader.h>
#include <vtkVolume16Reader.h>
#include <vtkNew.h>
#include <vtkNrrdReader.h>
#include <vtkImageShiftScale.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRendererCollection.h>

#include "ctkTransferFunction.h"
#include "ctkVTKColorTransferFunction.h"
#include "ctkTransferFunctionView.h"
#include "ctkTransferFunctionGradientItem.h"
#include "ctkTransferFunctionControlPointsItem.h"
#include "ctkVTKVolumePropertyWidget.h"

#include "vtkSlicerGPURayCastVolumeMapper.h"

#include "ui_mainwindow.h"

#include <vtkSmartVolumeMapper.h>
#include <vtkSLCReader.h>
#include <vtkCallbackCommand.h>
#include <qlabel.h>
#include <vtkObject.h>
#include "vtkoutputwindow.h"
#include "ctkVTKScalarsToColorsWidget.h"
#define RENDERER_BACKGROUND_R 1
#define RENDERER_BACKGROUND_G 1
#define RENDERER_BACKGROUND_B 1


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

	QString filename;
	vtkSmartPointer<vtkRenderWindowInteractor> interactor;
	QVTKWidget widget;
	ctkVTKVolumePropertyWidget volumePropertywidget;

	vtkSmartPointer<vtkVolume> currentVolume;
	QLabel fps_label;


private slots:
	void on_action_Exit_triggered();
	void on_action_About_triggered();
	void on_action_Open_triggered();
    void on_action_vtkSlicerGPURayCastVolumeMapper_triggered();
    void on_action_vtkGPUVolumeRayCastMapper_triggered();
	void on_action_vtkSmartVolumeMapper_triggered();


};

#endif // MAINWINDOW_H
