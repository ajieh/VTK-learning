#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtk3DSImporter.h>
#include <vtkDataSet.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtk3DSImporter> importer = vtkSmartPointer<vtk3DSImporter>::New();
	importer->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap04\\DATA\\R8.3DS");
	importer->ComputeNormalsOn();//开启法向量的计算
	importer->Read();

	vtkSmartPointer<vtkRenderer> renderer = importer->GetRenderer();
	renderer->GradientBackgroundOn();
	renderer->SetBackground(0.0, 0.0, 1.0);
	renderer->SetBackground2(0.0, 0.0, 0.0);

	vtkSmartPointer<vtkRenderWindow> renWin = importer->GetRenderWindow();
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renWin);
	renWin->Render();
	renWin->SetSize(640, 480);
	renWin->SetWindowName("Import3DS");

	interactor->Start();
	return 0;
}