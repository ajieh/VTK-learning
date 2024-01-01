#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkImageButterworthLowPass.h>
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

	vtkSmartPointer<vtkImageFFT> fftFilter = vtkSmartPointer<vtkImageFFT>::New();
	fftFilter->SetInputConnection(reader->GetOutputPort());
	fftFilter->Update();

	//巴特沃斯低通滤波器
	vtkSmartPointer<vtkImageButterworthLowPass> lowPassFilter =
		vtkSmartPointer<vtkImageButterworthLowPass>::New();
	lowPassFilter->SetInputConnection(fftFilter->GetOutputPort());
	lowPassFilter->SetXCutOff(0.05);
	lowPassFilter->SetYCutOff(0.05);
	lowPassFilter->Update();

	vtkSmartPointer<vtkImageRFFT> rfftFilter = vtkSmartPointer<vtkImageRFFT>::New();
	rfftFilter->SetInputConnection(lowPassFilter->GetOutputPort());
	rfftFilter->Update();

	vtkSmartPointer<vtkImageExtractComponents> ifftExtractReal =
		vtkSmartPointer<vtkImageExtractComponents>::New();
	ifftExtractReal->SetInputConnection(rfftFilter->GetOutputPort());
	ifftExtractReal->SetComponents(0);

	vtkSmartPointer<vtkImageCast> castFilter = vtkSmartPointer<vtkImageCast>::New();
	castFilter->SetInputConnection(ifftExtractReal->GetOutputPort());
	castFilter->SetOutputScalarTypeToUnsignedChar();
	castFilter->Update();

	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> outputActor = vtkSmartPointer<vtkImageActor>::New();
	outputActor->SetInputData(castFilter->GetOutput());

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
	renderWindow->SetWindowName("ButterworthLowPassExample");

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