#include "RandomMazeSpawner.h"
#include "DrawDebugHelpers.h"
#include "FallPlatform.h"

ARandomMazeSpawner::ARandomMazeSpawner()
	: Width(3)
	, Height(3)
	, Offset_X(0.0f)
	, Offset_Y(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComp = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRootComp);
}

void ARandomMazeSpawner::BeginPlay()
{
	Super::BeginPlay();

	GenerateMaze();
	SpawnMaze();
}

void ARandomMazeSpawner::Tick(float DeltaTime)
{
#if WITH_EDITOR
	// 에디터 Tick
	if (nullptr != GetWorld() && GetWorld()->WorldType == EWorldType::Editor)
	{
		if (nullptr == BPSpawnPlatformRef)
		{
			DrawDebugBox(GetWorld(), GetActorLocation(),FVector(100.f, 100.f, 100.f), FColor::Cyan);
		}
		else
		{
			FVector BounceExtent = BPSpawnPlatformRef->GetDefaultObject<AFallPlatform>()->GetComponentByClass<UStaticMeshComponent>()->GetStaticMesh()->GetBounds().BoxExtent;
			FVector StandardLocation = GetActorLocation() + FVector(BounceExtent.X, BounceExtent.Y, 0.f);
			float AccOffset_Y = 0.0f;
			for (int i = 0; i < Height; ++i)
			{
				float AccOffset_X = 0.0f;
				for (int j = 0; j <Width; ++j)
				{
					FVector Offset = FVector(AccOffset_X + j * BounceExtent.X * 2, AccOffset_Y + i * BounceExtent.Y * 2, 0.f);
					DrawDebugBox(GetWorld(),  StandardLocation + Offset,FVector(BounceExtent.X, BounceExtent.Y, BounceExtent.Z), FColor::Cyan);
					AccOffset_X += Offset_X;
				}
				AccOffset_Y += Offset_Y;
			}
		}
		return;
	}
#endif
	Super::Tick(DeltaTime);
	
}


void ARandomMazeSpawner::AddFrontier(int x, int y)
{
	if (x > 0 && x < Width - 1 && y > 0 && y < Height - 1
		&& Maze[y][x] == 1)
	{
		Maze[y][x] = 2;
		Pq_Frontier.push({x,y,rand() % 100});
	}
}

void ARandomMazeSpawner::MarkCell(int x, int y)
{
	Maze[y][x] = 0;
	AddFrontier(x - 2, y);
	AddFrontier(x + 2, y);
	AddFrontier(x, y - 2);
	AddFrontier(x, y + 2);
}

void ARandomMazeSpawner::GenerateMaze()
{
	srand(time(nullptr));

	// 미로 크기를 항상 홀수로 변환 (짝수면 한 칸 추가)
	if (Width % 2 == 0) Width++;
	if (Height % 2 == 0) Height++;

	Maze.assign(Height, std::vector<int>(Width, 1)); // 모든 셀을 벽(1)으로 초기화

	// 시작 위치를 랜덤하게 선택 (홀수 좌표)
	int startX = (rand() % (Width / 2)) * 2 + 1;
	int startY = (rand() % (Height / 2)) * 2 + 1;
	MarkCell(startX, startY);

	while (!Pq_Frontier.empty()) {
		FCell cell = Pq_Frontier.top(); // 가장 작은 가중치의 프런티어 선택
		Pq_Frontier.pop();

		std::vector<FCell> neighbors;
		if (cell.x >= 2 && Maze[cell.y][cell.x - 2] == 0) neighbors.push_back({ cell.x - 2, cell.y, 0 });
		if (cell.x < Width - 2 && Maze[cell.y][cell.x + 2] == 0) neighbors.push_back({ cell.x + 2, cell.y, 0 });
		if (cell.y >= 2 && Maze[cell.y - 2][cell.x] == 0) neighbors.push_back({ cell.x, cell.y - 2, 0 });
		if (cell.y < Height - 2 && Maze[cell.y + 2][cell.x] == 0) neighbors.push_back({ cell.x, cell.y + 2, 0 });

		if (!neighbors.empty()) {
			FCell neighbor = neighbors[rand() % neighbors.size()];
			Maze[(cell.y + neighbor.y) / 2][(cell.x + neighbor.x) / 2] = 0; // 벽 제거
			MarkCell(cell.x, cell.y);
		}
	}
	
	// 시작점과 끝점을 미로의 테두리에서 선택
	std::vector<std::pair<int, int>> edges;

	// 테두리의 가능한 위치 추가 (홀수 좌표만)
	// 랜덤하게 시작점과 끝점을 선택
	for (int x = 1; x < Width - 1; ++x) {
		if (Maze[1][x] == 0)
		{
			edges.push_back({ x, 0 });
		}
	}
	std::pair<int, int> StartPoint = edges[rand() % edges.size()];

	edges.clear();
	for (int x = 1; x < Width - 1; ++x) {
		if (Maze[Height - 2][x] == 0)
		{
			edges.push_back({ x, Height - 1 });
		}
	}
	std::pair<int, int> ExitPoint = edges[rand() % edges.size()];

	// 출입구를 뚫기
	Maze[StartPoint.second][StartPoint.first] = 0;
	Maze[ExitPoint.second][ExitPoint.first] = 0;
}

void ARandomMazeSpawner::SpawnMaze()
{
	FVector BounceExtent = BPSpawnPlatformRef->GetDefaultObject<AFallPlatform>()->GetComponentByClass<UStaticMeshComponent>()->GetStaticMesh()->GetBounds().BoxExtent;
	FVector StandardLocation = GetActorLocation();
	float AccOffset_Y = 0.0f;
	for (int i = 0; i < Height; ++i)
	{
		float AccOffset_X = 0.0f;
		for (int j = 0; j <Width; ++j)
		{
			FVector Offset = FVector(AccOffset_X + j * BounceExtent.X * 2, AccOffset_Y + i * BounceExtent.Y * 2, 0.f);
			FVector SpawnLocation = StandardLocation + Offset;
			AccOffset_X += Offset_X;

			AFallPlatform* NewPlatform = GetWorld()->SpawnActor<AFallPlatform>(BPSpawnPlatformRef, SpawnLocation, FRotator::ZeroRotator);
			NewPlatform->SetResetTime(0.0f);
			if (Maze[i][j])
			{
				NewPlatform->SetHolingTime(0.001f);
			}
			else
			{
				NewPlatform->SetHolingTime(0.0f);
				NewPlatform->GetComponentByClass<UStaticMeshComponent>()->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.f, 0.f, 0.f));
			}
		}
		AccOffset_Y += Offset_Y;
	}
}

bool ARandomMazeSpawner::ShouldTickIfViewportsOnly() const
{
	//return Super::ShouldTickIfViewportsOnly();
	if (nullptr != GetWorld() && GetWorld()->WorldType == EWorldType::Editor)
	{
		return true;
	}
	else
	{
		return false;
	}
}
