#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkImageAccumulate.h>
#include <vtkImageData.h>
#include <vtkImageExtractComponents.h>
#include <vtkBMPReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkXYPlotActor.h>
#include <vtkAxisActor2D.h>
#include <vtkProperty2D.h>
#include <vtkTextProperty.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingFreeType)

int main()
{
	vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap05\\DATA\\LENA.BMP");
	reader->Update();

	//获取读取的图像数据的每个像素的色彩通道数量
	int numComponents = reader->GetOutput()->GetNumberOfScalarComponents();

	vtkSmartPointer<vtkXYPlotActor> plot = vtkSmartPointer<vtkXYPlotActor>::New();
	plot->ExchangeAxesOff();// 设置是否交换坐标轴（默认为关闭）
	plot->SetLabelFormat("%g");// 设置标签格式
	plot->SetXTitle("Intensity");
	plot->SetYTitle("Frequency");
	plot->SetXValuesToValue();// 将X轴的值设为值域
	plot->GetProperty()->SetColor(0.0, 0.0, 0.0);
	plot->GetAxisLabelTextProperty()->SetColor(0.0, 0.0, 0.0);
	plot->GetAxisTitleTextProperty()->SetColor(0.0, 0.0, 0.0);

	double colors[3][3] = {
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 }
	};

	const char* labels[3] = { "Red", "Green", "Blue" };

	int xmax = 0;
	int ymax = 0;

	for (int i = 0; i < numComponents; ++i)
	{
		//提取每个组分图像
		vtkSmartPointer<vtkImageExtractComponents> extract =
			vtkSmartPointer<vtkImageExtractComponents>::New();
		extract->SetInputConnection(reader->GetOutputPort());
		extract->SetComponents(i);
		extract->Update();

		double range[2];
		extract->GetOutput()->GetScalarRange(range);
		int extent = static_cast<int>(range[1]) - static_cast<int>(range[0]) - 1;

		vtkSmartPointer<vtkImageAccumulate> histogram = vtkSmartPointer<vtkImageAccumulate>::New();
		histogram->SetInputConnection(extract->GetOutputPort());
		histogram->SetComponentExtent(0, extent, 0, 0, 0, 0);
		histogram->SetComponentOrigin(range[0], 0, 0);
		histogram->SetComponentSpacing(1, 0, 0);
		histogram->SetIgnoreZero(1);
		histogram->Update();

		if (range[1] > xmax)
		{
			xmax = range[1];
		}
		if (histogram->GetOutput()->GetScalarRange()[1] > ymax)
		{
			ymax = histogram->GetOutput()->GetScalarRange()[1];
		}

		plot->AddDataSetInput(histogram->GetOutput());//获取图像的像素点
		plot->SetPlotColor(i, colors[i]);
		plot->SetPlotLabel(i, labels[i]);
		plot->LegendOn();
	}

	plot->SetXRange(0, xmax);
	plot->SetYRange(0, ymax);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(plot);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(640, 480);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageAccumulateExample2");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	interactor->Initialize();
	interactor->Start();
	return 0;
}



