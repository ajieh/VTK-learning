#include <vtkSmartPointer.h>
#include <vtkCurvatures.h>
#include <vtkPolyDataReader.h>
#include <vtkLookupTable.h>
#include <vtkColorTransferFunction.h>
#include <vtkColorSeries.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkScalarBarActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <iostream>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap06\\DATA\\FRAN_CUT.VTK");
	reader->Update();

	//计算曲率
	vtkSmartPointer<vtkCurvatures> curvaturesFilter =
		vtkSmartPointer<vtkCurvatures>::New();
	curvaturesFilter->SetInputConnection(reader->GetOutputPort());
	curvaturesFilter->SetCurvatureTypeToMinimum();//最小主曲率
	//curvaturesFilter->SetCurvatureTypeToMaximum();//最大主曲率
	//curvaturesFilter->SetCurvatureTypeToGaussian();//高斯曲率
	//curvaturesFilter->SetCurvatureTypeToMean();//平均曲率
	curvaturesFilter->Update();

	double scalarRange[2];
	curvaturesFilter->GetOutput()->GetScalarRange(scalarRange);

	//定义颜色映射表
	vtkSmartPointer<vtkLookupTable> table = vtkSmartPointer<vtkLookupTable>::New();
	table->SetHueRange(0.0, 0.6);//设置色调的范围
	table->SetAlphaRange(1.0, 1.0);//设置透明度的范围
	table->SetValueRange(1.0, 1.0);//设置颜色值的范围
	table->SetSaturationRange(1.0, 1.0);//设置饱和度的范围
	table->SetNumberOfTableValues(256);//指定颜色映射表中颜色的数量。
	table->SetRange(scalarRange);
	table->Build();//数据集的标量值范围。

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(curvaturesFilter->GetOutput());
	mapper->SetLookupTable(table);
	mapper->SetScalarRange(scalarRange);

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	//创建标量颜色表
	vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
	scalarBar->SetLookupTable(mapper->GetLookupTable());
	scalarBar->SetTitle(curvaturesFilter->GetOutput()->GetPointData()->GetScalars()->GetName());
	scalarBar->SetNumberOfLabels(5);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->AddActor2D(scalarBar);
	renderer->ResetCamera();
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(600, 400);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataCurvature");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);
	interactor->Initialize();
	interactor->Start();
	return 0;
}




