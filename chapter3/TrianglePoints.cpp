#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataWriter.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	//创建vtkPoints对象，用于存储点的坐标信息
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint(1.0, 0.0, 0.0);
	points->InsertNextPoint(0.0, 0.0, 0.0);
	points->InsertNextPoint(0.0, 1.0, 0.0);

	//创建vtkPolyData对象，用于存储几何数据
	vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
	polydata->SetPoints(points);

	//写入vtkPolyData对象到文件
	vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetFileName("D:\\Desktop\\triangle.vtk");
	writer->SetInputData(polydata);
	writer->Write();
	return 0;
}