// Fill out your copyright notice in the Description page of Project Settings.

// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION
#include "PaintWidget.h"
#include "Rendering/DrawElements.h"




UPaintWidget::UPaintWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UPaintWidget::CleanGraph()
{
	PointSettingMap.Empty();
	PaintTimes = 0;
	return true;
}

bool UPaintWidget::CleanLastPaint()
{
	PointSettingMap.Remove(PaintTimes);
	PaintTimes--;
	return true;
}

bool UPaintWidget::SetBrushColor(FLinearColor inColor)
{
	BrushLineColor = inColor;
	return true;
}

bool UPaintWidget::SetBrushThickness(float inThickness)
{
	Brushthickness = inThickness;
	return true;
}

void UPaintWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();
}


void UPaintWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!IsVisible())
	{
		return;
	}
}

#if ENGINE_MAJOR_VERSION > 4 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 20)
int32 UPaintWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	LayerId = UUserWidget::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	Context.MaxLayer++;

	if (bMouseDown)
	{
		DrawOnWidget(InContext);
	}
	return FMath::Max(LayerId, Context.MaxLayer);
}
#else
void UPaintWidget::NativePaint(FPaintContext& InContext) const
{
	//Paint every tick
	UUserWidget::NativePaint(InContext);
	InContext.MaxLayer++;
	DrawOnWidget(InContext);

}
#endif


FReply UPaintWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	auto reply = UUserWidget::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (reply.IsEventHandled())
	{
		return reply;
	}
	if (bStart == false)
		return reply;
	bMouseDown = true;
	PaintTimes++;
	brushSetting.lineColor = BrushLineColor;
	brushSetting.thickness = Brushthickness;
	PointSettingMap.Add(PaintTimes) = brushSetting;
	return FReply::Handled();
}

FReply UPaintWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	auto reply = UUserWidget::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	if (reply.IsEventHandled())
	{
		return reply;
	}
	if (bStart == false)
	{
		return reply;
	}
	bMouseDown = false;

	return FReply::Handled();
}

FReply UPaintWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	auto Reply = UUserWidget::NativeOnMouseMove(InGeometry, InMouseEvent);

	if (Reply.IsEventHandled())
	{
		return Reply;
	}

	if (bMouseDown)
	{
		// Handle movement.
		//return HandleMinimapClick(InGeometry, InMouseEvent);
		PointSettingMap[PaintTimes].PointArray.Add(InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()));
	}

	return FReply::Unhandled();
}

void UPaintWidget::DrawOnWidget(FPaintContext& inPaintContext) const
{
	if (PointSettingMap.Num() > 0) {
		for (auto brushStruct : PointSettingMap)
		{
			FBrushSetting value = (brushStruct).Value;
			FSlateDrawElement::MakeLines(inPaintContext.OutDrawElements, inPaintContext.MaxLayer, inPaintContext.AllottedGeometry.ToPaintGeometry(), value.PointArray, ESlateDrawEffect::None, value.lineColor, true, value.thickness);
		}
	};
}


PRAGMA_ENABLE_OPTIMIZATION
