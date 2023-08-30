#include "gameapplication.h"
#include "Game/PrairieKing3D/defeatscreen.h"
#include "Game/PrairieKing3D/gamescreen.h"
#include "Game/PrairieKing3D/settingsscreen.h"
#include "Game/PrairieKing3D/titlescreen.h"
#include "Game/PrairieKing3D/overlayscreen.h"
#include "Game/PrairieKing3D/victoryscreen.h"
#include <iostream>

namespace PrairieKing3D {

GameApplication::GameApplication(){}

void GameApplication::initializeApp() {

    std::shared_ptr<GameScreen> m_gamescreen = std::make_shared<GameScreen>();
    addScreen("game", m_gamescreen);

    std::shared_ptr<TitleScreen> m_titlescreen = std::make_shared<TitleScreen>();
    addScreen("title", m_titlescreen);

    std::shared_ptr<SettingsScreen> m_settingsscreen = std::make_shared<SettingsScreen>();
    addScreen("settings", m_settingsscreen);

    std::shared_ptr<OverlayScreen> m_overlayscreen = std::make_shared<OverlayScreen>();
    addScreen("overlay", m_overlayscreen);

    std::shared_ptr<VictoryScreen> m_victoryscreen = std::make_shared<VictoryScreen>();
    addScreen("victory", m_victoryscreen);

    std::shared_ptr<DefeatScreen> m_defeatScreen = std::make_shared<DefeatScreen>();
    addScreen("defeat", m_defeatScreen);

    setVisibleScreens({"title", "overlay"});
    setActiveScreens({"title"});
}

}
