#include <vtkSmartPointer.h>
#include <vtkSmartPointer.h>
#include <vtkPolygon.h>
#include <vtkTriangle.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLookupTable.h>
#include <vtkTransformTextureCoords.h>
#include <iostream>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	//存储点集合
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint(0.0, 0.0, 0.0);
	points->InsertNextPoint(1.0, 0.0, 0.0);
	points->InsertNextPoint(1.0, 1.0, 0.0);
	points->InsertNextPoint(0.0, 1.0, 0.0);
	points->InsertNextPoint(2.0, 0.0, 0.0);

	//获取多边形的点索引对象，连接这些点形成多边形
	vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
	polygon->GetPointIds()->SetNumberOfIds(4);
	polygon->GetPointIds()->SetId(0, 0);
	polygon->GetPointIds()->SetId(1, 1);
	polygon->GetPointIds()->SetId(2, 2);
	polygon->GetPointIds()->SetId(3, 3);

	vtkSmartPointer<vtkTriangle> trianle = vtkSmartPointer<vtkTriangle>::New();
	trianle->GetPointIds()->SetId(0, 1);
	trianle->GetPointIds()->SetId(1, 2);
	trianle->GetPointIds()->SetId(2, 4);

	//存储多边形和三角形
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	cells->InsertNextCell(polygon);
	cells->InsertNextCell(trianle);

	vtkSmartPointer<vtkPolyData> polygonPolyData = vtkSmartPointer<vtkPolyData>::New();
	polygonPolyData->SetPoints(points);
	polygonPolyData->SetPolys(cells);

	double red[3] = { 255.0, 0.0, 0.0 };
	double green[3] = { 0.0, 255.0, 0.0 };
	double blue[3] = { 0.0, 0.0, 255.0 };

	//点数据设置颜色数据
	vtkSmartPointer<vtkUnsignedCharArray> pointColors =
		vtkSmartPointer<vtkUnsignedCharArray>::New();
	pointColors->SetNumberOfComponents(3);//设置颜色的组成方式为 RGB（红、绿、蓝）三个分量。
	pointColors->InsertNextTuple(red);
	pointColors->InsertNextTuple(green);
	pointColors->InsertNextTuple(blue);
	pointColors->InsertNextTuple(green);
	pointColors->InsertNextTuple(red);
	polygonPolyData->GetPointData()->SetScalars(pointColors);

	//单元数据设置颜色数据
	vtkSmartPointer<vtkUnsignedCharArray> cellColors =
		vtkSmartPointer<vtkUnsignedCharArray>::New();
	cellColors->SetNumberOfComponents(3);
	cellColors->InsertNextTuple(red);
	cellColors->InsertNextTuple(green);
	polygonPolyData->GetCellData()->SetScalars(cellColors);

	vtkSmartPointer<vtkIntArray> pointfield = vtkSmartPointer<vtkIntArray>::New();
	pointfield->SetName("Field");
	pointfield->SetNumberOfComponents(3);
	pointfield->InsertNextTuple3(1, 0, 0);
	pointfield->InsertNextTuple3(2, 0, 0);
	pointfield->InsertNextTuple3(3, 0, 0);
	pointfield->InsertNextTuple3(4, 0, 0);
	pointfield->InsertNextTuple3(5, 0, 0);
	polygonPolyData->GetPointData()->AddArray(pointfield);

	vtkSmartPointer<vtkLookupTable> table = vtkSmartPointer<vtkLookupTable>::New();
	table->SetNumberOfTableValues(10);//设置了10个颜色值
	table->Build();
	//为每个颜色值设置颜色数据
	table->SetTableValue(0, 0, 0, 0, 1);
	table->SetTableValue(1, 0.8900, 0.8100, 0.3400, 1);
	table->SetTableValue(2, 1.0000, 0.3882, 0.2784, 1);
	table->SetTableValue(3, 0.9608, 0.8706, 0.7020, 1);
	table->SetTableValue(4, 0.9020, 0.9020, 0.9804, 1);
	table->SetTableValue(5, 1.0000, 0.4900, 0.2500, 1);
	table->SetTableValue(6, 0.5300, 0.1500, 0.3400, 1);
	table->SetTableValue(7, 0.9804, 0.5020, 0.4471, 1);
	table->SetTableValue(8, 0.7400, 0.9900, 0.7900, 1);
	table->SetTableValue(9, 0.2000, 0.6300, 0.7900, 1);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(polygonPolyData);
	mapper->SetLookupTable(table);

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->SetBackground(1, 1, 1);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(600, 400);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataColor");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);
	interactor->Initialize();
	interactor->Start();
	return 0;
}




