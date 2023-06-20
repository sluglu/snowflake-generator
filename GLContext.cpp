#include "GLContext.h"

namespace GLContext {

    int oldWidth = 1000;
    int oldHeight = 1000;
    int oldX = 500;
    int oldY = 500;
    bool fullScreenUpdate = false;
    GLFWwindow* window;
    const char* window_name = "  ";
    bool alpha = false;
    bool fullscreen = false;
    int SCR_WIDTH = 1920;
    int SCR_HEIGHT = 1080;

    extern int viewportResW = 1000;
    extern int viewportResH = 1000;

    vec4 background = vec4(0, 0, 0, 0);
    void(*onInput)(int key) {};
    void(*onDraw)() {};
    void(*initialize)() {};
    void(*UI)();
    GLuint fbo = 0;
    GLuint texture = 0;


    

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glfwGetWindowSize(window, &SCR_WIDTH, &SCR_HEIGHT);
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS) {
            if (onInput) { onInput(key); }
        }
    }

    static void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }


    int init(int width, int height) {
        SCR_WIDTH = width;
        SCR_HEIGHT = height;
        const char* glsl_version = "#version 130";
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return 1;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, window_name, NULL, NULL);
        if (window == NULL)
        {
            glfwTerminate();
            return -1;
        }
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwMakeContextCurrent(window);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << endl;
            return -1;
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //cout << "OpenGL version : " << glGetString(GL_VERSION) << endl;
        if (initialize) { initialize(); }
        if(onDraw){
            //create fbo and texture
            glGenFramebuffers(1, &fbo);
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportResW, viewportResH, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
        renderLoop();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
    }

    void GLContext::updateFullScreen() {
        if (fullScreenUpdate == fullscreen) {
            return;
        }

        if (fullscreen)
        {
            // backup window position and window size
            glfwGetWindowPos(window, &oldX, &oldY);
            glfwGetWindowSize(window, &oldWidth, &oldHeight);

            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            // get resolution of monitor
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);

            // switch to full screen
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, 0);
        }
        else
        {
            // restore last window size and position
            glfwSetWindowMonitor(window, nullptr, oldX, oldY, oldWidth, oldHeight, 0);
        }
        fullScreenUpdate = !fullScreenUpdate;
    }

    void GLContext::renderLoop() {
        while (!glfwWindowShouldClose(window)) {
            updateFullScreen();
            glFlush();
            glfwPollEvents();
            
            //imgui init
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            ImGui::Begin("Snowflakes Generator", nullptr, window_flags);


            ImGui::PopStyleVar(2);

            // Submit the DockSpace
            ImGuiID dockspace_id = ImGui::GetID("main window");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

            ImGui::End();

            //Viewport
            if (onDraw) { 
                glBindFramebuffer(GL_FRAMEBUFFER, fbo);

                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

                //framebuffer creation error handling
                GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
                if (status != GL_FRAMEBUFFER_COMPLETE) {
                    std::cout << "framebuffer creation failure" << endl;
                }

                //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
                glViewport(0, 0, viewportResW, viewportResW);

                //draw
                onDraw(); 

                //draw viewport
                ImGui::Begin("Viewport");
                {
                    // Using a Child allow to fill all the space of the window.
                    // It also alows customization
                    ImGui::BeginChild("Render");
                    // Get the size of the child (i.e. the whole draw size of the windows).
                    ImVec2 wsize = ImGui::GetWindowSize();
                    // Because I use the texture from OpenGL, I need to invert the V from the UV.
                    ImGui::Image((void*)(intptr_t)texture, wsize);
                    ImGui::EndChild();
                }
                ImGui::End();
            }

            //unbind buffer and texture
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            //glBindTexture(GL_TEXTURE_2D, 0);

            //draw ui
            if (UI) { UI(); }

            ImGui::Render();
            //if (!alpha) { glClear(GL_COLOR_BUFFER_BIT); glClearColor(background.x, background.y, background.z, background.w); }
            glClear(GL_COLOR_BUFFER_BIT); 
            glClearColor(background.x, background.y, background.z, background.w);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);

        }
    }


    void GLContext::TakeScreenshot(string filePath) {
        std::vector<unsigned char> image(SCR_WIDTH * SCR_HEIGHT * 4);
        glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
        std::vector<unsigned char> PngBuffer;
        unsigned error = lodepng::encode(PngBuffer, image, SCR_WIDTH, SCR_HEIGHT);
        if (!error) { lodepng::save_file(PngBuffer, filePath); }
        if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    }

    void GLContext::drawPoint(vec2 position, float size, vec4 color) {
        glColor4f(color.x, color.y, color.z, color.w);
        glPointSize(size);
        glBegin(GL_POINTS);
        glVertex2f(position.x, position.y);
        glEnd();
    }

    void GLContext::drawLine(vec2 position1, vec2 position2, float width, vec4 color) {
        glColor4f(color.x, color.y, color.z, color.w);
        glLineWidth(width);
        glBegin(GL_LINES);
        glVertex2f(position1.x, position1.y);
        glVertex2f(position2.x, position2.y);
        glEnd();
    }

}