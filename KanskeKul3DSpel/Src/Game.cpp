#include "Game.h"
#include "Debug/ConsoleWindow.h"
#include "Debug/Profiler.h"
#include "Component/TransformComp.h"
#include "Component/ModelComp.h"
#include "Component/PlayerInputComp.h"
#include "Component/PhysicsComp.h"
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
    TransformComp::get().setPosition(e, { 0, 2, 1 });
    Camera::get().attachCamera(e);
    PhysicsComp::get().addComponent(e);
    PhysicsComp::get().setMass(e, 2);
    PlayerInputComp::get().addInput(e);
    DirectionalLightComp::get().addComponent(e);
    DirectionalLightComp::get().setColor(e, { 0.7, 0.2, 0 });
    DirectionalLightComp::get().setDir(e, {0.2, -0.6, 0.2});
    e = m_manager.createEntity();
    m_entities.push_back(e);

    TransformComp::get().addComponent(e);
    TransformComp::get().setScale(e, { 10, 1, 10 });
    ModelComp::get().addComponent(e);
    PhysicsComp::get().addComponent(e);

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
    Profiler::get().update(deltaTime);

    static bool consolePrev = false;
    if (m_consoleVisible)
        ConsoleWindow::get().update(!consolePrev);
    consolePrev = m_consoleVisible;

    PlayerInputComp::get().handleInputs(m_window);

    if (m_lockMouse)
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        m_mouseData.prevX = m_mouseData.x;
        m_mouseData.prevY = m_mouseData.y;
        
        glfwGetCursorPos(m_window, &m_mouseData.x, &m_mouseData.y);

        Camera::get().trackMouse(deltaTime, m_mouseData.x - m_mouseData.prevX, m_mouseData.prevY - m_mouseData.y);
    }

    else
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
    }

    Camera::get().calculateVP();
    DirectionalLightComp::get().calculateView();

    for (size_t i = 0; i < m_entities.size(); i++)
    {
        if (ModelComp::get().hasComponent(m_entities[i]))
            Renderer::queueModel(m_entities[i]);
    }

    PhysicsComp::get().stepSimulation(deltaTime);
    
    
    //Debug
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
            if (!m_lockMouse)
            {
                glfwGetCursorPos(m_window, &m_mouseData.x, &m_mouseData.y);
                m_mouseData.prevX = m_mouseData.x;
                m_mouseData.prevY = m_mouseData.y;
            }

            m_lockMouse = !m_lockMouse;
            break;

        case GLFW_KEY_P:
            if (Profiler::get().isOpen())
                Profiler::get().close();

            else
                Profiler::get().open();
            break;
        default:
            break;
        }
    }
}

void Game::debugEntities()
{
    using namespace ImGui;

    ShowDemoWindow();

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
            
            if (TreeNode(("Transform " + std::to_string(entity.id)).c_str()))
            {
                TransformComp::get().printImguiDebug(entity);
                TreePop();
            }
            Separator();

            if (TreeNode(("Model " + std::to_string(entity.id)).c_str()))
            {
                ModelComp::get().printImguiDebug(entity);
                TreePop();
            }
            Separator();

            if (TreeNode(("movementInput " + std::to_string(entity.id)).c_str()))
            {
                PlayerInputComp::get().printImguiDebug(entity);
                TreePop();
            }
            Separator();

            if (TreeNode(("Directional light " + std::to_string(entity.id)).c_str()))
            {
                DirectionalLightComp::get().printImguiDebug(entity);
                TreePop();
            }
            Separator();

            if (TreeNode(("Rigidbody " + std::to_string(entity.id)).c_str()))
            {
                PhysicsComp::get().printImguiDebug(entity);
                TreePop();
            }
            Separator();
            Separator();

        }

    }



    End();
}
