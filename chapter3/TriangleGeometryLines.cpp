#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkCellArray.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	//坐标点,InsertNextPoint() 函数返回一个 vtkIdType 类型的整数，
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint(1.0, 0.0, 0.0); 
	points->InsertNextPoint(0.0, 0.0, 1.0); 
	points->InsertNextPoint(0.0, 0.0, 0.0); 

	//每两个点创建一条线
	vtkSmartPointer<vtkLine> line0 = vtkSmartPointer<vtkLine>::New();
	line0->GetPointIds()->SetId(0, 0);
	line0->GetPointIds()->SetId(1, 1);

	vtkSmartPointer<vtkLine> line1 = vtkSmartPointer<vtkLine>::New();
	line1->GetPointIds()->SetId(0, 1);
	line1->GetPointIds()->SetId(1, 2);

	vtkSmartPointer<vtkLine> line2 = vtkSmartPointer<vtkLine>::New();
	line2->GetPointIds()->SetId(0, 2);
	line2->GetPointIds()->SetId(1, 0);

	//用Cell数组存储创建的线段
	vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
	lines->InsertNextCell(line0);
	lines->InsertNextCell(line1);
	lines->InsertNextCell(line2);

	vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
	polydata->SetPoints(points);
	polydata->SetLines(lines);

	vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetFileName("D:\\Desktop\\TriangleLines.vtk");
	writer->SetInputData(polydata);
	writer->Write();
	return 0;
}