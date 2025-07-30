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

GLuint fishTex1, fishTex2, fishTex3,fishTex4;
float timeValue = 0.0f;
float speed = 0.02f;

GLuint loadFishTexture(const char *filename) {
    int width, height, components;
    unsigned *texData = read_texture((char *)filename, &width, &height, &components);

    if (!texData) {
        fprintf(stderr, "Errore nel caricamento della texture '%s'.\n", filename);
        exit(EXIT_FAILURE);
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, texData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(texData);
    return texID;
}

void initScene() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1, 20.0);
    glMatrixMode(GL_MODELVIEW);

    fishTex1 = loadFishTexture("/Users/Bia/Desktop/fish/fisha.rgba");
    fishTex2 = loadFishTexture("/Users/Bia/Desktop/fish/fisha.rgba");
    fishTex3 = loadFishTexture("/Users/Bia/Desktop/fish/fisha.rgba");
    fishTex4 = loadFishTexture("/Users/Bia/Desktop/fish/fisha.rgba");

}

void update(GLFWwindow* window) {
    timeValue += speed;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 3.0,
              0.0, 0.0, 0.0,
              0, 1, 0);

    glPushMatrix();
    glRotatef(timeValue * 50.0f, 0.0f, 1.0f, 0.0f);  // rotazione
    glRotatef(timeValue * 20.0f, 1.0f, 0.0f, 0.0f);  // effetto "rotolamento"

    // Disegna sfera blu scuro senza texture
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.1f, 0.1f, 0.3f);
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_FALSE);
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluSphere(quad, 1.0, 50, 50);
    gluDeleteQuadric(quad);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);  // reset colore

    /// Pesce 1 – fronte
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 1.01f);
    glBindTexture(GL_TEXTURE_2D, fishTex1);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-0.2f, -0.2f, 0.0f);
        glTexCoord2f(1, 0); glVertex3f( 0.2f, -0.2f, 0.0f);
        glTexCoord2f(1, 1); glVertex3f( 0.2f,  0.2f, 0.0f);
        glTexCoord2f(0, 1); glVertex3f(-0.2f,  0.2f, 0.0f);
    glEnd();
    glPopMatrix();

    // Pesce 2 – retro
    glPushMatrix();
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 1.01f);
    glBindTexture(GL_TEXTURE_2D, fishTex2);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-0.2f, -0.2f, 0.0f);
        glTexCoord2f(1, 0); glVertex3f( 0.2f, -0.2f, 0.0f);
        glTexCoord2f(1, 1); glVertex3f( 0.2f,  0.2f, 0.0f);
        glTexCoord2f(0, 1); glVertex3f(-0.2f,  0.2f, 0.0f);
    glEnd();
    glPopMatrix();

    // Pesce 3 – destra
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 1.01f);
    glBindTexture(GL_TEXTURE_2D, fishTex3);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-0.2f, -0.2f, 0.0f);
        glTexCoord2f(1, 0); glVertex3f( 0.2f, -0.2f, 0.0f);
        glTexCoord2f(1, 1); glVertex3f( 0.2f,  0.2f, 0.0f);
        glTexCoord2f(0, 1); glVertex3f(-0.2f,  0.2f, 0.0f);
    glEnd();
    glPopMatrix();

    // Pesce 4 – sinistra
    glPushMatrix();
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 1.01f);
    glBindTexture(GL_TEXTURE_2D, fishTex4);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-0.2f, -0.2f, 0.0f);
        glTexCoord2f(1, 0); glVertex3f( 0.2f, -0.2f, 0.0f);
        glTexCoord2f(1, 1); glVertex3f( 0.2f,  0.2f, 0.0f);
        glTexCoord2f(0, 1); glVertex3f(-0.2f,  0.2f, 0.0f);
    glEnd();
    glPopMatrix();

    glPopMatrix();
    glfwSwapBuffers(window);
}

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Errore inizializzazione GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Sfera con Due Pesci", NULL, NULL);
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
