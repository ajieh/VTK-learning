#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkPolyDataReader.h>
#include <vtkPLYReader.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkPolyDataNormals.h>
#include <vtkGlyph3D.h>
#include <vtkArrowSource.h>
#include <vtkSphereSource.h>
#include <vtkMaskPoints.h>
#include <vtkProperty.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap06\\DATA\\FRAN_CUT.VTK");
	reader->Update();

	//计算法向量
	vtkSmartPointer<vtkPolyDataNormals> normFilter = vtkSmartPointer<vtkPolyDataNormals>::New();
	normFilter->SetInputData(reader->GetOutput());
	normFilter->SetComputePointNormals(1);//开启计算点法向量
	normFilter->SetComputeCellNormals(0);//关闭计算单元法向量
	normFilter->SetAutoOrientNormals(1);
	normFilter->SetSplitting(0);
	normFilter->Update();

	vtkSmartPointer<vtkMaskPoints> mask = vtkSmartPointer<vtkMaskPoints>::New();
	mask->SetInputData(normFilter->GetOutput());
	mask->SetMaximumNumberOfPoints(300);//设置最大显示点的数量为 300
	mask->RandomModeOn();
	mask->Update();

	//表示要绘制的法向量
	vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
	arrow->Update();

	vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
	glyph->SetInputData(mask->GetOutput());
	glyph->SetSourceData(arrow->GetOutput());
	glyph->SetVectorModeToUseNormal();////设置向量显示模式和法向量一致
	glyph->SetScaleFactor(0.01);//缩放系数为 0.01
	glyph->Update();

	vtkSmartPointer<vtkPolyDataMapper> originMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	originMapper->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkActor> originActor = vtkSmartPointer<vtkActor>::New();
	originActor->SetMapper(originMapper);

	vtkSmartPointer<vtkPolyDataMapper> normedMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	normedMapper->SetInputData(normFilter->GetOutput());

	vtkSmartPointer<vtkActor> normedActor = vtkSmartPointer<vtkActor>::New();
	normedActor->SetMapper(normedMapper);

	vtkSmartPointer<vtkPolyDataMapper> glyphMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	glyphMapper->SetInputData(glyph->GetOutput());

	vtkSmartPointer<vtkActor> glyphActor = vtkSmartPointer<vtkActor>::New();
	glyphActor->SetMapper(glyphMapper);
	glyphActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

	vtkSmartPointer<vtkRenderer> originalRenderer = vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(0.0, 0.0, 0.33, 1.0);
	originalRenderer->AddActor(originActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> normedRenderer = vtkSmartPointer<vtkRenderer>::New();
	normedRenderer->SetViewport(0.33, 0.0, 0.66, 1.0);
	normedRenderer->AddActor(normedActor);
	normedRenderer->ResetCamera();
	normedRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> glyphRenderer = vtkSmartPointer<vtkRenderer>::New();
	glyphRenderer->SetViewport(0.66, 0.0, 1.0, 1.0);
	glyphRenderer->AddActor(glyphActor);
	glyphRenderer->AddActor(normedActor);
	glyphRenderer->ResetCamera();
	glyphRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(normedRenderer);
	renderWindow->AddRenderer(glyphRenderer);
	renderWindow->SetSize(600, 400);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataNormal");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);
	interactor->Initialize();
	interactor->Start();
	return 0;
}




