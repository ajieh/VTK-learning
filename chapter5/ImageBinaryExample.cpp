#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkJPEGReader.h>
#include <vtkImageThreshold.h>
#include <vtkImageCast.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)//避免获取TextRenderer实例失败
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap05\\DATA\\lena-gray.jpg");
	reader->Update();

	vtkSmartPointer<vtkImageThreshold> thresholdFilter = vtkSmartPointer<vtkImageThreshold>::New();
	thresholdFilter->SetInputConnection(reader->GetOutputPort());
	thresholdFilter->ThresholdByUpper(100);//有效范围取大于100
	thresholdFilter->SetInValue(255);//范围内取值
	thresholdFilter->SetOutValue(0);//范围外取值
	thresholdFilter->Update();

	vtkSmartPointer<vtkImageActor> originalActor = vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> binaryActor = vtkSmartPointer<vtkImageActor>::New();
	binaryActor->SetInputData(thresholdFilter->GetOutput());

	vtkSmartPointer<vtkRenderer> renderer1 = vtkSmartPointer<vtkRenderer>::New();
	renderer1->SetViewport(0.0, 0.0, 0.5, 1.0);
	renderer1->AddActor(originalActor);
	renderer1->ResetCamera();
	renderer1->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> renderer2 = vtkSmartPointer<vtkRenderer>::New();
	renderer2->SetViewport(0.5, 0.0, 1.0, 1.0);
	renderer2->AddActor(binaryActor);
	renderer2->ResetCamera();
	renderer2->SetBackground(0.5, 0.5, 0.5);	

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer1);
	renderWindow->AddRenderer(renderer2);
	renderWindow->SetSize(600, 300);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageBinaryExample");

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


