#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkJPEGReader.h>
#include <vtkImageLuminance.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap05\\DATA\\lena-gray.jpg");
	reader->Update();

	//生成颜色映射表
	vtkSmartPointer<vtkLookupTable> lookupTable = vtkSmartPointer<vtkLookupTable>::New();
	lookupTable->SetRange(0, 255);//标量值范围
	lookupTable->SetHueRange(0.4, 0.8);//色调范围
	lookupTable->SetValueRange(0.6 ,1.0);//亮度范围
	lookupTable->Build();

	vtkSmartPointer<vtkImageMapToColors> colorMap = vtkSmartPointer<vtkImageMapToColors>::New();
	colorMap->SetInputConnection(reader->GetOutputPort());
	colorMap->SetLookupTable(lookupTable);
	colorMap->Update();

	vtkSmartPointer<vtkImageActor> originalActor = vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> shiftscaleActor = vtkSmartPointer<vtkImageActor>::New();
	shiftscaleActor->SetInputData(colorMap->GetOutput());
	
	vtkSmartPointer<vtkRenderer> originalRenderer = vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(0.0, 0.0, 0.5, 1.0);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> shiftscaleRenderer = vtkSmartPointer<vtkRenderer>::New();
	shiftscaleRenderer->SetViewport(0.5, 0.0, 1.0, 1.0);
	shiftscaleRenderer->AddActor(shiftscaleActor);
	shiftscaleRenderer->ResetCamera();
	shiftscaleRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(shiftscaleRenderer);
	renderWindow->SetSize(600,300);
	renderWindow->Render();
	renderWindow->SetWindowName("Gray2ColorImageExample");

	vtkSmartPointer<vtkRenderWindowInteractor> Interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	Interactor->SetInteractorStyle(style);
	Interactor->SetRenderWindow(renderWindow);

	Interactor->Initialize();
	Interactor->Start();
	return 0;
}