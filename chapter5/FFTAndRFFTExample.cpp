#include <vtkSmartPointer.h>
#include <vtkCamera.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkImageFFT.h>
#include <vtkImageRFFT.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkImageCast.h>
#include <vtkJPEGReader.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageFourierCenter.h>
#include <vtkImageMagnitude.h>
#include <vtkImageShiftScale.h>
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

	vtkSmartPointer<vtkImageExtractComponents> fftExtractReal =
		vtkSmartPointer<vtkImageExtractComponents>::New();
	fftExtractReal->SetInputConnection(fftFilter->GetOutputPort());
	fftExtractReal->SetComponents(0);

	double range[2];
	fftExtractReal->GetOutput()->GetScalarRange(range);

	vtkSmartPointer<vtkImageShiftScale> ShiftScale =
		vtkSmartPointer<vtkImageShiftScale>::New();
	ShiftScale->SetOutputScalarTypeToUnsignedChar();
	ShiftScale->SetScale(255.0 / (range[1] - range[0]));
	ShiftScale->SetShift(-range[0]);
	ShiftScale->SetInputConnection(fftExtractReal->GetOutputPort());
	ShiftScale->Update();

	//逆变换
	vtkSmartPointer<vtkImageRFFT> rfftFilter =
		vtkSmartPointer<vtkImageRFFT>::New();
	rfftFilter->SetInputConnection(fftFilter->GetOutputPort());
	rfftFilter->SetDimensionality(2);

	vtkSmartPointer<vtkImageExtractComponents> ifftExtractReal =
		vtkSmartPointer<vtkImageExtractComponents>::New();
	ifftExtractReal->SetInputConnection(rfftFilter->GetOutputPort());
	ifftExtractReal->SetComponents(0);
	ifftExtractReal->Update();

	vtkSmartPointer<vtkImageCast> rfftCast =
		vtkSmartPointer<vtkImageCast>::New();
	rfftCast->SetInputConnection(ifftExtractReal->GetOutputPort());
	rfftCast->SetOutputScalarTypeToUnsignedChar();
	rfftCast->Update();

	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> fftActor =
		vtkSmartPointer<vtkImageActor>::New();
	fftActor->SetInputData(ShiftScale->GetOutput());

	vtkSmartPointer<vtkImageActor> rfftActor =
		vtkSmartPointer<vtkImageActor>::New();
	rfftActor->SetInputData(rfftCast->GetOutput());

	vtkSmartPointer<vtkRenderer> originalRenderer = vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(0.0, 0.0, 0.33, 1.0);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> fftRenderer = vtkSmartPointer<vtkRenderer>::New();
	fftRenderer->SetViewport(0.33, 0.0, 0.66, 1.0);
	fftRenderer->AddActor(fftActor);
	fftRenderer->ResetCamera();
	fftRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> rfftRenderer = vtkSmartPointer<vtkRenderer>::New();
	rfftRenderer->SetViewport(0.66, 0.0, 1.0, 1.0);
	rfftRenderer->AddActor(rfftActor);
	rfftRenderer->ResetCamera();
	rfftRenderer->SetBackground(1.0, 1.0, 1.0);


	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(fftRenderer);
	renderWindow->AddRenderer(rfftRenderer);
	renderWindow->SetSize(900, 300);
	renderWindow->Render();
	renderWindow->SetWindowName("FFTAndRFFTExample");

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




