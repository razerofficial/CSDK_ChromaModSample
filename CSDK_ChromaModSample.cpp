// CSDK_ChromaModSample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <conio.h>
#include <iostream>
#include <vector>
#include <filesystem>
#include <stdio.h>

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
	void HandleInput(int input)
	{
		switch (input)
		{
		case 'Q':
		case 'q':
			exit(0);
			return;
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

		cout << "Press [Q], [S], [C], [E]";
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
	while (true)
	{
		int input = _getch();
		selections.HandleInput(input);
		selections.Print();
	}
}

int main()
{
	GameLoop();
	return 0;
}
