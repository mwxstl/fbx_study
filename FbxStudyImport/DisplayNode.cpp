#include "fbxsdk.h"
#include "DisplayCommon.h"

void displayMarker(FbxNode *pNode)
{
	FbxMarker *marker = (FbxMarker *)pNode->GetNodeAttribute();
	FbxString str;

	displayString("marker name: ", (char *)pNode->GetName());
	displayMetaDataConnections(marker);

	str = "    marker type: ";
	switch (marker->GetType())
	{
	case FbxMarker::eStandard:
		str += "standard";
		break;
	case FbxMarker::eOptical:
		str += "optical";
		break;
	case FbxMarker::eEffectorIK:
		str += "IK effector";
		break;
	case FbxMarker::eEffectorFK:
		str += "FK effector";
		break;
	default:
		break;
	}

	displayString(str.Buffer());

	str = "    marker look: ";
	switch (marker->Look.Get())
	{
	case FbxMarker::eCube:
		str += "cube";
		break;
	case FbxMarker::eHardCross:
		str += "hard cross";
		break;
	case FbxMarker::eLightCross:
		str += "light cross";
		break;
	case FbxMarker::eSphere:
		str += "sphere";
		break;
	default:
		break;
	}

	displayString(str.Buffer());

	// size
	str = FbxString("    size: ") + FbxString(marker->Size.Get());
	displayString(str.Buffer());

	// color
	FbxDouble3 c = marker->Color.Get();
	FbxColor color(c[0], c[1], c[2]);
	displayColor("    color: ", color);

	// ikPivot
	display3DVector("    IKPivot: ", marker->IKPivot.Get());
}

void displaySkeleton(FbxNode *pNode)
{
	FbxSkeleton *skeleton = (FbxSkeleton *)pNode->GetNodeAttribute();

	displayString("skeleton name: ", (char *)pNode->GetName());
	displayMetaDataConnections(skeleton);

	const char * skeletonTypes[] = { "root", "limb", "limb node", "effector" };

	displayString("    type: ", skeletonTypes[skeleton->GetSkeletonType()]);

	if (skeleton->GetSkeletonType() == FbxSkeleton::eLimb)
	{
		displayDouble("    limb length: ", skeleton->LimbLength.Get());
	}
	else if (skeleton->GetSkeletonType() == FbxSkeleton::eLimbNode)
	{
		displayDouble("    limb node size: ", skeleton->Size.Get());
	}
	else if (skeleton->GetSkeletonType() == FbxSkeleton::eRoot)
	{
		displayDouble("    limb root size: ", skeleton->Size.Get());
	}
	displayColor("    color: ", skeleton->GetLimbNodeColor());
}

void displayControlsPoints(FbxMesh *pMesh)
{
	int controlPointsCount = pMesh->GetControlPointsCount();
	FbxVector4 *controlPoints = pMesh->GetControlPoints();

	displayString("    control points");

	for (int i = 0; i < controlPointsCount; i++)
	{
		displayInt("        control point ", i);
		display3DVector("            coordinates: ", controlPoints[i]);

		for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
		{
			FbxGeometryElementNormal * elementNormal = pMesh->GetElementNormal(j);
			if (elementNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				char header[100];
				FBXSDK_printf(header, 100, "            normal vector: ");
				if (elementNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					display3DVector(header, elementNormal->GetDirectArray().GetAt(i));
				}
			}
		}
	}

	displayString("");
}

void displayPolygons(FbxMesh *pMesh)
{
	int polygonCount = pMesh->GetPolygonCount();
	FbxVector4 *controlPoints = pMesh->GetControlPoints();
	char header[100];

	displayString("    polygons");

	int vertexId = 0;
	for (int i = 0; i < polygonCount; i++)
	{
		displayInt("        polygon ", i);
		for (int j = 0; j < pMesh->GetElementPolygonGroupCount(); j++)
		{
			FbxGeometryElementPolygonGroup *elePolygonGroup = pMesh->GetElementPolygonGroup(j);
			switch (elePolygonGroup->GetMappingMode())
			{
			case FbxLayerElement::eByPolygon:
				if (elePolygonGroup->GetReferenceMode() == FbxGeometryElement::eIndex)
				{
					FBXSDK_printf(header, 100, "        assigned to group: ");
					int polygonGroupId = elePolygonGroup->GetIndexArray().GetAt(i);
					displayInt(header, polygonGroupId);
					break;
				}
			default:
				displayString("        \"unsupported group assignment\"");
				break;
			}
		}

		int polygonSize = pMesh->GetPolygonSize(i);

		for (int j = 0; j < polygonSize; j++)
		{
			int controlPointIndex = pMesh->GetPolygonVertex(i, j);
			if (controlPointIndex < 0)
			{
				displayString("            coordinates: invalid index found!");
				continue;
			}
			else
			{
				display3DVector("            coordinates: ", controlPoints[controlPointIndex]);
			}

			for (int k = 0; k < pMesh->GetElementVertexColorCount(); k++)
			{
				FbxGeometryElementVertexColor *vertexColor = pMesh->GetElementVertexColor(k);
				FBXSDK_sprintf(header, 100, "            color vertex: ");

				switch (vertexColor->GetMappingMode())
				{
				
				case FbxGeometryElement::eByControlPoint:
					switch (vertexColor->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						displayColor(header, vertexColor->GetDirectArray().GetAt(controlPointIndex));
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = vertexColor->GetIndexArray().GetAt(controlPointIndex);
							displayColor(header, vertexColor->GetDirectArray().GetAt(id));
						}
						break;
					default:
						break;
					}
					break;
				case FbxLayerElement::eByPolygonVertex:
					{
						switch (vertexColor->GetReferenceMode())
						{
						case FbxLayerElement::eDirect:
							displayColor(header, vertexColor->GetDirectArray().GetAt(vertexId));
							break;
						case FbxLayerElement::eIndexToDirect:
							{
								int id = vertexColor->GetIndexArray().GetAt(vertexId);
								displayColor(header, vertexColor->GetDirectArray().GetAt(id));
							}
							break;
						default:
							break;
						}
					}
					break;

				case FbxGeometryElement::eByPolygon:
				case FbxGeometryElement::eAllSame:
				case FbxGeometryElement::eNone:
					break;
				default:
					break;
				}
			}

			for (int k = 0; k < pMesh->GetElementUVCount(); k++)
			{
				FbxGeometryElementUV *elementUV = pMesh->GetElementUV(k);
				FBXSDK_sprintf(header, 100, "            texture uv: ");

				switch (elementUV->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
					switch (elementUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						display2DVector(header, elementUV->GetDirectArray().GetAt(controlPointIndex));
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = elementUV->GetIndexArray().GetAt(controlPointIndex);
							display2DVector(header, elementUV->GetDirectArray().GetAt(id));
						}
						break;
					default:
						break;
					}
				case FbxLayerElement::eByPolygonVertex:
					{
						int textureUVIndex = pMesh->GetTextureUVIndex(i, j);
						switch (elementUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
						case FbxGeometryElement::eIndexToDirect:
							{
								display2DVector(header, elementUV->GetDirectArray().GetAt(textureUVIndex));
							}
							break;
						default:
							break;
						}
					}
				case FbxGeometryElement::eByPolygon:
				case FbxGeometryElement::eAllSame:
				case FbxGeometryElement::eNone:
					break;
				default:
					break;
				}
			}

			for (int k = 0; k < pMesh->GetElementNormalCount(); k++)
			{
				FbxGeometryElementNormal *elementNormal = pMesh->GetElementNormal(k);
				FBXSDK_sprintf(header, 100, "            normal: ");

				if (elementNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (elementNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						display3DVector(header, elementNormal->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = elementNormal->GetIndexArray().GetAt(vertexId);
							display3DVector(header, elementNormal->GetDirectArray().GetAt(id));
						}
						break;
					default:
						break;
					}
				}
			}

			for (int k = 0; k < pMesh->GetElementTangentCount(); k++)
			{
				FbxGeometryElementTangent *tangent = pMesh->GetElementTangent();
				FBXSDK_sprintf(header, 100, "            tangent: ");

				if (tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (tangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						display3DVector(header, tangent->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = tangent->GetIndexArray().GetAt(vertexId);
							display3DVector(header, tangent->GetDirectArray().GetAt(id));
						}
						break;
					default:
						break;
					}
				}
			}

			for (int k = 0; k < pMesh->GetElementBinormalCount(); k++)
			{
				FbxGeometryElementBinormal * binormal = pMesh->GetElementBinormal(k);
				FBXSDK_sprintf(header, 100, "            binormal: ");
				if (binormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (binormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						display3DVector(header, binormal->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = binormal->GetIndexArray().GetAt(vertexId);
							display3DVector(header, binormal->GetDirectArray().GetAt(id));
						}
						break;
					default:
						break;
					}
				}
			}
			vertexId++;
		} // for polygonSize
	} // for polygonCount

	for (int i = 0; i < pMesh->GetElementVisibilityCount(); i++)
	{
		FbxGeometryElementVisibility *visibility = pMesh->GetElementVisibility(i);
		FBXSDK_snprintf(header, 100, "    edge visibility: ");
		displayString(header);

		switch (visibility->GetMappingMode())
		{
		case FbxGeometryElement::eByEdge:
			for (int j = 0; j != pMesh->GetMeshEdgeCount(); j++)
			{
				displayInt("        edge ", j);
				displayBool("            edge visibility: ", visibility->GetDirectArray().GetAt(j));
			}
			break;
		default:
			break;
		}
	}

	displayString("");
}

void displayMaterialMapping(FbxMesh* pMesh)
{
	const char *mappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "By Edge", "All Same" };
	const char *referenceMode[] = { "Direct", "Index", "Index to Direct" };

	int materialCount = 0;
	FbxNode *node = NULL;

	if (pMesh)
	{
		node = pMesh->GetNode();
		if (node)
		{
			materialCount = node->GetMaterialCount();
		}
	}

	for (int i = 0; i < pMesh->GetElementMaterialCount(); i++)
	{
		FbxGeometryElementMaterial *material = pMesh->GetElementMaterial(i);
		if (material)
		{
			char header[100];

			FBXSDK_sprintf(header, 100, "    material element %d: ", i);
			displayString(header);

			displayString("        mapping: ", mappingTypes[material->GetMappingMode()]);
			displayString("        referenceMode: ", referenceMode[material->GetReferenceMode()]);

			if (material->GetReferenceMode() == FbxGeometryElement::eIndex || material->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				FbxString str = "        indices: ";

				int indexArrayCount = material->GetIndexArray().GetCount();
				for (int i = 0; i < indexArrayCount; i++)
				{
					str += material->GetIndexArray().GetAt(i);

					if (i < indexArrayCount - 1)
					{
						str += ", ";
					}
				}
				str += "\n";
				FBXSDK_printf(str);
			}
		}
	}

	displayString("");
}

static const FbxImplementation* lookForImplementation(FbxSurfaceMaterial* pMaterial)
{
	const FbxImplementation *implementation = nullptr;
	if (!implementation) implementation = GetImplementation(pMaterial, FBXSDK_IMPLEMENTATION_CGFX);
	if (!implementation) implementation = GetImplementation(pMaterial, FBXSDK_IMPLEMENTATION_HLSL);
	if (!implementation) implementation = GetImplementation(pMaterial, FBXSDK_IMPLEMENTATION_SFX);
	if (!implementation) implementation = GetImplementation(pMaterial, FBXSDK_IMPLEMENTATION_OGS);
	if (!implementation) implementation = GetImplementation(pMaterial, FBXSDK_IMPLEMENTATION_SSSL);
	return implementation;
}

void displayMaterial(FbxGeometry* pGeometry)
{
	FbxNode *node = NULL;
	int materialCount = 0;
	if (pGeometry)
	{
		node = pGeometry->GetNode();
		if (node)
		{
			materialCount = node->GetMaterialCount();
		}
	}

	if (materialCount > 0)
	{

		for (int i = 0; i < materialCount; i++)
		{
			displayInt("        material ", i);

			FbxSurfaceMaterial *material = node->GetMaterial(i);

			displayString("            name: \"", (char *)material->GetName(), "\"");

			const FbxImplementation *implementation = lookForImplementation(material);
			if (implementation)
			{
				FBXSDK_printf("            language: %s\n", implementation->Language.Get().Buffer());
				FBXSDK_printf("            languageVersion: %s\n", implementation->LanguageVersion.Get().Buffer());
				FBXSDK_printf("            renderName: %s\n", implementation->RenderName.Buffer());
				FBXSDK_printf("            renderAPI: %s\n", implementation->RenderAPI.Get().Buffer());
				FBXSDK_printf("            renderAPIVersion: %s\n", implementation->RenderAPIVersion.Get().Buffer());

				const FbxBindingTable *rootTable = implementation->GetRootTable();
				FbxString fileName = rootTable->DescAbsoluteURL.Get();
				FbxString techniqueName = rootTable->DescTAG.Get();

				const FbxBindingTable *table = implementation->GetRootTable();
				size_t entryCount = table->GetEntryCount();

				for (int i = 0; i < (int)entryCount; i++)
				{
					const FbxBindingTableEntry &entry = table->GetEntry(i);
					const char *entrySrcType = entry.GetEntryType(true);
					FbxProperty fbxProp;

					FbxString test = entry.GetSource();
					FBXSDK_printf("            entry: %s\n", test.Buffer());

					if (strcmp(FbxPropertyEntryView::sEntryType, entrySrcType) == 0)
					{
						fbxProp = material->FindPropertyHierarchical(entry.GetSource());
						if (!fbxProp.IsValid())
						{
							fbxProp = material->RootProperty.FindHierarchical(entry.GetSource());
						}
					}
					else if (strcmp(FbxConstantEntryView::sEntryType, entrySrcType) == 0)
					{
						fbxProp = implementation->GetConstants().FindHierarchical(entry.GetSource());
					}
					if (fbxProp.IsValid())
					{
						if (fbxProp.GetSrcObjectCount<FbxTexture>() > 0)
						{
							for (int j = 0; j < fbxProp.GetSrcObjectCount<FbxFileTexture>(); j++)
							{
								FbxFileTexture *texture = fbxProp.GetSrcObject<FbxFileTexture>(j);
								FBXSDK_printf("            file texture: %s\n", texture->GetFileName());
							}

							for (int j = 0; j < fbxProp.GetSrcObjectCount<FbxLayeredTexture>(); j++)
							{
								FbxLayeredTexture *texture = fbxProp.GetSrcObject<FbxLayeredTexture>(j);
								FBXSDK_printf("        layered texture: %s\n", texture->GetName());
							}

							for (int j = 0; j < fbxProp.GetSrcObjectCount<FbxProceduralTexture>(); j++)
							{
								FbxProceduralTexture *texture = fbxProp.GetSrcObject<FbxProceduralTexture>(j);
								FBXSDK_printf("        procedural texture: %s\n", texture->GetName());
							}
						}
						else
						{
							FbxDataType fbxType = fbxProp.GetPropertyDataType();
							if (FbxBoolDT == fbxType)
							{
								displayBool("            bool: ", fbxProp.Get<FbxBool>());
							}
							else if (FbxIntDT == fbxType || FbxEnumDT == fbxType)
							{
								displayInt("            int: ", fbxProp.Get<FbxInt>());
							}
							else if (FbxFloatDT == fbxType)
							{
								displayDouble("            float: ", fbxProp.Get<FbxFloat>());
							}
							else if (FbxDoubleDT == fbxType)
							{
								displayDouble("            double: ", fbxProp.Get<FbxDouble>());
							}
							else if (FbxStringDT == fbxType || FbxUrlDT == fbxType || FbxXRefUrlDT == fbxType)
							{
								displayString("            string: ", fbxProp.Get<FbxString>().Buffer());
							}
							else if (FbxDouble2DT == fbxType)
							{
								FbxDouble2 double2 = fbxProp.Get<FbxDouble2>();
								FbxVector2 vect;
								vect[0] = double2[0];
								vect[1] = double2[1];

								display2DVector("            2d vector: ", vect);
							}
							else if (FbxDouble3DT == fbxType || FbxColor3DT == fbxType)
							{
								FbxDouble3 lDouble3 = fbxProp.Get<FbxDouble3>();


								FbxVector4 lVect;
								lVect[0] = lDouble3[0];
								lVect[1] = lDouble3[1];
								lVect[2] = lDouble3[2];
								display3DVector("                3D vector: ", lVect);
							}

							else if (FbxDouble4DT == fbxType || FbxColor4DT == fbxType)
							{
								FbxDouble4 lDouble4 = fbxProp.Get<FbxDouble4>();
								FbxVector4 lVect;
								lVect[0] = lDouble4[0];
								lVect[1] = lDouble4[1];
								lVect[2] = lDouble4[2];
								lVect[3] = lDouble4[3];
								display4DVector("                4D vector: ", lVect);
							}
							else if (FbxDouble4x4DT == fbxType)
							{
								FbxDouble4x4 lDouble44 = fbxProp.Get<FbxDouble4x4>();
								for (int j = 0; j < 4; ++j)
								{

									FbxVector4 lVect;
									lVect[0] = lDouble44[j][0];
									lVect[1] = lDouble44[j][1];
									lVect[2] = lDouble44[j][2];
									lVect[3] = lDouble44[j][3];
									display4DVector("                4x4D vector: ", lVect);
								}

							}
						}
					}
				} // for entryCount
			}
			else if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				FbxSurfacePhong* phongMaterial = (FbxSurfacePhong*)material;

				FbxColor ambient(phongMaterial->Ambient.Get()[0], phongMaterial->Ambient.Get()[1], phongMaterial->Ambient.Get()[2]);
				displayColor("            ambient: ", ambient);

				FbxColor diffuse(phongMaterial->Diffuse.Get()[0], phongMaterial->Diffuse.Get()[1], phongMaterial->Diffuse.Get()[2]);
				displayColor("            diffuse: ", diffuse);

				FbxColor specular(phongMaterial->Specular.Get()[0], phongMaterial->Specular.Get()[1], phongMaterial->Specular.Get()[2]);
				displayColor("            specular: ", specular);

				FbxColor emissive(phongMaterial->Emissive.Get()[0], phongMaterial->Emissive.Get()[1], phongMaterial->Emissive.Get()[2]);
				displayColor("            emissive: ", specular);

				double opacity = phongMaterial->TransparencyFactor.Get();
				displayDouble("            opacity: ", 1.0 - opacity);

				double shininess = phongMaterial->Shininess.Get();
				displayDouble("            shininess: ", shininess);

				double reflectivity = phongMaterial->ReflectionFactor.Get();
				displayDouble("            reflectivity: ", reflectivity);
			}
			else if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				FbxSurfaceLambert *lambertMaterial = (FbxSurfaceLambert *)material;

				FbxColor ambient(lambertMaterial->Ambient.Get()[0], lambertMaterial->Ambient.Get()[1], lambertMaterial->Ambient.Get()[2]);
				displayColor("            ambient: ", ambient);

				FbxColor diffuse(lambertMaterial->Diffuse.Get()[0], lambertMaterial->Diffuse.Get()[1], lambertMaterial->Diffuse.Get()[2]);
				displayColor("            ", diffuse);

				FbxColor emissive(lambertMaterial->Emissive.Get()[0], lambertMaterial->Emissive.Get()[1], lambertMaterial->Emissive.Get()[2]);
				displayColor("            ", emissive);

				double opacity = lambertMaterial->TransparencyFactor.Get();
				displayDouble("            opacity: ", 1.0 - opacity);
			}
			else
			{
				displayString("unknown type of material");
			}

			FbxString str = material->ShadingModel.Get();
			displayString("            shading model: ", str.Buffer());
			displayString("");
		}
	}
}

void displayTexture(FbxMesh* pMesh)
{
	
}

void displayMaterialConnections(FbxMesh* pMesh)
{
	
}

void displayLink(FbxMesh* pMesh)
{
	
}

void displayShape(FbxMesh* pMesh)
{
	
}

void displayCache(FbxMesh* mesh)
{
	
}

void displayMesh(FbxNode *pNode)
{
	FbxMesh *mesh = (FbxMesh *)pNode->GetNodeAttribute();

	displayString("mesh name: ", (char *)pNode->GetName());
	displayMetaDataConnections(mesh);

	displayControlsPoints(mesh);
	displayPolygons(mesh);
	displayMaterialMapping(mesh);
	displayMaterial(mesh);
	displayTexture(mesh);
	displayMaterialConnections(mesh);
	displayLink(mesh);
	displayShape(mesh);
	displayCache(mesh);
}

void displayNurb(FbxNode *pNode)
{
	
}

void displayPatch(FbxNode *pNode)
{
	
}

void displayCamera(FbxNode *pNode)
{
	
}
void displayLight(FbxNode *pNode)
{
	
}

void displayLodGroup(FbxNode *pNode)
{
	
}

void displayUserProperties(FbxNode *pNode)
{
	
}

void displayTarget(FbxNode *pNode)
{
	
}

void displayPivotsAndLimits(FbxNode *pNode)
{
	
}

void displayGeometricTransform(FbxNode *pNode)
{
	
}