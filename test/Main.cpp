#include <glad/gl.h>
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
    if (gladLoadGL(glfwGetProcAddress) == 0)
    {
      std::cerr << "Failed to load OpenGL";
      return EXIT_FAILURE;
    }

    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
  }

  // Set up a shader program
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
        std::cerr << "Failed to compile vertex shader:\n" << infoLog;
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
                                  outColor = vec4(color, 1.0);
                                })";

      glShaderSource(fragmentShader, 1, &source, nullptr);
      glCompileShader(fragmentShader);

      GLint success;
      glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        GLchar infoLog[shaderLogSize];
        glGetShaderInfoLog(fragmentShader, shaderLogSize, nullptr, infoLog);
        std::cerr << "Failed to compile fragment shader:\n" << infoLog;
        glfwTerminate();
        return EXIT_FAILURE;
      }
    }

    // Link and use the shader program
    GLuint shaderProgram;
    {
      shaderProgram = glCreateProgram();

      glAttachShader(shaderProgram, vertexShader);
      glAttachShader(shaderProgram, fragmentShader);

      glLinkProgram(shaderProgram);

      GLint success;
      glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
      if (!success)
      {
        GLchar infoLog[shaderLogSize];
        glGetProgramInfoLog(shaderProgram, shaderLogSize, nullptr, infoLog);
        std::cerr << "Failed to link mesh shader program:\n" << infoLog;
        glfwTerminate();
        return EXIT_FAILURE;
      }

      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      glUseProgram(shaderProgram);
    }

    // Locate and set the projection matrix uniform
    {
      const GLint projectionMatrixUniformLocation = glGetUniformLocation(shaderProgram, "projection");
      if (projectionMatrixUniformLocation < 0)
      {
        std::cerr << "Failed to get projection matrix uniform location in shader program";
        glfwTerminate();
        return EXIT_FAILURE;
      }

      constexpr float w = static_cast<float>(windowWidth);
      constexpr float h = static_cast<float>(windowHeight);
      const glm::mat4 projectionMatrix = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);
      glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }
  }

  // Set up a basic interface
  ModernUI::Context context;
  ModernUI::Window win = ModernUI::Window(0, 0, 10, 10);
  {
    win.setColor(foregroundColor.r, foregroundColor.g, foregroundColor.b);
    context.addWindow(win);
  }

  // Create and bind a vertex array
  {
    GLuint vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
  }

  // Create and bind a vertex buffer
  {
    GLuint uiVertexBuffer;
    glGenBuffers(1, &uiVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uiVertexBuffer);
  }

  // Apply the vertex definition
  {
    constexpr GLsizei vertexSize = static_cast<GLsizei>(sizeof(ModernUI::Vertex));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, vertexSize, reinterpret_cast<void*>(offsetof(ModernUI::Vertex, x)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(offsetof(ModernUI::Vertex, r)));
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

      // Update the vertex buffer
      {
        const GLsizeiptr totalSize = static_cast<GLsizeiptr>(sizeof(ModernUI::Vertex) * context.getNumVertices());
        const void* vertexData = static_cast<const void*>(context.getVertices());
        glBufferData(GL_ARRAY_BUFFER, totalSize, vertexData, GL_DYNAMIC_DRAW);
      }

      // Draw the vertex buffer
      {
        const GLsizei numVertices = static_cast<GLsizei>(context.getNumVertices());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, numVertices);
      }

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}