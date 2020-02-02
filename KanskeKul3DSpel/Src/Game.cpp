#include "Game.h"
#include "ConsoleWindow.h"
#include "Component/TransformComp.h"
#include "Component/ModelComp.h"
#include "Renderer.h"

Game::Game(GLFWwindow* window) :
    m_camera(90.f, 1920, 1080)
{
    m_window = window;
    m_lockMouse = true;
    m_debugEntities = false;

    ConsoleWindow::get().addCommand("toggleCursor", [&](Arguments args)->std::string
        {
            m_lockMouse = !m_lockMouse;
            return "Togglesia";
        });

    ConsoleWindow::get().addCommand("toggleEntityDebugger", [&](Arguments args)->std::string
        {
            m_debugEntities = !m_debugEntities;
            return "Togglesia";
        });

    Entity e = m_manager.createEntity();
    m_entities.push_back(e);

    TransformComp::get().addTransform(e);
    TransformComp::get().setPosition(e, { 0, 0, 1 });
    m_camera.attachCamera(e);

    e = m_manager.createEntity();
    m_entities.push_back(e);

    TransformComp::get().addTransform(e);
    ModelComp::get().addModel(e);

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

        //invert the inversion
        m_camera.trackMouse(deltaTime, x - prevX, prevY - y);
    }

    else
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
    }

    m_camera.calculateVP();

    if (glfwGetKey(m_window, GLFW_KEY_S))
        TransformComp::get().move(m_entities[0], glm::vec3(0, 0, 1));

    if (glfwGetKey(m_window, GLFW_KEY_W))
        TransformComp::get().move(m_entities[0], glm::vec3(0, 0, -1));

    for (size_t i = 0; i < m_entities.size(); i++)
    {
        if (ModelComp::get().hasModel(m_entities[i]))
            Renderer::queueModel(m_entities[i]);
    }

    m_camera.cameraDebug();

    if (m_debugEntities)
        debugEntities();
}

void Game::debugEntities()
{
    using namespace ImGui;

    //ShowDemoWindow();

    TransformComp* transform = &TransformComp::get();
    ModelComp* model = &ModelComp::get();

    Begin("Entity debugger");

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
            if (transform->hasTransform(entity))
            {
                Text("Transform");
                glm::vec3 pos = transform->getPosition(entity);
                DragFloat3(("Position " + std::to_string(i)).c_str(), &pos[0], 0.5);
                transform->setPosition(entity, pos);

                glm::vec3 scale = transform->getScale(entity);
                DragFloat3(("Scale " + std::to_string(i)).c_str(), &scale[0], 0.5);
                transform->setScale(entity, scale);

                glm::vec3 rot = transform->getRotation(entity);
                DragFloat3(("Rotation " + std::to_string(i)).c_str(), &rot[0], 0.1);
                transform->setRotation(entity, rot);

                if (TreeNode(("matrix " + std::to_string(i)).c_str()))
                {
                    glm::mat4 transformMat = transform->getTransformMat(entity);
                    for (int j = 0; j < 4; j++)
                    {
                        Columns(4);

                        for (int k = 0; k < 4; k++)
                        {
                            Text("%f", transformMat[j][k]);
                            NextColumn();
                        }

                    }

                    Columns(1);
                    TreePop();
                }

            }

            else
            {
                if (Button(("Add transform" + std::to_string(i)).c_str()))
                    transform->addTransform(entity);
            }
            Separator();

            if (model->hasModel(entity))
            {
                Text("Buffer ID: %d, vertices: %d", model->getBuffer(entity).bufferID, model->getBuffer(entity).size);
                if (BeginCombo(("Mesh " + std::to_string(i)).c_str(), std::to_string((int)model->getMesh(entity)).c_str()))
                {
                    for (int j = 0; j < 2; j++)
                    {
                        if (Selectable(std::to_string(j).c_str()))
                            model->setMesh(entity, (Meshes)j);
                    }

                    EndCombo();
                }
            }

            else
            {
                if (Button(("Add model " + std::to_string(i)).c_str()))
                    model->addModel(entity);
            }
            Separator();
            Separator();

        }

    }



    End();
}
