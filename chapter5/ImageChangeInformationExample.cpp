#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkBMPReader.h>
#include <vtkImageChangeInformation.h>
#include <vtkImageData.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap05\\DATA\\LENA.BMP");
	reader->Update();

	int dims[3];
	reader->GetOutput()->GetDimensions(dims);
	std::cout << "图像维数:" << dims[0] << " " << dims[1] << " " << dims[2] << std::endl;

	double origin[3];
	reader->GetOutput()->GetOrigin(origin);
	std::cout << "图像原点:" << origin[0] << " " << origin[1] << " " << origin[2] << std::endl;

	double spaceing[3];
	reader->GetOutput()->GetSpacing(spaceing);
	std::cout << "像素间隔:" << spaceing[0] << " " << spaceing[1] << " " << spaceing[2] << std::endl;
	
	vtkSmartPointer<vtkImageChangeInformation> changer =
		vtkSmartPointer<vtkImageChangeInformation>::New();
	changer->SetInputData(reader->GetOutput());
	changer->SetOutputOrigin(100, 100, 0);
	changer->SetOutputSpacing(5, 5, 1);
	changer->SetCenterImage(1);
	changer->Update();

	changer->GetOutput()->GetDimensions(dims);
	std::cout << "修改后图像维数:" << dims[0] << " " << dims[1] << " " << dims[2] << std::endl;
	changer->GetOutput()->GetOrigin(origin);
	std::cout << "修改后图像原点:" << origin[0] << " " << origin[1] << " " << origin[2] << std::endl;
	changer->GetOutput()->GetSpacing(spaceing);
	std::cout << "修改后像素间隔:" << spaceing[0] << " " << spaceing[1] << " " << spaceing[2] << std::endl;

	vtkSmartPointer<vtkRenderWindowInteractor> Interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection(changer->GetOutputPort());
	imageViewer->SetupInteractor(Interactor);
	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();
	imageViewer->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
	imageViewer->SetSize(600,600);
	imageViewer->GetRenderWindow()->SetWindowName("ImageChangeInformationExample");

	Interactor->Initialize();
	Interactor->Start();
	return 0;
}