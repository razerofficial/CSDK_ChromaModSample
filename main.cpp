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
	string GetGameAnimationDirectory()
	{
		return "Animations";
	}
	string GetAppDataDirectory()
	{
		char *pValue;
		size_t len;
		errno_t err = _dupenv_s(&pValue, &len, "APPDATA");
		string path(pValue);
		free(pValue);
		path += "\\..\\Local";
		return path;
	}
	string GetAppModDirectory()
	{
		string path(GetAppDataDirectory());
		path += "\\Razer\\CSDK_ChromaModSample\\Mods";
		return path;
	}
	void DetectMods()
	{
		// Sample Game Animations and Skins
		for (const auto & entry : std::filesystem::directory_iterator(GetGameAnimationDirectory()))
		{
			if (entry.path().extension().compare("") == 0)
			{
				wstring ws(entry.path().filename());
				string str(ws.begin(), ws.end());
				_mSkins.push_back(str);
			}
		}

		// Sample mods
		if (filesystem::exists(GetAppModDirectory()))
		{
			for (const auto & entry : std::filesystem::directory_iterator(GetAppModDirectory()))
			{
				if (entry.path().extension().compare("") == 0)
				{
					wstring ws(entry.path().filename());
					string str(ws.begin(), ws.end());
					_mSkins.push_back(str);
				}
			}
		}
		else
		{
			string path = GetAppDataDirectory();

			path += "\\Razer";
			if (!filesystem::exists(path))
			{
				filesystem::create_directory(path);
			}

			path += "\\CSDK_ChromaModSample";
			if (!filesystem::exists(path))
			{
				filesystem::create_directory(path);
			}

			path += "\\Mods";
			if (!filesystem::exists(path))
			{
				filesystem::create_directory(path);
			}
		}
	}
	vector<string> GetSkins()
	{
		return _mSkins;
	}
	vector<string> GetClasses()
	{
		vector<string> list;
		list.push_back("Fighter");
		list.push_back("Paladin");
		list.push_back("Thief");
		return list;
	}
	vector<string> GetEffects()
	{
		vector<string> list;
		list.push_back("Attack");
		list.push_back("Block");
		list.push_back("Damage");
		return list;
	}
	void StartEffect(const string& path)
	{
		// Stop and reset any loaded animations
		ChromaAnimationAPI::StopAll();
		ChromaAnimationAPI::CloseAll();

		// Reduce the number of frames to speed up the animation
		ChromaAnimationAPI::ReduceFramesName((path + "_ChromaLink.chroma").c_str(), 2);
		ChromaAnimationAPI::ReduceFramesName((path + "_Headset.chroma").c_str(), 2);
		ChromaAnimationAPI::ReduceFramesName((path + "_Keyboard.chroma").c_str(), 2);
		ChromaAnimationAPI::ReduceFramesName((path + "_Keypad.chroma").c_str(), 2);
		ChromaAnimationAPI::ReduceFramesName((path + "_Mouse.chroma").c_str(), 2);
		ChromaAnimationAPI::ReduceFramesName((path + "_Mousepad.chroma").c_str(), 2);

		// Se the animation speed to 30 FPS
		ChromaAnimationAPI::OverrideFrameDurationName((path + "_ChromaLink.chroma").c_str(), 0.033f);
		ChromaAnimationAPI::OverrideFrameDurationName((path + "_Headset.chroma").c_str(), 0.033f);
		ChromaAnimationAPI::OverrideFrameDurationName((path + "_Keyboard.chroma").c_str(), 0.033f);
		ChromaAnimationAPI::OverrideFrameDurationName((path + "_Keypad.chroma").c_str(), 0.033f);
		ChromaAnimationAPI::OverrideFrameDurationName((path + "_Mouse.chroma").c_str(), 0.033f);
		ChromaAnimationAPI::OverrideFrameDurationName((path + "_Mousepad.chroma").c_str(), 0.033f);

		ChromaAnimationAPI::PlayComposite(path.c_str(), true);
	}
	void PlayEffect()
	{
		string path;
		if (_mSkins[_mSkinId].compare("Base") == 0)
		{
			path = GetGameAnimationDirectory();
		}
		else
		{
			path = GetAppModDirectory();
		}
		path += "\\";
		path += _mSkins[_mSkinId];
		path += "\\";
		path += GetClasses()[_mClassId];
		path += "\\";
		path += GetEffects()[_mEffectId];
		StartEffect(path.c_str());
	}

	void Cleanup()
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
		if (inputEscape.WasReleased())
		{
			detectedInput = true;
			Cleanup();
			exit(0);
			return;
		}

		if (inputPlatform.WasReleased())
		{
			detectedInput = true;
			platform = (platform + 1) % 4; //PC, AMAZON LUNA, MS GAME PASS, NVIDIA GFN
		}

		if (inputO.WasReleased())
		{
			detectedInput = true;
			string cmd = "CD \"";
			cmd += GetGameAnimationDirectory();
			cmd += "\" && cmd /c start .";
			system(cmd.c_str());
		}

		if (inputM.WasReleased())
		{
			string cmd = "CD \"";
			cmd += GetAppModDirectory();
			cmd += "\" && cmd /c start .";
			system(cmd.c_str());
		}

		if (inputS.WasReleased())
		{
			detectedInput = true;
			_mSkinId = (_mSkinId + 1) % GetSkins().size();
		}

		if (inputC.WasReleased())
		{
			detectedInput = true;
			_mClassId = (_mClassId + 1) % GetClasses().size();
		}

		if (inputE.WasReleased())
		{
			detectedInput = true;
			_mEffectId = (_mEffectId + 1) % GetEffects().size();
		}		

		if (inputUp.WasReleased())
		{
			detectedInput = true;
			if (_mSelection > 0)
			{
				--_mSelection;
			}
		}

		if (inputDown.WasReleased())
		{
			detectedInput = true;
			if (_mSelection < MAX_SELECTION)
			{
				++_mSelection;
			}
		}

		if (inputEnter.WasReleased())
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
	void DisplayList(vector<string>& list, int index)
	{
		int i = 0;
		for (vector<string>::iterator iter = list.begin(); iter < list.end(); ++iter)
		{
			cout << "[";
			if (i == index)
			{
				cout << "*";
			}
			else
			{
				cout << " ";
			}
			cout << "] " << iter->c_str() << endl;
			++i;
		}
	}
	const char* IsSelected(int index)
	{
		return (index == _mSelection) ? "*" : " ";
	}
	void PrintLegend(bool supportsStreaming, BYTE platform)
	{
		system("CLS");
		cout << "Welcome to the C++ Chroma Mod Sample" << endl;
		if (supportsStreaming)
		{
			fprintf(stdout, "Use [P] to switch streaming platforms. ");
		}
		cout << "Press [Esc] to Quit" << endl;
		cout << "Press [O] to Open the animation directory" << endl;
		cout << "Press [M] to Open the mod directory (note: unpack Mod1.zip and Mod2.zip in the mod directory) " << endl;

		vector<string> list;

		cout << "Press [S] to change Selected Skin:" << endl;
		list = GetSkins();
		DisplayList(list, _mSkinId);		

		cout << "Press [C] to change Selected Class:" << endl;
		list = GetClasses();
		DisplayList(list, _mClassId);

		cout << "Press [E] to change Chroma Effects:" << endl;
		list = GetEffects();
		DisplayList(list, _mEffectId);

		if (supportsStreaming)
		{
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
	vector<string> _mSkins;
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
