#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

// Funzione per controllare eventuali errori OpenGL
void checkError(const char *label)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        printf("%s: OpenGL Error %d\n", label, error);
    }
}

// Funzione per disegnare un cerchio con gradiente dal blu al fucsia e centro nero
void drawCircleWithGradientAndBlackCenter(float centerX, float centerY, float outerRadius, float innerRadius, int segments)
{
    glEnable(GL_BLEND); // Abilita il blending per la trasparenza
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBegin(GL_TRIANGLE_STRIP); // Usa TRIANGLE_STRIP per creare il gradiente ad anello
    for (int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * M_PI * i / segments; // Calcola l'angolo attuale
        float xOuter = outerRadius * cos(angle);
        float yOuter = outerRadius * sin(angle);
        float xInner = innerRadius * cos(angle);
        float yInner = innerRadius * sin(angle);
        
        float distance = sqrt(xOuter * xOuter + yOuter * yOuter) / outerRadius; // Normalizza la distanza per il colore
        float red = 1.0 - distance; // Interpolazione per ottenere fucsia
        float green = 0.0;
        float blue = 1.0;
        
        glColor4f(red, green, blue, 1.0); // Imposta il colore del bordo
        glVertex2f(centerX + xOuter, centerY + yOuter);
        
        glColor4f(1.0, 0.0, 1.0, 1.0); // Fucsia verso l'interno
        glVertex2f(centerX + xInner, centerY + yInner);
    }
    glEnd();
    
    int segment = 360; // Suddividiamo il cerchio interno in 360 segmenti
    
    // Disegna il cerchio nero al centro
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(0.0, 0.0, 0.0, 1.0); // Imposta il colore nero
    glVertex2f(centerX, centerY); // Punto centrale
    
    for (int i = 0; i <= segment; i++) // Crea i punti per il cerchio interno
    {
        float angle = 2.0f * M_PI * i / segment;
        float x = innerRadius * cos(angle);
        float y = innerRadius * sin(angle);
        glVertex2f(centerX + x, centerY + y);
    }
    
    glEnd();
}

int main(void)
{
    // Inizializza GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Errore: Impossibile inizializzare GLFW\n");
        return -1;
    }

    // Crea una finestra di rendering
    GLFWwindow *window = glfwCreateWindow(800, 800, "Cerchio con sfumatura e centro nero", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Errore: Impossibile creare la finestra GLFW\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    checkError("glfwMakeContextCurrent");

    // Imposta la proiezione ortografica per la visualizzazione 2D
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glClearColor(0.0, 0.0, 0.0, 1.0); // Imposta il colore di sfondo (nero)

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT); // Pulisce lo schermo
        checkError("glClear");

        drawCircleWithGradientAndBlackCenter(0.0, 0.0, 0.5, 0.3, 20); // Disegna il cerchio con gradiente

        glfwSwapBuffers(window); // Scambia i buffer
        glfwPollEvents(); // Gestisce gli eventi della finestra
    }

    glfwDestroyWindow(window); // Chiude la finestra
    glfwTerminate(); // Termina GLFW
    return 0;
}
