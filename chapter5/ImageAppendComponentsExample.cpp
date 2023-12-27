#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkImageAppendComponents.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkImageCanvasSource2D> red = vtkSmartPointer<vtkImageCanvasSource2D>::New();
	red->SetScalarTypeToUnsignedChar();
	red->SetNumberOfScalarComponents(1);
	red->SetExtent(0, 100, 0, 100, 0, 0);
	red->SetDrawColor(0, 0, 0, 0);
	red->FillBox(0, 100, 0, 100);
	red->SetDrawColor(255, 0, 0, 0);
	red->FillBox(20, 40, 20, 40);
	red->Update();

	vtkSmartPointer<vtkImageCanvasSource2D> green = vtkSmartPointer<vtkImageCanvasSource2D>::New();
	green->SetScalarTypeToUnsignedChar();
	green->SetNumberOfScalarComponents(1);
	green->SetExtent(0, 100, 0, 100, 0, 0);
	green->SetDrawColor(0, 0, 0, 0);
	green->FillBox(0, 100, 0, 100);
	green->SetDrawColor(255, 0, 0, 0);
	green->FillBox(30, 50, 30, 50);
	green->Update();

	vtkSmartPointer<vtkImageCanvasSource2D> blue = vtkSmartPointer<vtkImageCanvasSource2D>::New();
	blue->SetScalarTypeToUnsignedChar();
	blue->SetNumberOfScalarComponents(1);
	blue->SetExtent(0, 100, 0, 100, 0, 0);
	blue->SetDrawColor(0, 0, 0, 0);
	blue->FillBox(0, 100, 0, 100);
	blue->SetDrawColor(255, 0, 0, 0);
	blue->FillBox(40, 60, 40, 60);
	blue->Update();

	vtkSmartPointer<vtkImageAppendComponents> append = vtkSmartPointer<vtkImageAppendComponents>::New();
	append->SetInputConnection(0, red->GetOutputPort());
	append->AddInputConnection(0, green->GetOutputPort());
	append->AddInputConnection(0, blue->GetOutputPort());
	append->Update();

	vtkSmartPointer<vtkImageActor> redActor = vtkSmartPointer<vtkImageActor>::New();
	redActor->SetInputData(red->GetOutput());

	vtkSmartPointer<vtkImageActor> greenActor = vtkSmartPointer<vtkImageActor>::New();
	greenActor->SetInputData(green->GetOutput());

	vtkSmartPointer<vtkImageActor> blueActor = vtkSmartPointer<vtkImageActor>::New();
	blueActor->SetInputData(blue->GetOutput());

	vtkSmartPointer<vtkImageActor> outputActor = vtkSmartPointer<vtkImageActor>::New();
	outputActor->SetInputData(append->GetOutput());

	vtkSmartPointer<vtkRenderer> redRenderer = vtkSmartPointer<vtkRenderer>::New();
	redRenderer->SetViewport(0.0, 0.0, 0.25, 1.0);
	redRenderer->AddActor(redActor);
	redRenderer->ResetCamera();
	redRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> greenRenderer = vtkSmartPointer<vtkRenderer>::New();
	greenRenderer->SetViewport(0.25, 0.0, 0.5, 1.0);
	greenRenderer->AddActor(greenActor);
	greenRenderer->ResetCamera();
	greenRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> blueRenderer = vtkSmartPointer<vtkRenderer>::New();
	blueRenderer->SetViewport(0.5, 0.0, 0.75, 1.0);
	blueRenderer->AddActor(blueActor);
	blueRenderer->ResetCamera();
	blueRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> outputRenderer = vtkSmartPointer<vtkRenderer>::New();
	outputRenderer->SetViewport(0.75, 0.0, 1.0, 1.0);
	outputRenderer->AddActor(outputActor);
	outputRenderer->ResetCamera();
	outputRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(redRenderer);
	renderWindow->AddRenderer(greenRenderer);
	renderWindow->AddRenderer(blueRenderer);
	renderWindow->AddRenderer(outputRenderer);
	renderWindow->SetSize(1200,300);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageAppendComponentsExample");

	vtkSmartPointer<vtkRenderWindowInteractor> Interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	Interactor->SetInteractorStyle(style);
	Interactor->SetRenderWindow(renderWindow);

	Interactor->Initialize();
	Interactor->Start();
	return 0;
}