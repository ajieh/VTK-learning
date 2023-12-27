#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkBMPReader.h>
#include <vtkImageData.h>
#include <vtkImageIterator.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap05\\DATA\\LENA.BMP");
	reader->Update();

	int subRegion[6] = { 0, 300, 0, 300, 0, 0 };
	//参数1：访问的图像，参数2：访问的区域
	vtkImageIterator<unsigned char> it(reader->GetOutput(), subRegion);
	while (!it.IsAtEnd())
	{
		unsigned char *inSI = it.BeginSpan();
		unsigned char *inSIEnd = it.EndSpan();

		while (inSI != inSIEnd)
		{
			*inSI = 255 - *inSI;//灰度值取反
			++inSI;
		}
		it.NextSpan();
	}
	
	vtkSmartPointer<vtkRenderWindowInteractor> Interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());
	imageViewer->SetupInteractor(Interactor);
	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();
	imageViewer->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
	imageViewer->SetSize(600,600);
	imageViewer->GetRenderWindow()->SetWindowName("VisitImagePixelIterativelyExample");

	Interactor->Initialize();
	Interactor->Start();
	return 0;
}