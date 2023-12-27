#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindow.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkImageData> image = vtkSmartPointer<vtkImageData>::New();
	image->SetDimensions(16,16,1);//设置图像数据的尺寸
	image->AllocateScalars(VTK_UNSIGNED_CHAR, 1);// 设置图像数据的标量类型和通道数

	unsigned char *ptr = static_cast<unsigned char*>(image->GetScalarPointer());
	for (int i = 0; i < 256; i++)
	{
		*ptr++ = i % 256;//每个像素的值在0到255的范围内
	}
	vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
	actor->SetInputData(image);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetViewport(0.0, 0.0, 1.0, 1.0);
	renderer->AddActor(actor);
	renderer->ResetCamera();
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(600, 600);
	renderWindow->Render();
	renderWindow->SetWindowName("CreateVTKImageData");

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