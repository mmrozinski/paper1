#include <Windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "ChunkManager.h"

// pls make this a class

int win_height = 600;
int win_width = 800;

int frame = 0, deltaTime, timebase = 0;
int keyboardTime, keyboardTimebase = 0;
double fps;
ChunkManager* chunkManager;
Camera camera = Camera(Vector3(0.0f, 0.0f, 3.0f), 1.0f);
bool firstMove = true;
int lastPosX = 0;
int lastPosY = 0;
float sensitivity = 1.0f;
float speed = 0.01f;

bool doListUpdates = true;

float fps_cap = 144.0f;

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

HHOOK keyboardHook;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        keyboardTime = glutGet(GLUT_ELAPSED_TIME);

        auto* pKeyStruct = (KBDLLHOOKSTRUCT*)lParam;
        if (pKeyStruct->vkCode == VK_ESCAPE) {
            glutLeaveMainLoop();
        }

        if (pKeyStruct->vkCode == 'W') {
            camera.setPosition(camera.getPosition() + camera.getFront() * speed * ((keyboardTime - keyboardTimebase)));
        }
        if (pKeyStruct->vkCode == 'S') {
            camera.setPosition(camera.getPosition() - camera.getFront() * speed * ((keyboardTime - keyboardTimebase)));
        }
        if (pKeyStruct->vkCode == 'A') {
            camera.setPosition(camera.getPosition() - camera.getRight() * speed * ((keyboardTime - keyboardTimebase)));
        }
        if (pKeyStruct->vkCode == 'D') {
            camera.setPosition(camera.getPosition() + camera.getRight() * speed * ((keyboardTime - keyboardTimebase)));
        }
        if (pKeyStruct->vkCode == VK_SPACE) {
            camera.setPosition(camera.getPosition() + camera.getUp() * speed * ((keyboardTime - keyboardTimebase)));
        }
        if (pKeyStruct->vkCode == VK_LSHIFT) {
            camera.setPosition(camera.getPosition() - camera.getUp() * speed * ((keyboardTime - keyboardTimebase)));
        }

        if (pKeyStruct->vkCode == 'P') {
            doListUpdates = !doListUpdates;
        }
    }
    keyboardTimebase = keyboardTime;
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}


void handleDisplay() {
    frame++;
    deltaTime = glutGet(GLUT_ELAPSED_TIME);

    chunkManager->render();

    countFPS();
}

void handleResize(int sizeX, int sizeY) {
    glViewport(0, 0, sizeX, sizeY);

    camera.setAspectRatio(static_cast<float>(sizeX) / static_cast<float>(sizeY));

    win_width = sizeX;
    win_height = sizeY;
}

void handleUpdate(int) {
    chunkManager->update(camera, doListUpdates);

    glutTimerFunc(10, handleUpdate, 0);
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

    glutWarpPointer(win_width/2, win_height/2);
    lastPosX = win_width/2;
    lastPosY = win_height/2;
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000 / fps_cap, timer, 0);
}

void initGLUT(int *argc, char **argv) {
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("nice squares");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(handleDisplay);
    glutReshapeFunc(handleResize);
    glutPassiveMotionFunc(handleMouseMove);
    glutTimerFunc(1000 / fps_cap, timer, 0);
    glutTimerFunc(10, handleUpdate, 0);

    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, nullptr, 0);

    while(ShowCursor(false) >= 0);
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

    UnhookWindowsHookEx(keyboardHook);

    return 0;
}
