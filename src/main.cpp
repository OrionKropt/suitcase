#include <iostream>
#include <modbus.h>
<<<<<<< Updated upstream
=======
#include "data_logger.h"
#include "registers.h"
#include "data_logger.h"

>>>>>>> Stashed changes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "registers.h"
#include "opengl.h"

<<<<<<< Updated upstream
=======



>>>>>>> Stashed changes
using namespace std;

int main()
{
    setlocale(LC_ALL, "RUS");

<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes
    OpenGL& opengl = OpenGL::get_instance();
    opengl.initialize();
    GLFWwindow* window = opengl.create_window(800, 800, "Main Window");

    GLfloat vertices[] ={
            // Вершины                // Цвета
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

    // Вершины, цвета
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

<<<<<<< Updated upstream
//	modbus_t* ctx;
//	ctx = modbus_new_rtu("\COM2", 19200, 'N', 8, 1);
//    if (ctx == NULL) {
//        std::cout << "Не удалось создать контекст Modbus" << std::endl;
//        return 1;
//    }
//
//    // Установка адреса устройства
//    modbus_set_slave(ctx, 1);
//
//    // Установка на RS-485
//    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
//
//    // Подключение к устройству
//    if (modbus_connect(ctx) == -1) {
//        std::cout << "Не удалось подключиться к устройству" << std::endl;
//        modbus_free(ctx);
//        return 1;
//    }
//    else cout << "Успешное подключение\n";
//
//
//
//    int reg_count = 2;
//    int buffer_size = 2;
//                                            // Чтение данных из регистров
//    uint16_t regs[10];                      // Для хранения данных из 2-х регистров
//    int rc = modbus_read_registers(
//        ctx,
//        REAL_ENERGY_TOTAL_FLOAT,                               // Адресс первого регистра
//        reg_count,                                  // Количестов регистров
//        regs);
//    cout << rc << endl;
//    if (rc == -1) {
//        std::cout << "Ошибка чтения регистров" << std::endl;
//       // cout << modbus_strerror(errno) << endl;
//        modbus_close(ctx);
//        modbus_free(ctx);
//        return 1;
//    }
//
//    float reg_content = modbus_get_float(regs);
//    //float reg_content = modbus_get_float_dcba(regs);
//
//    cout << "Значение регистра " << reg_content << endl;
//    // Вывод данных
//    /*for (int i = 0; i < buffer_size; ++i) {
//        std::cout << "Значение регистра " << i << ": " << regs[i] << std::endl;
//    }*/
//
//    // Закрытие соединения и освобождение контекста
//    modbus_close(ctx);
//    modbus_free(ctx);
=======

   
    Data_logger data_lgger;
    data_lgger.init();


    Data_logger data_logger;
    data_logger.init();

   // data_logger.write_data();
    if (data_logger.read_data())
    {
        return 0;
    }
    data_logger.write_data();
  // data_logger.print_setup();
>>>>>>> Stashed changes

	return 0;
}