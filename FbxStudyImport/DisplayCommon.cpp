#include <fbxsdk.h>
#include "DisplayCommon.h"

void displayMetaDataConnections(FbxObject* pObject)
{
	int nbMetaData = pObject->GetSrcObjectCount<FbxObjectMetaData>();
	if (nbMetaData > 0)
	{
		displayString("    metadata connections ");
	}

	for (int i = 0; i < nbMetaData; i++)
	{
		FbxObjectMetaData *metaData = pObject->GetSrcObject<FbxObjectMetaData>(i);
		displayString("        name: ", (char *)metaData->GetName());
	}
}


void displayInt(const char* pHeader, int pValue, const char* pSuffix)
{
	FbxString str;
	str = pHeader;
	str += pValue;
	str += pSuffix;
	str += "\n";
	FBXSDK_printf(str);
}

void displayBool(const char* pHeader, bool pValue, const char* pSuffix)
{
	FbxString str;
	str = pHeader;
	str += pValue ? "true" : "false";
	str += pSuffix;
	str += "\n";
	FBXSDK_printf(str);
}

void displayDouble(const char* pHeader, double pValue, const char* pSuffix)
{
	FbxString str;
	FbxString floatValue = (float)pValue;

	floatValue = pValue <= -HUGE_VAL ? "-INFINITY" : floatValue.Buffer();
	floatValue = pValue >= HUGE_VAL ? "INFINITY" : floatValue.Buffer();

	str = pHeader;
	str += floatValue;
	str += pSuffix;
	str += "\n";
	FBXSDK_printf(str);
}

void displayString(const char* pHeader, const char* pValue, const char* pSuffix)
{
	FbxString str;
	str = pHeader;
	str += pValue;
	str += pSuffix;
	str += "\n";
	FBXSDK_printf(str);
}

void displayColor(const char* pHeader, FbxColor pValue, const char* pSuffix)
{
	FbxString str;

	str = pHeader;
	str += (float)pValue.mRed;
	str += " r, ";

	str += (float)pValue.mGreen;
	str += " g, ";

	str += (float)pValue.mBlue;
	str += "b, ";

	str += pSuffix;
	str += "\n";
	FBXSDK_printf(str);
}

void display2DVector(const char* pHeader, FbxVector2 pValue, const char* pSuffix)
{
	FbxString str;
	FbxString floatValue1 = (float)pValue[0];
	FbxString floatValue2 = (float)pValue[1];

	floatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : floatValue1.Buffer();
	floatValue1 = pValue[0] >= HUGE_VAL ? "INFINITY" : floatValue1.Buffer();
	floatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : floatValue2.Buffer();
	floatValue2 = pValue[1] >= HUGE_VAL ? "INFINITY" : floatValue2.Buffer();

	str = pHeader;
	str += floatValue1;
	str += ", ";
	str += floatValue2;
	str += pSuffix;
	str += "\n";
	FBXSDK_printf(str);
}

void display3DVector(const char* pHeader, FbxVector4 pValue, const char* pSuffix)
{
	FbxString str;
	FbxString floatValue1 = (float)pValue[0];
	FbxString floatValue2 = (float)pValue[1];
	FbxString floatValue3 = (float)pValue[2];

	floatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : floatValue1.Buffer();
	floatValue1 = pValue[0] >= HUGE_VAL ? "INFINITY" : floatValue1.Buffer();
	floatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : floatValue2.Buffer();
	floatValue2 = pValue[1] >= HUGE_VAL ? "INFINITY" : floatValue2.Buffer();
	floatValue3 = pValue[2] <= -HUGE_VAL ? "-INFINITY" : floatValue3.Buffer();
	floatValue3 = pValue[2] >= HUGE_VAL ? "INFINITY" : floatValue3.Buffer();

	str = pHeader;
	str += floatValue1;
	str += ", ";
	str += floatValue2;
	str += ", ";
	str += floatValue3;
	str += pSuffix;
	str += "\n";
	FBXSDK_printf(str);
}

void display4DVector(const char* pHeader, FbxVector4 pValue, const char* pSuffix)
{
	FbxString str;
	FbxString floatValue1 = (float)pValue[0];
	FbxString floatValue2 = (float)pValue[1];
	FbxString floatValue3 = (float)pValue[2];
	FbxString floatValue4 = (float)pValue[3];

	floatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : floatValue1.Buffer();
	floatValue1 = pValue[0] >= HUGE_VAL ? "INFINITY" : floatValue1.Buffer();
	floatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : floatValue2.Buffer();
	floatValue2 = pValue[1] >= HUGE_VAL ? "INFINITY" : floatValue2.Buffer();
	floatValue3 = pValue[2] <= -HUGE_VAL ? "-INFINITY" : floatValue3.Buffer();
	floatValue3 = pValue[2] >= HUGE_VAL ? "INFINITY" : floatValue3.Buffer();
	floatValue4 = pValue[3] <= -HUGE_VAL ? "-INFINITY" : floatValue4.Buffer();
	floatValue4 = pValue[3] >= HUGE_VAL ? "INFINITY" : floatValue4.Buffer();

	str = pHeader;
	str += floatValue1;
	str += ", ";
	str += floatValue2;
	str += ", ";
	str += floatValue3;
	str += ", ";
	str += floatValue4;
	str += pSuffix;
	str += "\n";
	FBXSDK_printf(str);
}
