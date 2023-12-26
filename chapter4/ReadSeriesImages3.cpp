#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkImageAppend.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	//用vtkAppend把读取的一张张图像拼接成三维体数据
	vtkSmartPointer<vtkImageAppend > append = vtkSmartPointer<vtkImageAppend >::New();
	append->SetAppendAxis(2);//将拼接轴设置为轴方向为Z轴

	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	char fileNames[128];
	for (int i = 1; i < 100; i++)
	{
		sprintf_s(fileNames, "D:\\Documents\\Material\\VTK-learn\\Examples\\Chap04\\DATA\\Head\\head%03d.jpg", i);
		reader->SetFileName(fileNames);
		append->AddInputConnection(reader->GetOutputPort());
	}

	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetInteractorStyle(style);

	//显示图像
	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());
	imageViewer->SetupInteractor(interactor);
	imageViewer->SetSlice(50); 
	imageViewer->SetSliceOrientationToXY();
	imageViewer->Render();
	imageViewer->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
	imageViewer->SetSize(600,600);
	imageViewer->GetRenderWindow()->SetWindowName("ReadSeriesImages3");

	interactor->Start();
	return 0;
}