#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <modernui/ModernUI.h>

#include <iostream>

namespace
{
constexpr char windowTitle[] = "ModernUI Test";
constexpr int32_t windowWidth = 800;
constexpr int32_t windowHeight = 600;
constexpr GLsizei shaderLogSize = 512;
constexpr glm::vec4 backgroundColor = { 0.15f, 0.16f, 0.18f, 1.0f };
constexpr glm::vec3 foregroundColor = { 0.45f, 0.46f, 0.48f };
} // namespace

int main()
{
  // Create a window and load OpenGL
  GLFWwindow* window;
  {
    if (!glfwInit())
    {
      std::cerr << "Failed to initialize GLFW";
      return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr, nullptr);
    if (!window)
    {
      std::cerr << "Failed to create window";
      glfwTerminate();
      return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
    {
      std::cerr << "Failed to load OpenGL";
      return EXIT_FAILURE;
    }

    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  // Set up a color shader program
  GLuint colorShaderProgram;
  {
    // Compile the vertex shader
    GLuint vertexShader;
    {
      vertexShader = glCreateShader(GL_VERTEX_SHADER);

      const GLchar* source = R"(#version 330 core
                                uniform mat4 projection;
                                layout(location = 0) in vec2 inPosition;
                                layout(location = 1) in vec3 inColor;
                                out vec3 color;
                                void main()
                                {
                                  color = inColor;
                                  gl_Position = projection * vec4(inPosition, 0.0, 1.0);
                                })";

      glShaderSource(vertexShader, 1, &source, nullptr);
      glCompileShader(vertexShader);

      GLint success;
      glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        GLchar infoLog[shaderLogSize];
        glGetShaderInfoLog(vertexShader, shaderLogSize, nullptr, infoLog);
        std::cerr << "Failed to compile color vertex shader:\n" << infoLog;
        glfwTerminate();
        return EXIT_FAILURE;
      }
    }

    // Compile the fragment shader
    GLuint fragmentShader;
    {
      fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

      const GLchar* source = R"(#version 330 core
                                in vec3 color;
                                out vec4 outColor;
                                void main()
                                {
                                  outColor.rgb = color;
                                  outColor.a = 1.0;
                                })";

      glShaderSource(fragmentShader, 1, &source, nullptr);
      glCompileShader(fragmentShader);

      GLint success;
      glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        GLchar infoLog[shaderLogSize];
        glGetShaderInfoLog(fragmentShader, shaderLogSize, nullptr, infoLog);
        std::cerr << "Failed to compile color fragment shader:\n" << infoLog;
        glfwTerminate();
        return EXIT_FAILURE;
      }
    }

    // Link and use the shader program
    {
      colorShaderProgram = glCreateProgram();

      glAttachShader(colorShaderProgram, vertexShader);
      glAttachShader(colorShaderProgram, fragmentShader);

      glLinkProgram(colorShaderProgram);

      GLint success;
      glGetProgramiv(colorShaderProgram, GL_LINK_STATUS, &success);
      if (!success)
      {
        GLchar infoLog[shaderLogSize];
        glGetProgramInfoLog(colorShaderProgram, shaderLogSize, nullptr, infoLog);
        std::cerr << "Failed to link color shader program:\n" << infoLog;
        glfwTerminate();
        return EXIT_FAILURE;
      }

      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);
    }

    glUseProgram(colorShaderProgram);

    // Locate and set the projection matrix uniform
    {
      const GLint uniformLocation = glGetUniformLocation(colorShaderProgram, "projection");
      if (uniformLocation < 0)
      {
        std::cerr << "Failed to get projection matrix uniform location in color shader program";
        glfwTerminate();
        return EXIT_FAILURE;
      }

      constexpr float w = static_cast<float>(windowWidth);
      constexpr float h = static_cast<float>(windowHeight);
      const glm::mat4 projectionMatrix = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);
      glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }
  }

  // Set up a texture shader program
  GLuint textureShaderProgram;
  {
    // Compile the vertex shader
    GLuint vertexShader;
    {
      vertexShader = glCreateShader(GL_VERTEX_SHADER);

      const GLchar* source = R"(#version 330 core
                                uniform mat4 projection;
                                layout(location = 0) in vec2 inPosition;
                                layout(location = 1) in vec2 inUv;
                                out vec2 uv;
                                void main()
                                {
                                  uv = inUv;
                                  gl_Position = projection * vec4(inPosition, 0.0, 1.0);
                                })";

      glShaderSource(vertexShader, 1, &source, nullptr);
      glCompileShader(vertexShader);

      GLint success;
      glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        GLchar infoLog[shaderLogSize];
        glGetShaderInfoLog(vertexShader, shaderLogSize, nullptr, infoLog);
        std::cerr << "Failed to compile texture vertex shader:\n" << infoLog;
        glfwTerminate();
        return EXIT_FAILURE;
      }
    }

    // Compile the fragment shader
    GLuint fragmentShader;
    {
      fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

      const GLchar* source = R"(#version 330 core
                                uniform sampler2D image;
                                in vec2 uv;
                                out vec4 outColor;
                                void main()
                                {
                                  outColor.rgb = vec3(0.3);
                                  outColor.a = texture(image, uv).r;
                                })";

      glShaderSource(fragmentShader, 1, &source, nullptr);
      glCompileShader(fragmentShader);

      GLint success;
      glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        GLchar infoLog[shaderLogSize];
        glGetShaderInfoLog(fragmentShader, shaderLogSize, nullptr, infoLog);
        std::cerr << "Failed to compile texture fragment shader:\n" << infoLog;
        glfwTerminate();
        return EXIT_FAILURE;
      }
    }

    // Link and use the shader program
    {
      textureShaderProgram = glCreateProgram();

      glAttachShader(textureShaderProgram, vertexShader);
      glAttachShader(textureShaderProgram, fragmentShader);

      glLinkProgram(textureShaderProgram);

      GLint success;
      glGetProgramiv(textureShaderProgram, GL_LINK_STATUS, &success);
      if (!success)
      {
        GLchar infoLog[shaderLogSize];
        glGetProgramInfoLog(textureShaderProgram, shaderLogSize, nullptr, infoLog);
        std::cerr << "Failed to link texture shader program:\n" << infoLog;
        glfwTerminate();
        return EXIT_FAILURE;
      }

      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);
    }

    glUseProgram(textureShaderProgram);

    // Locate and set the projection matrix uniform
    {
      const GLint uniformLocation = glGetUniformLocation(textureShaderProgram, "projection");
      if (uniformLocation < 0)
      {
        std::cerr << "Failed to get projection matrix uniform location in texture shader program";
        glfwTerminate();
        return EXIT_FAILURE;
      }

      constexpr float w = static_cast<float>(windowWidth);
      constexpr float h = static_cast<float>(windowHeight);
      const glm::mat4 projectionMatrix = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);
      glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }

    // Locate and set the image sampler uniform
    {
      const GLint uniformLocation = glGetUniformLocation(textureShaderProgram, "image");
      if (uniformLocation < 0)
      {
        std::cerr << "Failed to get image sampler uniform location in texture shader program";
        glfwTerminate();
        return EXIT_FAILURE;
      }

      glUniform1i(uniformLocation, 0);
    }
  }

  // Set up a basic interface
  ModernUI::Context context;
  if (context.getError() == ModernUI::Context::Error::FontFileMissing)
  {
    std::cerr << "Failed to open font file for interface";
    glfwTerminate();
    return EXIT_FAILURE;
  }
  else if (context.getError() == ModernUI::Context::Error::FontBakeFailed)
  {
    std::cerr << "Failed to bake font glyphs for interface";
    glfwTerminate();
    return EXIT_FAILURE;
  }
  
  ModernUI::Window win = ModernUI::Window(0, 0, 100, 100);
  win.setColor(foregroundColor.r, foregroundColor.g, foregroundColor.b);
  context.addWindow(win);

  ModernUI::Button btn = ModernUI::Button("Open...", 10, 10, 150, 30);
  context.addButton(btn);

  // Generate and fill a font texture for the interface
  GLuint texture;
  {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, context.getFontTextureData());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  // Create two vertex arrays and buffers
  GLuint vertexArrays[2];
  GLuint vertexBuffers[2];
  {
    glGenVertexArrays(2, vertexArrays);
    glGenBuffers(2, vertexBuffers);
  }

  // Apply the vertex definition for the color vertex buffer
  {
    glBindVertexArray(vertexArrays[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[0]);

    constexpr GLsizei vertexSize = static_cast<GLsizei>(sizeof(ModernUI::ColorVertex));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vertexSize,
                          reinterpret_cast<void*>(offsetof(ModernUI::ColorVertex, x)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
                          reinterpret_cast<void*>(offsetof(ModernUI::ColorVertex, r)));
  }

  // Apply the vertex definition for the texture vertex buffer
  {
    glBindVertexArray(vertexArrays[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[1]);

    constexpr GLsizei vertexSize = static_cast<GLsizei>(sizeof(ModernUI::TextureVertex));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vertexSize,
                          reinterpret_cast<void*>(offsetof(ModernUI::TextureVertex, x)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexSize,
                          reinterpret_cast<void*>(offsetof(ModernUI::TextureVertex, u)));
  }

  // Main loop
  while (!glfwWindowShouldClose(window))
  {
    // Update
    {
      double mouseX, mouseY;
      glfwGetCursorPos(window, &mouseX, &mouseY);
      win.setPosition(static_cast<int>(mouseX), static_cast<int>(mouseY));
    }

    // Render
    {
      context.processFrame();

      glClear(GL_COLOR_BUFFER_BIT);

      // Draw color vertices
      {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[0]);

        // Update the color vertex buffer
        {
          const GLsizeiptr totalSize =
            static_cast<GLsizeiptr>(sizeof(ModernUI::ColorVertex) * context.getNumColorVertices());
          const void* vertexData = static_cast<const void*>(context.getColorVertices());
          glBufferData(GL_ARRAY_BUFFER, totalSize, vertexData, GL_DYNAMIC_DRAW);
        }

        glBindVertexArray(vertexArrays[0]);
        glUseProgram(colorShaderProgram);

        // Draw the color vertex buffer
        {
          const GLsizei numVertices = static_cast<GLsizei>(context.getNumColorVertices());
          glDrawArrays(GL_TRIANGLES, 0, numVertices);
        }
      }

      // Draw texture vertices
      {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[1]);

        // Update the texture vertex buffer
        {
          const GLsizeiptr totalSize =
            static_cast<GLsizeiptr>(sizeof(ModernUI::TextureVertex) * context.getNumTextureVertices());
          const void* vertexData = static_cast<const void*>(context.getTextureVertices());
          glBufferData(GL_ARRAY_BUFFER, totalSize, vertexData, GL_DYNAMIC_DRAW);
        }

        glBindVertexArray(vertexArrays[1]);
        glUseProgram(textureShaderProgram);

        glBindTexture(GL_TEXTURE_2D, texture);

        // Draw the texture vertex buffer
        {
          const GLsizei numVertices = static_cast<GLsizei>(context.getNumTextureVertices());
          glDrawArrays(GL_TRIANGLES, 0, numVertices);
        }
      }

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}