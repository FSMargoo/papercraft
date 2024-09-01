#define UIDemo

#include <include/game/Game.h>
#include <include/launcher/Launcher.h>
#include <include/game/object/Object.h>

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	try {
#ifndef _DEBUG
		PLanuncher launcher;
		launcher.Loop();
		launcher.Close();
		auto   gameSize = launcher.GetGameScreenSize();
		PLGame gameWindow(std::get<0>(gameSize), std::get<1>(gameSize), launcher.GetPlayerName());
		gameWindow.Loop();
#else
		PLGame gameWindow(640, 480, "Developer");
		gameWindow.Loop();
#endif

		return 0;
	} catch (std::exception &e) {
		printf("Sorry, but PaperCraft was crashed with information:\n%s\n", e.what());

		return -1;
	}
}