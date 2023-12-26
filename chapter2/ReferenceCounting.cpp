#include <vtkSmartPointer.h>
#include <vtkJPEGReader.h>
#include <vtkImageData.h>
#include <vtkObject.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

vtkSmartPointer<vtkImageData> MyFunction()
{
	vtkSmartPointer<vtkImageData> myObject = vtkSmartPointer<vtkImageData>::New();
	std::cout << "MyFunction::myObject引用计数：" << myObject->GetReferenceCount() << std::endl;
	return myObject;
}

int main()
{
	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("D:\\Pictures\\Saved Pictures\\test.jpeg");
	reader->Update();

	std::cout << "reader->GetOutput()引用计数： " << reader->GetOutput()->GetReferenceCount() << std::endl;

	vtkSmartPointer<vtkImageData> image1 = reader->GetOutput();
	std::cout << "reader->GetOutput()引用计数(赋给image1后)： " << reader->GetOutput()->GetReferenceCount() << std::endl;
	std::cout << "image1 引用计数： " << image1->GetReferenceCount() << std::endl;

	vtkSmartPointer<vtkImageData> image2 = reader->GetOutput();
	std::cout << "reader->GetOutput()引用计数(赋给image2后)： " << reader->GetOutput()->GetReferenceCount() << std::endl;
	std::cout << "image2 引用计数：" << image2->GetReferenceCount() << std::endl;

	std::cout << "myObject 引用计数：" << MyFunction()->GetReferenceCount() << std::endl;

	vtkSmartPointer<vtkImageData> MyImageData = MyFunction();
	std::cout << "MyFunction返回值 引用计数：" << MyFunction()->GetReferenceCount() << std::endl;
	std::cout << "MyImageData 引用计数：" << MyImageData->GetReferenceCount() << std::endl;

	return 0;
}