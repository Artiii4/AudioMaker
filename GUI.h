#ifndef GUI_H
#define GUI_H

#include <functional>

class GUI {
public:
  GUI();
  ~GUI();
  bool init(int width, int height, const char* title);
  void run(std::function<void()> renderCallback);
  bool shouldClose() const;
  void shutdown();

private:
  struct Impl;
  Impl* pImpl;
};

#endif // !GUI_H