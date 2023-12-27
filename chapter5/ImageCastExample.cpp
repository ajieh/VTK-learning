#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkBMPReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkMetaImageReader.h>
#include <vtkImageCast.h>
#include <vtkImageShiftScale.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap05\\DATA\\LENA.BMP");
	reader->Update();

	vtkSmartPointer<vtkImageCast> imageCast =
		vtkSmartPointer<vtkImageCast>::New();
	imageCast->SetInputData((vtkDataObject *)reader->GetOutput());
	imageCast->SetOutputScalarTypeToFloat();
	imageCast->Update();

	vtkSmartPointer<vtkImageShiftScale> imageShiftScale =
		vtkSmartPointer<vtkImageShiftScale>::New();
	imageShiftScale->SetInputData((vtkDataObject *)reader->GetOutput());
	imageShiftScale->SetOutputScalarTypeToUnsignedChar();
	//imageShiftScale->SetShift(1);
	//imageShiftScale->SetScale(127.5);
	imageShiftScale->Update();
	
	vtkSmartPointer<vtkRenderWindowInteractor> Interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	//imageViewer->SetInputConnection(imageCast->GetOutputPort());
	imageViewer->SetInputConnection(imageShiftScale->GetOutputPort());
	imageViewer->SetupInteractor(Interactor);
	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();
	imageViewer->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
	imageViewer->SetSize(600,600);
	imageViewer->GetRenderWindow()->SetWindowName("ImageCastExample");

	Interactor->Initialize();
	Interactor->Start();
	return 0;
}