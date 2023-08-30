#include "gameapplication.h"
#include "Engine/inputtracker.h"
#include "Game/ExtremeUnaliving/defeatscreen.h"
#include "Game/ExtremeUnaliving/gameoverlayscreen.h"
#include "Game/ExtremeUnaliving/gamescreen.h"
#include "Game/ExtremeUnaliving/settingsscreen.h"
#include "Game/ExtremeUnaliving/shopscreen.h"
#include "Game/ExtremeUnaliving/titlescreen.h"
#include "Game/ExtremeUnaliving/victoryscreen.h"

namespace ExtremeUnaliving {

GameApplication::GameApplication(){}

void GameApplication::initializeApp() {

    std::shared_ptr<TitleScreen> m_titlescreen = std::make_shared<TitleScreen>();
    addScreen("title", m_titlescreen);

    std::shared_ptr<GameScreen> m_gamescreen = std::make_shared<GameScreen>();
    addScreen("game", m_gamescreen);

    std::shared_ptr<GameOverlayScreen> m_overlayscreen = std::make_shared<GameOverlayScreen>();
    addScreen("overlay", m_overlayscreen);

    std::shared_ptr<SettingsScreen> m_settingsscreen = std::make_shared<SettingsScreen>();
    addScreen("settings", m_settingsscreen);

    std::shared_ptr<DefeatScreen> m_defeatscreen = std::make_shared<DefeatScreen>();
    addScreen("defeat", m_defeatscreen);

    std::shared_ptr<ShopScreen> m_shopscreen = std::make_shared<ShopScreen>();
    addScreen("shop", m_shopscreen);

    std::shared_ptr<VictoryScreen> m_victoryscreen = std::make_shared<VictoryScreen>();
    addScreen("victory", m_victoryscreen);


    setVisibleScreens({"title"});
    setActiveScreens({"title"});
    Input::tracker.setCursor(true);


//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

}
