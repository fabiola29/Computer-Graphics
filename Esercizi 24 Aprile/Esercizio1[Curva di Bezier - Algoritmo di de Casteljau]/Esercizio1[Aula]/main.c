#include <GL/glut.h>
#include <stdio.h>

#define MAX_POINTS 10
#define STEP 100

typedef struct {
    float x, y;
} Point;

int numPoints = 0;
Point controlPoints[MAX_POINTS] = {
    {50, 250}, {150, 450}, {250, 50}, {350, 450}, {450, 250}
};

// Algoritmo di de Casteljau
Point deCasteljau(Point* P, int n, float t) {
    Point temp[MAX_POINTS];
    for (int i = 0; i <= n; i++) temp[i] = P[i];

    for (int k = 1; k <= n; k++) {
        for (int i = 0; i <= n - k; i++) {
            temp[i].x = (1 - t) * temp[i].x + t * temp[i + 1].x;
            temp[i].y = (1 - t) * temp[i].y + t * temp[i + 1].y;
        }
    }
    return temp[0];
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Punti di controllo
    glPointSize(6);
    glColor3f(1, 0, 0);
    glBegin(GL_POINTS);
    for (int i = 0; i < numPoints; i++)
        glVertex2f(controlPoints[i].x, controlPoints[i].y);
    glEnd();

    // Poligonale di controllo
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < numPoints; i++)
        glVertex2f(controlPoints[i].x, controlPoints[i].y);
    glEnd();

    // Curva di Bézier
    glColor3f(0, 0, 1);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= STEP; i++) {
        float t = (float)i / STEP;
        Point p = deCasteljau(controlPoints, numPoints - 1, t);
        glVertex2f(p.x, p.y);
    }
    glEnd();

    glFlush();
}

void init(void) {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0, 500, 0, 500);  // sistema di coordinate
}

int main(int argc, char** argv) {
    numPoints = 5;  // cambia questo numero per provare curve di grado diverso

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Curva di Bezier - Algoritmo di de Casteljau");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
