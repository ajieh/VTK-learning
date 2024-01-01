#include <vtkSmartPointer.h>
#include <vtkSurfaceReconstructionFilter.h>
#include <vtkProgrammableSource.h>
#include <vtkContourFilter.h>
#include <vtkReverseSense.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkPolydataReader.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap06\\DATA\\FRAN_CUT.VTK");
	reader->Update();

	vtkSmartPointer<vtkPolyData> points = vtkSmartPointer<vtkPolyData>::New();
	points->SetPoints(reader->GetOutput()->GetPoints());

	//表面重建
	vtkSmartPointer<vtkSurfaceReconstructionFilter> surf =
		vtkSmartPointer<vtkSurfaceReconstructionFilter>::New();
	surf->SetInputData(points);
	surf->SetNeighborhoodSize(20);//设置邻域大小
	surf->SetSampleSpacing(0.005);//设置采样间距
	surf->Update();

	vtkSmartPointer<vtkContourFilter> contour = vtkSmartPointer<vtkContourFilter>::New();
	contour->SetInputConnection(surf->GetOutputPort());
	contour->SetValue(0, 0.0);
	contour->Update();

	vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
	vertexGlyphFilter->AddInputData(points);
	vertexGlyphFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> vertexMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vertexMapper->SetInputData(vertexGlyphFilter->GetOutput());
	vertexMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> vertexActor = vtkSmartPointer<vtkActor>::New();
	vertexActor->SetMapper(vertexMapper);
	vertexActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

	vtkSmartPointer<vtkRenderer> vertexRenderer = vtkSmartPointer<vtkRenderer>::New();
	vertexRenderer->AddActor(vertexActor);
	vertexRenderer->SetViewport(0.0, 0.0, 0.5, 1.0);
	vertexRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkPolyDataMapper> surfMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	surfMapper->SetInputData(contour->GetOutput());
	surfMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> surfActor = vtkSmartPointer<vtkActor>::New();
	surfActor->SetMapper(surfMapper);
	surfActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

	vtkSmartPointer<vtkRenderer> surfRenderer = vtkSmartPointer<vtkRenderer>::New();
	surfRenderer->AddActor(surfActor);
	surfRenderer->SetViewport(0.5, 0.0, 1.0, 1.0);
	surfRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(surfRenderer);
	renderWindow->AddRenderer(vertexRenderer);
	renderWindow->SetSize(600, 400);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataSurfaceReconstruction");
	renderWindow->Render();

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);
	renderWindow->Render();
	interactor->Initialize();
	interactor->Start();
	return 0;
}




