#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

// Funzione per controllare gli errori di OpenGL
void checkError(const char *label)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        printf("%s: OpenGL Error %d\n", label, error);
    }
}

// Funzione per disegnare un poligono regolare con un gradiente di colore
void drawRegularPolygon(float centerX, float centerY, float radius, int segments)
{
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(centerX, centerY);

    for (int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * cos(angle);
        float y = radius * sin(angle);

        float dist = sqrt(x * x + y * y) / radius;
        glColor3f(1.0 - dist, 0.0, dist);
        glVertex2f(centerX + x, centerY + y);
    }
    glEnd();
    checkError("drawRegularPolygon");
}

// Funzione per gestire l'input della tastiera e cambiare il numero di lati
void handleInput(GLFWwindow *window, int *sides)
{
    static int lastStateUp = GLFW_RELEASE;
    static int lastStateDown = GLFW_RELEASE;

    int currentStateUp = glfwGetKey(window, GLFW_KEY_UP);
    int currentStateDown = glfwGetKey(window, GLFW_KEY_DOWN);

    if (currentStateUp == GLFW_PRESS && lastStateUp == GLFW_RELEASE)
    {
        *sides += 1;
    }
    if (currentStateDown == GLFW_PRESS && lastStateDown == GLFW_RELEASE && *sides > 3)
    {
        *sides -= 1;
    }

    lastStateUp = currentStateUp;
    lastStateDown = currentStateDown;
}

int main(void)
{
    if (!glfwInit())
    {
        fprintf(stderr, "Errore: Impossibile inizializzare GLFW\n");
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(800, 800, "Poligono regolare interattivo", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Errore: Impossibile creare la finestra GLFW\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    checkError("glfwMakeContextCurrent");

    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    int sides = 3;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        checkError("glClear");

        drawRegularPolygon(0.0, 0.0, 0.5, sides);

        glfwSwapBuffers(window);
        glfwPollEvents();

        handleInput(window, &sides);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
