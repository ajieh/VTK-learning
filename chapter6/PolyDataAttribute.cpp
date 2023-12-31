#include <vtkSmartPointer.h>
#include <vtkPolygon.h>
#include <vtkTriangle.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkFloatArray.h>
#include <vtkPlaneSource.h>
#include <vtkCellDataToPointData.h>
#include <vtkPointDataToCellData.h>
#include <vtkDataSet.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLookupTable.h>
#include <iostream>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkPlaneSource> gridSource = vtkSmartPointer<vtkPlaneSource>::New();
	gridSource->SetXResolution(3);
	gridSource->SetYResolution(3);
	gridSource->Update();
	vtkSmartPointer<vtkPolyData> grid = gridSource->GetOutput();

	vtkSmartPointer<vtkFloatArray> cellScalars = vtkSmartPointer<vtkFloatArray>::New();
	vtkSmartPointer<vtkFloatArray> cellVectors = vtkSmartPointer<vtkFloatArray>::New();
	cellVectors->SetNumberOfComponents(3);
	for (int i = 0; i < 9; i++)
	{
		cellScalars->InsertNextValue(i + 1);
		cellVectors->InsertNextTuple3(0.0, 0.0, 1.0);
	}
	grid->GetCellData()->SetScalars(cellScalars);
	grid->GetCellData()->SetVectors(cellVectors);

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
	mapper->SetInputData(grid);

	//测试单元属性数据转点属性数据
	vtkSmartPointer<vtkCellDataToPointData> convert =
		vtkSmartPointer<vtkCellDataToPointData>::New();
	convert->SetInputData(grid);
	convert->SetPassCellData(true);
	convert->Update();
	mapper->SetInputData((vtkPolyData*)convert->GetOutput());
	mapper->SetScalarRange(0, 9);
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
	renderWindow->SetWindowName("PolyDataAttribute");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);
	interactor->Initialize();
	interactor->Start();
	return 0;
}




