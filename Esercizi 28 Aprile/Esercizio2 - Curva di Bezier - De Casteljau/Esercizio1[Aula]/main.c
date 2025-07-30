#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <GL/glut.h>

#define MAX_POINTS 10
#define STEP 100

typedef struct {
    float x, y;
} Point;

int numPoints = 5;
Point controlPoints[MAX_POINTS] = {
    {50, 250}, {150, 450}, {250, 50}, {350, 450}, {450, 250}
};

// Algoritmo De Casteljau
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

void drawCurve(int mode) {
    // Disegna punti di controllo
    glPointSize(6);
    glColor3f(1, 0, 0);
    glBegin(GL_POINTS);
    for (int i = 0; i < numPoints; i++) {
        glVertex2f(controlPoints[i].x, controlPoints[i].y);
    }
    glEnd();

    // Disegna poligonale di controllo
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < numPoints; i++) {
        glVertex2f(controlPoints[i].x, controlPoints[i].y);
    }
    glEnd();

    // Disegna curva
    glColor3f(0, 0, 1);

    if (mode == 0) {
        // Metodo De Casteljau manuale
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= STEP; i++) {
            float t = (float)i / STEP;
            Point p = deCasteljau(controlPoints, numPoints - 1, t);
            glVertex2f(p.x, p.y);
        }
        glEnd();
    } else {
        // Metodo OpenGL glMap1f + glEvalMesh1
        GLfloat ctrl[MAX_POINTS][3];
        for (int i = 0; i < numPoints; i++) {
            ctrl[i][0] = controlPoints[i].x;
            ctrl[i][1] = controlPoints[i].y;
            ctrl[i][2] = 0.0f;
        }

        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, numPoints, &ctrl[0][0]);
        glEnable(GL_MAP1_VERTEX_3);

        glMapGrid1f(STEP, 0.0, 1.0);
        glEvalMesh1(GL_LINE, 0, STEP);

        glDisable(GL_MAP1_VERTEX_3);
    }
}

void setupView() {
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(void) {
    if (!glfwInit()) {
        fprintf(stderr, "Errore inizializzazione GLFW\n");
        return -1;
    }

    GLFWwindow* window1 = glfwCreateWindow(600, 600, "Curva di Bezier - De Casteljau", NULL, NULL);
    if (!window1) {
        fprintf(stderr, "Errore creazione finestra 1\n");
        glfwTerminate();
        return -1;
    }

    GLFWwindow* window2 = glfwCreateWindow(600, 600, "Curva di Bezier - OpenGL glMap1f", NULL, NULL);
    if (!window2) {
        fprintf(stderr, "Errore creazione finestra 2\n");
        glfwDestroyWindow(window1);
        glfwTerminate();
        return -1;
    }

    glewInit();

    // Impostazione contesti
    glfwMakeContextCurrent(window1);
    setupView();

    glfwMakeContextCurrent(window2);
    setupView();

    while (!glfwWindowShouldClose(window1) && !glfwWindowShouldClose(window2)) {
        // Finestra 1
        glfwMakeContextCurrent(window1);
        glClear(GL_COLOR_BUFFER_BIT);
        drawCurve(0); // De Casteljau
        glfwSwapBuffers(window1);

        // Finestra 2
        glfwMakeContextCurrent(window2);
        glClear(GL_COLOR_BUFFER_BIT);
        drawCurve(1); // OpenGL glMap1f
        glfwSwapBuffers(window2);

        glfwPollEvents();
    }

    glfwDestroyWindow(window1);
    glfwDestroyWindow(window2);
    glfwTerminate();
    return 0;
}
