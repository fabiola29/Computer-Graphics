#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 100
#define STEP 100  // numero di passi per il disegno della curva

typedef struct {
    float x, y;
} Point;

Point controlPoints[MAX_POINTS];
int pointCount = 0;

// Funzione per calcolare un punto sulla curva di Bézier cubica
Point deCasteljau(Point* P, float t) {
    Point A = {
        (1 - t) * P[0].x + t * P[1].x,
        (1 - t) * P[0].y + t * P[1].y
    };
    Point B = {
        (1 - t) * P[1].x + t * P[2].x,
        (1 - t) * P[1].y + t * P[2].y
    };
    Point C = {
        (1 - t) * P[2].x + t * P[3].x,
        (1 - t) * P[2].y + t * P[3].y
    };
    Point D = {
        (1 - t) * A.x + t * B.x,
        (1 - t) * A.y + t * B.y
    };
    Point E = {
        (1 - t) * B.x + t * C.x,
        (1 - t) * B.y + t * C.y
    };
    Point F = {
        (1 - t) * D.x + t * E.x,
        (1 - t) * D.y + t * E.y
    };
    return F;
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Disegna i punti di controllo
    glPointSize(5);
    glColor3f(1, 0, 0);
    glBegin(GL_POINTS);
    for (int i = 0; i < pointCount; i++) {
        glVertex2f(controlPoints[i].x, controlPoints[i].y);
    }
    glEnd();

    // Disegna le linee di controllo
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < pointCount; i++) {
        glVertex2f(controlPoints[i].x, controlPoints[i].y);
    }
    glEnd();

    // Disegna le curve di Bézier composite
    glColor3f(0, 0, 1);
    for (int i = 0; i <= pointCount - 4; i += 3) {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= STEP; j++) {
            float t = (float)j / STEP;
            Point p = deCasteljau(&controlPoints[i], t);
            glVertex2f(p.x, p.y);
        }
        glEnd();
    }

    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && pointCount < MAX_POINTS) {
        controlPoints[pointCount].x = x;
        controlPoints[pointCount].y = 500 - y;  // Inverti l'asse y
        pointCount++;
        glutPostRedisplay();
    }
}

void init(void) {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Curve di Bézier Composite - Input da Mouse");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
