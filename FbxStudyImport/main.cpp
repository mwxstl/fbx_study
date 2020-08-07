#include "Common.h"
#include "DisplayAnimation.h"
#include "DisplayCommon.h"
#include "DisplayGenericInfo.h"
#include "DisplayGlobalSettings.h"
#include "DisplayHierarchy.h"
#include "DisplayPose.h"
#include "DisplayNode.h"

void displayContent(FbxScene *pScene);
void displayContent(FbxNode *pNode);
void displayMetaData(FbxScene *pScene);

static bool gVerbose = true;

int main(int argc, char** argv)
{
	FbxManager* manager = NULL;
	FbxScene *scene = NULL;
	bool result;

	initializeSdkObjects(manager, scene);

	/*FbxString filePath("F:\\resource\\farm-life\\AllModels_Sepearated\\Animals\\Animated\\CatAnimations.fbx");*/
	FbxString filePath("D:\\resource\\farm-life\\AllModels_Sepearated\\Animals\\Animated\\CatAnimations.fbx");

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

void displayContent(FbxScene* pScene)
{
	FbxNode *node = pScene->GetRootNode();

	if (node)
	{
		for (int i = 0; i < node->GetChildCount(); i++)
		{
			displayContent(node->GetChild(i));
		}
	}
}

void displayContent(FbxNode* pNode)
{
	FbxNodeAttribute::EType attributeType;

	if (pNode->GetNodeAttribute() == NULL)
	{
		FBXSDK_printf("NULL node attribute\n\n");
	}
	else
	{
		attributeType = (pNode->GetNodeAttribute()->GetAttributeType());

		switch (attributeType)
		{
		case FbxNodeAttribute::eMarker:
			displayMarker(pNode);
			break;
		case FbxNodeAttribute::eSkeleton:
			displaySkeleton(pNode);
			break;
		case FbxNodeAttribute::eMesh:
			displayMesh(pNode);
			break;
		case FbxNodeAttribute::eNurbs:
			displayNurb(pNode);
			break;
		case FbxNodeAttribute::ePatch:
			displayPatch(pNode);
			break;
		case FbxNodeAttribute::eCamera:
			displayCamera(pNode);
			break;
		case FbxNodeAttribute::eLight:
			displayLight(pNode);
			break;
		case FbxNodeAttribute::eLODGroup:
			displayLodGroup(pNode);
			break;
		default:
			break;
		}
	}

	displayUserProperties(pNode);
	displayTarget(pNode);
	displayPivotsAndLimits(pNode);
	displayGeometricTransform(pNode);
	displayGeometricTransform(pNode);

	for (int i = 0; i < pNode->GetChildCount(); i++)
	{
		displayContent(pNode->GetChild(i));
	}
}



void displayMetaData(FbxScene *pScene)
{
	FbxDocumentInfo *sceneInfo = pScene->GetSceneInfo();
	if (sceneInfo)
	{
		FBXSDK_printf("\n\n--------------------\nMeta-Data\n--------------------\n\n");
		FBXSDK_printf("    Title: %s\n", sceneInfo->mTitle.Buffer());
		FBXSDK_printf("    Subject: %s\n", sceneInfo->mSubject.Buffer());
		FBXSDK_printf("    Author: %s\n", sceneInfo->mAuthor.Buffer());
		FBXSDK_printf("    Keywords: %s\n", sceneInfo->mKeywords.Buffer());
		FBXSDK_printf("    revision: %s\n", sceneInfo->mKeywords.Buffer());
		FBXSDK_printf("    revision: %s\n", sceneInfo->mRevision.Buffer());
		FBXSDK_printf("    comment: %s\n", sceneInfo->mComment.Buffer());

		FbxThumbnail *thumbnail = sceneInfo->GetSceneThumbnail();
		if (thumbnail)
		{
			FBXSDK_printf("    thumbnail:\n");

			switch (thumbnail->GetDataFormat())
			{
			case FbxThumbnail::eRGB_24:
				FBXSDK_printf("        format: RGB\n");
				break;
			case FbxThumbnail::eRGBA_32:
				FBXSDK_printf("        format: RGBA\n");
				break;
			default:
				break;
			}

			switch (thumbnail->GetSize())
			{
			case FbxThumbnail::eNotSet:
				FBXSDK_printf("        size: no dimensions specified (%ld bytes)\n", thumbnail->GetSizeInBytes());
				break;
			case FbxThumbnail::e64x64:
				FBXSDK_printf("        size: 64 x 64 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
				break;
			case FbxThumbnail::e128x128:
				FBXSDK_printf("        size: 128 x 128 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
			}
		}
	}
}