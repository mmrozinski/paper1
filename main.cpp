#define STB_IMAGE_IMPLEMENTATION

#include "extern/stb_image.h"
#include <Windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>

#include "ChunkManager.h"

int win_height = 720;
int win_width = 1024;

int tickDelay = 50;
int subTickDelay = 1;

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
        std::cout << "FPS: " << fps << std::endl;
        std::cout << "Camera facing: " << camera.getFront().x << " " << camera.getFront().y << " " << camera.getFront().z << std::endl;
        std::cout << "Position: " << camera.getPosition().x << " " << camera.getPosition().y << " " << camera.getPosition().z << std::endl;
        std::cout << "Chunk position: " << Chunk::worldToChunkPosition(camera.getPosition()).x << " " << Chunk::worldToChunkPosition(camera.getPosition()).y << " " << Chunk::worldToChunkPosition(camera.getPosition()).z << std::endl;

        std::cout << std::endl << std::endl;
    }
}

HHOOK keyboardHook, mouseHook;

Vector3 movementVector(0, 0, 0);

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    keyboardTime = glutGet(GLUT_ELAPSED_TIME);
    if (nCode >= 0 && wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        auto* pKeyStruct = (KBDLLHOOKSTRUCT*)lParam;
        if (pKeyStruct->vkCode == VK_ESCAPE) {
            glutLeaveMainLoop();
        }

        if (pKeyStruct->vkCode == 'W') {
            movementVector.z = 1.0;
        }
        if (pKeyStruct->vkCode == 'S') {
            movementVector.z = -1.0;
        }
        if (pKeyStruct->vkCode == 'A') {
            movementVector.x = -1.0;
        }
        if (pKeyStruct->vkCode == 'D') {
            movementVector.x = 1.0;
        }
        if (pKeyStruct->vkCode == VK_SPACE) {
            movementVector.y = 1.0;
        }
        if (pKeyStruct->vkCode == VK_LSHIFT) {
            movementVector.y = -1.0;
        }

        if (pKeyStruct->vkCode == 'P') {
            doListUpdates = !doListUpdates;
        }

        if (pKeyStruct->vkCode == 'R') {
            chunkManager->breakBlock(camera);
        }

        if (pKeyStruct->vkCode == 'E') {
            chunkManager->placeBlock(camera);
        }
    }
    if (nCode >= 0 && wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
        auto* pKeyStruct = (KBDLLHOOKSTRUCT*)lParam;

        if (pKeyStruct->vkCode == 'W') {
            movementVector.z = 0.0;
        }
        if (pKeyStruct->vkCode == 'S') {
            movementVector.z = 0.0;
        }
        if (pKeyStruct->vkCode == 'A') {
            movementVector.x = 0.0;
        }
        if (pKeyStruct->vkCode == 'D') {
            movementVector.x = 0.0;
        }
        if (pKeyStruct->vkCode == VK_SPACE) {
            movementVector.y = 0.0;
        }
        if (pKeyStruct->vkCode == VK_LSHIFT) {
            movementVector.y = 0.0;
        }
    }
    keyboardTimebase = keyboardTime;
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_LBUTTONDOWN) {
            //chunkManager->breakBlock(camera);
        }
    }
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
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

void handleUpdateTick(int) {
    chunkManager->update(camera, doListUpdates, true);

    glutTimerFunc(tickDelay, handleUpdateTick, 0);
}

void handleUpdateSubTick(int) {
    keyboardTime = glutGet(GLUT_ELAPSED_TIME);

    chunkManager->update(camera, doListUpdates, false);

    camera.setPosition(
        camera.getPosition() + movementVector.x * camera.getRight() * speed * (keyboardTime - keyboardTimebase) +
        movementVector.y * camera.getUp() * speed * (keyboardTime - keyboardTimebase) +
        movementVector.z * camera.getFront() * speed * (keyboardTime - keyboardTimebase)
        );

    keyboardTimebase = keyboardTime;
    glutTimerFunc(subTickDelay, handleUpdateSubTick, 0);
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
    glutCreateWindow("cubes");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(handleDisplay);
    glutReshapeFunc(handleResize);
    glutPassiveMotionFunc(handleMouseMove);
    glutTimerFunc(1000 / fps_cap, timer, 0);
    glutTimerFunc(tickDelay, handleUpdateTick, 0);
    glutTimerFunc(subTickDelay, handleUpdateSubTick, 0);

    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, nullptr, 0);
    //mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, nullptr, 0);

    while(ShowCursor(false) >= 0);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures.png", &width, &height, &nrChannels, 0);

    TextureManager* textureManager = TextureManager::getInstance();
    textureManager->setTextureImage(data, width, height, nrChannels);

    stbi_image_free(data);
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
