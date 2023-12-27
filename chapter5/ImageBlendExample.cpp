#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkImageBlend.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkJPEGReader.h>
#include <vtkImageActor.h>
#include <vtkImageCast.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	//读取灰度图像
	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap05\\DATA\\lena-gray.jpg");
	reader->Update();

	//生成二值图像
	vtkSmartPointer<vtkImageCanvasSource2D> imageSource =
		vtkSmartPointer<vtkImageCanvasSource2D>::New();
	imageSource->SetNumberOfScalarComponents(1);
	imageSource->SetScalarTypeToUnsignedChar();
	imageSource->SetExtent(0, 512, 0, 512, 0, 0);
	imageSource->SetDrawColor(0.0);
	imageSource->FillBox(0, 512, 0, 512);
	imageSource->SetDrawColor(255.0);
	imageSource->FillBox(100, 400, 100, 400);
	imageSource->Update();

	//图像融合
	vtkSmartPointer<vtkImageBlend> imageBlend = vtkSmartPointer<vtkImageBlend>::New();
	imageBlend->AddInputData(reader->GetOutput());
	imageBlend->AddInputData(imageSource->GetOutput());
	imageBlend->SetOpacity(0, 0.4);//设置不透明度
	imageBlend->SetOpacity(1, 0.6);
	imageBlend->Update();

	vtkSmartPointer<vtkImageActor> originalActor1 = vtkSmartPointer<vtkImageActor>::New();
	originalActor1->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> originalActor2 = vtkSmartPointer<vtkImageActor>::New();
	originalActor2->SetInputData(imageSource->GetOutput());

	vtkSmartPointer<vtkImageActor> blendActor = vtkSmartPointer<vtkImageActor>::New();
	blendActor->SetInputData(imageBlend->GetOutput());

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
	renderer2->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> renderer3 = vtkSmartPointer<vtkRenderer>::New();
	renderer3->SetViewport(0.66, 0.0, 1.0, 1.0);
	renderer3->AddActor(blendActor);
	renderer3->ResetCamera();
	renderer3->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer1);
	renderWindow->AddRenderer(renderer2);
	renderWindow->AddRenderer(renderer3);
	renderWindow->SetSize(680,380);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageBlendExample");

	vtkSmartPointer<vtkRenderWindowInteractor> Interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();
	Interactor->SetInteractorStyle(style);
	Interactor->SetRenderWindow(renderWindow);
	Interactor->Initialize();
	Interactor->Start();
	return 0;
}