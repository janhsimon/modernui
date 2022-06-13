#pragma once

#include <cstdint>

namespace ModernUI
{
struct Vertex final
{
  int32_t x, y;
  float r, g, b;

  Vertex(int32_t x, int32_t y, float r, float g, float b);
};

class Context final
{
public:
  Context();

  void addWindow(const class Window& window);

  void processFrame();

  size_t getNumVertices() const;
  const Vertex* getVertices() const;

private:
  struct ContextData* d;
};

class Window final
{
public:
  Window(int32_t x, int32_t y, int32_t width, int32_t height);

  int32_t getX() const;
  int32_t getY() const;
  void setPosition(int32_t x, int32_t y);
  
  int32_t getWidth() const;
  int32_t getHeight() const;
  void setSize(int32_t width, int32_t height);

  float getColorR() const;
  float getColorG() const;
  float getColorB() const;
  void setColor(float r, float g, float b);

private:
  struct WindowData* d;
};
} // namespace ModernUI