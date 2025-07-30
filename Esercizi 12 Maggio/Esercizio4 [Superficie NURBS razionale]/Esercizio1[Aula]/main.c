#include <GL/glut.h>

GLfloat cp[4][4][3] = {
    {{-3, -3, -3}, {-3, -1, -3}, {-3, 1, -3}, {-3, 3, -3}},
    {{-1, -3, -3}, {-1, -1,  3}, {-1, 1,  3}, {-1, 3, -3}},
    {{ 1, -3, -3}, { 1, -1,  3}, { 1, 1,  3}, { 1, 3, -3}},
    {{ 3, -3, -3}, { 3, -1, -3}, { 3, 1, -3}, { 3, 3, -3}}
};

GLfloat pesi[4][4] = {
    {1, 0.5, 0.5, 1},
    {1, 0.5, 0.5, 1},
    {1, 0.5, 0.5, 1},
    {1, 0.5, 0.5, 1}
};

GLfloat cw[4][4][4]; // coordinate omogenee w*x, w*y, w*z, w

GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
GLUnurbsObj *theNurb;

float rotateX = 330.0, rotateY = 0.0, zoom = -20.0;

void calcolaCoordinateOmogenee() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            GLfloat w = pesi[i][j];
            cw[i][j][0] = cp[i][j][0] * w;
            cw[i][j][1] = cp[i][j][1] * w;
            cw[i][j][2] = cp[i][j][2] * w;
            cw[i][j][3] = w;
        }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(0.0, 0.0, zoom);
    glRotatef(rotateX, 1.0, 0.0, 0.0);
    glRotatef(rotateY, 0.0, 1.0, 0.0);

    // Superficie NURBS razionale
    gluBeginSurface(theNurb);
    gluNurbsSurface(theNurb, 8, knots, 8, knots,
                    4 * 4, 4, &cw[0][0][0], 4, 4,
                    GL_MAP2_VERTEX_4);
    gluEndSurface(theNurb);

    // Punti di controllo
    glPointSize(5.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            glVertex3fv(cp[i][j]);
    glEnd();

    // Linee di controllo: righe (gialle)
    glColor3f(1.0, 1.0, 0.0);
    for (int i = 0; i < 4; i++) {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < 4; j++)
            glVertex3fv(cp[i][j]);
        glEnd();
    }

    // Linee di controllo: colonne (azzurre)
    glColor3f(0.0, 1.0, 1.0);
    for (int j = 0; j < 4; j++) {
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < 4; i++)
            glVertex3fv(cp[i][j]);
        glEnd();
    }

    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 1.0, 40.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.1, 0.1, 1.0);

    theNurb = gluNewNurbsRenderer();
    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

    calcolaCoordinateOmogenee();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Superficie NURBS razionale (4x4)");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutMainLoop();
    return 0;
}
