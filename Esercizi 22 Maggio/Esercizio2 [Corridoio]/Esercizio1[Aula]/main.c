#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#define WIDTH 800
#define HEIGHT 600

float cameraSpeed = 0.1f;
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 1.0f;
float lookX = 0.0f, lookY = 0.0f, lookZ = -1.0f;
float angleY = 0.0f;

void drawCorridor() {
    // Pavimento blu
    GLfloat matAmbient[]  = {0.0f, 0.0f, 0.3f, 1.0f};
    GLfloat matDiffuse[]  = {0.0f, 0.0f, 1.0f, 1.0f};
    GLfloat matSpecular[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat matShininess  = 32.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

    glBegin(GL_QUADS);
        glVertex3f(-5, -1, -20);
        glVertex3f( 5, -1, -20);
        glVertex3f( 5, -1,  0);
        glVertex3f(-5, -1,  0);
    glEnd();

    // Soffitto bianco
    GLfloat matAmbientWhite[]  = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat matDiffuseWhite[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat matSpecularWhite[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat matShininessWhite  = 64.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbientWhite);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuseWhite);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecularWhite);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matShininessWhite);

    glBegin(GL_QUADS);
        glVertex3f(-5, 1, -20);
        glVertex3f( 5, 1, -20);
        glVertex3f( 5, 1,  0);
        glVertex3f(-5, 1,  0);
    glEnd();

    // Pareti beige
    GLfloat matAmbientBeige[]  = {0.9f, 0.8f, 0.6f, 1.0f};
    GLfloat matDiffuseBeige[]  = {0.96f, 0.87f, 0.70f, 1.0f};
    GLfloat matSpecularBeige[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat matShininessBeige  = 8.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbientBeige);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuseBeige);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecularBeige);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matShininessBeige);

    for (int i = -1; i <= 1; i += 2) {
        glBegin(GL_QUADS);
            glVertex3f(i * 5, -1, -20);
            glVertex3f(i * 5, -1,  0);
            glVertex3f(i * 5,  1,  0);
            glVertex3f(i * 5,  1, -20);
        glEnd();
    }

    // Applique viola semi-trasparente
    glDisable(GL_LIGHTING); // disabilita luci per la trasparenza
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.5f, 0.0f, 0.5f, 0.5f);

    for (int i = 0; i < 5; ++i) {
        float z = -3.0f * i - 2.0f;
        for (int side = -1; side <= 1; side += 2) {
            glBegin(GL_QUADS);
                glVertex3f(side * 4.9f,  0.2f, z - 0.2f);
                glVertex3f(side * 4.9f, -0.2f, z - 0.2f);
                glVertex3f(side * 4.9f, -0.2f, z + 0.2f);
                glVertex3f(side * 4.9f,  0.2f, z + 0.2f);
            glEnd();
        }
    }
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING); // riabilita luci
    glColor4f(1,1,1,1);    // reset colore
}

void update(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) angleY -= 0.03f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) angleY += 0.03f;

    lookX = sin(angleY);
    lookZ = -cos(angleY);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraX += lookX * cameraSpeed;
        cameraZ += lookZ * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraX -= lookX * cameraSpeed;
        cameraZ -= lookZ * cameraSpeed;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ,
              cameraX + lookX, cameraY + lookY, cameraZ + lookZ,
              0, 1, 0);

    drawCorridor();
    glfwSwapBuffers(window);
}

void initScene() {
    glEnable(GL_DEPTH_TEST);

    // Luci
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightPos[] = {0.0f, 5.0f, 5.0f, 1.0f};
    GLfloat lightAmbient[]  = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat lightDiffuse[]  = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0, (double)WIDTH / HEIGHT, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Corridoio con luce", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    initScene();

    while (!glfwWindowShouldClose(window)) {
        update(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
