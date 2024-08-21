/**
 * \file Launcher.cpp
 * \brief The launcher of paper craft
 */

#include <include/launcher/Launcher.h>

PLanuncher::PLanuncher() : PWindow(400, 600, "PaperCraft Launcher"), _assetManager(PGetSingleton<PAssetManager>()) {
	_running = true;

	_gameWindowWidth  = 640;
	_gameWindowHeight = 480;

	_configFile = new mINI::INIFile("./assets/config/launcher.ini");
	_configFile->read(_config);

	InitControl();
}
void PLanuncher::InitControl() {
	_manager = new PGUIManager();

	_gameWindowWidth  = atoi(_config["launcher"]["width"].c_str());
	_gameWindowHeight = atoi(_config["launcher"]["height"].c_str());

	_background							= new PImageLabel("./assets/ui/launcher/startup_background.png");
	_gameScreenText						= new PTextLabel("Game Screen Size");
	_gameScreenText->TextStyle.setHeight(14);
	_gameScreenText->ResizeAsText();
	_gameScreenText->Move(32, 219);

	_gameScreenButton					  = new PButton(342, 35, "");
	_gameScreenButton->TextStyle.setHeight(18);
	_gameScreenButton->SetText(ostr::format("{}x{}", _gameWindowWidth, _gameWindowHeight));
	_gameScreenButton->Move(32, 242);
	_gameScreenButton->OnClick.Connect(this, &PLanuncher::OnGameScreenButtonClicked);

	_playerNameText						= new PTextLabel("Player Name");
	_playerNameText->TextStyle.setHeight(14);
	_playerNameText->ResizeAsText();
	_playerNameText->Move(32, 300);

	_playerNameInput					 = new PInput(342, 35, "Input your game player name");
	_playerNameInput->TextStyle.setHeight(18);
	_playerNameInput->SetMaxLength(12);
	_playerNameInput->Move(32, 324);
	_playerNameInput->SetText(_config["launcher"]["name"].c_str());

	_playerNameInput->OnFinish.Connect(this, &PLanuncher::OnNameChanged);

	_launchButton					  = new PStressButton(227, 55, "LAUNCH THE GAME");
	_launchButton->TextStyle.setHeight(24);
	_launchButton->Move(91, 431);
	_launchButton->OnClick.Connect(this, &PLanuncher::OnLaunch);

	_windowSizeBackground = new PImageLabel("./assets/ui/launcher/size_background.png");
	_sizeTitleText		  = new PTextLabel("Select Your Size");

	_i4096x3112 = new PButton(351, 30, "4096x3112");
	_i3656x2664 = new PButton(351, 30, "3656x2664");
	_i2560x1440 = new PButton(351, 30, "2560x1440");
	_i1920x1080 = new PButton(351, 30, "1920x1080");
	_i1280x960	= new PButton(351, 30, "1280x960");
	_i640x480	= new PButton(351, 30, "640x480");

	_i4096x3112->Move(GetWidth() / 2 - _i4096x3112->GetWidth() / 2, 240);
	_i3656x2664->Move(_i4096x3112->GetX(), _i4096x3112->GetY() + _i4096x3112->GetHeight() + 20);
	_i2560x1440->Move(_i3656x2664->GetX(), _i3656x2664->GetY() + _i3656x2664->GetHeight() + 20);
	_i1920x1080->Move(_i2560x1440->GetX(), _i2560x1440->GetY() + _i2560x1440->GetHeight() + 20);
	_i1280x960->Move(_i1920x1080->GetX(), _i1920x1080->GetY() + _i1920x1080->GetHeight() + 20);
	_i640x480->Move(_i1280x960->GetX(), _i1280x960->GetY() + _i1280x960->GetHeight() + 20);

	_i4096x3112->OnClick.Connect(this, &PLanuncher::SetI4096x3112);
	_i3656x2664->OnClick.Connect(this, &PLanuncher::SetI3656x2664);
	_i2560x1440->OnClick.Connect(this, &PLanuncher::SetI2560x1440);
	_i1920x1080->OnClick.Connect(this, &PLanuncher::SetI1920x1080);
	_i1280x960->OnClick.Connect(this, &PLanuncher::SetI1280x960);
	_i640x480->OnClick.Connect(this, &PLanuncher::SetI640x480);

	_i4096x3112->Hide();
	_i3656x2664->Hide();
	_i2560x1440->Hide();
	_i1920x1080->Hide();
	_i1280x960->Hide();
	_i640x480->Hide();

	_sizeTitleText->Move(GetWidth() / 2 - _sizeTitleText->GetWidth() / 2, 218);

	_windowSizeBackground->Hide();
	_sizeTitleText->Hide();

	_mainUI.emplace_back(_background);
	_mainUI.emplace_back(_gameScreenText);
	_mainUI.emplace_back(_gameScreenButton);
	_mainUI.emplace_back(_playerNameText);
	_mainUI.emplace_back(_playerNameInput);
	_mainUI.emplace_back(_launchButton);

	_sizeUI.emplace_back(_windowSizeBackground);
	_sizeUI.emplace_back(_sizeTitleText);
	_sizeUI.emplace_back(_i4096x3112);
	_sizeUI.emplace_back(_i3656x2664);
	_sizeUI.emplace_back(_i2560x1440);
	_sizeUI.emplace_back(_i1920x1080);
	_sizeUI.emplace_back(_i1280x960);
	_sizeUI.emplace_back(_i640x480);

	_manager->AddObject(_background);
	_manager->AddObject(_gameScreenText);
	_manager->AddObject(_gameScreenButton);
	_manager->AddObject(_playerNameText);
	_manager->AddObject(_playerNameInput);
	_manager->AddObject(_launchButton);
	_manager->AddObject(_windowSizeBackground);
	_manager->AddObject(_sizeTitleText);
	_manager->AddObject(_i4096x3112);
	_manager->AddObject(_i3656x2664);
	_manager->AddObject(_i2560x1440);
	_manager->AddObject(_i1920x1080);
	_manager->AddObject(_i1280x960);
	_manager->AddObject(_i640x480);
}
void PLanuncher::SetI4096x3112() {
	_gameWindowWidth  = 4096;
	_gameWindowHeight = 3112;

	_config["launcher"]["width"]  = std::to_string(_gameWindowWidth);
	_config["launcher"]["height"] = std::to_string(_gameWindowHeight);
	_configFile->write(_config);

	for (auto &object : _sizeUI) {
		object->Hide();
	}
	for (auto &object : _mainUI) {
		object->Show();
	}

	_gameScreenButton->SetText(ostr::format("{}x{}", _gameWindowWidth, _gameWindowHeight));
}
void PLanuncher::SetI3656x2664() {
	_gameWindowWidth  = 3656;
	_gameWindowHeight = 2664;

	_config["launcher"]["width"]  = std::to_string(_gameWindowWidth);
	_config["launcher"]["height"] = std::to_string(_gameWindowHeight);
	_configFile->write(_config);

	_manager->UnfocusObject(nullptr);

	for (auto &object : _sizeUI) {
		object->Hide();
	}
	for (auto &object : _mainUI) {
		object->Show();
	}

	_gameScreenButton->SetText(ostr::format("{}x{}", _gameWindowWidth, _gameWindowHeight));
}
void PLanuncher::SetI2560x1440() {
	_gameWindowWidth  = 2560;
	_gameWindowHeight = 1440;

	_config["launcher"]["width"]  = std::to_string(_gameWindowWidth);
	_config["launcher"]["height"] = std::to_string(_gameWindowHeight);
	_configFile->write(_config);

	_manager->UnfocusObject(nullptr);

	for (auto &object : _sizeUI) {
		object->Hide();
	}
	for (auto &object : _mainUI) {
		object->Show();
	}

	_gameScreenButton->SetText(ostr::format("{}x{}", _gameWindowWidth, _gameWindowHeight));
}
void PLanuncher::SetI1920x1080() {
	_gameWindowWidth  = 1920;
	_gameWindowHeight = 1080;

	_config["launcher"]["width"]  = std::to_string(_gameWindowWidth);
	_config["launcher"]["height"] = std::to_string(_gameWindowHeight);
	_configFile->write(_config);

	_manager->UnfocusObject(nullptr);

	for (auto &object : _sizeUI) {
		object->Hide();
	}
	for (auto &object : _mainUI) {
		object->Show();
	}

	_gameScreenButton->SetText(ostr::format("{}x{}", _gameWindowWidth, _gameWindowHeight));
}
void PLanuncher::SetI1280x960() {
	_gameWindowWidth  = 1280;
	_gameWindowHeight = 960;

	_config["launcher"]["width"]  = std::to_string(_gameWindowWidth);
	_config["launcher"]["height"] = std::to_string(_gameWindowHeight);
	_configFile->write(_config);

	_manager->UnfocusObject(nullptr);

	for (auto &object : _sizeUI) {
		object->Hide();
	}
	for (auto &object : _mainUI) {
		object->Show();
	}

	_gameScreenButton->SetText(ostr::format("{}x{}", _gameWindowWidth, _gameWindowHeight));
}
void PLanuncher::SetI640x480() {
	_gameWindowWidth  = 640;
	_gameWindowHeight = 480;

	_config["launcher"]["width"]  = std::to_string(_gameWindowWidth);
	_config["launcher"]["height"] = std::to_string(_gameWindowHeight);
	_configFile->write(_config);

	_manager->UnfocusObject(nullptr);

	for (auto &object : _sizeUI) {
		object->Hide();
	}
	for (auto &object : _mainUI) {
		object->Show();
	}

	_gameScreenButton->SetText(ostr::format("{}x{}", _gameWindowWidth, _gameWindowHeight));
}
void PLanuncher::OnGameScreenButtonClicked() {
	for (auto &object : _sizeUI) {
		object->Show();
	}
	for (auto &object : _mainUI) {
		object->Hide();
	}
}
void PLanuncher::OnLaunch() {
	_running = false;
}
void PLanuncher::OnNameChanged(PString Name) {
	_config["launcher"]["name"] = Name.c_str();

	_configFile->write(_config);
}
void PLanuncher::Loop() {
	glfwMakeContextCurrent(_glfwWindow);

	sk_sp<VRenderTarget> glRenderTarget =
		sk_make_sp<VRenderTarget, VRenderTargetViewport>({.Width = _width, .Height = _height, .X = 0, .Y = 0});
	sk_sp<VRenderContext> glContext = sk_make_sp<VRenderContext, const sk_sp<VRenderInterface> &>(_glInterface);
	sk_sp<VSurface> glSurface =
		sk_make_sp<VSurface, const sk_sp<VRenderTarget> &, const sk_sp<VRenderContext> &>(glRenderTarget, glContext);
	auto canvas = glSurface->GetNativeSurface()->getCanvas();
	while (_running) {
		if (glfwWindowShouldClose(_glfwWindow)) {
			exit(0);
		}

		glfwPollEvents();

		ExMessage message;
		while (peekmessage(&message)) {
			_manager->OnMessage(message);
		}

		_manager->OnDraw(canvas);

		canvas->flush();

		glContext->GetNativeContext()->flushAndSubmit();

		glfwSwapBuffers(_glfwWindow);

		Sleep(1);
	}
}
