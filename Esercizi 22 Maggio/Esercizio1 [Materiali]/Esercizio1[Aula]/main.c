#include <GL/glut.h>
#include "stdbool.h"

// Angoli per la rotazione della scena
float angleX = 0.0, angleY = 0.0;
int lastX, lastY;
bool rotating = false;

void initLighting() {
    
    // Luce 0 - vicino alla sfera rossa
    GLfloat light0_pos[] = { 2.0, 2.0, 2.0, 1.0 };
    GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glEnable(GL_LIGHT0);

    // Luce 1 - vicino al cono blu
    GLfloat light1_pos[] = { -2.0, 2.0, -2.0, 1.0 };
    GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glEnable(GL_LIGHT1);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void drawObjects() {
    // Variabili comuni
    GLfloat no_specular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat shininess_low = 0.0;

    // Sfera rossa lucida
    GLfloat red_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat red_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat shininess_high = 80.0;

    glMaterialfv(GL_FRONT, GL_DIFFUSE, red_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, red_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess_high);
    glPushMatrix();
        glTranslatef(-2.0, 0.0, 0.0);
        glutSolidSphere(1.0, 50, 50);
    glPopMatrix();

    // Cono blu matto (stessi parametri)
    GLfloat blue_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess_low);
    glPushMatrix();
        glTranslatef(2.0, 0.0, 0.0);
        glutSolidCone(1.0, 2.0, 50, 50);
    glPopMatrix();

    // Toro giallo fosforescente (emissione)
    GLfloat yellow_emission[] = { 1.0, 1.0, 0.0, 1.0 };
    GLfloat black_diffuse[] = { 0.0, 0.0, 0.0, 1.0 };

    glMaterialfv(GL_FRONT, GL_DIFFUSE, black_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, yellow_emission);
    glPushMatrix();
        glTranslatef(0.0, -2.0, 0.0);
        glutSolidTorus(0.3, 1.0, 50, 50);
    glPopMatrix();

    // Reset emission
    GLfloat no_emission[] = { 0.0, 0.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        glRotatef(angleX, 1.0, 0.0, 0.0);
        glRotatef(angleY, 0.0, 1.0, 0.0);
        drawObjects();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w/h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 8.0,  // eye
              0.0, 0.0, 0.0,   // center
              0.0, 1.0, 0.0);  // up
}

// Mouse interaction
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        rotating = (state == GLUT_DOWN);
        lastX = x;
        lastY = y;
    }
}

void motion(int x, int y) {
    if (rotating) {
        angleY += (x - lastX);
        angleX += (y - lastY);
        lastX = x;
        lastY = y;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Materiali in OpenGL");

    initLighting();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}
