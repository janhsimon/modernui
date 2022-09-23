#include "ModernUI.h"

namespace ModernUI
{
ColorVertex::ColorVertex(float x, float y, float r, float g, float b) : x(x), y(y), r(r), g(g), b(b)
{
}

TextureVertex::TextureVertex(float x, float y, float u, float v) : x(x), y(y), u(u), v(v)
{
}
} // namespace ModernUI