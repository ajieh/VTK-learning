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
#include <vtkIterativeClosestPointTransform.h>
#include <vtkPolyDataReader.h>
#include <vtkTransform.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkPolyDataReader> reader =
		vtkSmartPointer<vtkPolyDataReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap06\\DATA\\FRAN_CUT.VTK");
	reader->Update();
	vtkSmartPointer<vtkPolyData> original = reader->GetOutput();

	//变换矩阵，设置X轴方向平移量为0.2，绕X轴旋转30°
	vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
	translation->Translate(0.2, 0.0, 0.0);
	translation->RotateX(30);

	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter1 =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter1->SetInputData(reader->GetOutput());
	transformFilter1->SetTransform(translation);
	transformFilter1->Update();

	vtkSmartPointer<vtkPolyData> source = vtkSmartPointer<vtkPolyData>::New();
	source->SetPoints(original->GetPoints());

	vtkSmartPointer<vtkPolyData> target = vtkSmartPointer<vtkPolyData>::New();
	target->SetPoints(transformFilter1->GetOutput()->GetPoints());

	vtkSmartPointer<vtkVertexGlyphFilter> sourceGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
	sourceGlyphFilter->SetInputData(source);
	sourceGlyphFilter->Update();

	vtkSmartPointer<vtkVertexGlyphFilter> targetGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
	targetGlyphFilter->SetInputData(target);
	targetGlyphFilter->Update();

	//迭代最近点算法
	vtkSmartPointer<vtkIterativeClosestPointTransform> icpTransform =
		vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
	icpTransform->SetSource(sourceGlyphFilter->GetOutput());
	icpTransform->SetTarget(targetGlyphFilter->GetOutput());
	icpTransform->GetLandmarkTransform()->SetModeToRigidBody();//设置为刚体变换模式
	icpTransform->SetMaximumNumberOfIterations(20);//最大迭代次数
	icpTransform->StartByMatchingCentroidsOn();//使用点云数据的质心进行匹配，并从中央位置开始逐步调整到正确的位置和方向
	icpTransform->Modified();
	icpTransform->Update();

	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter2 =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter2->SetInputData(sourceGlyphFilter->GetOutput());
	transformFilter2->SetTransform(icpTransform);
	transformFilter2->Update();

	vtkSmartPointer<vtkPolyDataMapper> sourceMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	sourceMapper->SetInputConnection(sourceGlyphFilter->GetOutputPort());

	vtkSmartPointer<vtkActor> sourceActor = vtkSmartPointer<vtkActor>::New();
	sourceActor->SetMapper(sourceMapper);
	sourceActor->GetProperty()->SetColor(0, 1, 0);
	sourceActor->GetProperty()->SetPointSize(3);

	vtkSmartPointer<vtkPolyDataMapper> targetMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	targetMapper->SetInputConnection(targetGlyphFilter->GetOutputPort());

	vtkSmartPointer<vtkActor> targetActor = vtkSmartPointer<vtkActor>::New();
	targetActor->SetMapper(targetMapper);
	targetActor->GetProperty()->SetColor(1, 0, 0);
	targetActor->GetProperty()->SetPointSize(3);

	vtkSmartPointer<vtkPolyDataMapper> solutionMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	solutionMapper->SetInputConnection(transformFilter2->GetOutputPort());

	vtkSmartPointer<vtkActor> solutionActor = vtkSmartPointer<vtkActor>::New();
	solutionActor->SetMapper(solutionMapper);
	solutionActor->GetProperty()->SetColor(0, 0, 1);
	solutionActor->GetProperty()->SetPointSize(3);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(sourceActor);
	renderer->AddActor(targetActor);
	renderer->AddActor(solutionActor);
	renderer->SetBackground(1, 1, 1);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(600, 400);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataICP");
	renderWindow->Render();

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);
	renderWindow->Render();
	interactor->Initialize();
	interactor->Start();
	return 0;
}




