#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkLookupTable.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkScalarBarActor.h>
#include <vtkScalarBarWidget.h>
#include <vtkTextActor.h>
#include <vtkTextWidget.h>
#include <vtkTextRepresentation.h>
#include <vtkTextProperty.h>
#include <vtkCaptionRepresentation.h>
#include <vtkCaptionWidget.h>
#include <vtkCaptionActor2D.h>
#include <vtkBalloonRepresentation.h> 
#include <vtkBalloonWidget.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);

int main()
{
	int widgetType = 2;
	vtkSmartPointer<vtkUnstructuredGridReader> reader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap08\\DATA\\scalarBarWidgetTestData.vtk");
	reader->Update();

	vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer< vtkLookupTable >::New();
	lut->Build();

	vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer< vtkDataSetMapper >::New();
	mapper->SetInputData(reader->GetOutput());
	mapper->SetScalarRange(reader->GetOutput()->GetScalarRange());
	mapper->SetLookupTable(lut);

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer< vtkActor >::New();
	actor->SetMapper(mapper);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(renderer);
	renWin->SetSize(600, 400);
	renWin->Render();
	renWin->SetWindowName("AnnotationWidget");

	vtkSmartPointer<vtkRenderWindowInteractor> iren =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	// vtkScalarBarWidget
	vtkSmartPointer<vtkScalarBarActor> scalarBarActor = vtkSmartPointer< vtkScalarBarActor >::New();
	scalarBarActor->SetOrientationToHorizontal();
	scalarBarActor->SetLookupTable(lut);

	vtkSmartPointer<vtkScalarBarWidget> scalarBarWidget = vtkSmartPointer< vtkScalarBarWidget >::New();
	scalarBarWidget->SetInteractor(iren);
	scalarBarWidget->SetScalarBarActor(scalarBarActor);
	scalarBarWidget->On();

	// vtkTextWidget
	vtkSmartPointer<vtkTextActor> textActor = vtkSmartPointer<vtkTextActor>::New();
	textActor->SetInput("VTK Widgets");
	textActor->GetTextProperty()->SetColor(0.0, 1.0, 0.0);

	vtkSmartPointer<vtkTextWidget> textWidget = vtkSmartPointer<vtkTextWidget>::New();

	vtkSmartPointer<vtkTextRepresentation> textRepresentation =
		vtkSmartPointer<vtkTextRepresentation>::New();
	textRepresentation->GetPositionCoordinate()->SetValue(.15, .15);
	textRepresentation->GetPosition2Coordinate()->SetValue(.3, .1);
	textWidget->SetRepresentation(textRepresentation);
	textWidget->SetInteractor(iren);
	textWidget->SetTextActor(textActor);
	textWidget->SelectableOff();
	textWidget->On();


	// vtkBalloonWidget
	vtkSmartPointer<vtkBalloonRepresentation> balloonRep =
		vtkSmartPointer<vtkBalloonRepresentation>::New();
	balloonRep->SetBalloonLayoutToImageRight();

	vtkSmartPointer<vtkBalloonWidget> balloonWidget =
		vtkSmartPointer<vtkBalloonWidget>::New();
	balloonWidget->SetInteractor(iren);
	balloonWidget->SetRepresentation(balloonRep);
	balloonWidget->AddBalloon(actor, "This is a widget example", NULL);
	balloonWidget->On();

	// vtkOrientationMarkerWidget
	vtkSmartPointer<vtkAxesActor> iconActor = vtkSmartPointer<vtkAxesActor>::New();
	vtkSmartPointer<vtkOrientationMarkerWidget> orientationWidget =
		vtkSmartPointer<vtkOrientationMarkerWidget>::New();
	orientationWidget->SetOutlineColor(0.9300, 0.5700, 0.1300);
	orientationWidget->SetOrientationMarker(iconActor);
	orientationWidget->SetInteractor(iren);
	orientationWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
	orientationWidget->SetEnabled(1);
	orientationWidget->InteractiveOn();

	// vtkCaptionWidget
	//vtkSmartPointer<vtkCaptionRepresentation> captionRepresentation = 
	//	vtkSmartPointer<vtkCaptionRepresentation>::New();
	//captionRepresentation->GetCaptionActor2D()->SetCaption("Caption Widget");
	//captionRepresentation->GetCaptionActor2D()->GetTextActor()->GetTextProperty()->SetFontSize(20);

	//double pos[3] = {.5,0,0};
	//captionRepresentation->SetAnchorPosition(pos);

	//vtkSmartPointer<vtkCaptionWidget> captionWidget = 
	//	vtkSmartPointer<vtkCaptionWidget>::New();
	//captionWidget->SetInteractor(iren);
	//captionWidget->SetRepresentation(captionRepresentation);
	//captionWidget->On();

	renWin->Render();
	iren->Initialize();
	iren->Start();
	return 0;
}

