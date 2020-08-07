#pragma once

#include <fbxsdk.h>

void displayMetaDataConnections(FbxObject *pObject);
void displayString(const char *pHeader, const char *pValue = "", const char *pSuffix = "");
void displayBool(const char *pHeader, bool pValue, const char *pSuffix = "");
void displayInt(const char *pHeader, int pValue, const char *pSuffix = "");
void displayDouble(const char *pHeader, double pValue, const char *pSuffix = "");
void display2DVector(const char *pHeader, FbxVector2 pValue, const char *pSuffix = "");
void display3DVector(const char *pheader, FbxVector4 pValue, const char *pSuffix = "");
void displayColor(const char *pHeader, FbxColor pValue, const char *pSuffix = "");
void display4DVector(const char *pHeader, FbxVector4 pValue, const char *pSuffix = "");