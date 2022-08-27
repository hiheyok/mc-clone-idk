#include "client.h"
#include "../utils/functions/clock/Clock.h"
#include <chrono>
#include <iostream>
#include <set>

#define _SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS


using namespace glm;
using namespace std;
using namespace chrono;

void APIENTRY Client::glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) {

    std::stringstream str;


    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    /* std::cout << "---------------" << std::endl;
     std::cout << "Debug message (" << id << "): " << message << std::endl;*/

     // std::cout  << "---------------" << std::endl;
    str << " (OPENGL) Debug message (" << id << "): " << message << " | ";



    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             str << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   str << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: str << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     str << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     str << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           str << "Source: Other"; break;
    } str << " | ";

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               str << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: str << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  str << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         str << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         str << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              str << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          str << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           str << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               str << "Type: Other"; break;
    } str << " | ";

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         str << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       str << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          str << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: str << "Severity: notification"; break;
    }

    str.seekg(0, ios::end);
    int size = str.tellg();

    if (size != 0) {
        cout << str.str();
        cout << "\n";
    }
    



}

void Client::start() {
    std::cout << "Window Size: " << sizex << ", " << sizey << "\n";
    initialize();
    mainGameLoop();
    stop();
}

void Client::initialize() {

    gui = new IGUI;
    camera = new Camera(vec3(-100.f, 50.f, 100.f), vec3(0.f, 1.f, 0.f), -50.f, -40.f);

    glfwInit();
    if (!glfwInit())
    {
        std::cout << "Initialization Failed: GLFW\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(sizex, sizey, "1.0.5A (DEV)", NULL, NULL);

    glfwMakeContextCurrent(window);
    
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;

        glfwTerminate();
        return;
    }
    else {
        cout << this << " | Created Window: " << window << "\n";
    }
   
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int a, int b) { static_cast<Client*>(glfwGetWindowUserPointer(win))->resize_window(win, a, b); });
    glfwSetCursorPosCallback(window, +[](GLFWwindow* win, double a, double b) { static_cast<Client*>(glfwGetWindowUserPointer(win))->mouse_callback(win, a, b); });
   // glfwSetScrollCallback(window, +[](GLFWwindow* win, double a, double b) { static_cast<Window*>(glfwGetWindowUserPointer(win))->scroll_callback(win, a, b); });
    glewExperimental = GL_TRUE;
    glewInit();

    if (glewInit() != GLEW_OK) {
        std::cout << "Initialization Failed: GLEW\n";
        return;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    if (OPENGL_EXTENSIONS_TEST) {
        GLint no_of_extensions = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &no_of_extensions);
        std::cout << "OpenGL Extensions: \n";
        std::set<std::string> ogl_extensions;
        for (int i = 0; i < no_of_extensions; ++i) {
            const char* ext = (const char*)glGetStringi(GL_EXTENSIONS, i);
            std::cout << ext << " \n";
            ogl_extensions.insert(ext);
        }
        std::cout << "Required OpenGL Extensions: \n";
        if (ogl_extensions.find("GL_ARB_shader_draw_parameters") != ogl_extensions.end()) {
            std::cout << "GL_ARB_shader_draw_parameters: GOOD\n";
        }

    }
   
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_DEPTH_TEST);


    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);



    glfwSwapInterval(0);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    worldRender.genBuffer(&sizex, &sizey, 1);

    gui->init(window, &CLIENT_EVENTS);

    gui->update();

    gui->configTexture(2, "assets/textures/atlas/background.png",1);
    gui->configTexture(3, "assets/textures/atlas/gui.png", 1);
    gui->configTexture(4, "assets/textures/atlas/logo.png", 1);

    gui->addGUI("worldRender", vec2(1, 1), vec2(0, 0), vec3(255, 255, 255), vec2(0, 0), vec2(1, 1), false, 1);
    gui->addGUI("background", vec2(2, 1), vec2(0, 0), vec3(64, 64, 64), vec2(0, 0), vec2(24, 12), false, 2);
    gui->addToRenderQueue("background");

    gui->addGUIresizedTexture("logo", vec2(1, 1), vec2(0, .75), vec3(200, 200, 200), vec2(0, 0), vec2(256, 45),vec2(256,256), false, 4);
    gui->addToRenderQueue("logo");
    
    gui->addGUIresizedTexturewFont("Start", "Play", vec2(1.f, 1.f), vec2(0.0, .2), vec3(255, 255, 255), vec2(0, 66), vec2(199, 86), vec2(256, 256), true, 3);
    gui->addToRenderQueue("Start");

    gui->addGUIresizedTexturewFont("Options", "Options", vec2(1.f, 1.f), vec2(0.0, 0.0), vec3(255, 255, 255), vec2(0, 66), vec2(199, 86), vec2(256, 256), true, 3);
    gui->addToRenderQueue("Options");

    gui->addGUIresizedTexturewFont("Start World", "Create a New World", vec2(1.f, 1.f), vec2(0.0, -.4), vec3(255, 255, 255), vec2(0, 66), vec2(199, 86), vec2(256, 256), true, 3);

  //  gui->addGUIresizedTexturewFont("(DEBUG) Gen Rand Chunk", "(DEBUG) Gen Rand Chunk", vec2(1.f, 1.f), vec2(0.0, -.4), vec3(255, 255, 255), vec2(0, 66), vec2(199, 86), vec2(256, 256), true, 3);

    gui->changeGUIcall("Start", {"Start World"}, {"logo","Options","Start"});
    gui->changeGUIEvent("Start World", EVENT_START_INTEGRATED_SERVER);
   // gui->changeGUIEvent("(DEBUG) Gen Rand Chunk", EVENT_DEBUG_GEN_RAND_CHUNK);
    gui->changeGUIcall("Start World", {"worldRender"}, {"Start World","background"});
    GLint max_layers;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers);
    cout << "Texture Array (MAX): " << max_layers << "\n";

    WorldRenderer.init(window, &cworld, camera);
    WorldRenderer.ReloadAssets();

    threads[0] = std::thread(&Client::updateTitleStats, this);
   


}

void Client::mainGameLoop() {

    cout << "Started main loop\n";
    while (!glfwWindowShouldClose(window)) {
   //    cout << this << "| (INFO): Sizeof(GUI): " << sizeof(gui) << "\n";
        auto time0 = high_resolution_clock::now();

        render();
        EventCall();
        IO();

        duration<double, milli> time1 = high_resolution_clock::now() - time0;
        if ((1.0 / maxFPS) - (time1.count() / 1000.0) > 0) {
         //   timerSleep((1.0 / maxFPS) - (time1.count() / 1000.0));
        }
        time1 = high_resolution_clock::now() - time0;
        delta = time1.count() / 1000.0f;
        fps = 1.0 / (time1.count() / 1000.0f);
    }
}

void Client::Reload() {
    WorldRenderer.init(window,&cworld, camera);
}

void Client::EventCall() {
    for (const auto& events : CLIENT_EVENTS) {
        if (events != NULL) {
            if (events == EVENT_START_INTEGRATED_SERVER) {
                server = new Server(INTEGRATED_SERVER);
                cout << "Starting Server\n";
                cworld.init(server);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                playMode = true;
            }
            if (events == EVENT_DEBUG_GEN_RAND_CHUNK) {

            }
            if (events == EVENT_CLIENT_RELOAD) {
                Reload();
            }
        }
    }

    CLIENT_EVENTS.clear();
}

void Client::IO() {
    glfwPollEvents();
    keycallback();
    gui->update();
    int err = glGetError();  if (err != 0) { std::cout << "OPENGL ERROR: " << err << "\n"; };
}

void Client::updateTitleStats() {

    std::cout << "updateWindowName : Thread ID: " << std::this_thread::get_id() << "\n";

    std::mutex mut;
    while (true) {

        mut.lock();
        std::string str = "Version: idek | " + to_string(fps) + " FPS | Camera Position: " + to_string(camera->Position.x) + ", " + to_string(camera->Position.y) + ", " + to_string(camera->Position.z);
        mut.unlock();
        glfwSetWindowTitle(window, str.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(128));
    }
}

//Callbacks

void Client::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll((float)yoffset);
}

void Client::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (server != nullptr) {
        RenderWorld();
    }
    else {
        gui->Render();
    }
    
    

    glfwSwapBuffers(window);
}

void Client::RenderWorld() {
    WorldRenderer.UpdateData();
    WorldRenderer.GenCallDrawCommands();
    

   // worldRender.bindFBO();
   // glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.47843137254f, 0.65490196078f, 0.96862745098f, 1.f);
    WorldRenderer.draw();
 //   worldRender.unbindFBO();
 //   worldRender.render();
}

void Client::getStats() {

}

void Client::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if (playMode) {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = (float)xpos - (float)lastX;
        float yoffset = (float)lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;
      //  std::cout << xoffset << ", " << yoffset << "\n";
        camera->ProcessMouseMovement(xoffset, yoffset);
    }

  
}


void Client::resize_window(GLFWwindow* window, int x, int y) {
    glViewport(0, 0, x, y);
    sizex = x;
    sizey = y;
    std::cout << this << " | CLIENT | Resized Window: " << sizex << ", " << sizey << "\n";
  //  if (camera != nullptr) { camera->screenRes = ivec2(sizex, sizey); };
   
}

void Client::keycallback() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwTerminate();
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->ProcessKeyboard(FORWARD, (float)delta);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->ProcessKeyboard(BACKWARD, (float)delta);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->ProcessKeyboard(LEFT, (float)delta);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->ProcessKeyboard(RIGHT, (float)delta);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera->FOV = 30;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
        camera->FOV = 90;
    }

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);    

    gui->MousePos = vec2((float)xpos, (float)ypos);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        gui->MouseLeftButtonPressPos = vec2((float)xpos, (float)ypos);
        gui->clicked = true;
    }

    cworld.playerpos = camera->Position;
}