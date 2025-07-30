#include <GL/glut.h>

GLfloat ctrlpoints[4][4][3] = {
    {{0, 0, 0}, {1, 0, 0.5}, {2, 0, 1}, {3, 0, 0}},
    {{0, 1, 0}, {1, 1, 3},   {2, 1, 3}, {3, 1, 0}},
    {{0, 2, 0}, {1, 2, 3},   {2, 2, 3}, {3, 2, 0}},
    {{0, 3, 0}, {1, 3, 1},   {2, 3, 0.5}, {3, 3, 0}}
};

GLfloat pesi[4][4] = {
    {1, 0.5, 0.5, 1},
    {1, 0.5, 0.5, 1},
    {1, 0.5, 0.5, 1},
    {1, 0.5, 0.5, 1}
};

GLfloat cw[4][4][4];  // Coordinate omogenee

float rotateX = 330.0f, rotateY = 0.0f;
float zoom = -2.0f;

void calcolaOmogenee() {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            float w = pesi[i][j];
            cw[i][j][0] = ctrlpoints[i][j][0] * w;
            cw[i][j][1] = ctrlpoints[i][j][1] * w;
            cw[i][j][2] = ctrlpoints[i][j][2] * w;
            cw[i][j][3] = w;
        }
}

void init(void) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_MAP2_VERTEX_4);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    calcolaOmogenee();

    glMap2f(GL_MAP2_VERTEX_4,
            0.0, 1.0,         // u range
            4, 4,             // u stride, u order
            0.0, 1.0,         // v range
            16, 4,            // v stride, v order
            &cw[0][0][0]);

    glMapGrid2f(30, 0.0, 1.0, 30, 0.0, 1.0);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(-1.5, -1.5, zoom);
    glRotatef(rotateX, 1.0, 0.0, 0.0);
    glRotatef(rotateY, 0.0, 1.0, 0.0);

    // Superficie Bézier razionale
    glColor3f(0.8, 0.8, 0.8);
    glEvalMesh2(GL_FILL, 0, 30, 0, 30);

    // Punti di controllo
    glPointSize(8.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            glVertex3fv(ctrlpoints[i][j]);
    glEnd();

    // Linee di controllo: righe
    glColor3f(1.0, 1.0, 0.0);
    for (int i = 0; i < 4; i++) {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < 4; j++)
            glVertex3fv(ctrlpoints[i][j]);
        glEnd();
    }

    // Linee di controllo: colonne
    glColor3f(0.0, 1.0, 1.0);
    for (int j = 0; j < 4; j++) {
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < 4; i++)
            glVertex3fv(ctrlpoints[i][j]);
        glEnd();
    }

    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP: rotateX -= 5; break;
        case GLUT_KEY_DOWN: rotateX += 5; break;
        case GLUT_KEY_LEFT: rotateY -= 5; break;
        case GLUT_KEY_RIGHT: rotateY += 5; break;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == '-') zoom -= 1.0f;
    if (key == '+') zoom += 1.0f;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Superficie di Bézier Razionale - Omogenea");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
