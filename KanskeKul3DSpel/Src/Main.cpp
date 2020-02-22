#include "ImguiInit.h"
#include "Game.h"
#include "Renderer/Renderer.h"
#include "Debug/ConsoleWindow.h"

constexpr bool showWarnings = false;

void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    if (type == GL_DEBUG_TYPE_ERROR)
    {
        printf("GL CALLBACK ERROR: type = 0x%x, severity = 0x%x, message = %s\n",
            type, severity, message);
        printfCon("GL CALLBACK ERROR: %s\n", message);
        printfCon("See terminal for details");
    }

    else if (showWarnings)
    {
        printf("GL CALLBACK: type = 0x%x, severity = 0x%x, message = %s\n",
            type, severity, message);
        printfCon("GL CALLBACK: %s\n", message);
        printfCon("See terminal for details");
    }
}

int main()
{
#ifdef _DEBUG
    int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
    _CrtSetDbgFlag(flag);
    //_CrtSetBreakAlloc(394); // Comment or un-comment on need basis
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

    glFrontFace(GL_CCW);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glfwSwapInterval(1);

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





        //Draw
        renderer.draw();
        renderer.rendererDebug();

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