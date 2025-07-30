#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLvoid drawTriangle(GLfloat red, GLfloat green, GLfloat blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5, -0.5);
    glVertex2f(0.5, -0.5);
    glVertex2f(0.0, 0.5);
    glEnd();
}

GLvoid drawScene(GLvoid) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Triangolo nero (base)
    drawTriangle(0.0, 0.0, 0.0);
    
    // Traslazione a destra e disegno grigio scuro
    glPushMatrix();
    glTranslatef(0.5, 0.0, 0.0);
    drawTriangle(0.3, 0.3, 0.3);
    glPopMatrix();
    
    // Traslazione e rotazione, disegno grigio chiaro
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(-15.0, 0.0, 0.0, 1.0);
    drawTriangle(0.7, 0.7, 0.7);
    glPopMatrix();
    
    // Traslazione e scaling, disegno bianco
    glPushMatrix();
    glTranslatef(1.5, 0.0, 0.0);
    glScalef(-0.5, 0.5, 0.5);
    drawTriangle(1.0, 1.0, 1.0);
    glPopMatrix();
    
    glFlush();
}

int main(void) {
    if (!glfwInit()) {
        fprintf(stderr, "Errore nell'inizializzazione di GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Esempio", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Errore nella creazione della finestra GLFW\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Errore nell'inizializzazione di GLEW\n");
        return -1;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 2, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    
    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        drawScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
