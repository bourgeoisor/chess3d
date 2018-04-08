#include "rules.h"

// Check that a move attempt is valid.
bool Rules::validMove(char board[][8], int fromI, int fromJ, int toI, int toJ, char turn) {
    char piece = board[fromI][fromJ];
    int dy = abs(fromI - toI);
    int dx = abs(fromJ - toJ);

    // Any pawn
    if (piece == 'P' || piece == 'p') {
        // White pawn
        if (piece == 'P') {
            if (fromI != 6 && dy > 1) return false;
            if (fromI == 6 && dy > 2) return false;
            if ((fromI - toI) < 0) return false;
        }
        // Black pawn
        else {
            if (fromI != 1 && dy > 1) return false;
            if (fromI == 1 && dy > 2) return false;
            if ((fromI - toI) > 0) return false;
        }
        
        if (dx > 0 && (dx != 1 || dy != 1 || board[toI][toJ] == ' ')) return false;
        if (dy > 0 && dx == 0 && board[toI][toJ] != ' ') return false;
        if (!isClear(board, fromI, fromJ, toI, toJ)) return false;
    }

    // Any rook
    if (piece == 'R' || piece == 'r') {
        if (dx > 0 && dy > 0) return false;
        if (!isClear(board, fromI, fromJ, toI, toJ)) return false;
    }

    // Any knight
    if (piece == 'N' || piece == 'n') {
        if (!((dx == 1 && dy == 2) || (dx == 2 && dy == 1))) return false;
    }

    // Any bishop
    if (piece == 'B' || piece == 'b') {
        if (dx != dy) return false;
        if (!isClear(board, fromI, fromJ, toI, toJ)) return false;
    }

    // Any queen
    if (piece == 'Q' || piece == 'q') {
        if (dx > 0 && dy > 0 && dx != dy) return false;
        if (!isClear(board, fromI, fromJ, toI, toJ)) return false;
    }

    // Any king
    if (piece == 'K' || piece == 'k') {
        if (dx > 1 || dy > 1) return false;
    }

    // Temporarily change the board to look for check
    bool check = false;
    char temp = board[toI][toJ];
    board[toI][toJ] = board[fromI][fromJ];
    board[fromI][fromJ] = ' ';

    if (isCheck(board, turn)) check = true;

    // Return the board to normal
    board[fromI][fromJ] = board[toI][toJ];
    board[toI][toJ] = temp;

    return !check;
}

// Check that there are no pieces between two points on the board.
bool Rules::isClear(char board[][8], int fromI, int fromJ, int toI, int toJ) {
    int dy = fromI - toI;
    int dx = fromJ - toJ;

    if (dx > 0) dx = 1;
    if (dy > 0) dy = 1;
    if (dx < 0) dx = -1;
    if (dy < 0) dy = -1;

    while (fromI-dy != toI || fromJ-dx != toJ) {
        fromI -= dy;
        fromJ -= dx;

        if (board[fromI][fromJ] != ' ') {
            return false;
        }
    }

    return true;
}

// Check if a player is in check.
bool Rules::isCheck(char board[][8], char player) {
    int kingI = 0;
    int kingJ = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((player == 'A' && board[i][j] == 'K') || (player == 'a' && board[i][j] == 'k')) {
                kingI = i;
                kingJ = j;
            }
        }
    }

    return isCheck(board, player, kingI, kingJ);
}

// Check if a player is in check based on the position of their king.
bool Rules::isCheck(char board[][8], char player, int kingI, int kingJ) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (player == 'a' && board[i][j] >= 65 && board[i][j] < 96) {
                if (validMove(board, i, j, kingI, kingJ, 'A')) return true;
            }
            else if (player == 'A' && board[i][j] >= 97 && board[i][j] < 123) {
                if (validMove(board, i, j, kingI, kingJ, 'a')) return true;
            }
        }
    }

    return false;
}

// Check if a player is in checkmate.
bool Rules::isCheckMate(char board[][8], char player) {
    int kingI = 0;
    int kingJ = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((player == 'A' && board[i][j] == 'K') || (player == 'a' && board[i][j] == 'k')) {
                kingI = i;
                kingJ = j;
            }
        }
    }

    char king = board[kingI][kingJ];
    board[kingI][kingJ] = ' ';

    int nonChecks = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int newKingI = kingI + i;
            int newKingJ = kingJ + j;

            if (newKingI < 0 || newKingI > 7 || newKingJ < 0 || newKingJ > 7) continue;
            if (player == 'A' && board[newKingI][newKingJ] >= 65 && board[newKingI][newKingJ] < 96) continue;
            if (player == 'a' && board[newKingI][newKingJ] >= 97 && board[newKingI][newKingJ] < 123) continue;

            char temp = board[newKingI][newKingJ];
            board[newKingI][newKingJ] = king;

            if (!isCheck(board, player, newKingI, newKingJ)) nonChecks++;

            board[newKingI][newKingJ] = temp;
        }
    }

    board[kingI][kingJ] = king;

    return nonChecks == 0;
}