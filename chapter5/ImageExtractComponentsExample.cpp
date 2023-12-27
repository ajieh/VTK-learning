#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkImageExtractComponents.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkBMPReader.h>
#include <vtkImageActor.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap05\\DATA\\LENA.BMP");
	reader->Update();

	//提取RGB组分
	vtkSmartPointer<vtkImageExtractComponents> extractRedFilter =
		vtkSmartPointer<vtkImageExtractComponents>::New();
	extractRedFilter->SetInputConnection(reader->GetOutputPort());
	extractRedFilter->SetComponents(0);
	extractRedFilter->Update();
	
	vtkSmartPointer<vtkImageExtractComponents> extractGreenFilter =
		vtkSmartPointer<vtkImageExtractComponents>::New();
	extractGreenFilter->SetInputConnection(reader->GetOutputPort());
	extractGreenFilter->SetComponents(1);
	extractGreenFilter->Update();

	vtkSmartPointer<vtkImageExtractComponents> extractBlueFilter =
		vtkSmartPointer<vtkImageExtractComponents>::New();
	extractBlueFilter->SetInputConnection(reader->GetOutputPort());
	extractBlueFilter->SetComponents(2);
	extractBlueFilter->Update();

	vtkSmartPointer<vtkImageActor> originalActor = vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> redActor = vtkSmartPointer<vtkImageActor>::New();
	redActor->SetInputData(extractRedFilter->GetOutput());

	vtkSmartPointer<vtkImageActor> greenActor = vtkSmartPointer<vtkImageActor>::New();
	greenActor->SetInputData(extractGreenFilter->GetOutput());

	vtkSmartPointer<vtkImageActor> blueActor = vtkSmartPointer<vtkImageActor>::New();
	blueActor->SetInputData(extractBlueFilter->GetOutput());
	
	vtkSmartPointer<vtkRenderer> originalRenderer = vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(0.0, 0.0, 0.25, 1.0);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> redRenderer = vtkSmartPointer<vtkRenderer>::New();
	redRenderer->SetViewport(0.25, 0.0, 0.5, 1.0);
	redRenderer->AddActor(redActor);
	redRenderer->ResetCamera();
	redRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> greenRenderer = vtkSmartPointer<vtkRenderer>::New();
	greenRenderer->SetViewport(0.5, 0.0, 0.75, 1.0);
	greenRenderer->AddActor(greenActor);
	greenRenderer->ResetCamera();
	greenRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> blueRenderer = vtkSmartPointer<vtkRenderer>::New();
	blueRenderer->SetViewport(0.75, 0.0, 1.0, 1.0);
	blueRenderer->AddActor(blueActor);
	blueRenderer->ResetCamera();
	blueRenderer->SetBackground(1.0, 1.0, 1.0);


	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(redRenderer);
	renderWindow->AddRenderer(greenRenderer);
	renderWindow->AddRenderer(blueRenderer);
	renderWindow->SetSize(1200,300);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageExtractComponentsExample");

	vtkSmartPointer<vtkRenderWindowInteractor> Interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	Interactor->SetInteractorStyle(style);
	Interactor->SetRenderWindow(renderWindow);

	Interactor->Initialize();
	Interactor->Start();
	return 0;
}