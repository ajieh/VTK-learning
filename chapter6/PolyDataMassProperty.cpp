#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkScalarBarActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkCubeSource.h>
#include <vtkProperty.h>
#include <vtkTriangleFilter.h>
#include <vtkMassProperties.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();
	cubeSource->Update();
	
	//将立方体的六个平面转换为三角形面
	vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
	triangleFilter->SetInputData(cubeSource->GetOutput());
	triangleFilter->Update();

	vtkSmartPointer<vtkMassProperties> massProp = vtkSmartPointer<vtkMassProperties>::New();
	massProp->SetInputData(triangleFilter->GetOutput());

	float volume = massProp->GetVolume();//立方体的体积
	float area = massProp->GetSurfaceArea();//表面积
	float maxArea = massProp->GetMaxCellArea();//最大单元面积
	float minArea = massProp->GetMinCellArea();//最小单元面积

	std::cout << "Volume      :" << volume << std::endl;
	std::cout << "Surface Area:" << area << std::endl;
	std::cout << "Max Area    :" << maxArea << std::endl;
	std::cout << "Min Area    :" << minArea << std::endl;

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputData(cubeSource->GetOutput());
	mapper->SetInputData(triangleFilter->GetOutput());

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(0, 1, 0);//将立方体的颜色设置为绿色
	actor->GetProperty()->SetEdgeColor(1, 0, 0);//将边界线的颜色设置为红色
	actor->GetProperty()->SetEdgeVisibility(1);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->SetBackground(1, 1, 1);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(600, 400);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataMassProperty");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);
	interactor->Initialize();
	interactor->Start();
	return 0;
}




