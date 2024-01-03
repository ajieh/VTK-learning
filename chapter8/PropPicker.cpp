#include <vtkSmartPointer.h>
#include <vtkMath.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropPicker.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);

class PropPickerInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static PropPickerInteractorStyle* New();
	vtkTypeMacro(PropPickerInteractorStyle, vtkInteractorStyleTrackballCamera);
	PropPickerInteractorStyle()
	{
		LastPickedActor = nullptr;
		LastPickedProperty = vtkProperty::New();
	}

	virtual void OnLeftButtonDown() {
		int* pos = this->GetInteractor()->GetEventPosition();
		vtkSmartPointer<vtkPropPicker>  picker = vtkSmartPointer<vtkPropPicker>::New();
		picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());
		double* pickPos = picker->GetPickPosition();
		if (this->LastPickedActor)
		{
			this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
		}
		this->LastPickedActor = picker->GetActor();
		if (this->LastPickedActor)
		{
			this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());
			//对拾取对象进行高亮显示
			this->LastPickedActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
			this->LastPickedActor->GetProperty()->SetDiffuse(1.0);
			this->LastPickedActor->GetProperty()->SetSpecular(0.0);
		}
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}
	virtual ~PropPickerInteractorStyle()
	{
		LastPickedProperty->Delete();
	}

private:
	vtkActor    *LastPickedActor;
	vtkProperty *LastPickedProperty;
};
vtkStandardNewMacro(PropPickerInteractorStyle);

int main()
{
	int numberOfSpheres = 10;

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->Render();
	renWin->SetSize(600, 400);
	renWin->SetWindowName("PropPicker");
	renWin->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> iren =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<PropPickerInteractorStyle> style =
		vtkSmartPointer<PropPickerInteractorStyle>::New();
	style->SetDefaultRenderer(renderer);
	iren->SetInteractorStyle(style);

	for (int i = 0; i < numberOfSpheres; ++i)
	{
		vtkSmartPointer<vtkSphereSource> source = vtkSmartPointer<vtkSphereSource>::New();
		double x, y, z, radius;
		x = vtkMath::Random(-5, 5);
		y = vtkMath::Random(-5, 5);
		z = vtkMath::Random(-5, 5);
		radius = vtkMath::Random(.5, 1.0);
		source->SetRadius(radius);
		source->SetCenter(x, y, z);
		source->SetPhiResolution(11);//纬线分辨率
		source->SetThetaResolution(21);//经线分辨率

		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(source->GetOutputPort());

		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);

		double r, g, b;
		r = vtkMath::Random(.4, 1.0);
		g = vtkMath::Random(.4, 1.0);
		b = vtkMath::Random(.4, 1.0);
		actor->GetProperty()->SetDiffuseColor(r, g, b);
		actor->GetProperty()->SetDiffuse(.8);//漫反射强度
		actor->GetProperty()->SetSpecular(.5);//镜面反射强度
		actor->GetProperty()->SetSpecularColor(1.0, 1.0, 1.0);
		actor->GetProperty()->SetSpecularPower(30.0);//镜面反射功率
		renderer->AddActor(actor);
	}
	renderer->SetBackground(1.0, 1.0, 1.0);
	renWin->Render();
	iren->Initialize();
	iren->Start();
	return 0;
}

