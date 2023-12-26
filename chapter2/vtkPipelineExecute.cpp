#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkImageData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	//GetMTime():用于获取对象的修改时间
	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	std::cout << "初始化后，reader修改时间: " << reader->GetMTime() << std::endl;
	reader->SetFileName("D:\\Pictures\\Saved Pictures\\test.jpeg");
	std::cout << "设置文件路径后，reader修改时间: " << reader->GetMTime() << std::endl;

	vtkImageData* imageData = reader->GetOutput();
	std::cout << "GetOutput()后，reader修改时间: " << reader->GetMTime() << std::endl;
	reader->Update();
	std::cout << "更新后，reader修改时间: " << reader->GetMTime() << std::endl;

	int extent[6];//在 x、y、z 三个方向上数据的起始和终止位置。
	imageData->GetExtent(extent);
	std::cout << "图片范围: " << extent[0] << " "
		<< extent[1] << " " << extent[2] << " " << extent[3] << " "
		<< extent[4] << " " << extent[5] << " " << std::endl;

	vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	viewer->SetupInteractor(interactor);
	viewer->SetInputData(imageData);
	viewer->Render();
	viewer->SetSize(600,600);
	viewer->GetRenderWindow()->SetWindowName("vtkPipelineExecute");

	interactor->Initialize();
	interactor->Start();

	return 0;
}