#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	//创建源对象
	vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
	vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
	vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New(); 
	vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New(); 
	
	// 设置源对象输出作为映射器的输入
	vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkSmartPointer<vtkPolyDataMapper>::New(); 
	coneMapper->SetInputConnection(cone->GetOutputPort());
	vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New(); 
	cubeMapper->SetInputConnection(cube->GetOutputPort());
	vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New(); 
	cylinderMapper->SetInputConnection(cylinder->GetOutputPort()); 
	vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New(); 
	sphereMapper->SetInputConnection(sphere->GetOutputPort());

	//创建锥体演员对象
	vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();
	coneActor->SetMapper(coneMapper); 
	vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New(); 
	cubeActor->SetMapper(cubeMapper);
	vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New(); 
	cylinderActor->SetMapper(cylinderMapper);
	vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New(); 
	sphereActor->SetMapper(sphereMapper); 

	//设置渲染器
	vtkSmartPointer<vtkRenderer> renderer1 = vtkSmartPointer<vtkRenderer>::New();
	renderer1->AddActor(coneActor);
	renderer1->SetBackground(1.0, 0.0, 0.0);
	renderer1->SetViewport(0.0, 0.0, 0.5, 0.5);
	vtkSmartPointer<vtkRenderer> renderer2 = vtkSmartPointer<vtkRenderer>::New();
	renderer2->AddActor(cubeActor);
	renderer2->SetBackground(0.0, 1.0, 0.0);
	renderer2->SetViewport(0.5, 0.0, 1.0, 0.5);
	vtkSmartPointer<vtkRenderer> renderer3 = vtkSmartPointer<vtkRenderer>::New();
	renderer3->AddActor(cylinderActor);
	renderer3->SetBackground(0.0, 0.0, 1.0);
	renderer3->SetViewport(0.0, 0.5, 0.5, 1.0);
	vtkSmartPointer<vtkRenderer> renderer4 = vtkSmartPointer<vtkRenderer>::New();
	renderer4->AddActor(sphereActor);
	renderer4->SetBackground(1.0, 1.0, 0.0);
	renderer4->SetViewport(0.5, 0.5, 1.0, 1.0);

	//设置窗口
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(renderer1);
	renWin->AddRenderer(renderer2);
	renWin->AddRenderer(renderer3);
	renWin->AddRenderer(renderer4);
	renWin->SetSize(600, 600);
	//renWin->Render();
	renWin->SetWindowName("Viewport");

	//设置交互器
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renWin);

	renWin->Render();
	interactor->Initialize();
	interactor->Start();

	return 0;
}