#include <vtkSmartPointer.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkCommand.h>
#include <vtkBoxWidget.h>
#include <vtkTransform.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);

class MyCallback : public vtkCommand
{
public:
	static MyCallback* New()
	{
		return new MyCallback;
	}
	void SetObject(vtkConeSource *cone)
	{
		this->m_Cone = cone;
	}
	virtual void Execute(vtkObject *caller, unsigned long eventId, void* callData)
	{
		std::cout << "Left button pressed.\n"
			<< "The Height: " << m_Cone->GetHeight() << "\n"
			<< "The Radius: " << m_Cone->GetRadius() << "\n"
			<< "The Resolution: " << m_Cone->GetResolution() << std::endl;
	}
private:
	vtkConeSource *m_Cone;
};

int main()
{
	vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
	cone->SetHeight(6.0);
	cone->SetRadius(3.0);
	cone->SetResolution(100);

	vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	coneMapper->SetInputConnection(cone->GetOutputPort());

	vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();
	coneActor->SetMapper(coneMapper);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(coneActor);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(renderer);
	renWin->SetSize(600, 400);
	renWin->Render();
	renWin->SetWindowName("ObserverCommandDemo2");

	vtkSmartPointer<vtkRenderWindowInteractor> iren =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	iren->SetInteractorStyle(style);

	vtkSmartPointer<MyCallback> callback = vtkSmartPointer<MyCallback>::New();
	callback->SetObject(cone);

	iren->AddObserver(vtkCommand::LeftButtonPressEvent, callback);

	iren->Initialize();
	iren->Start();
	return 0;
}

