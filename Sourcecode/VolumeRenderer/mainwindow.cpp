#include "mainwindow.h"
#include "ui_mainwindow.h"

static void FPSCallback(vtkObject* caller, long unsigned int eventId,
	void* clientData, void* callData);

void FPSCallback(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData)
{
	vtkRenderer* renderer = static_cast<vtkRenderer*>(caller);

	double timeInSeconds = renderer->GetLastRenderTimeInSeconds();
	double fps = 1.0 / timeInSeconds;
	//convert void* clientData to fps QLabel
	QLabel * fpsp = (QLabel *)(clientData);
	fpsp->setText("FPS:"+QString::number(fps, 10, 2));
	if(fps<60)
		fpsp->setStyleSheet("color:red;");
	else
		fpsp->setStyleSheet("color:black;");
}


static void PropertyCallback(vtkObject* caller, long unsigned int eventId,
	void* clientData, void* callData);


void PropertyCallback(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData)
{
	//std::cout << "PropertyCallback"<<std::endl;
	QVTKWidget * wp = (QVTKWidget *)(clientData);
	wp->GetRenderWindow()->Render();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	
    ui->setupUi(this);

	// add VTK widgets
	ui->verticalLayout->addWidget(&widget);
	ui->verticalLayout_2->addWidget(&fps_label);
	ui->verticalLayout_2->addWidget(&volumePropertywidget);

	// set up interactor
	interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(widget.GetRenderWindow());

	// allow the user to interactively manipulate (rotate, pan, etc.) the camera, the viewpoint of the scene.
	auto style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	interactor->SetInteractorStyle(style);

	// set default data
	filename = "../data/vm_foot.slc";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Exit_triggered()
{
	QApplication::quit();
}

void MainWindow::on_action_About_triggered()
{
	QMessageBox msgBox;
	msgBox.setText(QString::fromUtf8("Powered by Tang Boyi, Wang Teng and Xu Xuejun."));
	msgBox.exec();
}

void MainWindow::on_action_Open_triggered()
{
	// show file dialog. change filename only when the new filename is not empty.
	QString filter("slc file (*.slc)");
	QString filename_backup = filename;
	filename_backup = QFileDialog::getOpenFileName(this, QString(tr("Open a volume data")), filename_backup, filter);
	if (!filename_backup.trimmed().isEmpty())
	{
		filename = filename_backup;
	}
	else
	{
		return;
	}

	// show filename on window title
	this->setWindowTitle(QString::fromUtf8("Volume Renderer - ") + filename);

	// get local 8-bit representation of the string in locale encoding (in case the filename contains non-ASCII characters) 
	QByteArray ba = filename.toLocal8Bit();
	const char *filename_str = ba.data();

#if 1
	// read slc files
	auto reader = vtkSmartPointer<vtkSLCReader>::New();
	reader->SetFileName(filename_str);
#elif 1
	// read a series of raw files in the specified folder
	auto reader = vtkSmartPointer<vtkVolume16Reader>::New();
	reader->SetDataDimensions(512, 512);
	reader->SetImageRange(1, 361);
	reader->SetDataByteOrderToBigEndian();
	reader->SetFilePrefix(filename_str);
	reader->SetFilePattern("%s%d");
	reader->SetDataSpacing(1, 1, 1);
#else
	// read NRRD files
	vtkNew<vtkNrrdReader> reader;
	if (!reader->CanReadFile(filename_str))
	{
		std::cerr << "Reader reports " << filename_str << " cannot be read.";
		exit(EXIT_FAILURE);
	}
	reader->SetFileName(filename_str);
	reader->Update();
#endif

	// scale the volume data to unsigned char (0-255) before passing it to volume mapper
	auto shiftScale = vtkSmartPointer<vtkImageShiftScale>::New();
	shiftScale->SetInputConnection(reader->GetOutputPort());
	shiftScale->SetOutputScalarTypeToUnsignedChar();

	// Create transfer mapping scalar value to opacity.
	auto opacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
	opacityTransferFunction->AddPoint(0.0, 0.0);
	opacityTransferFunction->AddPoint(36.0, 0.125);
	opacityTransferFunction->AddPoint(72.0, 0.25);
	opacityTransferFunction->AddPoint(108.0, 0.375);
	opacityTransferFunction->AddPoint(144.0, 0.5);
	opacityTransferFunction->AddPoint(180.0, 0.625);
	opacityTransferFunction->AddPoint(216.0, 0.75);
	opacityTransferFunction->AddPoint(255.0, 0.875);

	// Create transfer mapping scalar value to color.
	auto colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
	colorTransferFunction->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
	colorTransferFunction->AddRGBPoint(36.0, 1.0, 0.0, 0.0);
	colorTransferFunction->AddRGBPoint(72.0, 1.0, 1.0, 0.0);
	colorTransferFunction->AddRGBPoint(108.0, 0.0, 1.0, 0.0);
	colorTransferFunction->AddRGBPoint(144.0, 0.0, 1.0, 1.0);
	colorTransferFunction->AddRGBPoint(180.0, 0.0, 0.0, 1.0);
	colorTransferFunction->AddRGBPoint(216.0, 1.0, 0.0, 1.0);
	colorTransferFunction->AddRGBPoint(255.0, 1.0, 1.0, 1.0);

	// set up volume property
	auto volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetColor(colorTransferFunction);
	volumeProperty->SetScalarOpacity(opacityTransferFunction);
	volumeProperty->ShadeOff();
	volumeProperty->SetInterpolationTypeToLinear();
	//volumeProperty->


	// assign volume property to the volume property widget
	volumePropertywidget.setVolumeProperty(volumeProperty);

	// choose a volume mapper according to the checked menu item
	vtkSmartPointer<vtkAbstractVolumeMapper> volumeMapper;
	if (ui->action_vtkSlicerGPURayCastVolumeMapper->isChecked())
	{
		volumeMapper = vtkSmartPointer<vtkSlicerGPURayCastVolumeMapper>::New();//from 3d slice
	} 
	else if(ui->action_vtkGPUVolumeRayCastMapper->isChecked())
	{
		volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();//gpu
	}
	else
	{
		volumeMapper  = vtkSmartPointer<vtkSmartVolumeMapper>::New(); //cpu or gpu
	}
	volumeMapper->SetInputConnection(shiftScale->GetOutputPort());

	// The volume holds the mapper and the property and can be used to position/orient the volume.
	auto volume = vtkSmartPointer<vtkVolume>::New();
	this->currentVolume = volume;
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	// add the volume into the renderer
	auto renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddVolume(volume);
	renderer->SetBackground(
		RENDERER_BACKGROUND_R,
		RENDERER_BACKGROUND_R,
		RENDERER_BACKGROUND_R);

	// clean previous renderers and then add the current renderer
	auto window = widget.GetRenderWindow();
	auto collection = window->GetRenderers();
	auto item = collection->GetNextItem();
	while (item != NULL)
	{
		window->RemoveRenderer(item);
		item = collection->GetNextItem();
	}


	vtkSmartPointer<vtkCallbackCommand> callback =
		vtkSmartPointer<vtkCallbackCommand>::New();

	callback->SetCallback(FPSCallback);
	callback->SetClientData(&this->fps_label);
	renderer->AddObserver(vtkCommand::EndEvent, callback);


	vtkSmartPointer<vtkCallbackCommand> propertycallback =
		vtkSmartPointer<vtkCallbackCommand>::New();

	propertycallback->SetCallback(PropertyCallback);
	propertycallback->SetClientData(&this->widget);

	volumeProperty->AddObserver(vtkCommand::AnyEvent, propertycallback);

	volumeProperty->GetRGBTransferFunction()->AddObserver(vtkCommand::AnyEvent, propertycallback);
	//volumeProperty->GetGrayTransferFunction()->AddObserver(vtkCommand::AnyEvent, propertycallback);


	window->AddRenderer(renderer);
	window->Render();

	// initialize the interactor
	interactor->Initialize();
	interactor->Start();

}

void MainWindow::on_action_vtkSlicerGPURayCastVolumeMapper_triggered()
{
	if(this->currentVolume != NULL)
	{
		//Change VolumeMapper
		vtkSmartPointer<vtkAbstractVolumeMapper> targetVolumeMapper;
		targetVolumeMapper = vtkSmartPointer<vtkSlicerGPURayCastVolumeMapper>::New();
		vtkSmartPointer<vtkAbstractVolumeMapper> oldVolumeMapper = this->currentVolume->GetMapper();
		targetVolumeMapper->SetInputConnection(oldVolumeMapper->GetInputConnection(0,0)); //why 0,0????????
		this->currentVolume->SetMapper(targetVolumeMapper);
		widget.GetRenderWindow()->Render();//refresh the window
	}
	ui->action_vtkGPUVolumeRayCastMapper->setChecked(false);
	ui->action_vtkSmartVolumeMapper->setChecked(false);
	ui->action_vtkSlicerGPURayCastVolumeMapper->setChecked(true);
}

void MainWindow::on_action_vtkGPUVolumeRayCastMapper_triggered()
{
	if (this->currentVolume != NULL)
	{
		//Change VolumeMapper
		vtkSmartPointer<vtkAbstractVolumeMapper> targetVolumeMapper;
		targetVolumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
		vtkSmartPointer<vtkAbstractVolumeMapper> oldVolumeMapper = this->currentVolume->GetMapper();
		targetVolumeMapper->SetInputConnection(oldVolumeMapper->GetInputConnection(0, 0));//why 0,0????????
		this->currentVolume->SetMapper(targetVolumeMapper);
		widget.GetRenderWindow()->Render();//refresh the window
		
	}
	ui->action_vtkSlicerGPURayCastVolumeMapper->setChecked(false);
	ui->action_vtkSmartVolumeMapper->setChecked(false);
	ui->action_vtkGPUVolumeRayCastMapper->setChecked(true);
}

void MainWindow::on_action_vtkSmartVolumeMapper_triggered()
{
	if (this->currentVolume != NULL)
	{
		//Change VolumeMapper
		vtkSmartPointer<vtkAbstractVolumeMapper> targetVolumeMapper;
		targetVolumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
		vtkSmartPointer<vtkAbstractVolumeMapper> oldVolumeMapper = this->currentVolume->GetMapper();
		targetVolumeMapper->SetInputConnection(oldVolumeMapper->GetInputConnection(0, 0));//why 0,0????????
		this->currentVolume->SetMapper(targetVolumeMapper);
		widget.GetRenderWindow()->Render();//refresh the window
	}
	ui->action_vtkGPUVolumeRayCastMapper->setChecked(false);
	ui->action_vtkSlicerGPURayCastVolumeMapper->setChecked(false);
	ui->action_vtkSmartVolumeMapper->setChecked(true);
 }
