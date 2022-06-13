#include "ModernUI.h"

namespace ModernUI
{
struct WindowData
{
  int32_t x, y;
  int32_t width, height;
  float colorR, colorG, colorB;
};

Window::Window(int32_t x, int32_t y, int32_t width, int32_t height) : d(new WindowData)
{
  d->x = x;
  d->y = y;
  d->width = width;
  d->height = height;

  setColor(1.0f, 1.0f, 1.0f);
}

int32_t Window::getX() const
{
  return d->x;
}

int32_t Window::getY() const
{
  return d->y;
}

void Window::setPosition(int32_t x, int32_t y)
{
  d->x = x;
  d->y = y;
}

int32_t Window::getWidth() const
{
  return d->width;
}

int32_t Window::getHeight() const
{
  return d->height;
}

void Window::setSize(int32_t width, int32_t height)
{
  d->width = width;
  d->height = height;
}

float Window::getColorR() const
{
  return d->colorR;
}

float Window::getColorG() const
{
  return d->colorG;
}

float Window::getColorB() const
{
  return d->colorB;
}

void Window::setColor(float r, float g, float b)
{
  d->colorR = r;
  d->colorG = g;
  d->colorB = b;
}

} // namespace ModernUI