// CSDK_ChromaModSample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Razer\ChromaAnimationAPI.h"
#include "HandleInput.h"
#include <conio.h>
#include <iostream>
#include <vector>
#include <filesystem>
#include <stdio.h>

using namespace ChromaSDK;
using namespace std;

class Selections
{
public:
	wstring GetGameAnimationDirectory()
	{
		return L"Animations";
	}
	wstring GetAppDataDirectory()
	{
		wchar_t *pValue;
		size_t len;
		errno_t err = _wdupenv_s(&pValue, &len, L"APPDATA");
		wstring path(pValue);
		free(pValue);
		path += L"\\..\\Local";
		return path;
	}
	wstring GetAppModDirectory()
	{
		wstring path(GetAppDataDirectory());
		path += L"\\Razer\\CSDK_ChromaModSample\\Mods";
		return path;
	}
	void DetectMods()
	{
		// Sample Game Animations and Skins
		for (const auto & entry : std::filesystem::directory_iterator(GetGameAnimationDirectory()))
		{
			if (entry.path().extension().compare(L"") == 0)
			{
				wstring ws(entry.path().filename());
				_mSkins.push_back(ws);
			}
		}

		// Sample mods
		if (filesystem::exists(GetAppModDirectory()))
		{
			for (const auto & entry : std::filesystem::directory_iterator(GetAppModDirectory()))
			{
				if (entry.path().extension().compare(L"") == 0)
				{
					wstring ws(entry.path().filename());
					_mSkins.push_back(ws);
				}
			}
		}
		else
		{
			wstring path = GetAppDataDirectory();

			path += L"\\Razer";
			if (!filesystem::exists(path))
			{
				filesystem::create_directory(path);
			}

			path += L"\\CSDK_ChromaModSample";
			if (!filesystem::exists(path))
			{
				filesystem::create_directory(path);
			}

			path += L"\\Mods";
			if (!filesystem::exists(path))
			{
				filesystem::create_directory(path);
			}
		}
	}
	vector<wstring> GetSkins()
	{
		return _mSkins;
	}
	vector<wstring> GetClasses()
	{
		vector<wstring> list;
		list.push_back(L"Fighter");
		list.push_back(L"Paladin");
		list.push_back(L"Thief");
		return list;
	}
	vector<wstring> GetEffects()
	{
		vector<wstring> list;
		list.push_back(L"Attack");
		list.push_back(L"Block");
		list.push_back(L"Damage");
		return list;
	}
	void StartEffect(const wstring& path)
	{
		// Stop and reset any loaded animations
		ChromaAnimationAPI::StopAll();
		ChromaAnimationAPI::CloseAll();

		// Reduce the number of frames to speed up the animation
		ChromaAnimationAPI::ReduceFramesName((path + L"_ChromaLink.chroma").c_str(), 2);
		ChromaAnimationAPI::ReduceFramesName((path + L"_Headset.chroma").c_str(), 2);
		ChromaAnimationAPI::ReduceFramesName((path + L"_Keyboard.chroma").c_str(), 2);
		ChromaAnimationAPI::ReduceFramesName((path + L"_KeyboardExtended.chroma").c_str(), 2);
		ChromaAnimationAPI::ReduceFramesName((path + L"_Keypad.chroma").c_str(), 2);
		ChromaAnimationAPI::ReduceFramesName((path + L"_Mouse.chroma").c_str(), 2);
		ChromaAnimationAPI::ReduceFramesName((path + L"_Mousepad.chroma").c_str(), 2);

		// Se the animation speed to 30 FPS
		ChromaAnimationAPI::OverrideFrameDurationName((path + L"_ChromaLink.chroma").c_str(), 0.033f);
		ChromaAnimationAPI::OverrideFrameDurationName((path + L"_Headset.chroma").c_str(), 0.033f);
		ChromaAnimationAPI::OverrideFrameDurationName((path + L"_Keyboard.chroma").c_str(), 0.033f);
		ChromaAnimationAPI::OverrideFrameDurationName((path + L"_KeyboardExtended.chroma").c_str(), 0.033f);
		ChromaAnimationAPI::OverrideFrameDurationName((path + L"_Keypad.chroma").c_str(), 0.033f);
		ChromaAnimationAPI::OverrideFrameDurationName((path + L"_Mouse.chroma").c_str(), 0.033f);
		ChromaAnimationAPI::OverrideFrameDurationName((path + L"_Mousepad.chroma").c_str(), 0.033f);

		ChromaAnimationAPI::PlayComposite(path.c_str(), true);
	}
	void PlayEffect()
	{
		wstring path;
		if (_mSkins[_mSkinId].compare(L"Base") == 0)
		{
			path = GetGameAnimationDirectory();
		}
		else
		{
			path = GetAppModDirectory();
		}
		path += L"\\";
		path += _mSkins[_mSkinId];
		path += L"\\";
		path += GetClasses()[_mClassId];
		path += L"\\";
		path += GetEffects()[_mEffectId];
		StartEffect(path.c_str());
	}

	void Cleanup()
	{
		if (ChromaAnimationAPI::GetIsInitializedAPI())
		{
			if (ChromaAnimationAPI::IsInitialized())
			{
				ChromaAnimationAPI::StopAll();
				ChromaAnimationAPI::CloseAll();
				RZRESULT result = ChromaAnimationAPI::Uninit();
				ChromaAnimationAPI::UninitAPI();
				if (result != RZRESULT_SUCCESS)
				{
					cerr << "Failed to uninitialize Chroma!" << endl;
					exit(1);
				}
			}
		}
	}

	void InputHandler(
		bool supportsStreaming,
		BYTE& platform,
		bool& detectedInput,
		HandleInput& inputEscape,
		HandleInput& inputO,
		HandleInput& inputM,
		HandleInput& inputS,
		HandleInput& inputC,
		HandleInput& inputE,
		HandleInput& inputEnter,
		HandleInput& inputUp,
		HandleInput& inputDown,
		HandleInput& inputPlatform)
	{
		if (inputEscape.WasReleased(true))
		{
			detectedInput = true;
			Cleanup();
			exit(0);
			return;
		}

		if (inputPlatform.WasReleased(true))
		{
			detectedInput = true;
			platform = (platform + 1) % 4; //PC, AMAZON LUNA, MS GAME PASS, NVIDIA GFN
		}

		if (inputO.WasReleased(true))
		{
			detectedInput = true;
			
			wstring gameAnimationDirectory = GetGameAnimationDirectory();
			
			wstring cmd;
			// switch to directory
			cmd += L"CD \"";
			cmd += gameAnimationDirectory;
			// open file explorer
			cmd += L"\" && cmd /c start .";
			_wsystem(cmd.c_str());
		}

		if (inputM.WasReleased(true))
		{
			detectedInput = true;

			wstring appModDirectory = GetAppModDirectory();

			wstring cmd;
			// switch to drive, i.e. L"C:"
			cmd += appModDirectory.substr(0, 2);
			// switch to directory
			cmd += L" && CD \"";
			cmd += appModDirectory;
			// open file explorer
			cmd += L"\" && cmd /c start .";
			_wsystem(cmd.c_str());
		}

		if (inputS.WasReleased(true))
		{
			detectedInput = true;
			_mSkinId = (_mSkinId + 1) % GetSkins().size();
		}

		if (inputC.WasReleased(true))
		{
			detectedInput = true;
			_mClassId = (_mClassId + 1) % GetClasses().size();
		}

		if (inputE.WasReleased(true))
		{
			detectedInput = true;
			_mEffectId = (_mEffectId + 1) % GetEffects().size();
		}		

		if (inputUp.WasReleased(true))
		{
			detectedInput = true;
			if (_mSelection > 0)
			{
				--_mSelection;
			}
		}

		if (inputDown.WasReleased(true))
		{
			detectedInput = true;
			if (_mSelection < MAX_SELECTION)
			{
				++_mSelection;
			}
		}

		if (inputEnter.WasReleased(true))
		{
			detectedInput = true;
			if (supportsStreaming)
			{
				switch (_mSelection)
				{
				case 0:
					{
						wstring strPlatform = L"PC";
						switch (platform)
						{
						case 0:
							strPlatform = L"PC";
							break;
						case 1:
							strPlatform = L"LUNA";
							break;
						case 2:
							strPlatform = L"GEFORCE_NOW";
							break;
						case 3:
							strPlatform = L"GAME_PASS";
							break;
						}
						ChromaAnimationAPI::CoreStreamGetAuthShortcode(_mShortcode, &_mLenShortcode, strPlatform.c_str(), L"C++ Chroma Mod Sample 好");
					}
					break;
				case 1:
					ChromaAnimationAPI::CoreStreamGetId(_mShortcode, _mStreamId, &_mLenStreamId);
					break;
				case 2:
					ChromaAnimationAPI::CoreStreamGetKey(_mShortcode, _mStreamKey, &_mLenStreamKey);
					break;
				case 3:
					if (ChromaAnimationAPI::CoreStreamReleaseShortcode(_mShortcode))
					{
						memset(_mShortcode, 0, size(_mShortcode));
						_mLenShortcode = 0;
					}
					break;
				case 4:
					ChromaAnimationAPI::CoreStreamBroadcast(_mStreamId, _mStreamKey);
					break;
				case 5:
					ChromaAnimationAPI::CoreStreamBroadcastEnd();
					break;
				case 6:
					ChromaAnimationAPI::CoreStreamWatch(_mStreamId, 0);
					break;
				case 7:
					ChromaAnimationAPI::CoreStreamWatchEnd();
					break;
				}
			}
		}
	}
	void DisplayList(vector<wstring>& list, int index)
	{
		int i = 0;
		for (vector<wstring>::iterator iter = list.begin(); iter < list.end(); ++iter)
		{
			wcout << L"[";
			if (i == index)
			{
				wcout << L"*";
			}
			else
			{
				wcout << L" ";
			}
			wcout << L"] " << iter->c_str() << endl;
			++i;
		}
	}
	const char* IsSelected(int index)
	{
		return (index == _mSelection) ? "*" : " ";
	}
	void PrintLegend(bool supportsStreaming, BYTE platform)
	{
		_wsystem(L"CLS");
		wcout << L"Welcome to the C++ Chroma Mod Sample" << endl;
		if (supportsStreaming)
		{
			fwprintf(stdout, L"Use [P] to switch streaming platforms. ");
		}
		wcout << L"Press [Esc] to Quit" << endl;
		wcout << L"Press [O] to Open the animation directory" << endl;
		wcout << L"Press [M] to Open the mod directory (note: unpack Mod1.zip and Mod2.zip in the mod directory) " << endl;

		vector<wstring> list;

		wcout << L"Press [S] to change Selected Skin:" << endl;
		list = GetSkins();
		DisplayList(list, _mSkinId);		

		wcout << L"Press [C] to change Selected Class:" << endl;
		list = GetClasses();
		DisplayList(list, _mClassId);

		wcout << L"Press [E] to change Chroma Effects:" << endl;
		list = GetEffects();
		DisplayList(list, _mEffectId);

		if (supportsStreaming)
		{
			//Stream API uses char* string
			fprintf(stdout, "Streaming Info (SUPPORTED):\r\n");
			ChromaSDK::Stream::StreamStatusType status = ChromaAnimationAPI::CoreStreamGetStatus();
			fprintf(stdout, "Status: %s\r\n", ChromaAnimationAPI::CoreStreamGetStatusString(status));
			if (_mLenShortcode > 0)
			{
				fprintf(stdout, "Shortcode: %s\r\n", _mShortcode);
			}
			if (_mLenStreamId > 0)
			{
				fprintf(stdout, "StreamId: %s\r\n", _mStreamId);
			}
			if (_mLenStreamKey > 0)
			{
				fprintf(stdout, "StreamKey: %s\r\n", _mStreamKey);
			}
			fprintf(stdout, "\r\n");
		}

		int index = -1;
		if (supportsStreaming)
		{
			fprintf(stdout, "[%s] Request Shortcode for Platform: ", IsSelected(++index));

			switch (platform)
			{
			case 0:
				fprintf(stdout, "Windows PC (PC)\r\n");
				break;
			case 1:
				fprintf(stdout, "Windows Cloud (LUNA)\r\n");
				break;
			case 2:
				fprintf(stdout, "Windows Cloud (GEFORCE NOW)\r\n");
				break;
			case 3:
				fprintf(stdout, "Windows Cloud (GAME PASS)\r\n");
				break;
			}
			fprintf(stdout, "[%s] Request StreamId\r\n", IsSelected(++index));
			fprintf(stdout, "[%s] Request StreamKey\r\n", IsSelected(++index));
			fprintf(stdout, "[%s] Release Shortcode\r\n", IsSelected(++index));
			fprintf(stdout, "[%s] Broadcast\r\n", IsSelected(++index));
			fprintf(stdout, "[%s] BroadcastEnd\r\n", IsSelected(++index));
			fprintf(stdout, "[%s] Watch\r\n", IsSelected(++index));
			fprintf(stdout, "[%s] WatchEnd\r\n", IsSelected(++index));
		}
	}
private:
	vector<wstring> _mSkins;
	int _mSkinId = 0;
	int _mClassId = 0;
	int _mEffectId = 0;
	bool _mShouldQuit = false;
	
	// for streaming
	int _mSelection = 0;
	const int MAX_SELECTION = 7;

	char _mShortcode[7] = { 0 };
	char _mStreamId[48] = { 0 };
	char _mStreamKey[48] = { 0 };
	unsigned char _mLenShortcode = 0;
	unsigned char _mLenStreamId = 0;
	unsigned char _mLenStreamKey = 0;
};

void GameLoop()
{
	bool supportsStreaming = ChromaAnimationAPI::CoreStreamSupportsStreaming();

	BYTE platform = 0;

	Selections selections;
	selections.DetectMods();
	selections.PrintLegend(supportsStreaming, platform);
	selections.PlayEffect();

	// for app
	HandleInput inputEscape = HandleInput(VK_ESCAPE);
	HandleInput inputO = HandleInput('O');
	HandleInput inputM = HandleInput('M');
	HandleInput inputS = HandleInput('S');
	HandleInput inputC = HandleInput('C');
	HandleInput inputE = HandleInput('E');
	HandleInput inputPlatform = HandleInput('P');

	// for streaming
	HandleInput inputEnter = HandleInput(VK_RETURN);
	HandleInput inputUp = HandleInput(VK_UP);
	HandleInput inputDown = HandleInput(VK_DOWN);

	bool detectedInput = true;
	while (true)
	{
		selections.InputHandler(
			supportsStreaming,
			platform,
			detectedInput,
			inputEscape,
			inputO,
			inputM,
			inputS,
			inputC,
			inputE,
			inputEnter,
			inputUp,
			inputDown,
			inputPlatform);
		if (detectedInput)
		{
			selections.PrintLegend(supportsStreaming, platform);
			selections.PlayEffect();
		}
		detectedInput = false;
		Sleep(0);
	}
}

int main()
{
	if (ChromaAnimationAPI::InitAPI() != 0)
	{
		cerr << "Failed to load Chroma library!" << endl;
		exit(1);
	}
	RZRESULT result = ChromaAnimationAPI::Init();
	if (result != RZRESULT_SUCCESS)
	{
		cerr << "Failed to initialize Chroma!" << endl;
		exit(1);
	}
	GameLoop();
	return 0;
}
