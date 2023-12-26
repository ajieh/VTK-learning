#include <vtkSmartPointer.h>
#include <vtkPNGReader.h>
#include <vtkJPEGWriter.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkPNGReader> reader = vtkSmartPointer<vtkPNGReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap04\\DATA\\VTK-logo.png");

	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetInteractorStyle(style);

	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());
	imageViewer->SetupInteractor(interactor);
	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();
	imageViewer->SetSize(600,600);
	imageViewer->GetRenderWindow()->SetWindowName("ReadWriteSingleImage");

	//保存成JPG图像
	vtkSmartPointer<vtkJPEGWriter> writer = vtkSmartPointer<vtkJPEGWriter>::New();
	writer->SetFileName("D:\\Desktop\\logo.jpg");
	writer->SetInputConnection(reader->GetOutputPort());
	writer->Write();

	interactor->Start();
	return 0;
}