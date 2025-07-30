#include <GL/glut.h>

GLfloat ctlpoints[4][4][3] = {
    {{-3, -3, -3}, {-3, -1, -3}, {-3, 1, -3}, {-3, 3, -3}},
    {{-1, -3, -3}, {-1, -1, 3},  {-1, 1, 3},  {-1, 3, -3}},
    {{1, -3, -3},  {1, -1, 3},   {1, 1, 3},   {1, 3, -3}},
    {{3, -3, -3},  {3, -1, -3},  {3, 1, -3},  {3, 3, -3}}
};

GLUnurbsObj *theNurb;
GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0}; // ordine 4 = molteplicità 4

float rotateX = 330.0, rotateY = 0.0, zoom = -20.0;

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);  // Colore bianco
    glPushMatrix();
    glTranslatef(0.0, 0.0, zoom);
    glRotatef(rotateX, 1.0, 0.0, 0.0);
    glRotatef(rotateY, 0.0, 1.0, 0.0);

    // Superficie NURBS
    gluBeginSurface(theNurb);
    gluNurbsSurface(theNurb,
        8, knots,
        8, knots,
        4 * 3, 3,
        &ctlpoints[0][0][0],
        4, 4,
        GL_MAP2_VERTEX_3);
    gluEndSurface(theNurb);

    // Punti di controllo
    glPointSize(5.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            glVertex3fv(ctlpoints[i][j]);
    glEnd();

    // Linee di controllo: righe (gialle)
    glColor3f(1.0, 1.0, 0.0);
    for (int i = 0; i < 4; i++) {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < 4; j++)
            glVertex3fv(ctlpoints[i][j]);
        glEnd();
    }

    // Linee di controllo: colonne (azzurre)
    glColor3f(0.0, 1.0, 1.0);
    for (int j = 0; j < 4; j++) {
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < 4; i++)
            glVertex3fv(ctlpoints[i][j]);
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

void init(void) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.1, 0.1, 1.0);

    glDisable(GL_LIGHTING);  // << DISATTIVA ILLUMINAZIONE
    glDisable(GL_LIGHT0);

    theNurb = gluNewNurbsRenderer();
    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Superficie B-Spline (NURBS) di ordine 4x4");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutMainLoop();
    return 0;
}
