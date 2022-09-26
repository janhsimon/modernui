#include "ModernUI.h"

namespace ModernUI
{
struct Button::Data final
{
  std::string text;
  int32_t x, y;
  int32_t width, height;
};

Button::Button(const std::string& text, int32_t x, int32_t y, int32_t width, int32_t height) : d(new Data)
{
  setText(text);
  setPosition(x, y);
  setSize(width, height);
}

Button::~Button() = default;

std::string Button::getText() const
{
  return d->text;
}

void Button::setText(const std::string& text)
{
  d->text = text;
}

int32_t Button::getX() const
{
  return d->x;
}

int32_t Button::getY() const
{
  return d->y;
}

void Button::setPosition(int32_t x, int32_t y)
{
  d->x = x;
  d->y = y;
}

int32_t Button::getWidth() const
{
  return d->width;
}

int32_t Button::getHeight() const
{
  return d->height;
}

void Button::setSize(int32_t width, int32_t height)
{
  d->width = width;
  d->height = height;
}

} // namespace ModernUI