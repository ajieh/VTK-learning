#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkDelaunay2D.h>
#include <vtkMath.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	//生成一个10 * 10的二维的点集数据
	unsigned int gridSize = 10;
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

	for (unsigned int x = 0; x < gridSize; x++)
	{
		for (unsigned int y = 0; y < gridSize; y++)
		{
			points->InsertNextPoint(x, y, vtkMath::Random(0.0, 3.0));
		}
	}

	vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
	polydata->SetPoints(points);
	
	//三角剖分
	vtkSmartPointer<vtkDelaunay2D> delaunay = vtkSmartPointer<vtkDelaunay2D>::New();
	delaunay->SetInputData(polydata);
	delaunay->Update();

	//创建一个多边形数据vtkPolyData，其中每个点都被创建了一个顶点Vertex
	vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter =
		vtkSmartPointer<vtkVertexGlyphFilter>::New();
	glyphFilter->SetInputData(polydata);
	glyphFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> pointsMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	pointsMapper->SetInputData(glyphFilter->GetOutput());

	vtkSmartPointer<vtkActor> pointsActor = vtkSmartPointer<vtkActor>::New();
	pointsActor->SetMapper(pointsMapper);
	pointsActor->GetProperty()->SetPointSize(3);
	pointsActor->GetProperty()->SetColor(1, 0, 0);

	vtkSmartPointer<vtkPolyDataMapper> triangulatedMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	triangulatedMapper->SetInputData(delaunay->GetOutput());

	vtkSmartPointer<vtkActor> triangulatedActor = vtkSmartPointer<vtkActor>::New();
	triangulatedActor->SetMapper(triangulatedMapper);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(pointsActor);
	renderer->AddActor(triangulatedActor);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	renderWindow->SetSize(600, 400);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataDelaunay2D");
	renderWindow->Render();

	interactor->Start();
	return 0;
}




