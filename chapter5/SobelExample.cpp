#include <vtkSmartPointer.h>
#include <vtkImageMathematics.h>
#include <vtkImageData.h>
#include <vtkImageSobel2D.h>
#include <vtkImageMagnitude.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageShiftScale.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkJPEGReader.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)//避免获取TextRenderer实例失败
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap05\\DATA\\lena-gray.jpg");
	reader->Update();

	//sobel算子
	vtkSmartPointer<vtkImageSobel2D> sobelFilter = vtkSmartPointer<vtkImageSobel2D>::New();
	sobelFilter->SetInputConnection(reader->GetOutputPort());
	sobelFilter->Update();

	//提取X方向上的梯度分量
	vtkSmartPointer<vtkImageExtractComponents> extractXFilter = vtkSmartPointer<vtkImageExtractComponents>::New();
	extractXFilter->SetComponents(0);
	extractXFilter->SetInputConnection(sobelFilter->GetOutputPort());
	extractXFilter->Update();

	double xRange[2];
	extractXFilter->GetOutput()->GetScalarRange(xRange);

	//将x方向的边缘图像中的负值转换为正值
	vtkSmartPointer<vtkImageMathematics> xImageAbs = vtkSmartPointer<vtkImageMathematics>::New();
	xImageAbs->SetOperationToAbsoluteValue();
	xImageAbs->SetInputConnection(extractXFilter->GetOutputPort());
	xImageAbs->Update();

	//将范围调节到0-255
	vtkSmartPointer<vtkImageShiftScale> xShiftScale =
		vtkSmartPointer<vtkImageShiftScale>::New();
	xShiftScale->SetOutputScalarTypeToUnsignedChar();
	xShiftScale->SetScale(255 / xRange[1]);
	xShiftScale->SetInputConnection(xImageAbs->GetOutputPort());
	xShiftScale->Update();

	//提取Y方向上的梯度分量
	vtkSmartPointer<vtkImageExtractComponents> extractYFilter = vtkSmartPointer<vtkImageExtractComponents>::New();
	extractYFilter->SetComponents(1);
	extractYFilter->SetInputConnection(sobelFilter->GetOutputPort());
	extractYFilter->Update();

	double yRange[2];
	extractYFilter->GetOutput()->GetScalarRange(yRange);

	vtkSmartPointer<vtkImageMathematics> yImageAbs = vtkSmartPointer<vtkImageMathematics>::New();
	yImageAbs->SetOperationToAbsoluteValue();
	yImageAbs->SetInputConnection(extractYFilter->GetOutputPort());
	yImageAbs->Update();

	vtkSmartPointer<vtkImageShiftScale> yShiftScale = vtkSmartPointer<vtkImageShiftScale>::New();
	yShiftScale->SetOutputScalarTypeToUnsignedChar();
	yShiftScale->SetScale(255 / yRange[1]);
	yShiftScale->SetInputConnection(yImageAbs->GetOutputPort());
	yShiftScale->Update();

	vtkSmartPointer<vtkImageActor> originalActor = vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> xActor = vtkSmartPointer<vtkImageActor>::New();
	xActor->SetInputData(xShiftScale->GetOutput());

	vtkSmartPointer<vtkImageActor> yActor = vtkSmartPointer<vtkImageActor>::New();
	yActor->SetInputData(yShiftScale->GetOutput());

	vtkSmartPointer<vtkRenderer> originalRenderer = vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(0.0, 0.0, 0.33, 1.0);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> xRenderer = vtkSmartPointer<vtkRenderer>::New();
	xRenderer->SetViewport(0.33, 0.0, 0.66, 1.0);
	xRenderer->AddActor(xActor);
	xRenderer->ResetCamera();
	xRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> yRenderer = vtkSmartPointer<vtkRenderer>::New();
	yRenderer->SetViewport(0.66, 0.0, 1.0, 1.0);
	yRenderer->AddActor(yActor);
	yRenderer->ResetCamera();
	yRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(xRenderer);
	renderWindow->AddRenderer(yRenderer);
	renderWindow->SetSize(900, 300);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageGradientExample");

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


