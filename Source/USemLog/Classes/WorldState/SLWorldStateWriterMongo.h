// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "CoreMinimal.h"
#include "ISLWorldStateWriter.h"
#if WITH_LIBMONGO
#include "mongoc.h"
#endif //WITH_LIBMONGO

// Forward declaration
class FSLWorldStateAsyncWorker;

/**
 * Raw data logger to a mongo database
 */
class FSLWorldStateWriterMongo : public ISLWorldStateWriter
{
public:
	// Default constr
	FSLWorldStateWriterMongo(float DistanceStepSize, float RotationStepSize, 
		const FString& Location, const FString& EpisodeId, const FString& HostIP, uint16 HostPort);

	// Destr
	virtual ~FSLWorldStateWriterMongo();

	// Write the data
	virtual void Write(TArray<TSLItemState<AActor>>& NonSkeletalActorPool,
		TArray<TSLItemState<ASLSkeletalMeshActor>>& SkeletalActorPool,
		TArray<TSLItemState<USceneComponent>>& NonSkeletalComponentPool,
		float Timestamp) override;

private:
	// Connect to mongo db
	bool ConnectToMongo(const FString& InLogDB,
		const FString& InEpisodeId,
		const FString& InMongoIP,
		uint16 MongoPort=27017);
#if WITH_LIBMONGO
	// Add actors
	void AddActors(bson_t& OutBsonEntitiesArr);

	// Add components
	void AddComponents(bson_t& OutBsonEntitiesArr);

	// Get entry as Bson object
	bson_t GetAsBsonEntry(const FString& InId,
		const FString& InClass,
		const FVector& InLoc,
		const FQuat& InQuat);

	// Write entry to db
	void WriteToMongo(bson_t*& InRootObj, mongoc_collection_t* &collection);
#endif //WITH_LIBMONGO
	// Pointer to worker parent (access to raw data structure)
	FSLWorldStateAsyncWorker* WorkerParent;

	bool bConnect;
#if WITH_LIBMONGO
	// Pointer to monge database
	mongoc_client_t *client;
	mongoc_database_t *database;
	mongoc_collection_t *collection;
#endif //WITH_LIBMONGO
};
