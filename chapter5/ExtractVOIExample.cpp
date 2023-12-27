#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkBMPReader.h>
#include <vtkExtractVOI.h>
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

	vtkSmartPointer<vtkExtractVOI> extractFilter = vtkSmartPointer<vtkExtractVOI>::New();
	extractFilter->SetInputConnection(reader->GetOutputPort());
	//感兴趣区域，6个参数，分别是x,y,z轴的最小，大值
	extractFilter->SetVOI(dims[0] / 4.0, 3.0 *dims[0] / 4.0, dims[1] / 4.0, 3.0 * dims[1] / 4.0, 0, 0);
	extractFilter->Update();

	vtkSmartPointer<vtkImageActor> originalActor = vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> outputActor = vtkSmartPointer<vtkImageActor>::New();
	outputActor->SetInputData(extractFilter->GetOutput());

	vtkSmartPointer<vtkRenderer> originalRenderer = vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(0.0, 0.0, 0.5, 1.0);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> outputRenderer = vtkSmartPointer<vtkRenderer>::New();
	outputRenderer->SetViewport(0.5, 0.0, 1.0, 1.0);
	outputRenderer->AddActor(outputActor);
	outputRenderer->ResetCamera();
	outputRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(outputRenderer);
	renderWindow->SetSize(800,400);
	renderWindow->Render();
	renderWindow->SetWindowName("ExtractVOIExample");

	vtkSmartPointer<vtkRenderWindowInteractor> Interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	Interactor->SetInteractorStyle(style);
	Interactor->SetRenderWindow(renderWindow);

	Interactor->Initialize();
	Interactor->Start();
	return 0;
}