#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>

GLUnurbsObj *theNurb;

void nurbsError(GLenum errorCode) {
    const GLubyte *estring;
    estring = gluErrorString(errorCode);
    fprintf(stderr, "Errore NURBS: %s\n", estring);
    exit(1);
}

void drawLabel(float x, float y, float z, const char *label) {
    glRasterPos3f(x, y, z);
    for (const char* c = label; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}


void init(void) {
    theNurb = gluNewNurbsRenderer();
    gluNurbsProperty(theNurb, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
    gluNurbsProperty(theNurb, GLU_U_STEP, 100);
    gluNurbsCallback(theNurb, GLU_ERROR, (GLvoid (*)(void)) nurbsError);

    glClearColor(1.0, 1.0, 1.0, 1.0); // Sfondo bianco
    glColor3f(0.0, 0.0, 0.0);         // Curva nera
}

void drawControlPoints(GLfloat cpw[7][4]) {
    glPointSize(5.0);
    glColor3f(1.0, 0.0, 0.0); // Rosso
    glBegin(GL_POINTS);
    for (int i = 0; i < 7; i++) {
        glVertex3f(cpw[i][0], cpw[i][1], cpw[i][2]);
    }
    glEnd();

    glColor3f(0.0, 0.0, 0.0); // Nero per il testo
      drawLabel(cpw[0][0] + 0.1f, cpw[0][1] + 0.1f, cpw[0][2], "P0 = P6");
      drawLabel(cpw[1][0] + 0.1f, cpw[1][1] + 0.1f, cpw[1][2], "P1");
      drawLabel(cpw[2][0] + 0.1f, cpw[2][1] + 0.1f, cpw[2][2], "P2");
      drawLabel(cpw[3][0] + 0.1f, cpw[3][1] + 0.1f, cpw[3][2], "P3"); // Aggiungi P4
      drawLabel(cpw[4][0] + 0.1f, cpw[4][1] + 0.1f, cpw[4][2], "P4");
      drawLabel(cpw[5][0] + 0.1f, cpw[5][1] + 0.1f, cpw[5][2], "P5");
}

void drawControlLines(GLfloat cpw[7][4]) {
    glEnable(GL_LINE_STIPPLE);               // Abilita il tratteggio
    glLineStipple(1, 0x00FF);                // Pattern tratteggiato (alternanza 1-bit on/off)
    glColor3f(0.0, 0.0, 0.0);                // Colore nero

    glBegin(GL_LINES);
    for (int i = 0; i < 6; i++) {
        glVertex3f(cpw[i][0], cpw[i][1], cpw[i][2]);
        glVertex3f(cpw[i + 1][0], cpw[i + 1][1], cpw[i + 1][2]);
    }
    glEnd();

    glDisable(GL_LINE_STIPPLE);             // Disabilita dopo il disegno
}


void display(void) {
    GLfloat cpw[7][4] = {
        {1.0, 2.0, 0.0, 1.0},
        {1.0, 3.0, 0.0, 1.0},
        {3.0, 3.0, 0.0, 1.0},
        {3.0, 2.0, 0.0, 1.0},
        {3.0, 1.0, 0.0, 1.0},
        {1.0, 1.0, 0.0, 1.0},
        {1.0, 2.0, 0.0, 1.0}
    };

    GLfloat knots[] = {0.0, 0.0, 0.0, 0.25, 0.5, 0.5, 0.75, 1.0, 1.0, 1.0};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();  // Reset matrice modello-vista

    drawControlPoints(cpw);
    drawControlLines(cpw);

    glColor3f(0.0, 0.0, 1.0); // Curva NURBS blu

    gluBeginCurve(theNurb);
    gluNurbsCurve(theNurb, 10, knots, 4, &cpw[0][0], 3, GL_MAP1_VERTEX_3);
    gluEndCurve(theNurb);

    glFlush();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Centrare la curva basandosi sui suoi punti (x: 1→3, y: 1→3) → margine ±1
    GLfloat left = 0.0f;
    GLfloat right = 4.0f;
    GLfloat bottom = 0.0f;
    GLfloat top = 4.0f;

    // Mantieni proporzione corretta tra larghezza e altezza
    if (w <= h)
        gluOrtho2D(left, right, bottom * (GLfloat)h / (GLfloat)w, top * (GLfloat)h / (GLfloat)w);
    else
        gluOrtho2D(left * (GLfloat)w / (GLfloat)h, right * (GLfloat)w / (GLfloat)h, bottom, top);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Curva NURBS");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
