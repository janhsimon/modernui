#pragma once

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>

namespace ModernUI
{
struct ColorVertex final
{
  float x, y;
  float r, g, b;

  ColorVertex(float x, float y, float r, float g, float b);
};

struct TextureVertex final
{
  float x, y;
  float u, v;

  TextureVertex(float x, float y, float u, float v);
};

class Context final
{
public:
  enum class Error
  {
    Success,
    FontFileMissing,
    FontBakeFailed
  };

  Context();
  ~Context();

  Error getError() const;

  void addWindow(const class Window& window);
  void addButton(const class Button& button);

  void processFrame();

  size_t getNumColorVertices() const;
  const ColorVertex* getColorVertices() const;

  size_t getNumTextureVertices() const;
  const TextureVertex* getTextureVertices() const;

  unsigned char* getFontTextureData() const;

private:
  struct Data;
  std::unique_ptr<Data> d;
};

class Window final
{
public:
  Window(int32_t x, int32_t y, int32_t width, int32_t height);
  ~Window();

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
  struct Data;
  std::unique_ptr<Data> d;
};

class Button final
{
public:
  Button(const std::string& text, int32_t x, int32_t y, int32_t width, int32_t height);
  ~Button();

  std::string getText() const;
  void setText(const std::string& text);

  int32_t getX() const;
  int32_t getY() const;
  void setPosition(int32_t x, int32_t y);

  int32_t getWidth() const;
  int32_t getHeight() const;
  void setSize(int32_t width, int32_t height);

private:
  struct Data;
  std::unique_ptr<Data> d;
};
} // namespace ModernUI