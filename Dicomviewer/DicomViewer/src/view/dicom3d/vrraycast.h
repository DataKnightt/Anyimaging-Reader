#ifndef VRRAYCAST_H
#define VRRAYCAST_H

#include <QWidget>

#include<vtkDICOMImageReader.h>
#include<vtkCamera.h>
#include<vtkActor.h>
#include<vtkNew.h>
#include<vtkRenderer.h>
#include<vtkRenderWindow.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkVolumeProperty.h>
#include<vtkProperty.h>
#include<vtkPolyDataNormals.h>
#include<vtkImageShiftScale.h>
#include<vtkFixedPointVolumeRayCastMapper.h>
#include<vtkPiecewiseFunction.h>
#include<vtkColorTransferFunction.h>
#include<vtkImageCast.h>
#include<vtkOBJExporter.h>
#include<vtkOutlineFilter.h>
#include<vtkPolyDataMapper.h>
#include<vtkFixedPointVolumeRayCastMIPHelper.h>
#include<vtkInteractorStyleTrackballCamera.h>
#include<vtkImageGaussianSmooth.h>
#include<vtkMetaImageReader.h>
#include<vtkLODProp3D.h>
#include<vtkLoopSubdivisionFilter.h>
#include<vtkGPUVolumeRayCastMapper.h>    //Gpu光照映射
#include<vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);   //体绘制加速

class Vrraycast :public QWidget
{
	Q_OBJECT
public:
	Vrraycast(QWidget* parent = 0);
	~Vrraycast();
public:
	void vrvolumeRendering();
};


#endif