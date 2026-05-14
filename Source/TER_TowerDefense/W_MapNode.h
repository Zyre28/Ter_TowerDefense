#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelType.h"
#include "W_MapNode.generated.h"

class UUA_Game;
class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNodeSelected, FLevelTypeData, NodeData);

UCLASS()
class TER_TOWERDEFENSE_API UW_MapNode : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	
	UPROPERTY(meta = (BindWidget))
	UButton* NodeButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NodeName;
	
	void InitNode(FLevelTypeData _NodeData);

	UPROPERTY(BlueprintAssignable)
	FOnNodeSelected OnNodeSelected;
	
	UFUNCTION(BlueprintCallable)
	void SetIsCurrent(bool bIsCurrent);
	
private:
	FLevelTypeData NodeData;

	UFUNCTION()
	void OnPressed();
};