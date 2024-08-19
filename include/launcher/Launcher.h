/**
 * \file Launcher.h
 * \brief The launcher of paper craft
 */

#pragma once

#include <mini/ini.h>
#include <include/EasyXBase.h>
#include <include/assets/AssetManager.h>
#include <include/gui/Button.h>
#include <include/gui/GUIManager.h>
#include <include/gui/ImageLabel.h>
#include <include/gui/Input.h>
#include <include/gui/TextLabel.h>
#include <include/singleton/Singleton.h>

/**
 * The launcher window
 */
class PLanuncher : public PWindow {
public:
	/**
	 * Create the launcher window
	 */
	PLanuncher();

public:
	/**
	 * Enter the message loop of the launcher
	 */
	void Loop();

private:
	/**
	 * Init the control of the window
	 */
	void InitControl();

private:
	/**
	 * When the player name editor finished the input, this function will be called
	 * @param Name The name of the player
	 */
	void OnNameChanged(PString Name);
	/**
	 * When the _gameScreenButton was clicked, this function will lead user into setting
	 * menu of game window size
	 */
	void OnGameScreenButtonClicked();
	/**
	 * Set window size into 4096x3112
	 */
	void SetI4096x3112();
	/**
	 * Set window size into 3656x2664
	 */
	void SetI3656x2664();
	/**
	 * Set window size into 2560x1440
	 */
	void SetI2560x1440();
	/**
	 * Set window size into 1920x1080
	 */
	void SetI1920x1080();
	/**
	 * Set window size into 1280x960
	 */
	void SetI1280x960();
	/**
	 * Set window size into 640x480
	 */
	void SetI640x480();
	/**
	 * When the game launch, exit the message loop
	 */
	void OnLaunch();

public:
	/**
	 * Get the game screen size in tuple form
	 * @return The game screen size tuple form
	 */
	std::tuple<int, int> GetGameScreenSize() const {
		return {_gameWindowWidth, _gameWindowHeight};
	}
	/**
	 * Get the player name text
	 * @return The text of player name
	 */
	PString GetPlayerName() const {
		return _playerNameInput->GetText();
	}

private:
	bool		   _running;
	int			   _gameWindowWidth;
	int			   _gameWindowHeight;
	PTextLabel	  *_gameScreenText;
	PStressButton *_launchButton;
	PButton		  *_gameScreenButton;
	PTextLabel	  *_playerNameText;
	PInput		  *_playerNameInput;
	PImageLabel	  *_background;
	PGUIManager	  *_manager;

	PAssetManager &_assetManager;

	PTextLabel	*_sizeTitleText;
	PImageLabel *_windowSizeBackground;

	std::vector<PGUIObject *> _mainUI;
	std::vector<PGUIObject *> _sizeUI;

	PButton *_i4096x3112;
	PButton *_i3656x2664;
	PButton *_i2560x1440;
	PButton *_i1920x1080;
	PButton *_i1280x960;
	PButton *_i640x480;

	mINI::INIFile	  *_configFile;
	mINI::INIStructure _config;
};