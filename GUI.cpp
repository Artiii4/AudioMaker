//#define GLFW_DLL
#include "GUI.h"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

struct GUI::Impl {
  GLFWwindow* window = nullptr;
};

GUI::GUI() : pImpl(new Impl) {}

GUI::~GUI() { shutdown(); }

bool GUI::init(int width, int height, const char* title) {
  if (!glfwInit()) {
    std::cerr << "GLFW init failed\n";
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  pImpl->window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!pImpl->window) {
    std::cerr << "Window creation failed\n";
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(pImpl->window);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui_ImplGlfw_InitForOpenGL(pImpl->window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  return true;
}

void GUI::run(std::function<void()> renderCallback) {
  while (!glfwWindowShouldClose(pImpl->window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (renderCallback) {
      renderCallback();
    }

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(pImpl->window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(pImpl->window);
  }
}

bool GUI::shouldClose() const {
  return glfwWindowShouldClose(pImpl->window);
}

void GUI::shutdown() {
  if (pImpl->window) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(pImpl->window);
    glfwTerminate();
    pImpl->window = nullptr;
  }
}