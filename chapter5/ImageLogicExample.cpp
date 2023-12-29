#include <vtkSmartPointer.h>
#include <vtkMath.h>
#include <vtkImageData.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkImageLogic.h>
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
	vtkSmartPointer<vtkImageCanvasSource2D> imageSource1 =
		vtkSmartPointer<vtkImageCanvasSource2D>::New();
	imageSource1->SetScalarTypeToUnsignedChar();
	imageSource1->SetNumberOfScalarComponents(1);
	imageSource1->SetExtent(0, 100, 0, 100, 0, 0);
	imageSource1->SetDrawColor(0.0);
	imageSource1->FillBox(0, 100, 0, 100);
	imageSource1->SetDrawColor(255);
	imageSource1->FillBox(20, 60, 20, 60);
	imageSource1->Update();

	vtkSmartPointer<vtkImageCanvasSource2D> imageSource2 =
		vtkSmartPointer<vtkImageCanvasSource2D>::New();
	imageSource2->SetNumberOfScalarComponents(1);
	imageSource2->SetScalarTypeToUnsignedChar();
	imageSource2->SetExtent(0, 100, 0, 100, 0, 0);
	imageSource2->SetDrawColor(0.0);
	imageSource2->FillBox(0, 100, 0, 100);
	imageSource2->SetDrawColor(255.0);
	imageSource2->FillBox(40, 80, 40, 80);
	imageSource2->Update();

	vtkSmartPointer<vtkImageLogic> imageLogic = vtkSmartPointer<vtkImageLogic>::New();
	imageLogic->SetInput1Data(imageSource1->GetOutput());
	imageLogic->SetInput2Data(imageSource2->GetOutput());
	imageLogic->SetOperationToXor();//异或
	imageLogic->SetOutputTrueValue(128);//结果为真时，输出像素值128
	imageLogic->Update();

	vtkSmartPointer<vtkImageActor> originalActor1 = vtkSmartPointer<vtkImageActor>::New();
	originalActor1->SetInputData(imageSource1->GetOutput());

	vtkSmartPointer<vtkImageActor> originalActor2 = vtkSmartPointer<vtkImageActor>::New();
	originalActor2->SetInputData(imageSource2->GetOutput());

	vtkSmartPointer<vtkImageActor> logicActor = vtkSmartPointer<vtkImageActor>::New();
	logicActor->SetInputData(imageLogic->GetOutput());

	vtkSmartPointer<vtkRenderer> renderer1 =
		vtkSmartPointer<vtkRenderer>::New();
	renderer1->SetViewport(0.0, 0.0, 0.33, 1.0);
	renderer1->AddActor(originalActor1);
	renderer1->ResetCamera();
	renderer1->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> renderer2 =
		vtkSmartPointer<vtkRenderer>::New();
	renderer2->SetViewport(0.33, 0.0, 0.66, 1.0);
	renderer2->AddActor(originalActor2);
	renderer2->ResetCamera();
	renderer2->SetBackground(0.8, 0.8, 0.8);

	vtkSmartPointer<vtkRenderer> logicRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	logicRenderer->SetViewport(0.66, 0.0, 1.0, 1.0);
	logicRenderer->AddActor(logicActor);
	logicRenderer->ResetCamera();
	logicRenderer->SetBackground(0.6, 0.6, 0.6);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer1);
	renderWindow->AddRenderer(renderer2);
	renderWindow->AddRenderer(logicRenderer);
	renderWindow->SetSize(900, 300);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageLogicExample");

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


