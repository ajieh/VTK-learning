#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkConeSource.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAppendPolyData.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetRadius(10);
	sphereSource->SetThetaResolution(10);
	sphereSource->SetPhiResolution(10);
	sphereSource->Update();

	vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
	coneSource->SetRadius(5);
	coneSource->SetHeight(10);
	coneSource->SetCenter(25, 0, 0);
	coneSource->Update();

	//构造一个含有多个连通区域的模型数据
	vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
	appendFilter->AddInputData(sphereSource->GetOutput());
	appendFilter->AddInputData(coneSource->GetOutput());
	appendFilter->Update();

	//连通区域分析
	vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter =
		vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	connectivityFilter->SetInputData(appendFilter->GetOutput());
	connectivityFilter->SetExtractionModeToCellSeededRegions();//设置提取模式为基于cell的种子区域。
	connectivityFilter->AddSeed(100);//添加一个种子
	connectivityFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> originalMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	originalMapper->SetInputConnection(appendFilter->GetOutputPort());
	originalMapper->Update();

	vtkSmartPointer<vtkActor> originalActor = vtkSmartPointer<vtkActor>::New();
	originalActor->SetMapper(originalMapper);

	vtkSmartPointer<vtkPolyDataMapper> extractedMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	extractedMapper->SetInputConnection(connectivityFilter->GetOutputPort());
	extractedMapper->Update();

	vtkSmartPointer<vtkActor> extractedActor = vtkSmartPointer<vtkActor>::New();
	extractedActor->SetMapper(extractedMapper);

	vtkSmartPointer<vtkRenderer> renderer1 = vtkSmartPointer<vtkRenderer>::New();
	renderer1->SetViewport(0.0, 0.0, 0.5, 1.0);
	renderer1->AddActor(originalActor);
	renderer1->ResetCamera();
	renderer1->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> renderer2 = vtkSmartPointer<vtkRenderer>::New();
	renderer2->SetViewport(0.5, 0.0, 1.0, 1.0);
	renderer2->AddActor(extractedActor);
	renderer2->ResetCamera();
	renderer2->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer1);
	renderWindow->AddRenderer(renderer2);
	renderWindow->SetSize(600, 400);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataConnectedCompExtract");

	renderer1->ResetCamera();
	renderer2->SetActiveCamera(renderer1->GetActiveCamera());

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	interactor->Initialize();
	interactor->Start();
	return 0;
}




