#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsReader.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolumeProperty.h>
#include <vtkAxesActor.h>
#include <vtkImageShiftScale.h>
#include <vtkImageCast.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkPlane.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);

int main()
{
	vtkSmartPointer<vtkStructuredPointsReader> reader = vtkSmartPointer<vtkStructuredPointsReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap07\\DATA\\mummy.128.vtk");
	reader->Update();

	vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper =
		vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
	volumeMapper->SetInputConnection(reader->GetOutputPort());
	
	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
	plane->SetOrigin(100, 100, 0);//设置原点
	plane->SetNormal(1, 1, 0);//设置法线
	volumeMapper->AddClippingPlane(plane);

	//设置体绘制属性
	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetInterpolationTypeToLinear();
	volumeProperty->ShadeOn();  //打开阴影测试
	volumeProperty->SetAmbient(0.4);
	volumeProperty->SetDiffuse(0.6);
	volumeProperty->SetSpecular(0.2);

	//设置不透明度传输函数
	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =
		vtkSmartPointer<vtkPiecewiseFunction>::New();
	compositeOpacity->AddPoint(70, 0.00);
	compositeOpacity->AddPoint(90, 0.40);
	compositeOpacity->AddPoint(180, 0.60);
	volumeProperty->SetScalarOpacity(compositeOpacity);

	//梯度不透明度传输函数
	vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity =
		vtkSmartPointer<vtkPiecewiseFunction>::New();
	volumeGradientOpacity->AddPoint(10, 0.0);
	volumeGradientOpacity->AddPoint(90, 0.5);
	volumeGradientOpacity->AddPoint(100, 1.0);
	volumeProperty->SetGradientOpacity(volumeGradientOpacity);

	//设置颜色
	vtkSmartPointer<vtkColorTransferFunction> color =
		vtkSmartPointer<vtkColorTransferFunction>::New();
	color->AddRGBPoint(0.000, 0.00, 0.00, 0.00);
	color->AddRGBPoint(64.00, 1.00, 0.52, 0.30);
	color->AddRGBPoint(190.0, 1.00, 1.00, 1.00);
	color->AddRGBPoint(220.0, 0.20, 0.20, 0.20);
	volumeProperty->SetColor(color);

	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(1.0, 1.0, 1.0);
	renderer->AddVolume(volume);

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(renderer);
	renWin->SetSize(600, 480);
	renWin->Render();

	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);
	renderer->ResetCamera();

	renWin->Render();
	iren->Start();
	return 0;
}




