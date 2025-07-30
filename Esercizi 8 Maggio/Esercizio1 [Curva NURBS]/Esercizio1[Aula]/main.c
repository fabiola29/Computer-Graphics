#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

// Puntatore all'oggetto NURBS
GLUnurbsObj *theNurb;

// Punti di controllo per la curva NURBS (x, y, z)
GLfloat cp[4][3] = {
    { -4.0, -4.0, 0.0},  // Punto iniziale
    { -2.0,  4.0, 0.0},  // Punto intermedio 1
    {  2.0, -4.0, 0.0},  // Punto intermedio 2
    {  4.0,  4.0, 0.0}   // Punto finale
};

// Vettore dei nodi per curva NURBS di ordine 4 (grado 3 + 1)
GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};

// Callback per gestione errori NURBS
void nurbsError(GLenum errorCode) {
    const GLubyte *estring = gluErrorString(errorCode);
    fprintf(stderr, "Errore NURBS: %s\n", estring);
    exit(0);
}

// Funzione di disegno
void display(void) {
    // Pulisce il buffer colore e profondità
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    // Scala tutta la scena al 50%
    glScaled(0.5, 0.5, 0.5);

    // Disegna la curva NURBS in verde
    glColor3f(0.0, 1.0, 0.0); // Colore verde
    gluBeginCurve(theNurb);
        gluNurbsCurve(theNurb, 8, knots, 3, &cp[0][0], 4, GL_MAP1_VERTEX_3);
    gluEndCurve(theNurb);

    // Disegna solo i punti di controllo iniziale e finale in rosso
    glPointSize(6.0);
    glColor3f(1.0, 0.0, 0.0); // Colore rosso
    glBegin(GL_POINTS);
        glVertex3fv(cp[0]);  // Punto iniziale
        glVertex3fv(cp[3]);  // Punto finale
    glEnd();

    glPopMatrix();
    glFlush();
    glutSwapBuffers();  // Scambia i buffer per animazione fluida (double buffering)
}

// Inizializzazione di OpenGL e NURBS
void init(void) {
    // Colore di sfondo nero (RGBA)
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Abilita test di profondità
    glEnable(GL_DEPTH_TEST);

    // Crea l'oggetto NURBS e imposta le sue proprietà
    theNurb = gluNewNurbsRenderer();
    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0); // Tolleranza di campionamento
    gluNurbsCallback(theNurb, GLU_ERROR, (void (*)(void)) nurbsError); // Callback per errori

    // Imposta la proiezione ortogonale 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-4.0, 4.0, -4.0, 4.0);

    // Modalità modello-vista
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Funzione principale
int main(int argc, char **argv) {
    glutInit(&argc, argv);  // Inizializzazione di GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Double buffer + RGB + Depth
    glutInitWindowSize(800, 800);  // Dimensione finestra
    glutCreateWindow("Curva NURBS - Ordine 4");  // Titolo finestra

    init();  // Chiamata a funzione di inizializzazione
    glutDisplayFunc(display);  // Registra funzione di rendering
    glutMainLoop();  // Avvia il loop principale di GLUT
    return 0;
}
