#ifndef RFCWINDOW
#define RFCWINDOW

#include "core.h"

class Window {
  using wincallTy = std::function<void(Window*,int,int)>;
  using keycallTy = std::function<void(Window*,int,int,int,int)>;
  using mousecallTy = std::function<void(Window*,int,int,int)>;
  using scrollcallTy = std::function<void(Window*,double,double)>;
  public:
    enum class WindowCallTy {
      KeyCall,
      FrameBufferSize,
      MouseScroll
    };
    Window(int, int, std::string&);
    ~Window();
    void setCurrent();
    bool shouldClose();
    void setShouldClose(bool v);
    void setCallBack(WindowCallTy, wincallTy&);
    void setCallBack(WindowCallTy setTy, wincallTy&& call) {
      setCallBack(setTy, call);
    }
    void setKeyCallBack(keycallTy&);
    void setKeyCallBack(keycallTy&& call) {
      setKeyCallBack(call);
    }
    void setScrollCallBack(scrollcallTy&);
    void setScrollCallBack(scrollcallTy&& call) {
      setScrollCallBack(call);
    }
    void setMouseCallBack(mousecallTy&);
    void setMouseCallBack(mousecallTy&& call) {
      setMouseCallBack(call);
    }
    void swap();
  private:
    GLFWwindow* win{nullptr};
    void *store{nullptr};
    static bool wasInit;
  public:
    static std::map<Window*, std::map<WindowCallTy,wincallTy>> windowCallBack;
    static std::map<Window*, keycallTy> windowKeyCallBack;
    static std::map<Window*, mousecallTy> windowMouseCallBack;
    static std::map<Window*, scrollcallTy> windowScrollCallBack;
};

#endif
