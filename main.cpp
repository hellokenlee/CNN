/*ALL Copyright Reversed by KenLee@2015*/
#include <cstdlib>
#include <cstdio>

#ifdef _MSC_VER // 适配Visual C++ 编译器
    #include <OpenBlas/cblas.h>
    #pragma comment(lib,"libopenblas.dll.a")
#endif // _MSC_VER

#ifdef __GNUC__ // 适配GCC
    #include <cblas.h>
#endif // __GNUC__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

/************************************************************************
*
*							Main 函数
*
************************************************************************/

#include "CNN/cNetwork.h"
#include "GUI/imgui/imgui.h"
#include "GUI/imgui/imgui_impl_glfw_gl3.h"
#include "GUI/Texture.h"
#include "GUI/Shader.h"
#include "GUI/Object.h"

GLfloat vertices[] = {
    // First triangle
     1.0f,  1.0f, 0.0f,  // Top Right
     1.0f, -1.0f, 0.0f,  // Bottom Right
    -1.0f,  1.0f, 0.0f,  // Top Left
    // Second triangle
     1.0f, -1.0f, 0.0f,  // Bottom Right
    -1.0f, -1.0f, 0.0f,  // Bottom Left
    -1.0f,  1.0f, 0.0f   // Top Left
};

static void error_callback(int error, const char* description){
    fprintf(stderr, "Error %d: %s\n", error, description);
}

cNetwork *net = new cNetwork("DefaultCNN");


void GUIMainWindow(){
    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(630, 510, "Number Regonizer by SYSU.KenLee", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

    ImVec4 clear_color = ImColor(100, 100, 100);

    char buf[256] = {"TestData/TestImages/1.png"};

    Texture *tex = new Texture("TestData/TestImages/1.png",
                TEXTURE_DIFFUSE, GL_BGRA, GL_RGBA,
                0, 0, GL_REPEAT, GL_LINEAR);
    char guessText[128] = {"Kizuna A.I. is thinking..."};
    //
    Shader shader("GUI/shaders/shader.vs","GUI/shaders/shader.frag");
    Shader shaderBG("GUI/shaders/shader.vs","GUI/shaders/shaderBG.frag");
    Object BG(vertices, 6, POSITIONS, GL_TRIANGLES);
    BG.setShader(&shaderBG);
    vector<GLfloat> painter;
    BYTE pixels[360 * 360 * 4];

    // Main loop
    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        ImGui_ImplGlfwGL3_NewFrame();

        {
            //Image Shower
            ImGui::Begin("ImageViewer",false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
            ImGui::SetWindowPos(ImVec2(0, 0));
            ImGui::SetWindowSize(ImVec2(250,270));
            ImGui::Text("Number Image:");
            ImGui::Image((ImTextureID)tex->texID, ImVec2(230, 230));
            ImGui::End();
        }
        {
            //Guess Box
            ImGui::Begin("Guess Box",false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
            ImGui::SetWindowPos(ImVec2(0, 270));
            ImGui::SetWindowSize(ImVec2(250, 110));
            ImGui::Text("           ");
            ImGui::SameLine();
            if(ImGui::Button("Guess!")){
                int res = net->testSingleImage(buf);
                sprintf(guessText, "Kizuna A.I. think that's a %d !", res);
            }
            ImGui::Text("\n\n");
            ImGui::TextColored(ImVec4(249.0/255.0, 204.0/255.0, 226.0/255.0, 1.0), guessText);
            ImGui::End();
        }

        {
            // Image Reader
            ImGui::Begin("Input Window", false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
            ImGui::SetWindowPos(ImVec2(0, 380));
            ImGui::SetWindowSize(ImVec2(250, 130));
            ImGui::Text("Load Image From File:");
            ImGui::Text("\n");
            ImGui::PushItemWidth(235);
            ImGui::InputText("", buf, 256);
            ImGui::PopItemWidth();
            ImGui::Text("\n");
            ImGui::Text("                           ");
            ImGui::SameLine();
            if(ImGui::Button("Load")){
                printf("Load Image from : %s\n", buf);
                tex = new Texture(buf, TEXTURE_DIFFUSE, GL_BGRA, GL_RGBA, 0, 0, GL_REPEAT, GL_LINEAR);
            }
            ImGui::End();

        }
        {
            // Drawer
            ImGui::Begin("Drawer Window", false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
            ImGui::SetWindowPos(ImVec2(250, 0));
            ImGui::SetWindowSize(ImVec2(380, 380));

            ImGui::End();
        }
        {
            // Drawer btns
            ImGui::Begin("Draw Buttons", false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
            ImGui::SetWindowPos(ImVec2(250, 380));
            ImGui::SetWindowSize(ImVec2(380, 130));
            ImGui::Text("\n");
            ImGui::Text("\n");
            ImGui::Text("\n");
            ImGui::Text("           ");
            ImGui::SameLine();
            if(ImGui::Button("Reset")){
                printf("Reset\n");
                painter.clear();
            }
            ImGui::SameLine();
            ImGui::Text("         ");
            ImGui::SameLine();
            if(ImGui::Button("Apply & Guess")){
                printf("Apply\n");
                glReadPixels(270, 150, 350, 350, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                FIBITMAP *image = FreeImage_ConvertFromRawBits(pixels, 350, 350, 4 * 350, 32, 0x0000FF, 0xFF0000, 0x00FF00, false);
                FIBITMAP *imgSmall = FreeImage_Rescale(image, 28, 28, FILTER_BILINEAR);
                FreeImage_Save(FIF_PNG, imgSmall, "TestData/test.png", 0);
                FreeImage_Unload(image);
                FreeImage_Unload(imgSmall);
                sprintf(buf, "TestData/test.png");
                tex = new Texture(buf, TEXTURE_DIFFUSE, GL_BGRA, GL_RGBA, 0, 0, GL_REPEAT, GL_LINEAR);
                int res = net->testSingleImage(buf);
                sprintf(guessText, "Kizuna A.I. think that's a %d !", res);
                painter.clear();
            }
            ImGui::End();
        }
        // RenderingGetMousePos
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        // 画板
        glViewport(260, 140, 360, 360);
        glPointSize(20.0);
        BG.draw();
        Object board((GLfloat*)&painter[0], painter.size() / 3, POSITIONS, GL_POINTS);
        board.setShader(&shader);
        board.draw();
        if(ImGui::IsMouseDown(0)){
            ImVec2 mousePos = ImGui::GetMousePos();
            if(mousePos.x > 270 && mousePos.y < 360){
                mousePos.x -= 270;
                mousePos.x /= 180.0;
                mousePos.y /= 180.0;
                mousePos.x -= 1.0;
                mousePos.y -= 1.0;
                mousePos.y = -mousePos.y;
                painter.push_back(mousePos.x);
                painter.push_back(mousePos.y);
                painter.push_back(0.0f);
            }
        }
        // 双缓冲
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();
}
int main(int, char**){

    GUIMainWindow();
    return 0;
}

