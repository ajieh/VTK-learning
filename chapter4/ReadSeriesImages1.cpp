#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkStringArray.h>
#include <vtkRenderWindow.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	//图像序列的文件名数组
	vtkSmartPointer<vtkStringArray> fileArray = vtkSmartPointer<vtkStringArray>::New();
	char fileNames[128];
	for(int i = 1; i < 100; i++)
	{
		sprintf_s(fileNames, "D:\\Documents\\Material\\VTK-learn\\Examples\\Chap04\\DATA\\Head\\head%03d.jpg", i);
		std::string fileStr(fileNames);
		fileArray->InsertNextValue(fileStr);
	}

	//读取序列图像
	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileNames(fileArray);

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
	imageViewer->GetRenderWindow()->SetWindowName("ReadSeriesImages1");

	interactor->Start();
	return 0;
}