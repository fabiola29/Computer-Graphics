#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <stdlib.h>
#include <math.h>

float angleX = 0.0f, angleY = 0.0f;
float lastX, lastY;
int dragging = 0;

// Gestione click mouse
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        dragging = (action == GLFW_PRESS);
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        lastX = xpos;
        lastY = ypos;
    }
}

// Trascinamento per rotazione
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (dragging) {
        float dx = xpos - lastX;
        float dy = ypos - lastY;
        angleX += dx * 0.5f;
        angleY += dy * 0.5f;
        lastX = xpos;
        lastY = ypos;
    }
}

// Disegna due sfere intersecanti
void drawSpheres() {
    GLUquadric* quad = gluNewQuadric();

    glPushMatrix();
        glTranslatef(-0.5f, 0.0f, 0.0f);
        gluSphere(quad, 1.0, 30, 30);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.5f, 0.0f, 0.0f);
        gluSphere(quad, 1.0, 30, 30);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Sfere Intersecanti con Luci", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // Setup iniziale OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    // Colori delle luci
    GLfloat white_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_diffuse);

    GLfloat red_diffuse[] = {1.0, 0.0, 0.0, 1.0};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, red_diffuse);

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // PROIEZIONE
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, ratio, 0.1f, 100.0f);

        // MODELVIEW
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0.0, 0.0, 6.0, 0, 0, 0, 0, 1, 0);

        // ➤ LUCE ROSSA: fissa rispetto alla camera (definita subito dopo gluLookAt)
        GLfloat red_pos[] = {0.0, 0.0, 1.0, 0.0}; // direzionale verso Z-
        glLightfv(GL_LIGHT1, GL_POSITION, red_pos);

        // ➤ Rotazioni della scena con il mouse
        glRotatef(angleY, 1.0f, 0.0f, 0.0f);
        glRotatef(angleX, 0.0f, 1.0f, 0.0f);

        // ➤ LUCE BIANCA: si muove con le sfere (definita dopo le rotazioni)
        GLfloat white_pos[] = {1.0, 1.0, 1.0, 1.0};
        glLightfv(GL_LIGHT0, GL_POSITION, white_pos);

        // Disegno
        drawSpheres();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
