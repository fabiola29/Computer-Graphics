#include <GL/glut.h>
#include <stdio.h>

// Controllo punti della superficie (4x4x3)
GLfloat cp[4][4][3] = {
    {{-3, -3, -3}, {-3, -1, -3}, {-3, 1, -3}, {-3, 3, -3}},
    {{-1, -3, -3}, {-1, -1,  3}, {-1, 1,  3}, {-1, 3, -3}},
    {{ 1, -3, -3}, { 1, -1,  3}, { 1, 1,  3}, { 1, 3, -3}},
    {{ 3, -3, -3}, { 3, -1, -3}, { 3, 1, -3}, { 3, 3, -3}}
};

// Knot vector per la superficie (uniforme per entrambi)
GLfloat knotu[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
GLfloat knotv[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};

// Contorno interno (curva chiusa, 7 punti)
// Esterno: quadrato chiuso in (u,v)
GLfloat E[5][2] = {
    {0.0, 0.0},
    {1.0, 0.0},
    {1.0, 1.0},
    {0.0, 1.0},
    {0.0, 0.0}
};

// Interno: forma a “loop” chiuso all'interno [0.3, 0.7]
GLfloat P[8][2] = {
    {0.3, 0.5},
    {0.35, 0.65},
    {0.5, 0.7},
    {0.65, 0.65},
    {0.7, 0.5},
    {0.65, 0.35},
    {0.5, 0.3},
    {0.3, 0.5}
};


// Knot vector trimming interno per curva P (grado 3, 7 punti => 11 nodi)
GLfloat knot_trim_internal[11] = {0,0,0,0, 0.25, 0.5, 0.75, 1,1,1,1};

// Contorno esterno (polilinea chiusa)

GLUnurbsObj *theNurb;

float rotateX = 330.0, rotateY = 0.0, zoom = -20.0;

void nurbsError(GLenum errorCode) {
    const GLubyte *estring = gluErrorString(errorCode);
    fprintf(stderr, "Nurbs error: %s\n", estring);
}

void checkGLError(void) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "GL ERROR: %s\n", gluErrorString(error));
    }
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 40.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void specialKeys(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP: rotateX -= 5; break;
        case GLUT_KEY_DOWN: rotateX += 5; break;
        case GLUT_KEY_LEFT: rotateY -= 5; break;
        case GLUT_KEY_RIGHT: rotateY += 5; break;
    }
    glutPostRedisplay();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();

    glTranslatef(0.0, 0.0, zoom);
    glRotatef(rotateX, 1.0, 0.0, 0.0);
    glRotatef(rotateY, 0.0, 1.0, 0.0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // → inserisci prima di gluBeginSurface

    gluBeginSurface(theNurb);

    gluNurbsSurface(theNurb,
                    8, knotu,
                    8, knotv,
                    4 * 3, 3,
                    &cp[0][0][0],
                    4, 4,
                    GL_MAP2_VERTEX_3);

    // Trimming esterno
    gluBeginTrim(theNurb);
    gluPwlCurve(theNurb, 5, &E[0][0], 2, GLU_MAP1_TRIM_2);
    gluEndTrim(theNurb);

    // Trimming interno (loop chiuso)
    gluBeginTrim(theNurb);
    gluPwlCurve(theNurb, 8, &P[0][0], 2, GLU_MAP1_TRIM_2);
    gluEndTrim(theNurb);


    gluEndSurface(theNurb);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // → dopo gluEndSurface


    glPopMatrix();

    checkGLError();
    glutSwapBuffers();
}

void init(void) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    GLfloat light_pos[] = {0.0f, 0.0f, 10.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    theNurb = gluNewNurbsRenderer();
    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

    gluNurbsCallback(theNurb, GLU_ERROR, (GLvoid (*)()) nurbsError);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Superficie NURBS con trimming");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutMainLoop();

    return 0;
}
