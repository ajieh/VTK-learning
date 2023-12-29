#include <vtkMath.h>
#include <vtkSmartPointer.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkImageMathematics.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)//避免获取TextRenderer实例失败
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkImageCanvasSource2D> imageSource =
		vtkSmartPointer<vtkImageCanvasSource2D>::New();
	imageSource->SetNumberOfScalarComponents(3);
	imageSource->SetScalarTypeToUnsignedChar();
	imageSource->SetExtent(0, 4, 0, 4, 0, 0);
	imageSource->SetDrawColor(100.0, 0, 0);
	imageSource->FillBox(0, 4, 0, 4);
	imageSource->Update();

	vtkSmartPointer<vtkImageMathematics> imageMath =
		vtkSmartPointer<vtkImageMathematics>::New();
	imageMath->SetOperationToMultiplyByK();//将像素值乘以常数k
	imageMath->SetConstantK(2);
	imageMath->SetInputConnection(imageSource->GetOutputPort());
	imageMath->Update();

	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(imageSource->GetOutput());

	vtkSmartPointer<vtkImageActor> mathActor = vtkSmartPointer<vtkImageActor>::New();
	mathActor->SetInputData(imageMath->GetOutput());

	vtkSmartPointer<vtkRenderer> originalRenderer = vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(0.0, 0.0, 0.5, 1.0);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> mathRenderer = vtkSmartPointer<vtkRenderer>::New();
	mathRenderer->SetViewport(0.5, 0.0, 1.0, 1.0);
	mathRenderer->AddActor(mathActor);
	mathRenderer->ResetCamera();
	mathRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(mathRenderer);
	renderWindow->SetSize(600, 300);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageMathematicsExample");

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


