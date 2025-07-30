#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "texture.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

GLuint fishTex1, fishTex2;
float timeValue = 0.0f;
float speed = 1.0f;

// Carica una texture RGBA
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

// Disegna un pesce con texture e angolo di rotazione
void drawFish(float x, float y, float angle, GLuint tex) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    
    // Inverti il pesce se l'angolo è negativo (sta andando verso sinistra)
    if (angle > 90 || angle < -90) {
        glScalef(-1.0f, 1.0f, 1.0f); // riflessione orizzontale
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

    // PESCE A - da sinistra verso destra
    float x1 = -2.0f + fmod(timeValue, 4.0f);
    float y1 = amplitude * sinf(2.0f * M_PI * x1 / wavelength);
    float dx1 = 1.0f;
    float dy1 = amplitude * (2.0f * M_PI / wavelength) * cosf(2.0f * M_PI * x1 / wavelength);
    float angle1 = atan2f(dy1, dx1) * 180.0f / M_PI;

    // PESCE B - da destra verso sinistra
    float x2 = 2.0f - fmod(timeValue, 4.0f);  // parte da destra
    float y2 = amplitude * sinf(2.0f * M_PI * x2 / wavelength);
    float dx2 = -1.0f;
    float dy2 = amplitude * (2.0f * M_PI / wavelength) * cosf(2.0f * M_PI * x2 / wavelength);
    float angle2 = atan2f(dy2, dx2) * 180.0f / M_PI;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.5, 2.5, -2.5, 2.5, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawFish(x1, y1, angle1, fishTex1); // pesce A
    drawFish(x2, y2, angle2, fishTex2); // pesce B

    glutSwapBuffers();
}


void idle(void) {
    timeValue += 0.01f;
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

void init(void) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fishTex1 = loadFishTexture("/Users/Bia/Desktop/fish/fish.rgba");
    fishTex2 = loadFishTexture("/Users/Bia/Desktop/fish/fisha.rgba");
    
    glClearColor(0.2f, 0.4f, 0.7f, 1.0f); // sfondo azzurro
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Due pesci su percorsi perpendicolari");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}

