#include <iostream>
#include <windows.h>
#include <mmdeviceapi.h>	//IMMDeviceEnumerator
#include <combaseapi.h>		//CoCreateInstance
#include <comdef.h>			//_COM_SMARTPTR_TYPEDEF
#include <audiopolicy.h>	//IAudioSessionManager2
#include <vector>

#include <cassert>

std::vector<LPWSTR> EnumerateApps() {
	const CLSID			 CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID			 IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	const IID			 IID_IAudioSessionControl2 = __uuidof(IAudioSessionControl2);
	IMMDeviceEnumerator* deviceEnumerator = nullptr;
	HRESULT				 hr;

	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&deviceEnumerator);
	IMMDevice* outputDevice;
	EDataFlow  dataFlow = eRender;
	ERole	   role = eMultimedia;

	deviceEnumerator->GetDefaultAudioEndpoint(dataFlow, role, &outputDevice);

	_COM_SMARTPTR_TYPEDEF(IAudioSessionManager2, __uuidof(IAudioSessionManager2));
	IAudioSessionManager2Ptr manager;

	hr = outputDevice->Activate(__uuidof(IAudioSessionManager2), 0, NULL, reinterpret_cast<void**>(&manager));
	assert(SUCCEEDED(hr));

	_COM_SMARTPTR_TYPEDEF(IAudioSessionEnumerator, __uuidof(IAudioSessionEnumerator));
	IAudioSessionEnumeratorPtr seshEnum;

	hr = manager->GetSessionEnumerator(&seshEnum);
	assert(SUCCEEDED(hr));

	int count = -1;
	hr = seshEnum->GetCount(&count);
	assert(SUCCEEDED(hr));

	std::cout << "Total number of audio sessions currently open is: " << count << std::endl;

	_COM_SMARTPTR_TYPEDEF(IAudioSessionControl, __uuidof(IAudioSessionControl));
	_COM_SMARTPTR_TYPEDEF(IAudioSessionControl2, __uuidof(IAudioSessionControl2));
	std::vector<LPWSTR> arr(count);

	for (int i = 0; i < count; i++) {
		IAudioSessionControlPtr seshControl;
		seshEnum->GetSession(i, &seshControl);

		//IAudioSessionControl2Ptr seshControl2;
		//hr = seshControl->QueryInterface(IID_IAudioSessionControl2, reinterpret_cast<void**>(&seshControl2));
		//assert(SUCCEEDED(hr));

		LPWSTR dn;
		hr = seshControl->GetDisplayName(&dn);
		arr[i] = dn;
		//Marshal.ReleaseComObject(seshControl);
	}
	//Marshal.ReleaseComObject(seshEnum);
	//Marshal.ReleaseComObject(manager);
	//Marshal.ReleaseComObject(outputDevice);
	//Marshal.ReleaseComObject(deviceEnumerator);

	return arr;
}

//	_COM_SMARTPTR_TYPEDEF(IAudioSessionControl, __uuidof(IAudioSessionControl));
//ISimpleAudioVolume volumeControl = NULL;
//for (int i = 0; i < count; i++) {
//	IAudioSessionControlPtr ctrl;
//	seshEnum->GetSession(i, &ctrl);
//	LPWSTR* dn;
//	ctrl->GetDisplayName(dn);
//	if (string.Compare(name, dn, StringComparison.OrdinalIgnoreCase) == 0) {
//		volumeControl = ctl as ISimpleAudioVolume;
//		break;
//	}
//	Marshal.ReleaseComObject(ctl);
//}
//Marshal.ReleaseComObject(sessionEnumerator);
//Marshal.ReleaseComObject(mgr);
//Marshal.ReleaseComObject(speakers);
//Marshal.ReleaseComObject(deviceEnumerator);
//return volumeControl;

int main(void) {
	std::vector<LPWSTR> apps = EnumerateApps();

	for (int i = 0; i < apps.size(); ++i) {
		std::wcout << L"name: " << apps[i] << std::endl;
		//if (name == app) {
		//	// display mute state & volume level (% of master)
		//	Console.WriteLine("Mute:" + GetApplicationMute(app));
		//	Console.WriteLine("Volume:" + GetApplicationVolume(app));

		//	// mute the application
		//	SetApplicationMute(app, true);

		//	// set the volume to half of master volume (50%)
		//	SetApplicationVolume(app, 50);
		//}
	}

	return 0;
}
