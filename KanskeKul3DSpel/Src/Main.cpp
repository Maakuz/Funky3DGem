#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "ImguiInit.h"
#include "Game.h"

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

    GLuint VertexArrayID;
    glGenVertexArrays(1, & VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLfloat vertexData[] = 
    {
        -1, -1, 0,
         1, -1, 0,
         0,  1, 0
    };

    ImGuiImpl::Initialize(window);

    GLuint vertexBuffer;

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    Shader vertexShader(SHADER_PATH "VS.glsl", ShaderType::Vertex);
    Shader fragmentShader(SHADER_PATH "PS.glsl", ShaderType::Fragment);

    Game game(window);
    
    Program prog({ vertexShader, fragmentShader });

    prog.initializeUniformLocation("VP");
    
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    bool running = true;

    while (running)
    {
        glfwPollEvents();

        ImGuiImpl::NewFrame();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) || glfwWindowShouldClose(window))
            running = false;

        //update
        game.run(0);

        //Clear
        glClearColor(20, 100, 100, 255);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        //Draw
        prog.use();
        glUniformMatrix4fv(prog.getUniformID("VP"), 1, GL_FALSE, &game.getCamera()->getVP()[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

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