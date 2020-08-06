#include "Common.h"
#include "DisplayAnimation.h"
#include "DisplayCommon.h"
#include "DisplayGenericInfo.h"
#include "DisplayGlobalSettings.h"
#include "DisplayHierarchy.h"
#include "DisplayPose.h"

void displayContent(FbxScene *scene);
void displayMetaData(FbxScene *pScene);

static bool gVerbose = true;

int main(int argc, char** argv)
{
	FbxManager* manager = NULL;
	FbxScene *scene = NULL;
	bool result;

	initializeSdkObjects(manager, scene);

	FbxString filePath("F:\\resource\\farm-life\\AllModels_Sepearated\\Animals\\Animated\\CatAnimations.fbx");

	if (filePath.IsEmpty())
	{
		result = false;
		FBXSDK_printf("\n\nusage: importscene <fbx file name>\n\n");
	}
	else
	{
		FBXSDK_printf("\n\nfile: %s\n\n", filePath.Buffer());
		result = loadScene(manager, scene, filePath.Buffer());
	}

	if (result = false)
	{
		FBXSDK_printf("\n\n an error occurred while loading the scene...");
	}
	else
	{
		displayMetaData(scene);

		FBXSDK_printf("\n\n---------------------\nGlobal Light Settings\n---------------------\n\n");
		if (gVerbose) displayGlobalLightSettings(&scene->GetGlobalSettings());

		FBXSDK_printf("\n\n----------------------\nGlobal Camera Settings\n----------------------\n\n");

		if (gVerbose) displayGlobalCameraSettings(&scene->GetGlobalSettings());

		FBXSDK_printf("\n\n--------------------\nGlobal Time Settings\n--------------------\n\n");

		if (gVerbose) displayGlobalTimeSettings(&scene->GetGlobalSettings());
		
		FBXSDK_printf("\n\n---------\nHierarchy\n---------\n\n");

		if (gVerbose) displayHierarchy(scene);

		FBXSDK_printf("\n\n------------\nNode Content\n------------\n\n");

		if (gVerbose) displayContent(scene);

		FBXSDK_printf("\n\n----\nPose\n----\n\n");

		if (gVerbose) displayPose(scene);

		FBXSDK_printf("\n\n---------\nAnimation\n---------\n\n");

		if (gVerbose) displayAnimation(scene);

		FBXSDK_printf("\n\n---------\nGeneric Information\n---------\n\n");

		if (gVerbose) displayGenericInfo(scene);
	}

	destroySdkObjects(manager, result);

	return 0;
}

void displayContent(FbxScene* scene)
{
	
}


void displayMetaData(FbxScene *pScene)
{
	
}