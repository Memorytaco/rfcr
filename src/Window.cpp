#include "Window.h"

bool Window::wasInit = false;
std::map<Window*, std::map<Window::WindowCallTy,Window::wincallTy>> Window::windowCallBack;
std::map<Window*, Window::keycallTy> Window::windowKeyCallBack;
std::map<Window*, Window::scrollcallTy> Window::windowScrollCallBack{};
std::map<Window*, Window::mousecallTy> Window::windowMouseCallBack;

void __glfwKeyCallback(GLFWwindow* gwin,int key,int scancode,int action,int mode)
{
  Window* win = static_cast
    <Window*>(glfwGetWindowUserPointer(gwin));
  Window::windowKeyCallBack[win](win, key, scancode, action, mode);
}

void __glfwWindowFrameBufferCallback(GLFWwindow* gwin, int w, int h)
{
  Window* win = static_cast
    <Window*>(glfwGetWindowUserPointer(gwin));
  Window::windowCallBack[win][Window::WindowCallTy::FrameBufferSize](win, w, h);
}

void __glfwScrollCallBack(GLFWwindow * gwin, double xoffset, double yoffset)
{
  Window* win = static_cast
    <Window*>(glfwGetWindowUserPointer(gwin));
  Window::windowScrollCallBack[win](win, xoffset, yoffset);
}

void __glfwMouseCallBack(GLFWwindow* gwin, int button, int action, int mods)
{
  Window* win = static_cast
    <Window*>(glfwGetWindowUserPointer(gwin));
  Window::windowMouseCallBack[win](win, button, action, mods);
}

Window::Window(int h, int w, std::string& name)
{
  win = glfwCreateWindow(h, w, name.c_str(), NULL, NULL);
  glfwSetWindowUserPointer(win, this);
  setCurrent();
}

Window::~Window()
{
}

void Window::setCurrent()
{
  glfwMakeContextCurrent(win);
}

bool Window::shouldClose()
{
  return glfwWindowShouldClose(win);
}

void Window::setShouldClose(bool v)
{
  glfwSetWindowShouldClose(win, v?1:0);
}

void Window::swap()
{
  glfwSwapBuffers(win);
}

void Window::setKeyCallBack(keycallTy &fun)
{
  glfwSetKeyCallback(win, __glfwKeyCallback);
  Window::windowKeyCallBack[this] = move(fun);
}

void Window::setScrollCallBack(scrollcallTy& call)
{
  glfwSetScrollCallback(win, __glfwScrollCallBack);
  Window::windowScrollCallBack[this] = std::move(call);
}

void Window::setMouseCallBack(mousecallTy& call)
{
  glfwSetMouseButtonCallback(win, __glfwMouseCallBack);
  Window::windowMouseCallBack[this] = std::move(call);
}

// set window callback here
void Window::setCallBack(WindowCallTy setType, wincallTy &fun)
{
  switch(setType) {
    case Window::WindowCallTy::FrameBufferSize:
      glfwSetFramebufferSizeCallback(win,
          __glfwWindowFrameBufferCallback);
      Window::windowCallBack[this][setType] = move(fun);
      break;
    default:
      return;
  }
}

