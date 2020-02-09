#include "Game.h"
#include "ConsoleWindow.h"
#include "Component/TransformComp.h"
#include "Component/ModelComp.h"
#include "Component/MovementComp.h"
#include "Component/MovementInputComp.h"
#include "Component/LightComp.h"
#include "Renderer.h"

Game* Game::s_instance = nullptr;
GLFWkeyfun Game::s_prevCallback;

Game::Game(GLFWwindow* window)
{
    m_window = window;
    m_lockMouse = true;
    m_debugEntities = false;
    m_consoleVisible = false;
    s_instance = this;
    m_debugCamera = false;

    Camera::get().initialize(90, 1920, 1080);

    ConsoleWindow::get().addCommand("toggleCursor", [&](Arguments args)->std::string
        {
            m_lockMouse = !m_lockMouse;
            return "Togglesia";
        });

    ConsoleWindow::get().addCommand("toggleEntityWindow", [&](Arguments args)->std::string
        {
            m_debugEntities = !m_debugEntities;
            return "Togglesia";
        });

    ConsoleWindow::get().addCommand("toggleCameraWindow", [&](Arguments args)->std::string
        {
            m_debugCamera = !m_debugCamera;
            return "Togglesia";
        });

    Entity e = m_manager.createEntity();
    m_entities.push_back(e);

    TransformComp::get().addComponent(e);
    TransformComp::get().setPosition(e, { 0, 0, 1 });
    Camera::get().attachCamera(e);
    MovementComp::get().addComponent(e);
    MovementInputComp::get().addInput(e);

    e = m_manager.createEntity();
    m_entities.push_back(e);

    TransformComp::get().addComponent(e);
    ModelComp::get().addComponent(e);

    s_prevCallback = glfwSetKeyCallback(m_window, inputCallbackWrapper);

    //Useful for getting geycodes
    /*glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int code, int action, int mods) 
        {
            printfCon("%d", key);
            printfCon("%d", code);
        });*/


    printfCon("removing things with dependencies is dangerous and needs to be addressed.");
    printfCon("Example: remove transform from object with movement");
}

void Game::run(float deltaTime)
{
    static bool consolePrev = false;
    if (m_consoleVisible)
        ConsoleWindow::get().update(!consolePrev);
    consolePrev = m_consoleVisible;

    MovementInputComp::get().handleInputs(m_window);
    MovementComp::get().applyToTransform(deltaTime);

    if (m_lockMouse)
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        static double x, y;
        static double prevX, prevY;
        prevX = x;
        prevY = y;
        glfwGetCursorPos(m_window, &x, &y);

        Camera::get().trackMouse(deltaTime, x - prevX, prevY - y);
    }

    else
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
    }

    Camera::get().calculateVP();

    for (size_t i = 0; i < m_entities.size(); i++)
    {
        if (ModelComp::get().hasComponent(m_entities[i]))
            Renderer::queueModel(m_entities[i]);
    }

    if (m_debugCamera)
        Camera::get().cameraDebug(&m_debugCamera);

    if (m_debugEntities)
        debugEntities();
}


void Game::inputCallback(int key, int code, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_GRAVE_ACCENT:
            m_consoleVisible = !m_consoleVisible;
            break;

        case GLFW_KEY_F1:
            m_debugEntities = !m_debugEntities;
            break;

        case GLFW_KEY_F2:
            m_debugCamera = !m_debugCamera;
            break;

        case GLFW_KEY_F3:
            m_lockMouse = !m_lockMouse;
            break;
        default:
            break;
        }
    }
}

void Game::debugEntities()
{
    using namespace ImGui;

    //ShowDemoWindow();

    TransformComp* transform = &TransformComp::get();
    ModelComp* model = &ModelComp::get();
    MovementComp* movement = &MovementComp::get();
    MovementInputComp* movementInput = &MovementInputComp::get();
    LightComp* light = &LightComp::get();

    Begin("Entity debugger", &m_debugEntities);

    if (Button("Add entity"))
    {
        m_entities.push_back(m_manager.createEntity());
    }

    for (size_t i = 0; i < m_entities.size(); i++)
    {
        Entity entity = m_entities[i];
        //Debug label
        std::string label = std::to_string(i);
        if (CollapsingHeader(label.c_str()))
        {
            
            transform->printImguiDebug(entity);

            Separator();

            model->printImguiDebug(entity);

            Separator();

            movement->printImguiDebug(entity);

            Separator();

            movementInput->printImguiDebug(entity);

            Separator();

            light->printImguiDebug(entity);

            Separator();
            Separator();

        }

    }



    End();
}
