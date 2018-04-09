#ifndef RULES_H
#define RULES_H

#include <iostream>
#include <cmath>

class Rules {
    public:
        static bool validMove(char board[][8], int fromI, int fromJ, int toI, int toJ, char turn);
        static bool isClear(char board[][8], int fromI, int fromJ, int toI, int toJ);
        static bool isCheck(char board[][8], char player);
        static bool isCheck(char board[][8], char player, int kingI, int kingJ);
        static bool isCheckMate(char board[][8], char player);
};

#endif