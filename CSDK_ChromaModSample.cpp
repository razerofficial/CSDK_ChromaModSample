// CSDK_ChromaModSample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <conio.h>
#include <iostream>
#include <vector>
#include <filesystem>
#include <stdio.h>
#include "Razer\ChromaAnimationAPI.h"

using namespace ChromaSDK;
using namespace std;

class Selections
{
public:
	string GetGameAnimationDirectory()
	{
		return "Animations";
	}
	string GetAppModDirectory()
	{
		char *pValue;
		size_t len;
		errno_t err = _dupenv_s(&pValue, &len, "APPDATA");
		string path(pValue);
		free(pValue);
		path += "\\..\\Local\\Razer\\CSDK_ChromaModSample";
		return path;
	}
	void DetectMods()
	{
		// Sample Game Animations and Skins
		for (const auto & entry : std::filesystem::directory_iterator(GetGameAnimationDirectory()))
		{
			wstring ws(entry.path().filename());
			string str(ws.begin(), ws.end());
			_mSkins.push_back(str);
		}

		// Sample mods
		if (filesystem::exists(GetAppModDirectory()))
		{
			for (const auto & entry : std::filesystem::directory_iterator(GetAppModDirectory()))
			{
				wstring ws(entry.path().filename());
				string str(ws.begin(), ws.end());
				_mSkins.push_back(str);
			}
		}
		else
		{
			filesystem::create_directory(GetAppModDirectory());
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
	void HandleInput(int input)
	{
		switch (input)
		{
		case 'Q':
		case 'q':
			ChromaAnimationAPI::StopAll();
			ChromaAnimationAPI::CloseAll();
			ChromaAnimationAPI::Uninit();
			exit(0);
			return;
		case 'O':
		case 'o':
			{
				string cmd = "CD \"";
				cmd += GetGameAnimationDirectory();
				cmd += "\" && cmd /c start .";
				system(cmd.c_str());
			}
			break;
		case 'M':
		case 'm':
			{
				string cmd = "CD \"";
				cmd += GetAppModDirectory();
				cmd += "\" && cmd /c start .";
				system(cmd.c_str());
			}
			break;
		case 'S':
		case 's':
			_mSkinId = (_mSkinId + 1) % GetSkins().size();
			break;
		case 'C':
		case 'c':
			_mClassId = (_mClassId + 1) % GetClasses().size();
			break;
		case 'E':
		case 'e':
			_mEffectId = (_mEffectId + 1) % GetEffects().size();
			break;
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
		cout << endl;
	}
	void Print()
	{
		system("CLS");
		cout << "Welcome to the C++ Chroma Mod Sample" << endl << endl;
		cout << "Press [Q] to Quit" << endl << endl;
		cout << "Press [O] to Open the app directory" << endl << endl;
		cout << "Press [M] to Open the mod directory" << endl << endl;

		vector<string> list;

		cout << "[S] Selected Skin:" << endl;
		list = GetSkins();
		DisplayList(list, _mSkinId);		

		cout << "[C] Classes:" << endl;
		list = GetClasses();
		DisplayList(list, _mClassId);

		cout << "[E] Chroma Effects:" << endl;
		list = GetEffects();
		DisplayList(list, _mEffectId);

		cout << "Press [Q], [S], [C], [E]" << endl << endl;
	}
private:
	vector<string> _mSkins;
	int _mSkinId = 0;
	int _mClassId = 0;
	int _mEffectId = 0;
	bool _mShouldQuit = false;
};

void GameLoop()
{
	Selections selections;
	selections.DetectMods();
	selections.Print();
	selections.PlayEffect();
	while (true)
	{
		int input = _getch();
		selections.HandleInput(input);
		selections.Print();
		selections.PlayEffect();
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
