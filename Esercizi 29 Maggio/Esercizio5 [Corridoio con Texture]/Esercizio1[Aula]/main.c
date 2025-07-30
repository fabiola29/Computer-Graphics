#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "texture.h" // Leggi texture RGBA in memoria

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#define WIDTH 800
#define HEIGHT 600

float cameraSpeed = 0.1f;
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 1.0f;
float lookX = 0.0f, lookY = 0.0f, lookZ = -1.0f;
float angleY = 0.0f;

GLuint texWall, texFloor;

GLuint loadTextureFromFile(const char *filename) {
    int width, height, channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 4);
    if (!data) {
        fprintf(stderr, "Failed to load texture %s\n", filename);
        return 0;
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // Carica i dati della texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Genera mipmap
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Imposta filtri (usa mipmap per MIN_FILTER)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return texID;
}



GLuint createCheckerTexture(int size, int color1[3], int color2[3]) {
    unsigned char *data = malloc(size * size * 3);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int checker = ((x / 8) % 2) ^ ((y / 8) % 2);
            int *color = checker ? color1 : color2;
            int i = (y * size + x) * 3;
            data[i + 0] = color[0];
            data[i + 1] = color[1];
            data[i + 2] = color[2];
        }
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, size, size, GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(data);
    return texID;
}

void loadTextures() {
    texWall = loadTextureFromFile("/Users/Bia/Desktop/OpenGL/Texture/walks_texture.jpg");
    texFloor = loadTextureFromFile("/Users/Bia/Desktop/OpenGL/Texture/floor_texture.jpg");

    if (!texWall || !texFloor) {
        fprintf(stderr, "Error loading textures\n");
        exit(1);
    }
}



void drawCorridor() {
    // Pavimento
    glBindTexture(GL_TEXTURE_2D, texFloor);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-5, -1, -20);
        glTexCoord2f(5, 0); glVertex3f( 5, -1, -20);
        glTexCoord2f(5, 5); glVertex3f( 5, -1,  0);
        glTexCoord2f(0, 5); glVertex3f(-5, -1,  0);
    glEnd();

    // Soffitto bianco (senza texture)
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glVertex3f(-5, 1, -20);
        glVertex3f( 5, 1, -20);
        glVertex3f( 5, 1,  0);
        glVertex3f(-5, 1,  0);
    glEnd();
    glEnable(GL_TEXTURE_2D);

    // Pareti laterali
    glBindTexture(GL_TEXTURE_2D, texWall);
    for (int i = -1; i <= 1; i += 2) {
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f(i * 5, -1, -20);
            glTexCoord2f(5, 0); glVertex3f(i * 5, -1,  0);
            glTexCoord2f(5, 2); glVertex3f(i * 5,  1,  0);
            glTexCoord2f(0, 2); glVertex3f(i * 5,  1, -20);
        glEnd();
    }

    // Applique viola con alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.5, 0.0, 0.5, 0.5); // viola semi-trasparente

    for (int i = 0; i < 5; ++i) {
        float z = -3.0f * i - 2.0f;
        for (int side = -1; side <= 1; side += 2) {
            glBegin(GL_QUADS);
                glVertex3f(side * 4.9,  0.2, z - 0.2);
                glVertex3f(side * 4.9, -0.2, z - 0.2);
                glVertex3f(side * 4.9, -0.2, z + 0.2);
                glVertex3f(side * 4.9,  0.2, z + 0.2);
            glEnd();
        }
    }
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}


void update(GLFWwindow* window) {
    // Rotazione con frecce
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) angleY -= 0.03f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) angleY += 0.03f;

    // Aggiorna direzione della telecamera in base a angleY
    lookX = sin(angleY);
    lookZ = -cos(angleY);

    // Movimento avanti/indietro relativo alla direzione
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraX += lookX * cameraSpeed;
        cameraZ += lookZ * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraX -= lookX * cameraSpeed;
        cameraZ -= lookZ * cameraSpeed;
    }

    // Rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ,
              cameraX + lookX, cameraY + lookY, cameraZ + lookZ,
              0, 1, 0);

    drawCorridor();
    glfwSwapBuffers(window);
}


void initScene() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0, (double)WIDTH / HEIGHT, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    loadTextures();
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Corridoio", NULL, NULL);
    if (!window) {
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
