/**
* \file Game.cpp
* \brief The game window of papercraft
*/

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include "../../include/game/Game.h"

#include <codecvt>

PLGame::PLGame(const int &Width, const int &Height, const PString &PlayerName)
	: PWindow(Width, Height, ostr::format("PaperCraft : {}'s world", PlayerName)), _playerName(PlayerName),
	  _assetManager(PGetSingleton<PAssetManager>()) {
	ReadPuns();

	_manager	  = new PGUIManager;

	_backgroundColor = SkColorSetRGB(240, 56, 67);

	InitFakeLoadUI();
	InitLoadMenuUI();
}
void PLGame::InitFakeLoadUI() {
	_logo		 = new PImageLabel("./assets/ui/voidjang.png");
	_progressBar = new PProgressBar(GetWidth() * 0.8, 20);
	_logo->Resize(GetWidth() * 0.4, GetHeight() * 0.5333);
	_logo->Move(GetWidth() / 2 - _logo->GetWidth() / 2, GetHeight() / 2 - _logo->GetHeight() / 2);
	_progressBar->Move(GetWidth() / 2 - _progressBar->GetWidth() / 2, _logo->GetY() + _logo->GetHeight());

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

	auto buttonWidth  = 0.309 * GetWidth();
	auto buttonHeight = 0.113 * GetHeight();
	auto fontSize	  = GetWidth() * 0.03125 > 28 ? 28 : GetWidth() * 0.03125;
	_playButton		  = new PButton(buttonWidth, buttonHeight, "Play");
	_settingButton	  = new PButton(buttonWidth, buttonHeight, "Setting");
	_quitButton		  = new PButton(buttonWidth, buttonHeight, "Quit");
	_backgroundImage  = new PImageLabel("./assets/ui/menu_background.png");
	_gameLogoImage	  = new PImageLabel("./assets/ui/title.png");
	_backgroundImage->Resize(GetWidth(), GetHeight());

	_playButton->TextStyle.setHeight(fontSize);
	_settingButton->TextStyle.setHeight(fontSize);
	_quitButton->TextStyle.setHeight(fontSize);

	_playButton->Move(GetWidth() / 2 - _playButton->GetWidth() / 2, GetHeight() * 0.423);
	_settingButton->Move(_playButton->GetX(), _playButton->GetY() + _playButton->GetHeight() + 20);
	_quitButton->Move(_settingButton->GetX(), _settingButton->GetY() + _settingButton->GetHeight() + 20);
	_quitButton->OnClick.Connect([]() { exit(0); });

	_gameLogoImage->Resize(GetWidth() * 0.569, GetHeight() * 0.1224);
	_gameLogoImage->Move(GetWidth() * 0.234, GetHeight() * 0.208);

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
		glfwPollEvents();

		ExMessage message;
		while (peekmessage(&message)) {
			_manager->OnMessage(message);
		}

		sk_sp<VRenderTarget> glRenderTarget =
			sk_make_sp<VRenderTarget, VRenderTargetViewport>({.Width = _width, .Height = _height, .X = 0, .Y = 0});
		sk_sp<VRenderContext> glContext = sk_make_sp<VRenderContext, const sk_sp<VRenderInterface> &>(_glInterface);
		sk_sp<VSurface> glSurface =
			sk_make_sp<VSurface, const sk_sp<VRenderTarget> &, const sk_sp<VRenderContext> &>(glRenderTarget, glContext);

		auto canvas = glSurface->GetNativeSurface()->getCanvas();
		canvas->drawColor(_backgroundColor);

		_manager->OnDraw(canvas);
		canvas->flush();

		glContext->GetNativeContext()->flushAndSubmit();

		glfwSwapBuffers(_glfwWindow);

		// We are actually loading resource of music :)
		if ((progress >= 70 && musicLoadDone) || progress < 70) {
			progress += 0.2;
		}

		_progressBar->SetPercentage(progress);

		progress += 0.2;

		Sleep(16);
	}
	for (auto &object : _fakeLoadUI) {
		object->Hide();
	}
	for (auto &object : _menuUI) {
		object->Show();
	}

	int	 punX			 = GetWidth() * 0.639;
	int	 punY			 = GetHeight() * 0.338;
	auto pun			 = _punList[rand() % _punList.size()].c_str();
	auto titileAnimation = 0.1f;
	while (true) {
		glfwPollEvents();

		ExMessage message;
		while (peekmessage(&message)) {
			_manager->OnMessage(message);
		}

		sk_sp<VRenderTarget> glRenderTarget =
			sk_make_sp<VRenderTarget, VRenderTargetViewport>({.Width = _width, .Height = _height, .X = 0, .Y = 0});
		sk_sp<VRenderContext> glContext = sk_make_sp<VRenderContext, const sk_sp<VRenderInterface> &>(_glInterface);
		sk_sp<VSurface> glSurface =
			sk_make_sp<VSurface, const sk_sp<VRenderTarget> &, const sk_sp<VRenderContext> &>(glRenderTarget, glContext);

		auto canvas = glSurface->GetNativeSurface()->getCanvas();
		canvas->drawColor(_backgroundColor);

		_manager->OnDraw(canvas);
		canvas->flush();

		glContext->GetNativeContext()->flushAndSubmit();

		glfwSwapBuffers(_glfwWindow);

		titileAnimation += 0.02f;

		Sleep(1);
	}
}
