#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <stdlib.h>
#include <stdio.h>

int currentLight = 0;

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE); // normali uniformi anche dopo trasformazioni

    // Luce ambientale globale
    GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    // Osservatore vicino (non all'infinito)
    GLfloat viewer = 1.0;
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, viewer);

    // Calcoli anche per facce posteriori
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    // Disabilita tutte le luci inizialmente
    for (int i = 0; i < 3; ++i) glDisable(GL_LIGHT0 + i);
}

void setCurrentLight() {
    // Spegne tutte le luci
    for (int i = 0; i < 3; ++i) glDisable(GL_LIGHT0 + i);

    // Attiva solo la corrente
    switch (currentLight) {
        case 1: {
            GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
            // Luce rossa
            GLfloat pos[] = {0.0, 0.0, 3.0, 1.0}; // Z positivo → davanti al piano
            glLightfv(GL_LIGHT0, GL_DIFFUSE, red);
            glLightfv(GL_LIGHT0, GL_POSITION, pos);
            glEnable(GL_LIGHT0);
            break;
        }
        case 2: {
            GLfloat green[] = {0.0, 1.0, 0.0, 1.0};
            GLfloat pos[] = {-2.0, 1.0, 3.0, 1.0}; // anche qui Z = 3.0
            glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
            glLightfv(GL_LIGHT1, GL_POSITION, pos);
            glEnable(GL_LIGHT1);
            break;
        }
        case 3: {
            GLfloat blue[] = {0.0, 0.0, 1.0, 1.0};
            GLfloat pos[] = {-2.0, 1.0, 3.0, 1.0}; // anche qui Z = 3.0
            glLightfv(GL_LIGHT2, GL_DIFFUSE, blue);
            glLightfv(GL_LIGHT2, GL_POSITION, pos);
            glEnable(GL_LIGHT2);
            break;
        }
        default:
            break;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        printf("Pressed key: %d\n", key); // AGGIUNGI QUESTO
        if (key == GLFW_KEY_1) currentLight = 1;
        if (key == GLFW_KEY_2) currentLight = 2;
        if (key == GLFW_KEY_3) currentLight = 3;
        if (key == GLFW_KEY_0) currentLight = 0; // spegne tutto tranne ambient
    }
}

void drawSurface() {
    GLUquadric* quad = gluNewQuadric();
    gluSphere(quad, 1.5, 30, 30);
    gluDeleteQuadric(quad);
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Esercizio 2 - Illuminazione", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    

    glEnable(GL_DEPTH_TEST);
    setupLighting();
    
    GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);


    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, ratio, 1.0f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0.0, 0.0, 6.0,  0, 0, 0,  0, 1, 0);

        setCurrentLight();
        drawSurface();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
} 
