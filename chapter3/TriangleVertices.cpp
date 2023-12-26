#include <vtkCellArray.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	//点坐标
	double X[3] = { 1.0, 0.0, 0.0 };
	double Y[3] = { 0.0, 0.0, 1.0 };
	double Z[3] = { 0.0, 0.0, 0.0 };

	//点数据定义几何结构
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	//单元数据定义拓扑结构
	vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
	
	for (int i = 0; i < 3; ++i)
	{
		vtkIdType pid[1];
		pid[0] = points->InsertNextPoint(X[i], Y[i], Z[i]);
		vertices->InsertNextCell(1, pid);
	}

	vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
	polydata->SetPoints(points);//几何
	polydata->SetVerts(vertices);//拓扑

	vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetFileName("D:\\Desktop\\TriangleVerts.vtk");
	writer->SetInputData(polydata);
	writer->Write();
	return 0;
}