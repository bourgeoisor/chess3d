#ifndef GAME_H
#define GAME_H

#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <vector>

#include "vao.h"
#include "rules.h"

class Game {
    public:
        static void start(int *argc, char *argv[]);

    private:
        static void init();
        static void display();
        static void handleNewSelection(int i, int j);
        static void mouseInput(int button, int state, int x, int y);
        static void mouseMotion(int x, int y);
        static void motionTimer(int value);
        static void resetMessageTimer(int value);
        static void reshape(int newWidth, int newHeight);

        static GLuint boardObj;
        static GLuint kingObj;
        static GLuint queenObj;
        static GLuint bishopObj;
        static GLuint knightObj;
        static GLuint rookObj;
        static GLuint pawnObj;

        static float cameraAlpha;
        static float cameraBeta;
        static float cameraRadius;

        static bool mouseRightClick;
        static int motionLastX;
        static int motionLastY;

        static char board[8][8];
        static int selectionI;
        static int selectionJ;
        static char turn;

        static std::string msgWhite;
        static std::string msgBlack;

        static std::vector<std::vector<int>> stars;
};

#endif