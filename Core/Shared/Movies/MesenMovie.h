#pragma once

#include "stdafx.h"
#include "Utilities/VirtualFile.h"
#include "Shared/BatteryManager.h"
#include "Shared/Interfaces/INotificationListener.h"
#include "Shared/Movies/MovieManager.h"

class ZipReader;
class Emulator;
struct CheatCode;

class MesenMovie : public IMovie, public INotificationListener, public IBatteryProvider, public std::enable_shared_from_this<MesenMovie>
{
private:
	Emulator* _emu;

	VirtualFile _movieFile;
	unique_ptr<ZipReader> _reader;
	bool _playing = false;
	size_t _deviceIndex = 0;
	uint32_t _lastPollCounter = 0;
	vector<vector<string>> _inputData;
	vector<string> _cheats;
	vector<CheatCode> _originalCheats;
	std::unordered_map<string, string> _settings;
	string _filename;
	bool _forTest;

private:
	void ParseSettings(stringstream &data);
	void ApplySettings();
	bool LoadGame();
	void Stop();

	uint32_t LoadInt(std::unordered_map<string, string> &settings, string name, uint32_t defaultValue = 0);
	bool LoadBool(std::unordered_map<string, string> &settings, string name);
	string LoadString(std::unordered_map<string, string> &settings, string name);

	void LoadCheats();
	bool LoadCheat(string cheatData, CheatCode &code);

public:
	MesenMovie(Emulator* emu, bool silent);
	virtual ~MesenMovie();

	bool Play(VirtualFile &file) override;
	bool SetInput(BaseControlDevice* device) override;
	bool IsPlaying() override;

	//Inherited via IBatteryProvider
	vector<uint8_t> LoadBattery(string extension) override;

	//Inherited via INotificationListener
	void ProcessNotification(ConsoleNotificationType type, void * parameter) override;
};