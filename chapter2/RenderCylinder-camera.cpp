#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkLight.h>
#include <vtkCamera.h>
#include <vtkSmartPointer.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
int main()
{
	// 创建一个圆柱体源
	vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
	cylinder->SetHeight(8.0);//高度
	cylinder->SetRadius(3.0);//半径
	cylinder->SetResolution(12);//底面多边形的边数

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(cylinder->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	//渲染器
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->SetBackground(0.1, 0.2, 0.4);

	//渲染窗口
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(renderer);
	renWin->SetSize(500, 500);

	//交互器
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renWin->SetInteractor(interactor);

	//交互器样式
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	interactor->SetInteractorStyle(style);

	//相机
	vtkSmartPointer<vtkCamera> mcamera = vtkSmartPointer<vtkCamera>::New();
	mcamera->SetFocalPoint(0,0,0); //焦点
	mcamera->SetPosition(0,0,30); //位置
	mcamera->SetViewAngle(60); //视角
	renderer->SetActiveCamera(mcamera);

	//灯光
	vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
	light->SetColor(0, 1, 0);//green
	light->SetPosition(0, 0, 1);
	light->SetFocalPoint(renderer->GetActiveCamera()->GetFocalPoint());
	renderer->AddLight(light);

	vtkSmartPointer<vtkLight> light2 = vtkSmartPointer<vtkLight>::New();
	light2->SetColor(0, 0, 1);//blue
	light2->SetPosition(0, 0, -1);
	light2->SetFocalPoint(renderer->GetActiveCamera()->GetFocalPoint());
	renderer->AddLight(light2);
	

	// 初始化交互器并开始渲染循环
	interactor->Initialize();
	interactor->Start();

	return 0;
}