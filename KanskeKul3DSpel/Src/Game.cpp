#include "Game.h"
#include "ConsoleWindow.h"
#include "Component/TransformComp.h"

Game::Game(GLFWwindow* window) :
    m_player(m_manager.createEntity())
{
    m_window = window;

    TransformComp::get().addTransform(m_player);
}

void Game::run(float deltaTime)
{

    static bool consoleOn = false;
    static bool consolePrev = false;
    static bool prevKeyP = false;
    if (glfwGetKey(m_window, GLFW_KEY_P) && !prevKeyP)
        consoleOn = !consoleOn;

    prevKeyP = glfwGetKey(m_window, GLFW_KEY_P);

    if (consoleOn)
        ConsoleWindow::get().update(!consolePrev);

    consolePrev = consoleOn;
}
