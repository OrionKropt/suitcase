#include <iostream>
#include <modbus.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "logger.h"
#include "registers.h"
#include "opengl.h"
#include "primitives.h"
#include "graph.h"

#include <cmath>
#include <chrono>
#include <thread>

inline auto radians(float degrees) -> float
{
    return degrees * M_PI / 180;
}

int main()
{
//    setlocale(LC_ALL, "RUS");
    LOG_INFO("Suitcase started");

    OpenGL& opengl = OpenGL::get_instance();
    opengl.initialize();

    GLFWwindow* window = opengl.get_window();

    opengl.create_shader(
            "primitives",
            "resources/shaders/primitives/primitives.vert",
            "resources/shaders/primitives/primitives.frag"
    );
    opengl.create_shader(
            "line",
            "resources/shaders/primitives/primitives.vert",
            "resources/shaders/primitives/primitives.frag",
            "resources/shaders/primitives/line.geom"
    );
    opengl.create_shader(
            "text",
            "resources/shaders/text/text.vert",
            "resources/shaders/text/text.frag"
    );
    opengl.create_shader(
            "semicircle",
            "resources/shaders/primitives/semicircle.vert",
            "resources/shaders/primitives/semicircle.frag"
    );

//    Point dot(glm::vec2(0.0f, 0.0f), 0.01f);
//    dot.set_color(glm::vec3(0.0f, 0.0f, 1.0f));

//    Line line(glm::vec2(-1.0f, -1.0f), glm::vec2(1.0f, 1.0f));

//    Text text("Power Meter 710 Graphics.", glm::vec2(-1.0f, -1.0f), 1.0f, 0.0f);

    // # LOG_WARNING:   System_clock mode works properly only if hor_value_step is a signed integer.
    // #            Fractions and negative numbers cause errors and undefined behaviour.
    Graph graph(L"t, сек", L"U, вольт",
            /*std::chrono::system_clock::now()*/ 0.0f, -300.0f,
            /*1_s*/ 1.0f, 50.0f /*1.0f*/,
                19, 13,
                -6, -9,
                0, 0,
                0.08f, 0.08f);
    graph.set_enabled(true);

    Button button1(glm::vec2(-0.75, 0.75), 0.10f, 0.065f,
                  [] { std::cout << "Pressed #1 \r"; },
                  [] { std::cout << "Released #1\r"; },
                   L"<");

    Button button2(glm::vec2(0.75, 0.75), 0.10f, 0.065f,
                  [] { std::cout << "Pressed #2 \r"; },
                  [] { std::cout << "Released #2\r"; },
                   L">");

    Button button3(glm::vec2(-0.75, -0.75), 0.10f, 0.065f,
                  [] { std::cout << "Pressed #3 \r"; },
                  [] { std::cout << "Released #3\r"; },
                   L"<<");

    Button button4(glm::vec2(0.75, -0.75), 0.10f, 0.065f,
                  [] { std::cout << "Pressed #4 \r"; },
                  [] { std::cout << "Released #4\r"; },
                   L">>");

    auto now = std::chrono::system_clock::now();
    float counter = 0;

    graph.create_curve(L"sin1", glm::vec3(1.0f, 0.0f, 0.0f));
    graph.create_curve(L"sin2", glm::vec3(0.0f, 1.0f, 0.0f));
    graph.create_curve(L"sin3", glm::vec3(1.0f, 1.0f, 0.0f));

    graph.create_caption_container(L"sin container", 0.415f, 0.15f, glm::vec2(0.0f, -0.8f));
//    graph.set_container_borders_enabled(L"sin container", true);

    graph.create_caption(L"sin container", L"sin1", false);
    graph.create_caption(L"sin container", L"sin2", false);
    graph.create_caption(L"sin container", L"sin3", false);

    // # TEST
    Semicircle semicircle(glm::vec2(0.0f, 0.75f), 0.15f, 0.135f);
    Semicircle semicircle2(glm::vec2(-0.4f, 0.75f), 0.05f, 0.0f);
    Semicircle semicircle3(glm::vec2(0.4f, 0.75f), 0.20f, 0.15f);

    while (!glfwWindowShouldClose(window))
    {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - now).count() >= 5)
        {
//            GLfloat rand = 0 + std::rand() % (600 - 0 + 1);
//            GLfloat y = (sinf(counter) * 50 + 300);
            GLfloat y1 = sinf(counter) * 50;
            GLfloat y2 = sinf(counter + radians(60)) * 50;
            GLfloat y3 = sinf(counter + radians(120)) * 50;

            graph.add_point(L"sin1", counter, y1);
            graph.add_point(L"sin2", counter, y2);
            graph.add_point(L"sin3", counter, y3);

            counter += 0.05f;
            now = std::chrono::system_clock::now();
        }

        glfwPollEvents();

        if (opengl.get_key(GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window, true);
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        graph.draw();

        button1.draw();
        button2.draw();
        button3.draw();
        button4.draw();


        semicircle.draw();
        semicircle2.draw();
        semicircle3.draw();
//        line.draw();
//        dot.draw();

//        text.draw();

        glfwSwapBuffers(window);
    }

    LOG_INFO("Suitcase shut down");

	return 0;
}