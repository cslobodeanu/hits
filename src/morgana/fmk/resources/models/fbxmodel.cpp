#include "fbxmodel.h"

#include "fbxsdk.h"

using namespace MorganaEngine::Framework::Resources::Models;

__implement_class(FBXModel, ResourceFile);

FBXModel::FBXModel() : ResourceFile()
{

}

FBXModel::~FBXModel()
{

}

void FBXModel::ImportFromStreams(const StreamList&  streams, const char* args /*= NULL*/)
{
	// create a SdkManager
	FbxManager* lSdkManager = FbxManager::Create();
	// create an IOSettings object
	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	// create an empty scene
	FbxScene* lScene = FbxScene::Create(lSdkManager, "");
	// create an importer.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	lImporter->Destroy();

}

const bool FBXModel::SaveToStream(Stream* s)
{
	return false;
}