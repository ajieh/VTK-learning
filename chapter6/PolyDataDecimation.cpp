#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkDecimatePro.h>
#include <vtkQuadricDecimation.h>
#include <vtkQuadricClustering.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkPolydataReader.h>
#include <vtkCamera.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkPolyDataReader> reader =
		vtkSmartPointer<vtkPolyDataReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap06\\DATA\\FRAN_CUT.VTK");
	reader->Update();

	vtkSmartPointer<vtkPolyData> original = reader->GetOutput();

	std::cout << "抽取前：" << std::endl << "------------" << std::endl;
	std::cout << "模型点数为： " << original->GetNumberOfPoints() << std::endl;
	std::cout << "模型面数为： " << original->GetNumberOfPolys() << std::endl;

	//网格抽取
	vtkSmartPointer<vtkDecimatePro> decimate = vtkSmartPointer<vtkDecimatePro>::New();
	decimate->SetInputData(original);
	decimate->SetTargetReduction(.80);//缩减的三角面片数量的目标比例为 0.8
	decimate->Update();

	vtkSmartPointer<vtkPolyData> decimated = decimate->GetOutput();
	std::cout << "抽取后" << std::endl << "------------" << std::endl;
	std::cout << "模型点数为：" << decimated->GetNumberOfPoints() << std::endl;
	std::cout << "模型面数为：" << decimated->GetNumberOfPolys() << std::endl;
	

	vtkSmartPointer<vtkPolyDataMapper> originalMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	originalMapper->SetInputData(original);
	originalMapper->Update();

	vtkSmartPointer<vtkActor> originalActor = vtkSmartPointer<vtkActor>::New();
	originalActor->SetMapper(originalMapper);

	vtkSmartPointer<vtkPolyDataMapper> decimatedMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	decimatedMapper->SetInputData(decimated);

	vtkSmartPointer<vtkActor> decimatedActor = vtkSmartPointer<vtkActor>::New();
	decimatedActor->SetMapper(decimatedMapper);

	vtkSmartPointer<vtkRenderer> renderer1 = vtkSmartPointer<vtkRenderer>::New();
	renderer1->SetViewport(0.0, 0.0, 0.5, 1.0);
	renderer1->AddActor(originalActor);
	renderer1->ResetCamera();
	renderer1->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> renderer2 = vtkSmartPointer<vtkRenderer>::New();
	renderer2->SetViewport(0.5, 0.0, 1.0, 1.0);
	renderer2->AddActor(decimatedActor);
	renderer2->ResetCamera();
	renderer2->SetBackground(1.0, 1.0, 1.0);

	renderer1->GetActiveCamera()->SetPosition(0, -1, 0);
	renderer1->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer1->GetActiveCamera()->SetViewUp(0, 0, 1);
	renderer1->GetActiveCamera()->Azimuth(30);
	renderer1->GetActiveCamera()->Elevation(30);
	renderer1->ResetCamera();
	renderer2->SetActiveCamera(renderer1->GetActiveCamera());

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer1);
	renderWindow->AddRenderer(renderer2);
	renderWindow->SetSize(600, 400);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataDecimation");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	interactor->Initialize();
	interactor->Start();
	return 0;
}




