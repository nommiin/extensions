#include <Windows.h>
#include "YYRunnerInterface.h"
#include "YYRValue.h"

#define GMEXPORT __declspec(dllexport)
#define GMFUNC(name) GMEXPORT void name(RValue& Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg)

YYRunnerInterface gs_runnerInterface;
YYRunnerInterface* g_pYYRunnerInterface;
GMEXPORT void YYExtensionInitialise(const struct YYRunnerInterface* _pFunctions, size_t _functions_size) {
	memcpy(&gs_runnerInterface, _pFunctions, sizeof(YYRunnerInterface));
	g_pYYRunnerInterface = &gs_runnerInterface;
	if (_functions_size < sizeof(YYRunnerInterface)) YYError("Runner interface size mismatch");
	DebugConsoleOutput("Successfully initialized runner interface\n");
	return;
}

HANDLE handles[32];
int handle_index = 0;
GMFUNC(directory_watch_create) {
	// directory_watch(path, callback, interval=0) -> {}
	HANDLE notification = FindFirstChangeNotificationA((LPCSTR)YYGetString(arg, 0), (BOOL)YYGetBool(arg, 1), (DWORD)YYGetReal(arg, 2));
	if (notification == INVALID_HANDLE_VALUE) {
		return YYError("Could not call directory_watch_create, FindFirstChangeNotification returned an invalid pointer");
	}

	handles[handle_index] = notification;
	Result.kind = VALUE_REAL;
	Result.val = handle_index++;
}

GMFUNC(directory_watch_update) {
	int index = YYGetReal(arg, 0);
	if (index < 0 || index > handle_index) {
		return YYError("Could not call directory_watch_update, invalid index was provided as first argument");
	}
	Result.kind = VALUE_BOOL;

	HANDLE notification = handles[index];
	DWORD status = WaitForSingleObject(handles[index], 0);
	FindNextChangeNotification(notification);
	switch (status) {
		case WAIT_OBJECT_0:
		{
			Result.val = TRUE;
			return;
		}
	}
	Result.val = FALSE;
	return;
}

GMFUNC(directory_watch_destroy) {
	int index = YYGetReal(arg, 0);
	if (index < 0 || index > handle_index) {
		return YYError("Could not call directory_watch_destroy, invalid index was provided as first argument");
	}

	Result.kind = VALUE_BOOL;
	Result.val = FindCloseChangeNotification(handles[index]);
	handles[index] = NULL;
}