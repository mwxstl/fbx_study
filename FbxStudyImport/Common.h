#pragma once

#include <fbxsdk.h>

void initializeSdkObjects(FbxManager *& pManager, FbxScene *& pScene);
void destroySdkObjects(FbxManager *pManager, bool pExitStatus);

bool loadScene(FbxManager *pManager, FbxScene *pScene, const char *pFilePath);