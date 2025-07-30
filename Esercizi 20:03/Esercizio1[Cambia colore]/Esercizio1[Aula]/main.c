#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

// Variabili globali per il colore di sfondo
float bgColor[3] = {0.2f, 0.3f, 0.4f}; // Colore iniziale (blu scuro)

// Funzione per controllare gli errori OpenGL
void checkError(const char *label)
{
    GLenum error;
    // Ciclo attraverso gli errori di OpenGL, se ce ne sono
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        // Stampa l'errore con il relativo label
        printf("%s: OpenGL Error %d\n", label, error);
    }
}

// Callback per la gestione dell'input da tastiera
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // Gestisce le azioni quando i tasti vengono premuti
    switch (key)
    {
    case GLFW_KEY_SPACE:
        // Se viene premuto il tasto SPAZIO, cambia il colore dello sfondo
        if (action == GLFW_PRESS)
        {
            // Genera valori casuali per il nuovo colore dello sfondo (RGB)
            bgColor[0] = (float)rand() / RAND_MAX; // Componente rossa
            bgColor[1] = (float)rand() / RAND_MAX; // Componente verde
            bgColor[2] = (float)rand() / RAND_MAX; // Componente blu
            // Aggiorna il colore di sfondo con il nuovo colore
            glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
            checkError("Color Update"); // Controlla se ci sono errori dopo l'aggiornamento
        }
        break;

    case GLFW_KEY_ESCAPE:
        // Se viene premuto il tasto ESC, chiude la finestra
        if (action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;

    default:
        break;
    }
}

int main(void)
{
    // Inizializzazione di GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Errore: Impossibile inizializzare GLFW\n");
        return -1;
    }

    // Creazione della finestra GLFW con dimensioni 800x600
    GLFWwindow *window = glfwCreateWindow(800, 800, "Cambia colori", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Errore: Impossibile creare la finestra GLFW\n");
        glfwTerminate();
        return -1;
    }

    // Imposto il contesto OpenGL per la finestra appena creata
    glfwMakeContextCurrent(window);
    // Imposto la funzione di callback per la gestione dei tasti
    glfwSetKeyCallback(window, key_callback);

    // Impostazione del colore iniziale di sfondo (definito nelle variabili globali)
    glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);

    // Ciclo di rendering principale
    while (!glfwWindowShouldClose(window))
    {
        // Pulisce il buffer del colore con il colore di sfondo definito
        glClear(GL_COLOR_BUFFER_BIT);

        // Scambia il buffer (mostra il contenuto renderizzato)
        glfwSwapBuffers(window);
        // Gestisce gli eventi della finestra (ad esempio, input da tastiera, chiusura finestra)
        glfwPollEvents();
    }

    // Distrugge la finestra e termina GLFW al termine del ciclo
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
