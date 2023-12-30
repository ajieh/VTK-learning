#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkImageButterworthHighPass.h>
#include <vtkImageActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkJPEGReader.h>
#include <vtkImageFFT.h>
#include <vtkImageRFFT.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageCast.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap05\\DATA\\lena-gray.jpg");
	reader->Update();

	//快速傅里叶变换
	vtkSmartPointer<vtkImageFFT> fftFilter = vtkSmartPointer<vtkImageFFT>::New();
	fftFilter->SetInputConnection(reader->GetOutputPort());
	fftFilter->SetDimensionality(2);
	fftFilter->Update();

	//巴特沃斯高通滤波器
	vtkSmartPointer<vtkImageButterworthHighPass> highPassFilter =
		vtkSmartPointer<vtkImageButterworthHighPass>::New();
	highPassFilter->SetInputConnection(fftFilter->GetOutputPort());
	highPassFilter->SetXCutOff(0.1);
	highPassFilter->SetYCutOff(0.1);
	highPassFilter->Update();

	//逆变换
	vtkSmartPointer<vtkImageRFFT> rfftFilter = vtkSmartPointer<vtkImageRFFT>::New();
	rfftFilter->SetInputConnection(highPassFilter->GetOutputPort());
	rfftFilter->SetDimensionality(2);

	vtkSmartPointer<vtkImageExtractComponents> ifftExtractReal =
		vtkSmartPointer<vtkImageExtractComponents>::New();
	ifftExtractReal->SetInputConnection(rfftFilter->GetOutputPort());
	ifftExtractReal->SetComponents(0);
	ifftExtractReal->Update();

	vtkSmartPointer<vtkImageCast> rfftCast = vtkSmartPointer<vtkImageCast>::New();
	rfftCast->SetInputConnection(ifftExtractReal->GetOutputPort());
	rfftCast->SetOutputScalarTypeToUnsignedChar();
	rfftCast->Update();

	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> outputActor =
		vtkSmartPointer<vtkImageActor>::New();
	outputActor->SetInputData(rfftCast->GetOutput());

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
	renderWindow->SetSize(600, 300);
	renderWindow->Render();
	renderWindow->SetWindowName("ButterworthHighPassExample");

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




