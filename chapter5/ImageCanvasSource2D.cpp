#include <vtkSmartPointer.h>
#include <vtkImageData.h>
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
	vtkSmartPointer<vtkImageCanvasSource2D> canvas = vtkSmartPointer<vtkImageCanvasSource2D>::New();
	canvas->SetScalarTypeToUnsignedChar();
	canvas->SetNumberOfScalarComponents(1);
	canvas->SetExtent(0, 100, 0, 100, 0, 0);
	canvas->SetDrawColor(0, 0, 0, 0);
	canvas->FillBox(0, 100, 0, 100);
	canvas->SetDrawColor(255, 0, 0, 0);
	canvas->FillBox(15, 35, 60, 80);
	canvas->SetDrawColor(255, 0, 0, 0);
	canvas->FillBox(65, 85, 60, 80);
	canvas->SetDrawColor(255, 0, 0, 0);
	canvas->FillBox(35, 65, 10, 40);
	canvas->Update();

	vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
	actor->SetInputData(canvas->GetOutput());

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetViewport(0.0, 0.0, 1.0, 1.0);
	renderer->AddActor(actor);
	renderer->ResetCamera();
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(600, 600);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageCanvasSource2D");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();

	interactor->SetInteractorStyle(style);
	interactor->SetRenderWindow(renderWindow);
	interactor->Initialize();
	interactor->Start();
	return 0;
}