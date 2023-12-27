#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkBMPReader.h>
#include <vtkImageLuminance.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap05\\DATA\\LENA.BMP");
	reader->Update();

	vtkSmartPointer<vtkImageLuminance> luminance = vtkSmartPointer<vtkImageLuminance>::New();
	luminance->SetInputConnection(reader->GetOutputPort());
	luminance->Update();

	vtkSmartPointer<vtkImageActor> originalActor = vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> shiftscaleActor = vtkSmartPointer<vtkImageActor>::New();
	shiftscaleActor->SetInputData(luminance->GetOutput());
	
	vtkSmartPointer<vtkRenderer> originalRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(0.0, 0.0, 0.5, 1.0);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> shiftscaleRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	shiftscaleRenderer->SetViewport(0.5, 0.0, 1.0, 1.0);
	shiftscaleRenderer->AddActor(shiftscaleActor);
	shiftscaleRenderer->ResetCamera();
	shiftscaleRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(shiftscaleRenderer);
	renderWindow->SetSize(680, 340);
	renderWindow->Render();
	renderWindow->SetWindowName("Color2GrayImageExample");

	vtkSmartPointer<vtkRenderWindowInteractor> Interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	Interactor->SetInteractorStyle(style);
	Interactor->SetRenderWindow(renderWindow);

	Interactor->Initialize();
	Interactor->Start();
	return 0;
}