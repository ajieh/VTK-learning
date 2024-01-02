#include <vtkSmartPointer.h>
#include <vtkUnstructuredGridVolumeRayCastMapper.h>
#include <vtkUnstructuredGridVolumeZSweepMapper.h>
#include <vtkProjectedTetrahedraMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkStructuredPointsReader.h>
#include <vtkSLCReader.h>
#include <vtkStructuredPoints.h>
#include <vtkUnstructuredGrid.h>
#include <vtkThreshold.h>
#include <vtkDataSetTriangleFilter.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkVolume.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkStdString.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);

int main()
{
	vtkSmartPointer<vtkStructuredPointsReader> reader = vtkSmartPointer<vtkStructuredPointsReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap07\\DATA\\ironProt.vtk");
	reader->Update();

	vtkSmartPointer<vtkThreshold> thresh = vtkSmartPointer<vtkThreshold>::New();
	thresh->ThresholdByUpper(80);//上阈值为 80 
	thresh->AllScalarsOff();
	thresh->SetInputConnection(reader->GetOutputPort());

	//将输入的数据集转换为三角形数据集
	vtkSmartPointer<vtkDataSetTriangleFilter> trifilter =
		vtkSmartPointer<vtkDataSetTriangleFilter>::New();
	trifilter->SetInputConnection(thresh->GetOutputPort());

	//设置体绘制属性
	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetInterpolationTypeToLinear();
	volumeProperty->ShadeOff();


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
	volume->SetProperty(volumeProperty);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(renderer);
	

	int RenderType = 3;
	if (RenderType == 1)
	{
		vtkSmartPointer<vtkUnstructuredGridVolumeRayCastMapper> volumeMapper =
			vtkSmartPointer<vtkUnstructuredGridVolumeRayCastMapper>::New();
		volumeMapper->SetInputConnection(trifilter->GetOutputPort());
		volume->SetMapper(volumeMapper);
		renderer->AddVolume(volume);
	}
	else if (RenderType == 2)
	{
		vtkSmartPointer<vtkUnstructuredGridVolumeZSweepMapper> volumeMapper =
			vtkSmartPointer<vtkUnstructuredGridVolumeZSweepMapper>::New();
		volumeMapper->SetInputConnection(trifilter->GetOutputPort());
		volume->SetMapper(volumeMapper);
		renderer->AddVolume(volume);
	}
	else if (RenderType == 3)
	{
		vtkSmartPointer<vtkProjectedTetrahedraMapper> volumeMapper =
			vtkSmartPointer<vtkProjectedTetrahedraMapper>::New();
		volumeMapper->SetInputConnection(trifilter->GetOutputPort());
		volume->SetMapper(volumeMapper);
		renderer->AddVolume(volume);
	}

	renWin->SetSize(600, 480);
	renWin->Render();

	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);
	iren->SetDesiredUpdateRate(10);//设置交互器的期望更新频率为 3，单位是帧/秒

	renWin->Render();
	iren->Start();
	return 0;
}

