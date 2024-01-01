#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkLandmarkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkAxesActor.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkPoints> sourcePoints = vtkSmartPointer<vtkPoints>::New();
	sourcePoints->InsertNextPoint(0.5, 0.0, 0.0);
	sourcePoints->InsertNextPoint(0.0, 0.5, 0.0);
	sourcePoints->InsertNextPoint(0.0, 0.0, 0.5);

	vtkSmartPointer<vtkPoints> targetPoints = vtkSmartPointer<vtkPoints>::New();
	targetPoints->InsertNextPoint(0.0, 0.0, 0.55);
	targetPoints->InsertNextPoint(0.0, 0.55, 0.0);
	targetPoints->InsertNextPoint(-0.55, 0.0, 0.0);

	//基于标记点，两个点集在配准后的平均距离最小
	vtkSmartPointer<vtkLandmarkTransform> landmarkTransform =
		vtkSmartPointer<vtkLandmarkTransform>::New();
	landmarkTransform->SetSourceLandmarks(sourcePoints);
	landmarkTransform->SetTargetLandmarks(targetPoints);
	landmarkTransform->SetModeToRigidBody();//设置为刚体变换模式
	landmarkTransform->Update();

	vtkSmartPointer<vtkPolyData> source = vtkSmartPointer<vtkPolyData>::New();
	source->SetPoints(sourcePoints);

	vtkSmartPointer<vtkPolyData> target = vtkSmartPointer<vtkPolyData>::New();
	target->SetPoints(targetPoints);

	vtkSmartPointer<vtkVertexGlyphFilter> sourceGlyphFilter =
		vtkSmartPointer<vtkVertexGlyphFilter>::New();
	sourceGlyphFilter->SetInputData(source);
	sourceGlyphFilter->Update();

	vtkSmartPointer<vtkVertexGlyphFilter> targetGlyphFilter =
		vtkSmartPointer<vtkVertexGlyphFilter>::New();
	targetGlyphFilter->SetInputData(target);
	targetGlyphFilter->Update();

	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter->SetInputData(sourceGlyphFilter->GetOutput());
	transformFilter->SetTransform(landmarkTransform);
	transformFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> sourceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	sourceMapper->SetInputConnection(sourceGlyphFilter->GetOutputPort());

	vtkSmartPointer<vtkActor> sourceActor = vtkSmartPointer<vtkActor>::New();
	sourceActor->SetMapper(sourceMapper);
	sourceActor->GetProperty()->SetColor(1, 1, 0);
	sourceActor->GetProperty()->SetPointSize(5);

	vtkSmartPointer<vtkPolyDataMapper> targetMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	targetMapper->SetInputConnection(targetGlyphFilter->GetOutputPort());

	vtkSmartPointer<vtkActor> targetActor = vtkSmartPointer<vtkActor>::New();
	targetActor->SetMapper(targetMapper);
	targetActor->GetProperty()->SetColor(1, 0, 0);
	targetActor->GetProperty()->SetPointSize(5);

	vtkSmartPointer<vtkPolyDataMapper> solutionMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	solutionMapper->SetInputConnection(transformFilter->GetOutputPort());

	vtkSmartPointer<vtkActor> solutionActor = vtkSmartPointer<vtkActor>::New();
	solutionActor->SetMapper(solutionMapper);
	solutionActor->GetProperty()->SetColor(0, 0, 1);
	solutionActor->GetProperty()->SetPointSize(5);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(sourceActor);
	renderer->AddActor(targetActor);
	renderer->AddActor(solutionActor);

	//坐标轴
	vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
	axes->SetScale(30);
	renderer->AddActor(axes);
	renderer->SetBackground(0.3, 0.6, 0.3);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(600, 400);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataLandmarkReg");
	renderWindow->Render();

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);
	renderWindow->Render();
	interactor->Initialize();
	interactor->Start();
	return 0;
}




