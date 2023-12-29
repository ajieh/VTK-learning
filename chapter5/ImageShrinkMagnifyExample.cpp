#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkBMPReader.h>
#include <vtkImageShrink3D.h>
#include <vtkImageMagnify.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)//避免获取TextRenderer实例失败
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap05\\DATA\\LENA.BMP");
	reader->Update();

	//缩小16分之一
	vtkSmartPointer<vtkImageShrink3D> shrinkFilter = vtkSmartPointer<vtkImageShrink3D>::New();
	shrinkFilter->SetInputConnection(reader->GetOutputPort());
	shrinkFilter->SetShrinkFactors(16, 16, 1);
	shrinkFilter->Update();

	//扩大10倍
	vtkSmartPointer<vtkImageMagnify> magnifyFilter = vtkSmartPointer<vtkImageMagnify>::New();
	magnifyFilter->SetInputConnection(reader->GetOutputPort());
	magnifyFilter->SetMagnificationFactors(10, 10, 1);
	magnifyFilter->Update();

	int originalDims[3];
	reader->GetOutput()->GetDimensions(originalDims);

	double originalSpace[3];
	reader->GetOutput()->GetSpacing(originalSpace);

	int shrinkDims[3];
	shrinkFilter->GetOutput()->GetDimensions(shrinkDims);

	double shrinkSpace[3];
	shrinkFilter->GetOutput()->GetSpacing(shrinkSpace);

	int magnifyDims[3];
	magnifyFilter->GetOutput()->GetDimensions(magnifyDims);

	double magnifySpace[3];
	magnifyFilter->GetOutput()->GetSpacing(magnifySpace);

	std::cout << "原图图像维数      ：" << originalDims[0] << " " << originalDims[1] << " " << originalDims[2] << std::endl;
	std::cout << "原图图像像素间隔  ：" << originalSpace[0] << " " << originalSpace[1] << " " << originalSpace[2] << std::endl;
	std::cout << "降采样图像维数    ：" << shrinkDims[0] << " " << shrinkDims[1] << " " << shrinkDims[2] << std::endl;
	std::cout << "降采样图像像素间隔：" << shrinkSpace[0] << " " << shrinkSpace[1] << " " << shrinkSpace[2] << std::endl;
	std::cout << "升采样图像维数    ：" << magnifyDims[0] << " " << magnifyDims[1] << " " << magnifyDims[2] << std::endl;
	std::cout << "升采样图像像素间隔：" << magnifySpace[0] << " " << magnifySpace[1] << " " << magnifySpace[2] << std::endl;

	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> shrinkActor = vtkSmartPointer<vtkImageActor>::New();
	shrinkActor->SetInputData(shrinkFilter->GetOutput());

	vtkSmartPointer<vtkImageActor> magnifyActor =
		vtkSmartPointer<vtkImageActor>::New();
	magnifyActor->SetInputData(magnifyFilter->GetOutput());

	vtkSmartPointer<vtkRenderer> originalRenderer = vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(0.0, 0.0, 0.33, 1.0);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> shrinkRenderer = vtkSmartPointer<vtkRenderer>::New();
	shrinkRenderer->SetViewport(0.33, 0.0, 0.66, 1.0);
	shrinkRenderer->AddActor(shrinkActor);
	shrinkRenderer->ResetCamera();
	shrinkRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> magnifyRenderer = vtkSmartPointer<vtkRenderer>::New();
	magnifyRenderer->SetViewport(0.66, 0.0, 1.0, 1.0);
	magnifyRenderer->AddActor(magnifyActor);
	magnifyRenderer->ResetCamera();
	magnifyRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(shrinkRenderer);
	renderWindow->AddRenderer(magnifyRenderer);
	renderWindow->SetSize(900, 300);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageShrinkMagnifyExample");

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


