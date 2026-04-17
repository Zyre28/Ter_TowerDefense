#include "GridManager.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "EnemyBase.h"
#include "Engine/World.h"

AGridManager::AGridManager()
{
    PrimaryActorTick.bCanEverTick = false;

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
    RootComponent = VisualMesh;
}

void AGridManager::BeginPlay()
{
    Super::BeginPlay();

    FVector MeshOrigin, MeshExtent;
    VisualMesh->GetLocalBounds(MeshOrigin, MeshExtent);

    CellSizeX = (MeshExtent.X * 2.f) / NumCols;
    CellSizeY = (MeshExtent.Y * 2.f) / NumRows;
    GridOrigin = GetActorLocation() - FVector(MeshExtent.X, MeshExtent.Y, 0.f);
    float GridZ = GetActorLocation().Z + MeshExtent.Z;
    GridOrigin.Z = GridZ; 

    UE_LOG(LogTemp, Warning, TEXT("MeshExtent: %s"), *MeshExtent.ToString());
    UE_LOG(LogTemp, Warning, TEXT("GridOrigin: %s"), *GridOrigin.ToString());
    UE_LOG(LogTemp, Warning, TEXT("CellSize: %f x %f"), CellSizeX, CellSizeY);

    Cells.SetNum(NumRows * NumCols);
    for (int32 R = 0; R < NumRows; R++)
    {
        for (int32 C = 0; C < NumCols; C++)
        {
            FGridCell& Cell = Cells[R * NumCols + C];
            Cell.Row = R;
            Cell.Col = C;
        }
    }

    for (const FGridCell& Cell : Cells)
    {
        FVector Center = Cell.GetWorldCenter(GridOrigin, CellSizeX, CellSizeY);
        DrawDebugBox(GetWorld(), Center,
            FVector(CellSizeX * 0.5f, CellSizeY * 0.5f, 1.f),
            FColor::Green, false, 30.f, 0, 0.3f);
    }
}

bool AGridManager::IsValidCell(int32 Row, int32 Col) const
{
    return Row >= 0 && Row < NumRows && Col >= 0 && Col < NumCols;
}

FGridCell* AGridManager::GetCellFromWorld(FVector WorldPos)
{
    int32 Col = FMath::FloorToInt((WorldPos.X - GridOrigin.X) / CellSizeX);
    int32 Row = FMath::FloorToInt((WorldPos.Y - GridOrigin.Y) / CellSizeY);

    if (!IsValidCell(Row, Col))
        return nullptr;

    return &Cells[Row * NumCols + Col];
}

FVector AGridManager::SnapToGrid(FVector WorldPos)
{
    FGridCell* Cell = GetCellFromWorld(WorldPos);
    if (!Cell)
        return WorldPos;

    return Cell->GetWorldCenter(GridOrigin, CellSizeX, CellSizeY);
}

bool AGridManager::PlacePlant(APlantBase* Plant, int32 Row, int32 Col)
{
    if (!IsValidCell(Row, Col))
        return false;

    FGridCell& Cell = Cells[Row * NumCols + Col];
    if (!Cell.IsEmpty())
        return false;

    Cell.OccupyingPlant = Plant;
    return true;
}

void AGridManager::RemovePlant(int32 Row, int32 Col)
{
    if (!IsValidCell(Row, Col))
        return;

    Cells[Row * NumCols + Col].OccupyingPlant = nullptr;
}

APlantBase* AGridManager::SpawnPlant(TSubclassOf<APlantBase> PlantClass, int32 Row, int32 Col)
{
    UE_LOG(LogTemp, Warning, TEXT("SpawnPlant called [%d,%d]"), Row, Col);
    if (!IsValidCell(Row, Col) || !PlantClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cell already used [%d,%d]"), Row, Col);
        return nullptr;
    }

    FGridCell& Cell = Cells[Row * NumCols + Col];

    FVector WorldCenter = Cell.GetWorldCenter(GridOrigin, CellSizeX, CellSizeY);
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    APlantBase* NewPlant = GetWorld()->SpawnActor<APlantBase>(
        PlantClass, WorldCenter, FRotator::ZeroRotator, Params);

    if (NewPlant)
    {   
        NewPlant->InitOnGrid(Row, Col, WorldCenter);
        Cell.OccupyingPlant = NewPlant;

        UE_LOG(LogTemp, Warning, TEXT("Spawned %s at [%d,%d]"),
            *PlantClass->GetName(), Row, Col);
        
        NewPlant->SetResourceManager(ResourceManager);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Spawn FAILED at [%d,%d]"), Row, Col);
    }
    return NewPlant;
}

AEnemyBase* AGridManager::SpawnEnemy(TSubclassOf<AEnemyBase> EnemyClass, int32 Row, int32 Col)
{
    UE_LOG(LogTemp, Warning, TEXT("SpawnPlant called [%d,%d]"), Row, Col);
    if (!IsValidCell(Row, Col) || !EnemyClass)
        return nullptr;

    FGridCell& Cell = Cells[Row * NumCols + Col];
    if (!Cell.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cell [%d,%d] already occupied"), Row, Col);
        return nullptr;
    }

    FVector WorldCenter = Cell.GetWorldCenter(GridOrigin, CellSizeX, CellSizeY);
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AEnemyBase* NewEnemy = GetWorld()->SpawnActor<AEnemyBase>(
        EnemyClass, WorldCenter, FRotator::ZeroRotator, Params);

    if (NewEnemy)
    {   
        NewEnemy->InitOnGrid(Row, Col, WorldCenter);

        UE_LOG(LogTemp, Warning, TEXT("Spawned %s at [%d,%d]"),
            *EnemyClass->GetName(), Row, Col);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Spawn FAILED at [%d,%d]"), Row, Col);
    }
    return NewEnemy;
}

bool AGridManager::WorldToGrid(FVector WorldPos, int32& OutRow, int32& OutCol)
{
    FVector LocalPos = WorldPos - GridOrigin;

    OutCol = FMath::FloorToInt(LocalPos.X / CellSizeX);
    OutRow = FMath::FloorToInt(LocalPos.Y / CellSizeY);

    return IsValidCell(OutRow, OutCol);
}

#if WITH_EDITOR
void AGridManager::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    FVector MeshOrigin, MeshExtent;
    VisualMesh->GetLocalBounds(MeshOrigin, MeshExtent);

    float SizeX = (MeshExtent.X * 2.f) / NumCols;
    float SizeY = (MeshExtent.Y * 2.f) / NumRows;

    float GridZ = GetActorLocation().Z + MeshExtent.Z;

    FVector Origin = GetActorLocation() + MeshOrigin - FVector(MeshExtent.X, MeshExtent.Y, 0.f);
    Origin.Z = GridZ;

    for (int32 R = 0; R < NumRows; R++)
    {
        for (int32 C = 0; C < NumCols; C++)
        {
            FVector Center = Origin + FVector(
                (C + 0.5f) * SizeX,
                (R + 0.5f) * SizeY,
                0.f
            );
            DrawDebugBox(GetWorld(), Center,
                FVector(SizeX * 0.5f, SizeY * 0.5f, 1.f),
                FColor::Cyan, false, 5.f, 0, 0.3f);
        }
    }
}
#endif