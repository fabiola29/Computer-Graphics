#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 800
#define PI 3.14159265359

float secondAngle = 0.5f;
float minuteAngle = 0.5f;
float hourAngle =  0.5f;
int isRunning = 0;
double prevTime = 0.0;
float angleX = 0.0f, angleY = 0.0f;


void drawLine(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawNumbers(void) {
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 12; i++) {
        int displayNum = (i + 12) % 12;
        if (displayNum == 0) displayNum = 12;
        
        float angle = (i * -30.0f + 90.0f) * PI / 180.0f;
        float x = 0.45f * cos(angle);
        float y = 0.45f * sin(angle);
        glRasterPos2f(x - 0.02f, y - 0.02f);
        
        char num[3];
        snprintf(num, 3, "%d", displayNum);
        for (int j = 0; num[j] != '\0'; j++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, num[j]);
        }
    }
}

void drawClock(float secondAngle, float minuteAngle, float hourAngle) {
    glPushMatrix();
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    glColor3f(0.8f, 0.8f, 0.8f);
    glutSolidTorus(0.01, 0.5, 30, 30);
    drawNumbers();

    // Lancetta dei secondi - rossa (aggiungiamo un offset PI/2)
    glColor3f(1.0f, 0.0f, 0.0f);
    drawLine(0.0f, 0.0f, 0.4f * cos(PI/2 - secondAngle), 0.4f * sin(PI/2 - secondAngle));

    // Lancetta dei minuti - blu
    glColor3f(0.0f, 0.0f, 1.0f);
    drawLine(0.0f, 0.0f, 0.35f * cos(PI/2 - minuteAngle), 0.35f * sin(PI/2 - minuteAngle));

    // Lancetta delle ore - verde
    glColor3f(0.0f, 1.0f, 0.0f);
    drawLine(0.0f, 0.0f, 0.25f * cos(PI/2 - hourAngle), 0.25f * sin(PI/2 - hourAngle));

    glPopMatrix();
}


void updateClock(void) {
    if (isRunning) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - prevTime;
        if (deltaTime >= 1.0) {
            prevTime = currentTime;
            secondAngle += PI / 30.0f;
            if (secondAngle >= 2 * PI) {
                secondAngle -= 2 * PI;
                minuteAngle += PI / 30.0f;
                if (minuteAngle >= 2 * PI) {
                    minuteAngle -= 2 * PI;
                    hourAngle += PI / 6.0f;
                    if (hourAngle >= 2 * PI) {
                        hourAngle -= 2 * PI;
                    }
                }
            }
        }
    }
}

void handleMovement(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        angleX += 1.0f;
        if (angleX > 60.0f) angleX = 60.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        angleX -= 1.0f;
        if (angleX < -60.0f) angleX = -60.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        angleY -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        angleY += 1.0f;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_S) isRunning = 1;
        else if (key == GLFW_KEY_T) isRunning = 0;
        else if (key == GLFW_KEY_P) {
            isRunning = 0;
            secondAngle = 0.5f;
            minuteAngle = 0.5f;
            hourAngle = 0.5f;
        }
    }
}

int main(void) {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Orologio 3D", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glfwSetKeyCallback(window, key_callback);
    prevTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        handleMovement(window);
        updateClock();
        drawClock(secondAngle, minuteAngle, hourAngle);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
