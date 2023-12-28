#include <vtkSmartPointer.h>
#include <vtkImageReader2.h>
#include <vtkMatrix4x4.h>
#include <vtkImageReslice.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkCommand.h>
#include <vtkImageData.h>
#include <vtkMetaImageReader.h>
#include <vtkImageCast.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
class vtkImageInteractionCallback : public vtkCommand
{
public:
	static vtkImageInteractionCallback*  New()
	{
		return new vtkImageInteractionCallback;
	}

	vtkImageInteractionCallback()
	{
		this->Slicing = 0;
		this->ImageReslice = 0;
		this->Interactor = 0;
	}

	void SetImageReslice(vtkImageReslice *reslice)
	{
		this->ImageReslice = reslice;
	}

	vtkImageReslice *GetImageReslice()
	{
		return this->ImageReslice;
	}

	void SetImageMapToColors(vtkImageMapToColors *mapColor)
	{
		this->mapColor = mapColor;
	}

	void SetInteractor(vtkRenderWindowInteractor *interactor)
	{
		this->Interactor = interactor;
	}

	vtkRenderWindowInteractor *GetInteractor()
	{
		return this->Interactor;
	}

	virtual void Execute(vtkObject *, unsigned long event, void *) override
	{
		vtkRenderWindowInteractor* interactor = this->GetInteractor();
		//获取鼠标前后两次点击的位置
		int lastPos[2];
		interactor->GetLastEventPosition(lastPos);
		int currPos[2];
		interactor->GetEventPosition(currPos);

		if (event == vtkCommand::LeftButtonPressEvent)
		{
			this->Slicing = 1;
		}
		else if (event == vtkCommand::LeftButtonReleaseEvent)
		{
			this->Slicing = 0;
		}
		else if (event == vtkCommand::MouseMoveEvent)
		{
			//只有当鼠标左键按下的时候才执行切面提取
			if (this->Slicing)
			{
				vtkImageReslice *reslice = this->ImageReslice;
				//鼠标纵向移动的距离
				int deltaY = lastPos[1] - currPos[1];
				reslice->Update();
				//每个切片的间距（在Z方向上）
				double sliceSpacing = reslice->GetOutput()->GetSpacing()[2];
				vtkMatrix4x4* matrix = reslice->GetResliceAxes();
				double point[4];
				double center[4];
				point[0] = 0.0;
				point[1] = 0.0;
				point[2] = sliceSpacing * deltaY;//Z 方向上的新的切片位置。
				point[3] = 1.0;
				matrix->MultiplyPoint(point, center);
				matrix->SetElement(0, 3, center[0]);
				matrix->SetElement(1, 3, center[1]);
				matrix->SetElement(2, 3, center[2]);
				mapColor->Update();
				interactor->Render();
			}
			else
			{
				vtkInteractorStyle *style = vtkInteractorStyle::SafeDownCast(
					interactor->GetInteractorStyle());
				if (style)
				{
					style->OnMouseMove();
				}
			}
		}
	}

private:
	int Slicing;
	vtkImageReslice* ImageReslice;
	vtkImageMapToColors* mapColor;
	vtkRenderWindowInteractor* Interactor;
};

int main()
{
	vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName("D:\\Documents\\Material\\VTK-learn\\Examples\\Chap05\\DATA\\BRAIN.MHD");
	reader->Update();

	int extent[6];
	double spacing[3];
	double origin[3];

	reader->GetOutput()->GetExtent(extent);
	reader->GetOutput()->GetSpacing(spacing);
	reader->GetOutput()->GetOrigin(origin);

	//计算图像中心坐标
	double center[3];
	center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
	center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
	center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);

	//切面的变换矩阵
	static double transMatrix[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	vtkSmartPointer<vtkMatrix4x4> matrix= vtkSmartPointer<vtkMatrix4x4>::New();
	matrix->DeepCopy(transMatrix);
	matrix->SetElement(0, 3, center[0]);
	matrix->SetElement(1, 3, center[1]);
	matrix->SetElement(2, 3, center[2]);

	vtkSmartPointer<vtkImageReslice> reslice = vtkSmartPointer<vtkImageReslice>::New();
	reslice->SetInputConnection(reader->GetOutputPort());
	reslice->SetOutputDimensionality(2);//二维图像
	reslice->SetResliceAxes(matrix);//设置变换矩阵
	reslice->SetInterpolationModeToLinear();//线性插值
	reslice->Update();

	vtkSmartPointer<vtkLookupTable> colorTable = vtkSmartPointer<vtkLookupTable>::New();
	colorTable->SetRange(0, 1000);
	colorTable->SetValueRange(0.0, 1.0);
	colorTable->SetSaturationRange(0.0, 0.0);//饱和度，0.0指黑白灰阶
	colorTable->SetRampToLinear();//映射方式为线性映射
	colorTable->Build();

	vtkSmartPointer<vtkImageMapToColors> colorMap = vtkSmartPointer<vtkImageMapToColors>::New();
	colorMap->SetLookupTable(colorTable);
	colorMap->SetInputConnection(reslice->GetOutputPort());
	colorMap->Update();

	vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
	actor->SetInputData(colorMap->GetOutput());

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(600,400);
	renderWindow->SetWindowName("ImageResliceExample2");
	renderWindow->Render();

	vtkSmartPointer<vtkRenderWindowInteractor> Interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	Interactor->SetInteractorStyle(style);
	Interactor->SetRenderWindow(renderWindow);
	Interactor->Initialize();

	vtkSmartPointer<vtkImageInteractionCallback> callback =
		vtkSmartPointer<vtkImageInteractionCallback>::New();
	callback->SetImageReslice(reslice);
	callback->SetImageMapToColors(colorMap);
	callback->SetInteractor(Interactor);

	style->AddObserver(vtkCommand::LeftButtonPressEvent, callback);
	style->AddObserver(vtkCommand::LeftButtonReleaseEvent, callback);
	style->AddObserver(vtkCommand::MouseMoveEvent, callback);
	Interactor->Start();
	return 0;
}
