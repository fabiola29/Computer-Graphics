#include <GL/glut.h>

GLfloat ctrlpoints[4][4][3] = {
    {{0, 0, 0}, {1, 0, 0.5}, {2, 0, 1}, {3, 0, 0}},
    {{0, 1, 0}, {1, 1, 3},   {2, 1, 3}, {3, 1, 0}},
    {{0, 2, 0}, {1, 2, 3},   {2, 2, 3}, {3, 2, 0}},
    {{0, 3, 0}, {1, 3, 1},   {2, 3, 0.5}, {3, 3, 0}}
};

// Variabili globali per rotazione
float rotateX = 330.0f;
float rotateY = 0.0f;

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MAP2_VERTEX_3);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glMapGrid2f(30, 0.0, 1.0, 30, 0.0, 1.0);

    // Luce
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_pos[] = { 0.0, 0.0, 2.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Superficie Bézier riempita
    glColor3f(0.8, 0.8, 0.8);
    glPushMatrix();
    glRotatef(rotateX, 1.0, 0.0, 0.0);
    glRotatef(rotateY, 0.0, 1.0, 0.0);
    glTranslatef(-1.5, -1.5, 0.0);
    glEvalMesh2(GL_FILL, 0, 30, 0, 30);
    glPopMatrix();

    // Griglia wireframe
    glColor3f(0.0, 1.0, 0.0);
    glPushMatrix();
    glRotatef(rotateX, 1.0, 0.0, 0.0);
    glRotatef(rotateY, 0.0, 1.0, 0.0);
    glTranslatef(-1.5, -1.5, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEvalMesh2(GL_LINE, 0, 30, 0, 30);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPopMatrix();

    // Griglia dei punti di controllo con GL_LINE_STRIP
    glDisable(GL_LIGHTING); // Disattiva luce per colore puro
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glRotatef(rotateX, 1.0, 0.0, 0.0);
    glRotatef(rotateY, 0.0, 1.0, 0.0);
    glTranslatef(-1.5, -1.5, 0.0);

    // Linee per righe
    for (int i = 0; i < 4; i++) {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < 4; j++) {
            glVertex3fv(ctrlpoints[i][j]);
        }
        glEnd();
    }

    // Linee per colonne
    for (int j = 0; j < 4; j++) {
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < 4; i++) {
            glVertex3fv(ctrlpoints[i][j]);
        }
        glEnd();
    }
    
    // Disegna i punti di controllo
    glColor3f(1.0, 0.0, 1.0);  // Viola
    glPointSize(5.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            glVertex3fv(ctrlpoints[i][j]);
    glEnd();

    glPopMatrix();
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 10.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
}

// Gestione input tastiera per frecce
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            rotateX -= 5.0f;
            break;
        case GLUT_KEY_DOWN:
            rotateX += 5.0f;
            break;
        case GLUT_KEY_LEFT:
            rotateY -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            rotateY += 5.0f;
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Superficie di Bézier con griglia");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutMainLoop();
    return 0;
}
