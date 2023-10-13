#include <GL/freeglut.h>
#include <iostream>

int frame = 0, deltaTime, timebase = 0;
double fps;

void countFPS() {
    if (deltaTime - timebase > 1000) {
        fps = frame * 1000.0 / (deltaTime - timebase);
        timebase = deltaTime;
        frame = 0;
        std::cout << fps << std::endl;
    }
}

void handleDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glBegin(GL_POLYGON);
    glColor3f(1, 0, 0);
    glVertex3f(-0.6, -0.75, 0.5);
    glColor3f(0, 1, 0);
    glVertex3f(0.6, -0.75, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0.75, 0);
    glEnd();

    frame++;
    deltaTime = glutGet(GLUT_ELAPSED_TIME);

    countFPS();

    glFlush();
}

void handleKeyboardInput(unsigned char key, int x, int y) {
    if (key == 27) {
        glutLeaveMainLoop();
    }
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

void initGLUT(int *argc, char **argv) {
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(800, 600);
    glutCreateWindow("Bartke");

    glutDisplayFunc(handleDisplay);
    glutKeyboardFunc(handleKeyboardInput);
    glutTimerFunc(1000 / 60, timer, 0);
}

int main(int argc, char **argv) {
    initGLUT(&argc, argv);

    glutMainLoop();

    return 0;
}
