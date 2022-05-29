#include "ModernUI.h"

#include <vector>

namespace ModernUI
{
struct ContextData
{
  std::vector<const Window*> windows;
  std::vector<Vertex> vertices;
};

Context::Context() : d(new ContextData)
{
}

void Context::addWindow(const Window& window)
{
  d->windows.push_back(&window);
}

void Context::processFrame()
{
  d->vertices.clear();

  for (const Window* window : d->windows)
  {
    const int32_t& x = window->getX();
    const int32_t& y = window->getY();
    const int32_t& w = window->getWidth();
    const int32_t& h = window->getHeight();

    const float& r = window->getColorR();
    const float& g = window->getColorG();
    const float& b = window->getColorB();

    // clang-format off
    d->vertices.push_back({ x,     y,     r, g, b });
    d->vertices.push_back({ x + w, y,     r, g, b });
    d->vertices.push_back({ x,     y + h, r, g, b });
    d->vertices.push_back({ x + w, y + h, r, g, b });
    // clang-format on
  }
}

size_t Context::getNumVertices() const
{
  return d->vertices.size();
}

const Vertex* Context::getVertices() const
{
  return d->vertices.data();
}
} // namespace ModernUI