#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include "../../include/game/Game.h"

#include <codecvt>

PLGame::PLGame(const int &Width, const int &Height, const PString &PlayerName)
	: PWindow(Width, Height, ostr::format("PaperCraft : {}'s world", PlayerName)), _playerName(PlayerName),
	  _assetManager(PGetSingleton<PAssetManager>()) {
	ReadPuns();

	_windowDevice = GetDevice();
	_manager	  = new PGUIManager;

	setbkcolor(RGB(240, 56, 67));

	InitFakeLoadUI();
	InitLoadMenuUI();
}
void PLGame::InitFakeLoadUI() {
	_logo		 = new PImageLabel("./assets/ui/voidjang.png");
	_progressBar = new PProgressBar(getwidth() * 0.8, 20);
	_logo->Resize(getwidth() * 0.4, getheight() * 0.5333);
	_logo->Move(getwidth() / 2 - _logo->GetWidth() / 2, getheight() / 2 - _logo->GetHeight() / 2);
	_progressBar->Move(getwidth() / 2 - _progressBar->GetWidth() / 2, _logo->GetY() + _logo->GetHeight());

	_fakeLoadUI.emplace_back(_logo);
	_fakeLoadUI.emplace_back(_progressBar);

	_manager->AddObject(_logo);
	_manager->AddObject(_progressBar);
}
void PLGame::ReadPuns() {
	std::wifstream file("./assets/texts/splashes.txt");
	file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	std::wstring line;
	while (!file.eof()) {
		std::getline(file, line);
		_punList.emplace_back(line);
	}
}
void PLGame::InitLoadMenuUI() {
	srand(time(nullptr));

	// 0.639 0.338

	auto buttonWidth  = 0.309 * getwidth();
	auto buttonHeight = 0.113 * getheight();
	auto fontSize	  = getwidth() * 0.03125 > 28 ? 28 : getwidth() * 0.03125;
	_playButton		  = new PButton(buttonWidth, buttonHeight, "Play");
	_settingButton	  = new PButton(buttonWidth, buttonHeight, "Setting");
	_quitButton		  = new PButton(buttonWidth, buttonHeight, "Quit");
	_backgroundImage  = new PImageLabel("./assets/ui/menu_background.png");
	_gameLogoImage	  = new PImageLabel("./assets/ui/title.png");
	_backgroundImage->Resize(getwidth(), getheight());

	_playButton->FontStyle.lfHeight	   = fontSize;
	_settingButton->FontStyle.lfHeight = fontSize;
	_quitButton->FontStyle.lfHeight	   = fontSize;

	_playButton->Move(getwidth() / 2 - _playButton->GetWidth() / 2, getheight() * 0.423);
	_settingButton->Move(_playButton->GetX(), _playButton->GetY() + _playButton->GetHeight() + 20);
	_quitButton->Move(_settingButton->GetX(), _settingButton->GetY() + _settingButton->GetHeight() + 20);
	_quitButton->OnClick.Connect([]() { exit(0); });

	_gameLogoImage->Resize(getwidth() * 0.569, getheight() * 0.1224);
	_gameLogoImage->Move(getwidth() * 0.234, getheight() * 0.208);

	_menuUI.emplace_back(_backgroundImage);
	_menuUI.emplace_back(_playButton);
	_menuUI.emplace_back(_settingButton);
	_menuUI.emplace_back(_quitButton);
	_menuUI.emplace_back(_gameLogoImage);

	for (auto &object : _menuUI) {
		_manager->AddObject(object);
		object->Hide();
	}
}
float PLGame::SmoothInterpolation(const float &X) const {
	return pow(X, 2);
}
void PLGame::Loop() {
	bool musicLoadDone = false;
	std::thread([&musicLoadDone]() {
		PGetSingleton<PAssetManager>().LoadSounds();
		musicLoadDone = true;
	}).detach();
	float progress = 0.0;
	while (_progressBar->GetPercentage() < 100) {
		ExMessage message;
		while (peekmessage(&message)) {
			_manager->OnMessage(message);
		}

		cleardevice();

		_progressBar->SetPercentage(SmoothInterpolation(progress));

		_manager->OnDraw(_windowDevice);

		_windowDevice->Flush();

		// We are actually loading resource of music :)
		if ((progress >= 70 && musicLoadDone) || progress < 70) {
			progress += 0.2;
		}

		Sleep(16);
	}
	for (auto &object : _fakeLoadUI) {
		object->Hide();
	}
	for (auto &object : _menuUI) {
		object->Show();
	}

#ifdef _DEBUG
	LOGFONT debugFont;
	gettextstyle(&debugFont);
	_tcscpy_s(debugFont.lfFaceName, _T("Minecraft"));
	debugFont.lfHeight = 16;
#endif

	LOGFONT titleFont;
	gettextstyle(&titleFont);
	_tcscpy_s(titleFont.lfFaceName, _T("Minecraft"));
	titleFont.lfHeight		= getwidth() * 0.0375;
	titleFont.lfEscapement	= 209;
	titleFont.lfOrientation = 209;

	int	 punX			 = getwidth() * 0.639;
	int	 punY			 = getheight() * 0.338;
	auto pun			 = _punList[rand() % _punList.size()].c_str();
	auto titileAnimation = 0.1f;
	while (true) {
		titleFont.lfEscapement = 140 * cos(titileAnimation) + 209;

		ExMessage message;
		while (peekmessage(&message)) {
			_manager->OnMessage(message);
		}

		cleardevice();

		_manager->OnDraw(_windowDevice);

#ifdef _DEBUG
		settextstyle(&debugFont);
		settextcolor(BLACK);
		outtextxy(getwidth() / 2 - textwidth(_T("PAPERCRAFT DEBUG MODE")) / 2 + 1, 1, _T("PAPERCRAFT DEBUG MODE"));
		settextcolor(WHITE);
		outtextxy(getwidth() / 2 - textwidth(_T("PAPERCRAFT DEBUG MODE")) / 2, 0, _T("PAPERCRAFT DEBUG MODE"));
#endif

		settextstyle(&titleFont);

		settextcolor(RGB(34, 34, 34));
		outtextxy(punX + 2, punY + 2, pun);

		settextcolor(YELLOW);
		outtextxy(punX, punY, pun);

		_windowDevice->Flush();

		titileAnimation += 0.02f;

		Sleep(1);
	}
}
