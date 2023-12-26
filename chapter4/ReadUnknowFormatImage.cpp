#include <vtkSmartPointer.h>
#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkImageViewer2.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	//读取位置格式的图像可以用vtkImageReader2Factory
	vtkSmartPointer<vtkImageReader2Factory> readerFactory =
		vtkSmartPointer<vtkImageReader2Factory>::New();
	vtkImageReader2 * reader = readerFactory->CreateImageReader2("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap04\\DATA\\VTK-logo.png");
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap04\\DATA\\VTK-logo.png");
	reader->Update();


	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetInteractorStyle(style);

	//显示读取的图像
	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());
	imageViewer->SetupInteractor(interactor);
	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();
	imageViewer->SetSize(600,600);
	imageViewer->GetRenderWindow()->SetWindowName("ReadUnknowFormatImage");

	interactor->Start();
	return 0;
}