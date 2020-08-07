#include <fbxsdk.h>
#include "DisplayCommon.h"
void displayGlobalLightSettings(FbxGlobalSettings* pGlobalSettings)
{
	displayColor("ambient color: ", pGlobalSettings->GetAmbientColor());
	displayString("");
}

void displayGlobalCameraSettings(FbxGlobalSettings* pGlobalSettings)
{
	displayString(("default camera: ", pGlobalSettings->GetDefaultCamera()));
	displayString("");
}

void displayGlobalTimeSettings(FbxGlobalSettings* pGlobalSettings)
{
	char timeStr[256];

	displayString("time mode: ", FbxGetTimeModeName(pGlobalSettings->GetTimeMode()));

	FbxTimeSpan timeSpan;
	FbxTime start, end;
	pGlobalSettings->GetTimelineDefaultTimeSpan(timeSpan);
	start = timeSpan.GetStart();
	end = timeSpan.GetStop();

	displayString("timeline default timespan: ");
	displayString("    start: ", start.GetTimeString(timeStr, FbxUShort(256)));
	displayString("    stop: ", end.GetTimeString(timeStr, FbxUShort(256)));

	displayString("");
}
