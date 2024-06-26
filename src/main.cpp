#include <iostream>
#include <modbus.h>


#include "registers.h"



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "registers.h"
#include "opengl.h"



using namespace std;

int main()
{
    setlocale(LC_ALL, "RUS");


    OpenGL& opengl = OpenGL::get_instance();
    opengl.initialize();
    GLFWwindow* window = opengl.create_window(800, 800, "Main Window");

    GLfloat vertices[] ={
            // �������                // �����
            -0.05f,  -0.05f,  0.0f,   1.0f, 0.0f, 0.0f,
            -0.05f,   0.05f,  0.0f,   1.0f, 0.0f, 0.0f,
             0.05f,   0.05f,  0.0f,   1.0f, 0.0f, 0.0f,
             0.05f,  -0.05f,  0.0f,   1.0f, 0.0f, 0.0f
    };

    GLuint indices[] = {
            0, 1, 2,
            0, 2, 3
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // �������, �����
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint dot_shader_program = opengl.create_shader_program(
            "../src/shaders/dot_shader.vert",
            "../src/shaders/dot_shader.frag"
    );

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(dot_shader_program);
        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }




	return 0;
}