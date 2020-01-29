#include "Game.h"
#include "ConsoleWindow.h"

Game::Game(GLFWwindow* window)
{
    m_window = window;


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

    Entity e = m_manager.createEntity();

    if (m_manager.isAlive(e))
        printfCon("still alive woo");

    consolePrev = consoleOn;
}
