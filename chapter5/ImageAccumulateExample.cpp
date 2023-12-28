#include <vtkActor.h>
#include <vtkBarChartActor.h>
#include <vtkFieldData.h>
#include <vtkImageAccumulate.h>
#include <vtkImageData.h>
#include <vtkIntArray.h>
#include <vtkJPEGReader.h>
#include <vtkLegendBoxActor.h>
#include <vtkProperty2D.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkTextProperty.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)//避免获取TextRenderer实例失败
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap05\\DATA\\lena-gray.jpg");
	reader->Update();

	int bins = 16;//直方图的间隔数目
	int comps = 1;//组件数

	vtkSmartPointer<vtkImageAccumulate> histogram = vtkSmartPointer<vtkImageAccumulate>::New();
	histogram->SetInputData(reader->GetOutput());
	histogram->SetComponentExtent(0, bins - 1, 0, 0, 0, 0);//设置每个组分的直方图的最小和最大值
	histogram->SetComponentOrigin(0, 0, 0);//设置每个组分直方图的起始灰度值
	histogram->SetComponentSpacing(256.0 / bins, 0, 0);//设置直方图每个间隔的灰度范围
	histogram->Update();

	int* output = static_cast<int*>(histogram->GetOutput()->GetScalarPointer());

	vtkSmartPointer<vtkIntArray> frequencies = vtkSmartPointer<vtkIntArray>::New();
	frequencies->SetNumberOfComponents(1);

	for (int j = 0; j < bins; ++j)
	{
		for (int i = 0; i < comps; i++)
		{
			frequencies->InsertNextTuple1(*output++);
		}
	}

	vtkSmartPointer<vtkDataObject> dataObject = vtkSmartPointer<vtkDataObject>::New();
	dataObject->GetFieldData()->AddArray(frequencies);

	vtkSmartPointer<vtkBarChartActor> barChart = vtkSmartPointer<vtkBarChartActor>::New();
	barChart->SetInput(dataObject);
	barChart->SetTitle("Histogram");
	barChart->GetPositionCoordinate()->SetValue(0.05, 0.05, 0.0);
	barChart->GetPosition2Coordinate()->SetValue(0.95, 0.95, 0.0);
	barChart->GetProperty()->SetColor(0, 0, 0);
	barChart->GetTitleTextProperty()->SetColor(0, 0, 0);
	barChart->GetLabelTextProperty()->SetColor(0, 0, 0);
	barChart->GetLegendActor()->SetNumberOfEntries(dataObject->GetFieldData()->GetArray(0)->GetNumberOfTuples());
	barChart->LegendVisibilityOff();
	barChart->LabelVisibilityOff();

	double colors[3][3] = {
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 }
	};

	int count = 0;
	for (int i = 0; i < bins; ++i)
	{
		for (int j = 0; j < comps; ++j)
		{
			barChart->SetBarColor(count++, colors[j]);
		}
	}

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(barChart);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(640, 480);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageAccumulateExample");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	interactor->Initialize();
	interactor->Start();
	return 0;
}


