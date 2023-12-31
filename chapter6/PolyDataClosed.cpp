#include <vtkSmartPointer.h>
#include <vtkSelectionNode.h>
#include <vtkInformation.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkPointData.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkIdTypeArray.h>
#include <vtkExtractSelection.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkFeatureEdges.h>
#include <vtkFillHolesFilter.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkInteractionStyle)

void GenerateData(vtkSmartPointer<vtkPolyData> inputData)
{
	vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->Update();

	//存储想要提取的单元格的id
	vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
	ids->SetNumberOfComponents(1);
	ids->InsertNextValue(2);
	ids->InsertNextValue(10);

	//指定要提取的单元格的id
	vtkSmartPointer<vtkSelectionNode> selectionNode =
		vtkSmartPointer<vtkSelectionNode>::New();
	selectionNode->SetFieldType(vtkSelectionNode::CELL);
	selectionNode->SetContentType(vtkSelectionNode::INDICES);
	selectionNode->SetSelectionList(ids);
	selectionNode->GetProperties()->Set(vtkSelectionNode::INVERSE(), 1);

	vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
	selection->AddNode(selectionNode);

	vtkSmartPointer<vtkExtractSelection> extractSelection =
		vtkSmartPointer<vtkExtractSelection>::New();
	extractSelection->SetInputData(0, sphereSource->GetOutput());
	extractSelection->SetInputData(1, selection);
	extractSelection->Update();

	//将提取结果转换为表面模型
	vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter =
		vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
	surfaceFilter->SetInputConnection(extractSelection->GetOutputPort());
	surfaceFilter->Update();

	inputData->ShallowCopy(surfaceFilter->GetOutput());
}


int main()
{
	vtkSmartPointer<vtkPolyData> input = vtkSmartPointer<vtkPolyData>::New();
	GenerateData(input);

	vtkSmartPointer<vtkFeatureEdges> featureEdges = vtkSmartPointer<vtkFeatureEdges>::New();
	featureEdges->SetInputData(input);
	featureEdges->BoundaryEdgesOn();//设置提取边界边
	featureEdges->FeatureEdgesOff();
	featureEdges->ManifoldEdgesOff();
	featureEdges->NonManifoldEdgesOff();
	featureEdges->Update();

	int numberOfOpenEdges = featureEdges->GetOutput()->GetNumberOfCells();
	if (numberOfOpenEdges)
	{
		std::cout << "该网格模型不是封闭的..." << std::endl;
	}
	else
	{
		std::cout << "该网格模型是封闭的..." << std::endl;
		return 0;
	}

	//填补输入的网格中的空洞
	vtkSmartPointer<vtkFillHolesFilter> fillHolesFilter = vtkSmartPointer<vtkFillHolesFilter>::New();
	fillHolesFilter->SetInputData(input);
	fillHolesFilter->Update();

	vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	normals->SetInputConnection(fillHolesFilter->GetOutputPort());
	normals->ConsistencyOn();
	normals->SplittingOff();
	normals->Update();

	vtkSmartPointer<vtkPolyDataMapper> originalMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	originalMapper->SetInputData(input);

	vtkSmartPointer<vtkProperty> backfaceProp = vtkSmartPointer<vtkProperty>::New();
	backfaceProp->SetDiffuseColor(0.89, 0.81, 0.34);

	vtkSmartPointer<vtkActor> originalActor = vtkSmartPointer<vtkActor>::New();
	originalActor->SetMapper(originalMapper);
	originalActor->SetBackfaceProperty(backfaceProp);
	originalActor->GetProperty()->SetDiffuseColor(1.0, 0.3882, 0.2784);

	vtkSmartPointer<vtkPolyDataMapper> edgeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	edgeMapper->SetInputData(featureEdges->GetOutput());

	vtkSmartPointer<vtkActor> edgeActor = vtkSmartPointer<vtkActor>::New();
	edgeActor->SetMapper(edgeMapper);
	edgeActor->GetProperty()->SetEdgeColor(0., 0., 1.0);
	edgeActor->GetProperty()->SetEdgeVisibility(1);
	edgeActor->GetProperty()->SetLineWidth(5);

	vtkSmartPointer<vtkPolyDataMapper> filledMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	filledMapper->SetInputData(normals->GetOutput());

	vtkSmartPointer<vtkActor> filledActor = vtkSmartPointer<vtkActor>::New();
	filledActor->SetMapper(filledMapper);
	filledActor->GetProperty()->SetDiffuseColor(1.0, 0.3882, 0.2784);

	vtkSmartPointer<vtkRenderer> renderer1 = vtkSmartPointer<vtkRenderer>::New();
	renderer1->SetViewport(0.0, 0.0, 0.5, 1.0);
	renderer1->AddActor(originalActor);
	renderer1->AddActor(edgeActor);
	renderer1->ResetCamera();
	renderer1->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> renderer2 = vtkSmartPointer<vtkRenderer>::New();
	renderer2->SetViewport(0.5, 0.0, 1.0, 1.0);
	renderer2->AddActor(filledActor);
	renderer2->ResetCamera();
	renderer2->SetBackground(1.0, 1.0, 1.0);


	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer1);
	renderWindow->AddRenderer(renderer2);
	renderWindow->SetSize(600, 400);
	renderWindow->Render();
	renderWindow->SetWindowName("PolyDataClosed");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	renderer1->GetActiveCamera()->SetPosition(0, -1, 0);
	renderer1->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer1->GetActiveCamera()->SetViewUp(0, 0, 1);
	renderer1->GetActiveCamera()->Azimuth(30);
	renderer1->GetActiveCamera()->Elevation(30);
	renderer1->ResetCamera();
	renderer2->SetActiveCamera(renderer1->GetActiveCamera());

	interactor->Initialize();
	interactor->Start();
	return 0;
}




