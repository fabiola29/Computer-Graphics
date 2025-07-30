#include <GLFW/glfw3.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "texture.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

GLuint fishTex;
float cameraZ = 8.0f;
GLint currentFilter = GL_LINEAR_MIPMAP_LINEAR; // default

GLuint loadFishTextureWithMipmaps(const char *filename) {
    int width, height, components;
    unsigned *texData = read_texture((char *)filename, &width, &height, &components);
    if (!texData) {
        fprintf(stderr, "Errore nel caricamento della texture '%s'.\n", filename);
        exit(EXIT_FAILURE);
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // Generazione automatica delle mipmap
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height,
                      GL_RGBA, GL_UNSIGNED_BYTE, texData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, currentFilter);

    free(texData);
    return texID;
}

void updateTextureFilter(void) {
    glBindTexture(GL_TEXTURE_2D, fishTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, currentFilter);
}

int pressed1 = 0, pressed2 = 0, pressed3 = 0, pressed4 = 0;

void processInput(GLFWwindow *window) {
    // Zoom
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cameraZ -= 0.05f;
        if (cameraZ < 1.0f) cameraZ = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cameraZ += 0.05f;
        if (cameraZ > 10.0f) cameraZ = 10.0f;
    }

    // Cambio filtro mipmap
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !pressed1) {
        currentFilter = GL_NEAREST_MIPMAP_NEAREST;
        updateTextureFilter();
        printf("Filtro: GL_NEAREST_MIPMAP_NEAREST\n");
        pressed1 = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE) pressed1 = 0;

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !pressed2) {
        currentFilter = GL_LINEAR_MIPMAP_NEAREST;
        updateTextureFilter();
        printf("Filtro: GL_LINEAR_MIPMAP_NEAREST\n");
        pressed2 = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE) pressed2 = 0;

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !pressed3) {
        currentFilter = GL_NEAREST_MIPMAP_LINEAR;
        updateTextureFilter();
        printf("Filtro: GL_NEAREST_MIPMAP_LINEAR\n");
        pressed3 = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE) pressed3 = 0;

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && !pressed4) {
        currentFilter = GL_LINEAR_MIPMAP_LINEAR;
        updateTextureFilter();
        printf("Filtro: GL_LINEAR_MIPMAP_LINEAR\n");
        pressed4 = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_RELEASE) pressed4 = 0;

}

void initScene(void) {
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1, 50.0);
    glMatrixMode(GL_MODELVIEW);

    fishTex = loadFishTextureWithMipmaps("/Users/Bia/Desktop/fish/fisha.rgba");

}

void drawFish(void) {
    glBindTexture(GL_TEXTURE_2D, fishTex);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, 0.0f);
        glTexCoord2f(1, 0); glVertex3f( 1.0f, -1.0f, 0.0f);
        glTexCoord2f(1, 1); glVertex3f( 1.0f,  1.0f, 0.0f);
        glTexCoord2f(0, 1); glVertex3f(-1.0f,  1.0f, 0.0f);
    glEnd();
}

void update(GLFWwindow* window) {
    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, cameraZ,
              0.0, 0.0, 0.0,
              0, 1, 0);

    drawFish();
    glfwSwapBuffers(window);
}

int main(void) {
    if (!glfwInit()) {
        fprintf(stderr, "Errore inizializzazione GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pesce con Mipmap", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Errore creazione finestra\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    initScene();

    while (!glfwWindowShouldClose(window)) {
        update(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
