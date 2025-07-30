#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "texture.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

GLuint fishTex;
float timeValue = 0.0f;
float speed = 1.0f;

float xPos = -2.0f;
int direction = 1;


GLuint loadFishTexture(const char *filename) {
    int width, height, components;
    unsigned *texData = read_texture((char *)filename, &width, &height, &components);

    if (!texData) {
        fprintf(stderr, "Errore caricamento texture '%s'\n", filename);
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

void drawFish(float x, float y, GLuint tex, int direction) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    // Se va verso sinistra, riflessione orizzontale
    if (direction == -1) {
        glScalef(-1.0f, 1.0f, 1.0f);
    }

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(-0.1f, -0.1f);
        glTexCoord2f(1, 0); glVertex2f( 0.1f, -0.1f);
        glTexCoord2f(1, 1); glVertex2f( 0.1f,  0.1f);
        glTexCoord2f(0, 1); glVertex2f(-0.1f,  0.1f);
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    float amplitude = 0.5f;
    float wavelength = 2.0f;

    float x = xPos;
    float y = amplitude * sinf(2.0f * M_PI * x / wavelength);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.5, 2.5, -2.5, 2.5, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawFish(x, y, fishTex, direction);

    glutSwapBuffers();
}


void idle(void) {
    xPos += 0.01f * direction;

    if (xPos > 2.0f) {
        direction = -1;
    } else if (xPos < -2.0f) {
        direction = 1;
    }

    glutPostRedisplay();
}


void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

void init(void) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fishTex = loadFishTexture("/Users/Bia/Desktop/fish/fish.rgba");

    glClearColor(0.2f, 0.4f, 0.7f, 1.0f); // sfondo azzurro
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Pesce su traiettoria sinusoidale");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
