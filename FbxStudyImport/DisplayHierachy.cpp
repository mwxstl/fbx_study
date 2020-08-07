#include <fbxsdk.h>

void displayHierarchy(FbxNode *pNode, int pDepth)
{
	FbxString str;

	for (int i = 0; i < pDepth; i++)
	{
		str += "    ";
	}

	str += pNode->GetName();
	str += "\n";

	FBXSDK_printf(str.Buffer());

	for (int i = 0; i < pNode->GetChildCount(); i++)
	{
		displayHierarchy(pNode->GetChild(i), pDepth + 1);
	}
}

void displayHierarchy(FbxScene *pScene)
{
	FbxNode* rootNode = pScene->GetRootNode();

	for (int i = 0; i < rootNode->GetChildCount(); i++)
	{
		displayHierarchy(rootNode->GetChild(i), 0);
	}
}