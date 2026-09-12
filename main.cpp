// file:    compocxx/main.cpp
// compile: g++ -std=c++20 main.cpp -lglfw -lGL
// run:     ./a.out

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

#include "compocxx.hpp"

struct theme {
    vector3 rosewater;
    vector3 flamingo;
    vector3 pink;
    vector3 mauve;
    vector3 red;
    vector3 maroon;
    vector3 peach;
    vector3 yellow;
    vector3 green;
    vector3 teal;
    vector3 sky;
    vector3 sapphire;
    vector3 blue;
    vector3 lavender;

    vector3 text;

    vector3 subtext1;
    vector3 subtext0;

    vector3 overlay2;
    vector3 overlay1;
    vector3 overlay0;

    vector3 surface2;
    vector3 surface1;
    vector3 surface0;

    vector3 base;
    vector3 mantle;
    vector3 crust;
};

const theme catppuccin_mocha = {
    .rosewater = {0.961, 0.878, 0.863},
    .flamingo  = {0.949, 0.804, 0.804},
    .pink      = {0.961, 0.761, 0.906},
    .mauve     = {0.796, 0.651, 0.969},
    .red       = {0.953, 0.545, 0.659},
    .maroon    = {0.922, 0.627, 0.675},
    .peach     = {0.980, 0.702, 0.529},
    .yellow    = {0.976, 0.886, 0.686},
    .green     = {0.651, 0.890, 0.631},
    .teal      = {0.580, 0.886, 0.835},
    .sky       = {0.537, 0.863, 0.922},
    .sapphire  = {0.455, 0.780, 0.925},
    .blue      = {0.537, 0.706, 0.980},
    .lavender  = {0.706, 0.745, 0.996},

    .text      = {0.804, 0.839, 0.957},

    .subtext1  = {0.729, 0.761, 0.871},
    .subtext0  = {0.651, 0.678, 0.784},

    .overlay2  = {0.576, 0.600, 0.698},
    .overlay1  = {0.498, 0.518, 0.612},
    .overlay0  = {0.424, 0.439, 0.525},

    .surface2  = {0.345, 0.357, 0.439},
    .surface1  = {0.271, 0.278, 0.353},
    .surface0  = {0.192, 0.196, 0.267},

    .base      = {0.118, 0.118, 0.180},
    .mantle    = {0.094, 0.094, 0.145},
    .crust     = {0.067, 0.067, 0.106},
};

auto* current_theme = &catppuccin_mocha;

void draw_rect(vector2 position,
               vector2 size,
               vector3 color) {
    glColor3f(color.r, color.g, color.b);

    glBegin(GL_QUADS);

    float x = position.x;
    float y = position.y;
    float width = size.x;
    float height = size.y;

    glVertex2f(x,         y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x,         y + height);

    glEnd();
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return EXIT_FAILURE;
    }

    GLFWwindow* window =
        glfwCreateWindow(1280, 720, "CompoCXX Game Engine", nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(current_theme->base.r,
                     current_theme->base.g,
                     current_theme->base.b,
                     1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        draw_rect({-0.5f, -0.5f},
                  { 1.0f,  1.0f},
                  current_theme->overlay0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
