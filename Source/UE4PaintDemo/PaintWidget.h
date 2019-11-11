// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Launch/Resources/Version.h"
#include "PaintWidget.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FBrushSetting
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite)
		TArray<FVector2D> PointArray;
	UPROPERTY(BlueprintReadWrite)
		FLinearColor lineColor;
	UPROPERTY(BlueprintReadWrite)
		float thickness;
};
UCLASS(Blueprintable)
class UE4PAINTDEMO_API UPaintWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()
	
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PaintGraph")
		float Brushthickness = 5.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PaintGraph")
		FLinearColor BrushLineColor = FLinearColor::Yellow;
	UFUNCTION(BlueprintCallable, Category = "PaintGraph")
		bool CleanGraph();
	UFUNCTION(BlueprintCallable, Category = "PaintGraph")
		bool CleanLastPaint();
	UFUNCTION(BlueprintCallable, Category = "PaintGraph")
		bool SetBrushColor(FLinearColor inColor);
	UFUNCTION(BlueprintCallable, Category = "PaintGraph")
		bool SetBrushThickness(float inThickness);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PaintGraph")
		bool bStart;
protected:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
#if ENGINE_MAJOR_VERSION > 4 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 20)
	int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
#else
	void NativePaint(FPaintContext& InContext) const override;
#endif

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override; //parent Mouse click event
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	void DrawOnWidget(FPaintContext& inPaintContext) const;
	FVector2D startPoint, endPonint;
	TMap<int32, FBrushSetting> PointSettingMap;
	bool bMouseDown;
	int32 PaintTimes = 0;
	FBrushSetting brushSetting;
	
};
