///****************************************************************************
/// @author  : 苏一舟                                                         
/// @date    : 2022.4.1                                                                
/// @file    :                                                                 
/// @brief   : 体绘制（光线投射算法）                                                                
///----------------------------------------------------------------------------
///                           Change History                                   
///----------------------------------------------------------------------------
///                              
///****************************************************************************

#include "vrraycast.h"
#include <ui_dicomviewer.h>

Vrraycast::Vrraycast(QWidget* parent)
	:QWidget(parent)
{
	vrvolumeRendering();
}

Vrraycast::~Vrraycast()
{

}

void Vrraycast::vrvolumeRendering()
{
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	QSettings s;

	reader->SetDirectoryName(s.value(OPEN_FILE_PATH).toString().toLatin1());
	reader->SetDataByteOrderToLittleEndian();

	vtkSmartPointer<vtkImageGaussianSmooth> gaussianSmoothFilter =
		vtkSmartPointer<vtkImageGaussianSmooth>::New();
	gaussianSmoothFilter->SetInputConnection(reader->GetOutputPort());
	gaussianSmoothFilter->SetDimensionality(3);
	gaussianSmoothFilter->SetRadiusFactor(5);
	gaussianSmoothFilter->SetStandardDeviation(1);
	gaussianSmoothFilter->Update();

	vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper =
		vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
	volumeMapper->SetInputConnection(gaussianSmoothFilter->GetOutputPort());
	volumeMapper->SetBlendModeToComposite();

	vtkSmartPointer<vtkVolumeProperty> volumeProperty =
		vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetInterpolationTypeToLinear();
	volumeProperty->ShadeOn();
	volumeProperty->SetAmbient(0.4);
	volumeProperty->SetDiffuse(0.6);
	volumeProperty->SetSpecular(0.2);

	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =
		vtkSmartPointer<vtkPiecewiseFunction>::New();
	compositeOpacity->AddPoint(70, 0.00);
	compositeOpacity->AddPoint(90, 0.40);
	compositeOpacity->AddPoint(180, 0.60);
	volumeProperty->SetScalarOpacity(compositeOpacity);

	//设置梯度不透明属性
	vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity =
		vtkSmartPointer<vtkPiecewiseFunction>::New();
	volumeGradientOpacity->AddPoint(10, 0.0);
	volumeGradientOpacity->AddPoint(90, 0.5);
	volumeGradientOpacity->AddPoint(100, 1.0);
	volumeProperty->SetGradientOpacity(volumeGradientOpacity);//设置梯度不透明度效果对比

	vtkSmartPointer<vtkColorTransferFunction> color =
		vtkSmartPointer<vtkColorTransferFunction>::New();
	color->AddRGBPoint(0.000, 0.00, 0.00, 0.00);
	color->AddRGBPoint(64.00, 1.00, 0.52, 0.30);
	color->AddRGBPoint(190.0, 1.00, 1.00, 1.00);
	color->AddRGBPoint(220.0, 1.00, 1.00, 1.00);
	volumeProperty->SetColor(color);

	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	vtkNew<vtkOutlineFilter> outlineData;
	vtkNew<vtkPolyDataMapper> mapOutline;
	mapOutline->SetInputConnection(outlineData->GetOutputPort());
	vtkNew<vtkActor> outline;
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(0, 0, 0);

	vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
	render->SetBackground(1, 1, 1);
	render->AddVolume(volume);

	vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
	rw->AddRenderer(render);
	rw->SetSize(640, 480);
	rw->Render();
	rw->SetWindowName("vrVolumeRendering");

	vtkSmartPointer<vtkRenderWindowInteractor> rwi =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	rwi->SetRenderWindow(rw);

	render->ResetCamera();
	rw->Render();
	rwi->Start();
}