#include <GL/glut.h>
#include <stdio.h>

GLfloat sphereX = -2.0, sphereY = 0.0, sphereZ = 0.0;  // Posizione iniziale della sfera
GLfloat cylinderX = 2.0, cylinderY = 0.0, cylinderZ = 0.0;  // Posizione iniziale del cilindro
GLfloat diskX = 0.0, diskY = -2.0, diskZ = 0.0;  // Posizione iniziale del disco

GLfloat sphereRotation = 0.0;  // Angolo di rotazione della sfera
GLfloat cylinderRotation = 0.0;  // Angolo di rotazione del cilindro
GLfloat diskRotation = 0.0;  // Angolo di rotazione del disco

// Funzione di gestione errori
void errorCallback(GLenum errorCode) {
    const GLubyte *errorString = gluErrorString(errorCode);
    fprintf(stderr, "Errore GLU: %s\n", errorString);
    exit(1);
}

// Funzione di illuminazione
void setupLighting(void) {
    GLfloat ambientLight[] = {0.2, 0.2, 0.2, 1.0}; // luce debole
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

    GLfloat diffuseLight[] = {1.0, 1.0, 1.0, 1.0}; // luce bianca
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    GLfloat lightPosition[] = {5.0, 5.0, 5.0, 0.0}; // Posizione della luce
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
}

// Funzione di disegno
void display(void) {
    GLUquadricObj *qobj = gluNewQuadric();
    gluQuadricCallback(qobj, GLU_ERROR, (GLvoid (*))errorCallback);
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluQuadricNormals(qobj, GLU_SMOOTH);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Disegna la sfera (colorata di rosso)
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(sphereX, sphereY, sphereZ);
    glRotatef(sphereRotation, 0.0, 1.0, 0.0);  // Ruota la sfera attorno all'asse Y
    gluSphere(qobj, 1.0, 20, 20);
    glPopMatrix();

    // Disegna il cilindro (colorato di verde)
    glColor3f(0.0, 1.0, 0.0);
    glPushMatrix();
    glTranslatef(cylinderX, cylinderY, cylinderZ);
    glRotatef(cylinderRotation, 1.0, 0.0, 0.0);  // Ruota il cilindro attorno all'asse X
    gluCylinder(qobj, 0.5, 0.5, 2.0, 20, 20);
    glPopMatrix();

    // Disegna il disco (colorato di blu)
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(diskX, diskY, diskZ);
    glRotatef(diskRotation, 0.0, 0.0, 1.0);  // Ruota il disco attorno all'asse Z
    gluDisk(qobj, 0.0, 1.0, 20, 20);
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

// Funzione di inizializzazione di OpenGL
void init(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);

    setupLighting();
}

// Funzione di gestione dei tasti
void keyboard(unsigned char key, int x, int y) {
    GLfloat moveSpeed = 0.1;
    GLfloat rotateSpeed = 5.0;  // Velocità di rotazione

    switch (key) {
        case 'w':  // Muove la sfera in alto
            sphereY += moveSpeed;
            break;
        case 's':  // Muove la sfera in basso
            sphereY -= moveSpeed;
            break;
        case 'a':  // Muove la sfera a sinistra
            sphereX -= moveSpeed;
            break;
        case 'd':  // Muove la sfera a destra
            sphereX += moveSpeed;
            break;
        case 'i':  // Muove il cilindro in alto
            cylinderY += moveSpeed;
            break;
        case 'k':  // Muove il cilindro in basso
            cylinderY -= moveSpeed;
            break;
        case 'j':  // Muove il cilindro a sinistra
            cylinderX -= moveSpeed;
            break;
        case 'l':  // Muove il cilindro a destra
            cylinderX += moveSpeed;
            break;
        case 'o':  // Muove il disco in alto
            diskY += moveSpeed;
            break;
        case 'p':  // Muove il disco in basso (modifica da 'u' a 'p')
            diskY -= moveSpeed;
            break;
        case 'q':  // Ruota la sfera a sinistra
            sphereRotation += rotateSpeed;
            break;
        case 'e':  // Ruota la sfera a destra
            sphereRotation -= rotateSpeed;
            break;
        case 'r':  // Ruota il cilindro a sinistra
            cylinderRotation += rotateSpeed;
            break;
        case 't':  // Ruota il cilindro a destra
            cylinderRotation -= rotateSpeed;
            break;
        case 'y':  // Ruota il disco a sinistra
            diskRotation += rotateSpeed;
            break;
        case 'u':  // Ruota il disco a destra
            diskRotation -= rotateSpeed;
            break;
        case 27:  // Tasto ESC per uscire
            exit(0);
            break;
    }

    glutPostRedisplay();
}

// Funzione principale
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Sfera, Cilindro e Disco in OpenGL con rotazione e movimento");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
