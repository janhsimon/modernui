#include "ModernUI.h"

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include <stb/stb_truetype.h>

#include <string>
#include <vector>

namespace ModernUI
{
struct Context::Data final
{
  Context::Error error;

  std::vector<const Window*> windows;
  std::vector<const Button*> buttons;
  std::vector<ColorVertex> colorVertices;
  std::vector<TextureVertex> textureVertices;

  // Font stuff
  unsigned char fontBitmap[512 * 512];
  stbtt_bakedchar fontCharacters[96];
};

Context::Context() : d(new Data)
{
  d->error = Error::Success;

  FILE* file = nullptr;
  if (fopen_s(&file, "C:\\Users\\janhs\\dev\\modernui-build\\src\\Debug\\Arial.ttf", "rb") != 0)
  {
    d->error = Error::FontFileMissing;
    return;
  }

  constexpr size_t bufferSize = 1u << 20u;
  unsigned char* ttfBuffer = new unsigned char[bufferSize];
  fread(ttfBuffer, 1, bufferSize, file);
  fclose(file);

  if (!stbtt_BakeFontBitmap(ttfBuffer, 0, 32.0f, d->fontBitmap, 512, 512, 32, 96, d->fontCharacters))
  {
    d->error = Error::FontBakeFailed;
    return;
  }

  delete[] ttfBuffer;
  ttfBuffer = nullptr;
}

Context::~Context() = default;

Context::Error Context::getError() const
{
  return d->error;
}

void Context::addWindow(const Window& window)
{
  d->windows.push_back(&window);
}

void Context::addButton(const Button& button)
{
  d->buttons.push_back(&button);
}

void Context::processFrame()
{
  d->colorVertices.clear();
  d->textureVertices.clear();

  for (const Window* window : d->windows)
  {
    const float x = static_cast<float>(window->getX());
    const float y = static_cast<float>(window->getY());
    const float w = static_cast<float>(window->getWidth());
    const float h = static_cast<float>(window->getHeight());

    const float r = window->getColorR();
    const float g = window->getColorG();
    const float b = window->getColorB();

    // clang-format off
    d->colorVertices.push_back({ x,     y,     r, g, b });
    d->colorVertices.push_back({ x + w, y,     r, g, b });
    d->colorVertices.push_back({ x,     y + h, r, g, b });

    d->colorVertices.push_back({ x + w, y,     r, g, b });
    d->colorVertices.push_back({ x,     y + h, r, g, b });
    d->colorVertices.push_back({ x + w, y + h, r, g, b });
    // clang-format on
  }

  for (const Button* button : d->buttons)
  {
    // Draw the box
    {
      const float x = static_cast<float>(button->getX());
      const float y = static_cast<float>(button->getY());
      const float w = static_cast<float>(button->getWidth());
      const float h = static_cast<float>(button->getHeight());

      const float r = 1.0f;
      const float g = 1.0f;
      const float b = 1.0f;

      // clang-format off
      d->colorVertices.push_back({ x,     y,     r, g, b });
      d->colorVertices.push_back({ x + w, y,     r, g, b });
      d->colorVertices.push_back({ x,     y + h, r, g, b });

      d->colorVertices.push_back({ x + w, y,     r, g, b });
      d->colorVertices.push_back({ x,     y + h, r, g, b });
      d->colorVertices.push_back({ x + w, y + h, r, g, b });
      // clang-format on
    }

    // Draw the label
    {
      float x = button->getX() + 5.0f;
      float y = button->getY() + button->getHeight() - 5.0f;

      const std::string& text = button->getText();
      for (const char& c : text)
      {
        if (c < 32u || c >= 128u)
        {
          continue;
        }

        stbtt_aligned_quad quad;
        stbtt_GetBakedQuad(d->fontCharacters, 512, 512, c - 32u, &x, &y, &quad, 1);

        d->textureVertices.push_back({ quad.x0, quad.y0, quad.s0, quad.t0 });
        d->textureVertices.push_back({ quad.x1, quad.y0, quad.s1, quad.t0 });
        d->textureVertices.push_back({ quad.x0, quad.y1, quad.s0, quad.t1 });

        d->textureVertices.push_back({ quad.x1, quad.y0, quad.s1, quad.t0 });
        d->textureVertices.push_back({ quad.x0, quad.y1, quad.s0, quad.t1 });
        d->textureVertices.push_back({ quad.x1, quad.y1, quad.s1, quad.t1 });
      }
    }
  }
}

size_t Context::getNumColorVertices() const
{
  return d->colorVertices.size();
}

const ColorVertex* Context::getColorVertices() const
{
  return d->colorVertices.data();
}

size_t Context::getNumTextureVertices() const
{
  return d->textureVertices.size();
}

const TextureVertex* Context::getTextureVertices() const
{
  return d->textureVertices.data();
}

unsigned char* Context::getFontTextureData() const
{
  return d->fontBitmap;
}
} // namespace ModernUI