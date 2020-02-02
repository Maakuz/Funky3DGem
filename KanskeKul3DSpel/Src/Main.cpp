#include "ImguiInit.h"
#include "Game.h"
#include "Renderer.h"

int main()
{
#ifdef _DEBUG
    int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
    _CrtSetDbgFlag(flag);
    //_CrtSetBreakAlloc(742196); // Comment or un-comment on need basis
#endif

    if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "The game good", NULL, NULL);

    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
        return -2;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    ImGuiImpl::Initialize(window);

    Game game(window);
    Renderer renderer;
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


    bool running = true;

    while (running)
    {
        glfwPollEvents();

        ImGuiImpl::NewFrame();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) || glfwWindowShouldClose(window))
            running = false;

        //DT
        static double dt = 0;
        static double last = 0;
        static double time = 0;
        last = time;
        time = glfwGetTime();
        dt = time - last;

        //update
        game.run(dt);

        //Clear
        glClearColor(20, 100, 100, 255);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        //Draw
        renderer.draw(game.getCamera());


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //Display
        glfwSwapBuffers(window);
    }

    ImGuiImpl::Shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}