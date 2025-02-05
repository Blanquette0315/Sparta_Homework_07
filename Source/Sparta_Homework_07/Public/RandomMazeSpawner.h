#pragma once

#include <queue>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomMazeSpawner.generated.h"

class AFallPlatform;

UCLASS()
class SPARTA_HOMEWORK_07_API ARandomMazeSpawner : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maze|Properties" , Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFallPlatform> BPSpawnPlatformRef;
	
	struct FCell
	{
		int x, y, weight;
		bool operator > (const FCell& other) const
		{
			return weight > other.weight;
		}
	};

	std::priority_queue<FCell, std::vector<FCell>, std::greater<FCell>>Pq_Frontier;

	std::vector<std::vector<int>> Maze;

	virtual bool ShouldTickIfViewportsOnly() const override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze|Components")
	USceneComponent* SceneRootComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Maze|Properties")
	int Width;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Maze|Properties")
	int Height;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Maze|Properties")
	int Offset_X;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Maze|Properties")
	int Offset_Y;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Maze")
	//int StartPoint_X;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Maze")
	//int StartPoint_Y;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Maze")
	//int EndPoint_X;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Maze")
	//int EndPoint_Y;
	
public:	
	ARandomMazeSpawner();

protected:
	virtual void BeginPlay() override;

	void AddFrontier(int x, int y);
	void MarkCell(int x, int y);

public:	
	virtual void Tick(float DeltaTime) override;
	
	void GenerateMaze();
	void SpawnMaze();
};
