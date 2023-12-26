#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	//创建点集数据
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint(0, 0, 0);
	points->InsertNextPoint(1, 0, 0);
	points->InsertNextPoint(0, 1, 0);

	//创建多边形数据
	vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
	polydata->SetPoints(points);

	//设置标量值
	vtkSmartPointer<vtkDoubleArray> weights = vtkSmartPointer<vtkDoubleArray>::New();
	weights->SetNumberOfValues(3);
	weights->SetValue(0, 1);
	weights->SetValue(1, 2);
	weights->SetValue(2, 3);

	//先获取多边形数据的点数据指针，然后设置该点数据的标量属性值。
	polydata->GetPointData()->SetScalars(weights);

	//输出点的标量值
	for (int i = 0; i < 3; i++)
	{
		double weight = vtkDoubleArray::SafeDownCast(polydata->GetPointData()->GetScalars())->GetValue(i);
		std::cout <<"Points" << i << " "<< "weight: " << weight << std::endl;
	}
	return 0;
}