#include <vtkSmartPointer.h>
#include <vtkMarchingCubes.h>
#include <vtkContourFilter.h>
#include <vtkVoxelModeller.h>
#include <vtkSphereSource.h>
#include <vtkImageData.h>
#include <vtkMetaImageReader.h>
#include <vtkInteractorStyleImage.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkProperty.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap06\\DATA\\HeadMRVolume.mhd");
	reader->Update();

	//移动立方体算法
	vtkSmartPointer<vtkMarchingCubes> surface = vtkSmartPointer<vtkMarchingCubes>::New();
	surface->SetInputData(reader->GetOutput());
	surface->ComputeNormalsOn();
	surface->SetValue(0, 100.0);


	vtkSmartPointer<vtkPolyDataMapper> surfMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	surfMapper->SetInputConnection(surface->GetOutputPort());
	surfMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> surfActor = vtkSmartPointer<vtkActor>::New();
	surfActor->SetMapper(surfMapper);
	surfActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(surfActor);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(600, 400);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataMarchingCubes");
	renderWindow->Render();

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);
	renderWindow->Render();
	interactor->Initialize();
	interactor->Start();
	return 0;
}




