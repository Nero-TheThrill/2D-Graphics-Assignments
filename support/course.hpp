#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace course
{

constexpr float pi = 3.14159265358979323846f;

inline void require(bool condition, const std::string& message)
{
    if (!condition) throw std::runtime_error(message);
}


// -----------------------------------------------------------------------------
// Math
// -----------------------------------------------------------------------------

struct Vec2
{
    float x = 0.0f, y = 0.0f;
};

struct Mat3
{
    // Column-major storage. Column vectors. A * B applies B first.
    std::array<float, 9> m{};

    [[nodiscard]]
    static Mat3 identity()
    {
        return {{
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        }};
    }

    [[nodiscard]]
    Mat3 operator*(const Mat3& other) const
    {
        Mat3 result;

        for (int column = 0; column < 3; ++column)
            for (int row = 0; row < 3; ++row)
                for (int k = 0; k < 3; ++k)
                    result.m[column * 3 + row] +=
                        m[k * 3 + row] * other.m[column * 3 + k];

        return result;
    }

    [[nodiscard]]
    Vec2 point(Vec2 p) const
    {
        return {
            m[0] * p.x + m[3] * p.y + m[6],
            m[1] * p.x + m[4] * p.y + m[7]
        };
    }
};

// u/v are reserved in A1-A3. A4 connects them to a vertex attribute.
struct Vertex
{
    float x = 0.0f, y = 0.0f;
    float r = 0.0f, g = 0.0f, b = 0.0f;
    float u = 0.0f, v = 0.0f;
};


// -----------------------------------------------------------------------------
// File Utilities
// -----------------------------------------------------------------------------

inline std::string readText(const std::string& path)
{
    std::ifstream file(path);
    require(static_cast<bool>(file), "Cannot open: " + path);

    std::ostringstream stream;
    stream << file.rdbuf();

    return stream.str();
}


// -----------------------------------------------------------------------------
// Shader Utilities
// -----------------------------------------------------------------------------

inline GLuint compileShader(GLenum type, const std::string& source)
{
    const GLuint shader = glCreateShader(type);
    const char* sourcePtr = source.c_str();

    glShaderSource(shader, 1, &sourcePtr, nullptr);
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        char log[4096]{};
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        glDeleteShader(shader);
        throw std::runtime_error(log);
    }

    return shader;
}

inline GLuint compileProgram(
    const std::string& vertexSource,
    const std::string& fragmentSource)
{
    const GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    const GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    const GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE)
    {
        char log[4096]{};
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        glDeleteProgram(program);
        throw std::runtime_error(log);
    }

    return program;
}

struct Program
{
    GLuint id = 0;

    Program(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
        : id{compileProgram(readText(vertexShaderPath), readText(fragmentShaderPath))}
    {
    }

    ~Program()
    {
        if (id != 0) glDeleteProgram(id);
    }

    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;

    Program(Program&&) = delete;
    Program& operator=(Program&&) = delete;
};


// -----------------------------------------------------------------------------
// Image
// -----------------------------------------------------------------------------

struct Image
{
    int width = 0, height = 0;
    std::vector<unsigned char> rgb;
};

inline Image loadPpm(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    require(static_cast<bool>(file), "Cannot open image: " + path);

    Image image;
    std::string magic;
    int maxValue = 0;

    file >> magic >> image.width >> image.height >> maxValue;
    file.get();

    require(
        magic == "P6" && image.width > 0 && image.height > 0 && maxValue == 255,
        "Expected bundled P6 PPM"
    );

    image.rgb.resize(
        static_cast<std::size_t>(image.width) *
        static_cast<std::size_t>(image.height) * 3
    );

    file.read(
        reinterpret_cast<char*>(image.rgb.data()),
        static_cast<std::streamsize>(image.rgb.size())
    );

    require(static_cast<bool>(file), "Incomplete image");

    return image;
}

// PPM loader leaves the first file scanline at v = 0.
// A4 handles orientation through UV coordinates.


// -----------------------------------------------------------------------------
// Application
// -----------------------------------------------------------------------------

struct App
{
    GLFWwindow* window = nullptr;

    int width = 960, height = 640;
    float dt = 0.0f;

    GLuint indicatorProgram = 0, indicatorVao = 0;

    std::array<bool, GLFW_KEY_LAST + 1> keys{};
    std::array<bool, GLFW_KEY_LAST + 1> lastKeys{};

    bool leftClick = false, lastMouse = false;

    double previousTime = 0.0;

    int frames = 0, frameLimit = 0;
    std::string capturePath;

    App(int argc, char** argv, const char* title)
    {
        parseArguments(argc, argv);
        initializeGlfw();
        createWindow(title);
        initializeOpenGL();
        createIndicator();

        previousTime = glfwGetTime();
    }

    ~App()
    {
        glDeleteVertexArrays(1, &indicatorVao);
        glDeleteProgram(indicatorProgram);

        if (window != nullptr) glfwDestroyWindow(window);

        glfwTerminate();
    }

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    [[nodiscard]]
    bool down(int key) const { return keys.at(key); }

    [[nodiscard]]
    bool pressed(int key) const { return keys.at(key) && !lastKeys.at(key); }

    bool begin()
    {
        if (glfwWindowShouldClose(window)) return false;
        if (frameLimit > 0 && frames >= frameLimit) return false;

        updateInput();
        updateTime();

        glfwGetFramebufferSize(window, &width, &height);

        // Viewport is supplied in A1/A2.
        // A3/A4 explicitly update it in main.
        glClearColor(0.075f, 0.095f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        return true;
    }

    void end()
    {
        GLint previousViewport[4]{};
        glGetIntegerv(GL_VIEWPORT, previousViewport);

        drawIndicator();

        if (shouldCapture()) captureFrame();

        glViewport(
            previousViewport[0],
            previousViewport[1],
            previousViewport[2],
            previousViewport[3]
        );

        const GLenum error = glGetError();
        require(error == GL_NO_ERROR, "OpenGL error: " + std::to_string(error));

        glfwSwapBuffers(window);
        ++frames;
    }


private:

    void parseArguments(int argc, char** argv)
    {
        for (int i = 1; i < argc; ++i)
        {
            const std::string argument = argv[i];

            if (argument == "--frames" && i + 1 < argc)
                frameLimit = std::stoi(argv[++i]);
            else if (argument == "--capture" && i + 1 < argc)
                capturePath = argv[++i];
        }
    }

    void initializeGlfw()
    {
        glfwSetErrorCallback([](int, const char* message)
        {
            std::fprintf(stderr, "GLFW: %s\n", message);
        });

        require(glfwInit() != 0, "GLFW initialization failed");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void createWindow(const char* title)
    {
        const std::string caption =
            std::string(title) + " [" + COURSE_TRACK + "]";

        window = glfwCreateWindow(
            width,
            height,
            caption.c_str(),
            nullptr,
            nullptr
        );

        require(
            window != nullptr,
            "OpenGL 4.5 Core context required. Update the GPU driver."
        );

        glfwMakeContextCurrent(window);
    }

    void initializeOpenGL()
    {
        glewExperimental = GL_TRUE;

        require(glewInit() == GLEW_OK, "GLEW initialization failed");

        // GLEW may generate a benign GL error during initialization.
        while (glGetError() != GL_NO_ERROR) {}

        require(GLEW_VERSION_4_5 != 0, "OpenGL 4.5 DSA is required");

        std::printf(
            "Track: %s | OpenGL: %s\n",
            COURSE_TRACK,
            glGetString(GL_VERSION)
        );

        glfwSwapInterval(1);
    }

    void createIndicator()
    {
        static const std::string vertexShader = R"(
#version 450 core

const vec2 positions[3] = vec2[3](
    vec2(-0.95, 0.78),
    vec2(-0.79, 0.78),
    vec2(-0.87, 0.95)
);

void main()
{
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
}
)";

        static const std::string fragmentShader = R"(
#version 450 core

out vec4 color;

void main()
{
    color = vec4(1.0, 0.8, 0.2, 1.0);
}
)";

        indicatorProgram = compileProgram(vertexShader, fragmentShader);
        glCreateVertexArrays(1, &indicatorVao);
    }

    void updateInput()
    {
        glfwPollEvents();

        lastKeys = keys;

        for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key)
            keys[key] = glfwGetKey(window, key) == GLFW_PRESS;

        const bool mouseDown =
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

        leftClick = mouseDown && !lastMouse;
        lastMouse = mouseDown;

        if (pressed(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    void updateTime()
    {
        const double currentTime = glfwGetTime();

        dt = std::min(
            static_cast<float>(currentTime - previousTime),
            0.1f
        );

        previousTime = currentTime;
    }

    void drawIndicator()
    {
        glViewport(0, 0, width, height);
        glUseProgram(indicatorProgram);
        glBindVertexArray(indicatorVao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    [[nodiscard]]
    bool shouldCapture() const
    {
        if (capturePath.empty()) return false;
        if (frameLimit == 0) return true;

        return frames == frameLimit - 1;
    }

    void captureFrame()
    {
        std::vector<unsigned char> rgb(
            static_cast<std::size_t>(width) *
            static_cast<std::size_t>(height) * 3
        );

        glPixelStorei(GL_PACK_ALIGNMENT, 1);

        glReadPixels(
            0,
            0,
            width,
            height,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            rgb.data()
        );

        std::ofstream file(capturePath, std::ios::binary);
        require(static_cast<bool>(file), "Cannot write capture");

        file << "P6\n" << width << ' ' << height << "\n255\n";

        for (int y = height - 1; y >= 0; --y)
        {
            const auto offset =
                static_cast<std::size_t>(y) *
                static_cast<std::size_t>(width) * 3;

            file.write(
                reinterpret_cast<const char*>(rgb.data() + offset),
                static_cast<std::streamsize>(width * 3)
            );
        }
    }
};

} // namespace course