#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "ChunkManager.h"

// pls make this a class

int frame = 0, deltaTime, timebase = 0;
double fps;
ChunkManager* chunkManager;
Camera camera = Camera(Vector3(0.0f, 0.0f, 3.0f), 1.0f);
bool firstMove = true;
int lastPosX = 0;
int lastPosY = 0;
float sensitivity = 1.0f;

void countFPS() {
    if (deltaTime - timebase > 1000) {
        fps = frame * 1000.0 / (deltaTime - timebase);
        timebase = deltaTime;
        frame = 0;
        std::cout << fps << std::endl;
        std::cout << camera.getFront().x << camera.getFront().y << camera.getFront().z << std::endl;
        std::cout << camera.getPitch() << camera.getYaw() << std::endl;
    }
}

void handleDisplay() {
    chunkManager->update(camera);

    frame++;
    deltaTime = glutGet(GLUT_ELAPSED_TIME);

    chunkManager->render();

    countFPS();
}

void handleResize(int sizeX, int sizeY) {
    glViewport(0, 0, sizeX, sizeY);

    camera.setAspectRatio(static_cast<float>(sizeX) / static_cast<float>(sizeY));
}

void handleKeyboardInput(unsigned char key, int x, int y) {
    if (key == 27) {
        glutLeaveMainLoop();
    }
}

void handleMouseMove(int x, int y) {
    if (firstMove)
    {
        lastPosX = x;
        lastPosY = y;
        firstMove = false;
    }
    else
    {
        float deltaX = static_cast<float>(x) - static_cast<float>(lastPosX);
        float deltaY = static_cast<float>(y) - static_cast<float>(lastPosY);
        lastPosX = x;
        lastPosY = y;

        camera.setYaw(camera.getYaw() + deltaX * sensitivity);
        camera.setPitch(camera.getPitch() - deltaY * sensitivity);
    }

    glutWarpPointer(800/2, 600/2);
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

void initGLUT(int *argc, char **argv) {
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(800, 600);
    glutCreateWindow("nice squares");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(handleDisplay);
    glutReshapeFunc(handleResize);
    glutKeyboardFunc(handleKeyboardInput);
    glutPassiveMotionFunc(handleMouseMove);
    glutTimerFunc(1000 / 60, timer, 0);
}

int main(int argc, char **argv) {
    initGLUT(&argc, argv);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }

    chunkManager = new ChunkManager();
    camera.setYaw(0);

    glutMainLoop();

    delete chunkManager;

    return 0;
}
