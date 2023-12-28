#include <vtkSmartPointer.h>
#include <vtkMatrix4x4.h>
#include <vtkImageReslice.h>
#include <vtkImageActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageData.h>
#include <vtkMetaImageReader.h>
#include <vtkImageCast.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap05\\DATA\\BRAIN.MHD");
	reader->Update();

	int extent[6];
	double spacing[3];
	double origin[3];

	reader->GetOutput()->GetExtent(extent);
	reader->GetOutput()->GetSpacing(spacing);
	reader->GetOutput()->GetOrigin(origin);

	//计算图像中心坐标
	double center[3];
	center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
	center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
	center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);

	//切面的变换矩阵
	static double transMatrix[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	vtkSmartPointer<vtkMatrix4x4> matrix= vtkSmartPointer<vtkMatrix4x4>::New();
	matrix->DeepCopy(transMatrix);
	matrix->SetElement(0, 3, center[0]);
	matrix->SetElement(1, 3, center[1]);
	matrix->SetElement(2, 3, center[2]);

	vtkSmartPointer<vtkImageReslice> reslice = vtkSmartPointer<vtkImageReslice>::New();
	reslice->SetInputConnection(reader->GetOutputPort());
	reslice->SetOutputDimensionality(2);//二维图像
	reslice->SetResliceAxes(matrix);//设置变换矩阵
	reslice->SetInterpolationModeToLinear();//线性插值
	reslice->Update();

	vtkSmartPointer<vtkLookupTable> colorTable = vtkSmartPointer<vtkLookupTable>::New();
	colorTable->SetRange(0, 1000);
	colorTable->SetValueRange(0.0, 1.0);
	colorTable->SetSaturationRange(0.0, 0.0);//饱和度，0.0指黑白灰阶
	colorTable->SetRampToLinear();//映射方式为线性映射
	colorTable->Build();

	vtkSmartPointer<vtkImageMapToColors> colorMap = vtkSmartPointer<vtkImageMapToColors>::New();
	colorMap->SetLookupTable(colorTable);
	colorMap->SetInputConnection(reslice->GetOutputPort());
	colorMap->Update();

	vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
	actor->SetInputData(colorMap->GetOutput());

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(600,400);
	renderWindow->SetWindowName("ImageResliceExample");
	renderWindow->Render();

	vtkSmartPointer<vtkRenderWindowInteractor> Interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	Interactor->SetInteractorStyle(style);
	Interactor->SetRenderWindow(renderWindow);

	Interactor->Initialize();
	Interactor->Start();
	return 0;
}
