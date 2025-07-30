#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define _USE_MATH_DEFINES
#define MAX_BALLS 100
#define MAX_GIFTS 50
#define MAX_LIGHTS 50

typedef struct {
    float x, y;
} Point;

float angle = 0.0f;
float offsetY = 0.0f;
float zoom = 1.0f;
int mouseX, mouseY;
bool dragging = false;

// Posizioni statiche
int numGifts = 15;

typedef struct {
    float x, y;
    int colorIndex;
} Gift;

Gift giftPositions[MAX_GIFTS];


//Point ballPositions[MAX_BALLS];
int numBalls = 100;

typedef struct {
    float x, y;
    float r, g, b;
} ColoredPoint;

ColoredPoint ballPositions[MAX_BALLS];


Point lightPositions[MAX_LIGHTS];
int numLights = 50;
bool lightOn = true;

void toggleLights(int value) {
    lightOn = !lightOn;
    glutPostRedisplay();
    glutTimerFunc(500, toggleLights, 0);
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        dragging = (state == GLUT_DOWN);
        mouseX = x;
        mouseY = y;
    }
    else if (button == 3) zoom *= 1.1f;
    else if (button == 4) zoom /= 1.1f;
    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    if (dragging) {
        angle += (x - mouseX) * 0.5f;
        offsetY += (y - mouseY) * 0.005f;
        mouseX = x;
        mouseY = y;
        glutPostRedisplay();
    }
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) angle += 5.0f;
    if (key == GLUT_KEY_RIGHT) angle -= 5.0f;
    if (key == GLUT_KEY_UP) offsetY += 0.05f;
    if (key == GLUT_KEY_DOWN) offsetY -= 0.05f;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'A') zoom *= 1.1f;
    if (key == 'B') zoom /= 1.1f;
    glutPostRedisplay();
}

void drawGift(float x, float y, float size, int colorIndex) {
    float colors[4][3] = {
        {0.8f, 0.0f, 0.0f},   // Rosso
        {0.0f, 0.6f, 0.0f},   // Verde
        {0.0f, 0.0f, 0.8f},   // Blu
        {0.9f, 0.9f, 0.0f}    // Giallo
    };

    glColor3f(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);

    glBegin(GL_QUADS);
    glVertex2f(x - size, y - size); glVertex2f(x + size, y - size);
    glVertex2f(x + size, y + size); glVertex2f(x - size, y + size);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);  // Fiocco bianco
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(x, y - size); glVertex2f(x, y + size);
    glVertex2f(x - size, y); glVertex2f(x + size, y);
    glEnd();
}


void drawGifts() {
    float size = 0.025f;
    for (int i = 0; i < numGifts; i++)
        drawGift(giftPositions[i].x, giftPositions[i].y, size, giftPositions[i].colorIndex);
}


void generateGifts() {
    float yMin = -0.15f, yMax = 0.0f, xMin = -0.5f, xMax = 0.5f;
    float size = 0.025f, minDist = size * 2.5f, excludeWidth = 0.07f;
    int count = 0;
    while (count < numGifts) {
        float x, y;
        int attempts = 0;
        bool valid;
        do {
            y = yMin + (float)rand() / RAND_MAX * (yMax - yMin);
            do {
                x = xMin + (float)rand() / RAND_MAX * (xMax - xMin);
            } while (x > -excludeWidth && x < excludeWidth);
            valid = true;
            for (int j = 0; j < count; j++) {
                float dx = x - giftPositions[j].x;
                float dy = y - giftPositions[j].y;
                if (sqrtf(dx * dx + dy * dy) < minDist) valid = false;
            }
            attempts++;
        } while (!valid && attempts < 100);
        int colorIndex = rand() % 4;
        giftPositions[count++] = (Gift){x, y, colorIndex};
    }
}


float halfWidthAtY(float y, float yBottom, float yTop, float halfBase) {
    float t = (yTop - y) / (yTop - yBottom);
    return halfBase * fmaxf(0.0f, fminf(1.0f, t));
}

void drawBalls() {
    float radius = 0.015f, borderRadius = radius * 1.1f;
    for (int i = 0; i < numBalls; i++) {
        float x = ballPositions[i].x;
        float y = ballPositions[i].y;
        float r = ballPositions[i].r;
        float g = ballPositions[i].g;
        float b = ballPositions[i].b;

        // bordo nero
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int j = 0; j <= 20; j++) {
            float angle = j * 2.0f * M_PI / 20;
            glVertex2f(x + cos(angle) * borderRadius, y + sin(angle) * borderRadius);
        }
        glEnd();

        // colore salvato
        glColor3f(r, g, b);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int j = 0; j <= 20; j++) {
            float angle = j * 2.0f * M_PI / 20;
            glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
        }
        glEnd();
    }
}


void generateBalls() {
    float yBottom = 0.15f, yTop = 0.7f;
    int levels = 6; // numero di fasce orizzontali
    float radius = 0.015f;
    int count = 0;

    for (int level = 0; level < levels; level++) {
        float t = (float)level / (levels - 1);
        float y = yBottom + t * (yTop - yBottom);

        // Calcola metà larghezza dell'albero a questa altezza
        float halfWidth = halfWidthAtY(y, yBottom, yTop, 0.4f);
        int ballsPerLevel = 4 + (int)(4 * (1.0f - t)); // più palline in basso

        for (int i = 0; i < ballsPerLevel && count < MAX_BALLS; i++) {
            float fx = (float)i / (ballsPerLevel - 1); // 0.0 a 1.0
            float x = -halfWidth + fx * 2 * halfWidth;

            // Colore casuale
            float r = (float)(rand() % 100) / 100.0f;
            float g = (float)(rand() % 100) / 100.0f;
            float b = (float)(rand() % 100) / 100.0f;

            ballPositions[count++] = (ColoredPoint){x, y, r, g, b};
        }
    }

    numBalls = count;
}


void generateLights() {
    float yBottom = 0.1f, yTop = 0.7f;
    float treeHeight = yTop - yBottom;
    float treeBaseWidth = 0.8f; // intera larghezza alla base

    for (int i = 0; i < numLights; i++) {
        float t = (float)i / numLights;
        float angle = t * 10.0f * M_PI; // 5 giri completi
        float radius = (treeBaseWidth / 2.0f) * (1.0f - t); // restringe verso l'alto
        float x = radius * cos(angle);
        float y = yBottom + t * treeHeight;

        lightPositions[i].x = x;
        lightPositions[i].y = y;
    }
}


void drawLights() {
    float r = 0.01f;
    for (int i = 0; i < numLights; i++) {
        // Alternanza accensione: pari/dispari
        bool luceAccesa = (i % 2 == 0 && lightOn) || (i % 2 != 0 && !lightOn);

        // Colore giallo acceso o grigio spento
        if (luceAccesa) {
            glColor3f(1.0f, 1.0f, 0.0f); // giallo
        } else {
            glColor3f(0.2f, 0.2f, 0.2f); // grigio spento
        }

        float x = lightPositions[i].x;
        float y = lightPositions[i].y;

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int j = 0; j <= 20; j++) {
            float a = j * 2.0f * M_PI / 20;
            glVertex2f(x + cos(a) * r, y + sin(a) * r);
        }
        glEnd();
    }
}


void drawLightWire() {
    glColor3f(0.3f, 0.3f, 0.3f); // Colore del filo: grigio scuro
    glLineWidth(1.0f); // Spessore del filo

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < numLights; i++) {
        glVertex2f(lightPositions[i].x, lightPositions[i].y);
    }
    glEnd();
}


void drawTrunkWithRoundedBottomCorners(void) {
    float baseY = -0.1f, topY = 0.1f, halfWidth = 0.05f, radius = 0.05f;
    glColor3f(0.55f, 0.27f, 0.07f);

    glBegin(GL_QUADS);
    glVertex2f(-halfWidth, topY); glVertex2f(halfWidth, topY);
    glVertex2f(halfWidth, baseY + radius); glVertex2f(-halfWidth, baseY + radius);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(-halfWidth + radius, baseY + radius);
    for (int i = 0; i <= 18; ++i) {
        float angle = M_PI + i * M_PI / 18.0f;
        glVertex2f(-halfWidth + radius + cosf(angle) * radius, baseY + radius + sinf(angle) * radius);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(halfWidth - radius, baseY + radius);
    for (int i = 0; i <= 18; ++i) {
        float angle = M_PI + i * M_PI / 18.0f;
        glVertex2f(halfWidth - radius + cosf(angle) * radius, baseY + radius + sinf(angle) * radius);
    }
    glEnd();
}

void drawStar(float x, float y, float size) {
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 5; ++i) {
        float angle1 = i * 2.0f * M_PI / 5.0f;
        float angle2 = (i + 2) * 2.0f * M_PI / 5.0f;
        glVertex2f(x, y);
        glVertex2f(x + cos(angle1) * size, y + sin(angle1) * size);
        glVertex2f(x + cos(angle2) * size, y + sin(angle2) * size);
    }
    glEnd();
}

void drawTree(void) {
    glColor3f(0.0f, 0.3f, 0.0f);
    float yBottom = 0.1f, yTop = 0.7f, step = 0.01f;
    glBegin(GL_LINES);
    glVertex2f(0.0f, yBottom); glVertex2f(0.0f, yTop);
    for (float y = yBottom; y <= yTop - step; y += step) {
        float t = (y - yBottom) / (yTop - yBottom);
        glVertex2f(0.0f, y); glVertex2f(-0.4f * t, y + step);
        glVertex2f(0.0f, y); glVertex2f(0.4f * t, y + step);
    }
    glEnd();
}

void display(void) {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(0, offsetY, 0);
    glScalef(zoom, zoom, 1);
    glRotatef(angle, 0, 0, 1);

    glPushMatrix();
    glTranslatef(0.0f, 0.4f, 0.0f);
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, -0.4f, 0.0f);
    drawTree();
    glPopMatrix();

    drawBalls();

    drawTrunkWithRoundedBottomCorners();
    drawGifts();
    drawLightWire();
    drawLights();

    glColor3f(1, 1, 0);
    drawStar(0, 0.74f, 0.04f);

    glPopMatrix();
    glutSwapBuffers();
}

void init(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -0.2, 1.0);
    srand(time(NULL));
    generateGifts();
    generateBalls();
    generateLights();
    glutTimerFunc(500, toggleLights, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Albero di Natale");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);

    glutMainLoop();
    return 0;
}
