#include "Common.h"

#ifdef IOS_REF
	#undef IOS_REF
	#define IOS_REF (*(pManager->GetIOSettings()))
#endif

void initializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
{
	pManager = FbxManager::Create();
	if (!pManager)
	{
		FBXSDK_printf("error: unable to create fbx manager!\n");
		exit(1);
	}
	else
	{
		FBXSDK_printf("autodesk fbx sdk version %s\n", pManager->GetVersion());

		FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
		pManager->SetIOSettings(ios);

		FbxString path = FbxGetApplicationDirectory();
		pManager->LoadPluginsDirectory(path.Buffer());

		pScene = FbxScene::Create(pManager, "My Scene");
		if (!pScene)
		{
			FBXSDK_printf("error: unable to create fbx scene!\n");
			exit(1);
		}
	}
}
void destroySdkObjects(FbxManager *pManager, bool pExitStatus)
{
	
}
bool loadScene(FbxManager* pManager, FbxScene* pScene, const char* pFilePath)
{
	int fileMajor, fileMinor, fileRevision;
	int sdkMajor, sdkMinor, sdkRevision;

	int animStackCount;
	bool status;
	char password[1024];

	FbxManager::GetFileFormatVersion(sdkMajor, sdkMinor, sdkRevision);

	FbxImporter *importer = FbxImporter::Create(pManager, "");

	const bool importStatus = importer->Initialize(pFilePath, -1, pManager->GetIOSettings());

	importer->GetFileVersion(fileMajor, fileMinor, fileRevision);

	if (!importStatus)
	{
		FbxString error = importer->GetStatus().GetErrorString();
		FBXSDK_printf("call to fbximporter::initialize() failed.\n");
		FBXSDK_printf("error returned: %s\n\n", error.Buffer());

		if (importer->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			FBXSDK_printf("fbx file format version for this fbx sdk is %d.%d.%d", sdkMajor, sdkMinor, sdkRevision);
			FBXSDK_printf("fbx file format version for file '%s' is %d.%d.%d", pFilePath, fileMajor, fileMinor, fileRevision);
		}
		return false;
	}
	FBXSDK_printf("fbx file format version for this fbx sdk is %d.%d.%d", sdkMajor, sdkMinor, sdkRevision);

	if (importer->IsFBX())
	{
		FBXSDK_printf("fbx file format version for file '%s' is %d.%d.%d", pFilePath, fileMajor, fileMinor, fileRevision);

		FBXSDK_printf("animation stack information\n");

		animStackCount = importer->GetAnimStackCount();

		FBXSDK_printf("    Number of Animation Stacks: %d\n", animStackCount);
		FBXSDK_printf("    current animation stack: \"%s\"\n", importer->GetActiveAnimStackName().Buffer());
		FBXSDK_printf("\n");

		for (int i = 0; i < animStackCount; i++)
		{
			FbxTakeInfo *takeInfo = importer->GetTakeInfo(i);

			FBXSDK_printf("    animation stack %d\n", i);
			FBXSDK_printf("        Name: \"%s\"\n", takeInfo->mName.Buffer());
			FBXSDK_printf("        Description: \"%s\"\n", takeInfo->mDescription.Buffer());

			FBXSDK_printf("        Import Name: \"%s\"\n", takeInfo->mImportName.Buffer());
			FBXSDK_printf("        Import State: %s\n", takeInfo->mSelect ? "true" : "false");
			FBXSDK_printf("\n");
		}

		IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
		IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
		IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
		IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
		IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
		IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
		IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);

	}

	status = importer->Import(pScene);
	if (status == false && importer->GetStatus()== FbxStatus::ePasswordError)
	{
		FBXSDK_printf("please enter password: ");
		password[0] = '\0';

		FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
			scanf("%s", password);
		FBXSDK_CRT_SECURE_NO_WARNING_END

			FbxString string(password);

		IOS_REF.SetStringProp(IMP_FBX_PASSWORD, string);
		IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

		status = importer->Import(pScene);

		if (status == false && importer->GetStatus() == FbxStatus::ePasswordError)
		{
			FBXSDK_printf("\npassword is wrong, import aborted.\n");
		}
	}

	if (!status || (importer->GetStatus() != FbxStatus::eSuccess))
	{
		FBXSDK_printf("********************************************************************************\n");
		if (status)
		{
			FBXSDK_printf("warning:\n");
			FBXSDK_printf("    the importer was able to read the file but with errors.\n");
			FBXSDK_printf("    load scene may be incomplete.\n\n");
		}
		else
		{
			FBXSDK_printf("importer failed to load the file!\n\n");
		}

		if (importer->GetStatus() != FbxStatus::eSuccess)
		{
			FBXSDK_printf("    last error message: %s\n", importer->GetStatus().GetErrorString());
		}

		FbxArray<FbxString *> history;
		importer->GetStatus().GetErrorStringHistory(history);
		if (history.GetCount() > 1)
		{
			FBXSDK_printf("    error history stack:\n");
			for (int i = 0; i < history.GetCount(); i++)
			{
				FBXSDK_printf("        %s\n", history[i]->Buffer());
			}
		}
		FbxArrayDelete<FbxString *>(history);
		FBXSDK_printf("********************************************************************************\n");
	}

	importer->Destroy();

	return status;
}
