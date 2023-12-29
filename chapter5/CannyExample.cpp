#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkImageShiftScale.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkJPEGReader.h>
#include <vtkImageCast.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkImageGradient.h>
#include <vtkImageMagnitude.h>
#include <vtkImageNonMaximumSuppression.h>
#include <vtkImageConstantPad.h>
#include <vtkImageToStructuredPoints.h>
#include <vtkLinkEdgels.h>
#include <vtkThreshold.h>
#include <vtkGeometryFilter.h>
#include <vtkSubPixelPositionEdgels.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkStripper.h>
#include <vtkPolyDataMapper.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)//避免获取TextRenderer实例失败
VTK_MODULE_INIT(vtkInteractionStyle)

int main()
{
	vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap05\\DATA\\lena-gray.jpg");
	reader->Update();

	//将输入的图像数据转换成浮点型数据
	vtkSmartPointer<vtkImageCast> imageCast = vtkSmartPointer<vtkImageCast>::New();
	imageCast->SetOutputScalarTypeToFloat();
	imageCast->SetInputConnection(reader->GetOutputPort());
	imageCast->Update();

	//高斯平滑滤波器
	vtkSmartPointer<vtkImageGaussianSmooth> smoothFilter =
		vtkSmartPointer<vtkImageGaussianSmooth>::New();
	smoothFilter->SetInputConnection(imageCast->GetOutputPort());
	smoothFilter->SetDimensionality(2);
	smoothFilter->SetRadiusFactors(1, 1, 0);
	smoothFilter->Update();

	//对平滑后的图像数据进行梯度计算和幅值计算
	vtkSmartPointer<vtkImageGradient> imageGradient = vtkSmartPointer<vtkImageGradient>::New();
	imageGradient->SetInputConnection(smoothFilter->GetOutputPort());
	imageGradient->SetDimensionality(2);
	imageGradient->Update();

	vtkSmartPointer<vtkImageMagnitude> imageMagnitude = vtkSmartPointer<vtkImageMagnitude>::New();
	imageMagnitude->SetInputConnection(imageGradient->GetOutputPort());
	imageMagnitude->Update();

	//非局部峰值设置为0
	vtkSmartPointer<vtkImageNonMaximumSuppression> nonMax = vtkSmartPointer<vtkImageNonMaximumSuppression>::New();
	nonMax->SetMagnitudeInputData(imageMagnitude->GetOutput());//模值图像
	nonMax->SetVectorInputData(imageGradient->GetOutput());//梯度图像
	nonMax->SetDimensionality(2);
	nonMax->Update();

	//进行常数填充(padding)处理
	vtkSmartPointer<vtkImageConstantPad> pad = vtkSmartPointer<vtkImageConstantPad>::New();
	pad->SetInputConnection(imageGradient->GetOutputPort());
	pad->SetOutputNumberOfScalarComponents(3);//将输出图像的标量组件数设置为3
	pad->SetConstant(0);//将填充数值设置为0
	pad->Update();

	//将图像数据转换为结构化点数据
	vtkSmartPointer<vtkImageToStructuredPoints> i2sp1 =
		vtkSmartPointer<vtkImageToStructuredPoints>::New();
	i2sp1->SetInputConnection(nonMax->GetOutputPort());
	i2sp1->SetVectorInputData(pad->GetOutput());
	i2sp1->Update();

	//根据点的相邻关系连接成折线
	vtkSmartPointer<vtkLinkEdgels> imgLink =
		vtkSmartPointer<vtkLinkEdgels>::New();
	imgLink->SetInputData(i2sp1->GetOutput());
	imgLink->SetGradientThreshold(2);
	imgLink->Update();

	vtkSmartPointer<vtkThreshold> thresholdEdgels = vtkSmartPointer<vtkThreshold>::New();
	thresholdEdgels->SetInputConnection(imgLink->GetOutputPort());
	thresholdEdgels->ThresholdByUpper(10);//将阈值设置为10，即将大于10的边缘线分割出来。
	thresholdEdgels->AllScalarsOff();
	thresholdEdgels->Update();

	//将数据转化为几何数据
	vtkSmartPointer<vtkGeometryFilter> geometryFilter =
		vtkSmartPointer<vtkGeometryFilter>::New();
	geometryFilter->SetInputConnection(thresholdEdgels->GetOutputPort());
	geometryFilter->Update();

	vtkSmartPointer<vtkImageToStructuredPoints> i2sp =
		vtkSmartPointer<vtkImageToStructuredPoints>::New();
	i2sp->SetInputConnection(imageMagnitude->GetOutputPort());
	i2sp->SetVectorInputData(pad->GetOutput());
	i2sp->Update();

	//接收一系列连续曲线及其对应的梯度系信息作为输入，利用梯度信息来调整曲线位置
	vtkSmartPointer<vtkSubPixelPositionEdgels> spe =
		vtkSmartPointer<vtkSubPixelPositionEdgels>::New();
	spe->SetInputConnection(geometryFilter->GetOutputPort());
	spe->SetGradMapsData(i2sp->GetStructuredPointsOutput());
	spe->Update();

	//用于从输入多边形、三角形带和线中生成三角形带和/或折线
	vtkSmartPointer<vtkStripper> strip = vtkSmartPointer<vtkStripper>::New();
	strip->SetInputConnection(spe->GetOutputPort());
	strip->Update();

	vtkSmartPointer<vtkPolyDataMapper> dsm = vtkSmartPointer<vtkPolyDataMapper>::New();
	dsm->SetInputConnection(strip->GetOutputPort());
	dsm->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> planeActor = vtkSmartPointer<vtkActor>::New();
	planeActor->SetMapper(dsm);
	planeActor->GetProperty()->SetAmbient(1.0);
	planeActor->GetProperty()->SetDiffuse(0.0);
	planeActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

	vtkSmartPointer<vtkImageActor> originalActor = vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkRenderer> originalRenderer = vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(0.0, 0.0, 0.5, 1.0);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> outputRenderer = vtkSmartPointer<vtkRenderer>::New();
	outputRenderer->SetViewport(0.5, 0.0, 1.0, 1.0);
	outputRenderer->AddActor(planeActor);
	outputRenderer->ResetCamera();
	outputRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(outputRenderer);
	renderWindow->SetSize(600, 300);
	renderWindow->Render();
	renderWindow->SetWindowName("CannyExample");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();

	interactor->SetInteractorStyle(style);
	interactor->SetRenderWindow(renderWindow);

	interactor->Initialize();
	interactor->Start();
	return 0;
}


