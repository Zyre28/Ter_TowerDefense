#include "W_MapNode.h"

#include "UA_Game.h"
#include "LevelManager.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UW_MapNode::NativeConstruct()
{
	Super::NativeConstruct();
	if (NodeButton)
		NodeButton->OnPressed.AddDynamic(this, &UW_MapNode::OnPressed);
}

void UW_MapNode::InitNode(FLevelTypeData _NodeData)
{
	NodeData = _NodeData;

	if (NodeName)
	{
		FString Name = StaticEnum<ELevelType>()->GetNameStringByValue((int64)NodeData.LevelType);
		NodeName->SetText(FText::FromString(Name));
	}
}

void UW_MapNode::OnPressed()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Node pressed: %s"), *StaticEnum<ELevelType>()->GetNameStringByValue((int64)NodeData.LevelType));

	OnNodeSelected.Broadcast(NodeData);
}

void UW_MapNode::SetIsCurrent(bool bIsCurrent)
{
	if (NodeName)
	{
		NodeName->SetColorAndOpacity(bIsCurrent ? FSlateColor(FLinearColor::Red) : FSlateColor(FLinearColor::White));
	}
}