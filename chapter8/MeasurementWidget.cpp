#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkBMPReader.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
#include <vtkAngleWidget.h>
#include <vtkAngleRepresentation2D.h>
#include <vtkBiDimensionalWidget.h>
#include <vtkBiDimensionalRepresentation2D.h>
#include <vtkLeaderActor2D.h>
#include <vtkProperty2D.h>
#include <vtkCommand.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
class vtkBiDimensionalCallback : public vtkCommand
{
public:
	static vtkBiDimensionalCallback* New()
	{
		return new vtkBiDimensionalCallback;
	}
	
	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		vtkBiDimensionalWidget* biDimensionalWidget =
			reinterpret_cast<vtkBiDimensionalWidget*>(caller);
		vtkBiDimensionalRepresentation2D* representation =
			static_cast<vtkBiDimensionalRepresentation2D*>(biDimensionalWidget->GetRepresentation());
		//获取四个点的屏幕坐标（以像素为单位），并将其存储在 p1，p2，p3 和 p4 数组中
		double p1[3];
		representation->GetPoint1DisplayPosition(p1);
		double p2[3];
		representation->GetPoint1DisplayPosition(p2);
		double p3[3];
		representation->GetPoint1DisplayPosition(p3);
		double p4[3];
		representation->GetPoint1DisplayPosition(p4);

		std::cout << "P1: " << p1[0] << " " << p1[1] << " " << p1[2] << std::endl;
	}
};
int main()
{
	int widgetType = 2;
	vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap08\\DATA\\lena.bmp");
	reader->Update();

	vtkSmartPointer<vtkImageActor> imageActor = vtkSmartPointer<vtkImageActor>::New();
	imageActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(imageActor);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(renderer);
	renWin->SetSize(600, 400);
	renWin->Render();
	renWin->SetWindowName("MeasurementWidget");

	vtkSmartPointer<vtkRenderWindowInteractor> iren =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();
	iren->SetInteractorStyle(style);

	//vtkDistanceWidget:测量二维平面上两个点的距离
	if (widgetType == 0)
	{
		vtkSmartPointer<vtkDistanceWidget> disWidget = vtkSmartPointer<vtkDistanceWidget>::New();
		disWidget->SetInteractor(iren);
		disWidget->CreateDefaultRepresentation();
		static_cast<vtkDistanceRepresentation*>(disWidget->GetRepresentation())->
			SetLabelFormat("%-#6.3g px");
		iren->Initialize();
		renWin->Render();
		disWidget->On();
		iren->Start();
	}
	//vtkAngleWidget:用于二维平面的角度测量
	else if (widgetType == 1)
	{
		vtkSmartPointer<vtkAngleWidget> angleWidget = vtkSmartPointer<vtkAngleWidget>::New();
		angleWidget->SetInteractor(iren);
		vtkSmartPointer< vtkAngleRepresentation2D > angleRep =
			vtkSmartPointer< vtkAngleRepresentation2D >::New();
		//获取角度测量表示的第一条射线，并设置为红色
		angleRep->GetRay1()->GetProperty()->SetColor(1.0, 0.0, 0.0);
		//获取角度测量表示的第二条射线。并设置为绿色
		angleRep->GetRay2()->GetProperty()->SetColor(0.0, 1.0, 0.0);
		//获取角度测量表示的弧，并设置为蓝色
		angleRep->GetArc()->GetProperty()->SetColor(0.0, 0.0, 1.0);

		angleWidget->SetRepresentation(angleRep);
		iren->Initialize();
		renWin->Render();
		angleWidget->On();
		iren->Start();
	}
	//vtkBiDimensionalWidget:用于测量二维平面上任意两个正交方向的轴长
	else if (widgetType == 2)
	{
		vtkSmartPointer<vtkBiDimensionalWidget> biDimensionalWidget =
			vtkSmartPointer<vtkBiDimensionalWidget>::New();
		biDimensionalWidget->SetInteractor(iren);
		biDimensionalWidget->CreateDefaultRepresentation();

		vtkSmartPointer<vtkBiDimensionalCallback> callback =
			vtkSmartPointer<vtkBiDimensionalCallback>::New();

		biDimensionalWidget->AddObserver(vtkCommand::InteractionEvent, callback);
		iren->Initialize();
		renWin->Render();
		biDimensionalWidget->On();
		iren->Start();
	}
	return 0;
}

