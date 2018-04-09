#include "game.h"

GLuint Game::boardObj;
GLuint Game::kingObj;
GLuint Game::queenObj;
GLuint Game::bishopObj;
GLuint Game::knightObj;
GLuint Game::rookObj;
GLuint Game::pawnObj;

float Game::cameraAlpha;
float Game::cameraBeta;
float Game::cameraRadius;

bool Game::mouseRightClick;
int Game::motionLastX;
int Game::motionLastY;
char Game::turn;

char Game::board[8][8] = {
    {'r', 'n', 'b', 'k', 'q', 'b', 'n', 'r'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'N', 'B', 'K', 'Q', 'B', 'N', 'R'}
};

int Game::selectionI;
int Game::selectionJ;

std::string Game::msgWhite;
std::string Game::msgBlack;

std::vector<std::vector<int>> Game::stars;

const std::string MSG_YOUR_TURN = "Your turn!";
const std::string MSG_INVALID = "Invalid..";
const std::string MSG_CHECK = "Check..";
const std::string MSG_WIN = "YOU WIN!";
const std::string MSG_LOSE = "Sorry..";

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

const float MIN_CAMERA_BETA = 0;
const float MAX_CAMERA_BETA = 1.4;

const int TILE_SIZE = 75;

// Main point of entry of the game. Resets the instance variables and sets
// the glut callback functions.
void Game::start(int *argc, char *argv[]) {
    cameraAlpha = 0;
    cameraBeta = 0.5;
    cameraRadius = 800;

    selectionI = -1;
    selectionJ = -1;
    turn = 'A';

    msgWhite = MSG_YOUR_TURN;
    msgBlack = "";

    mouseRightClick = false;

    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-WINDOW_WIDTH)/2, (glutGet(GLUT_SCREEN_HEIGHT)-WINDOW_HEIGHT)/2);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    glutCreateWindow("Chess3D");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouseInput);
    glutMotionFunc(mouseMotion);
    glutReshapeFunc(reshape);

    glutMainLoop();
}

// Initializes the OpenGL environment (lights, tests, and model loading).
void Game::init() {
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.01, 0.01, 0.06, 0.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glFrustum(-0.80, 0.80, -0.60, 0.60, 1, 2000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Load the object models.
    boardObj = Vao::loadObj("models/board.obj");
    kingObj = Vao::loadObj("models/king.obj");
    queenObj = Vao::loadObj("models/queen.obj");
    bishopObj = Vao::loadObj("models/bishop.obj");
    knightObj = Vao::loadObj("models/knight.obj");
    rookObj = Vao::loadObj("models/rook.obj");
    pawnObj = Vao::loadObj("models/pawn.obj");

    // Generate the stars.
    for (int i = 0; i < 200; i++) {
        for (int j = 0; j < 200; j++) {
            for (int k = 0; k < 200; k++) {
                if ((i < 30 || i > 170) || (j < 30 || j > 170) || (k < 30 || k > 170)) {
                    if (rand() % 1000 == 0) {
                        std::vector<int> star = {i*10-1000, j*10-1000, k*10-1000};
                        stars.push_back(star);
                    }
                }
            }
        }
    }
}

// Handles displaying a single frame of the game.
void Game::display() {
    // Resets the camera position values.
    float cameraZ = cameraRadius * std::cos(cameraBeta) * std::sin(cameraAlpha);
    float cameraX = cameraRadius * std::cos(cameraBeta) * std::cos(cameraAlpha);
    float cameraY = cameraRadius * std::sin(cameraBeta);

    // Sets the camera position accordingly.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);

    // Define light positions.
    GLfloat light_position0[] = {0.0, 600.0, 0.0, 1.0};
    GLfloat light_position1[] = {400.0, 100.0, 0.0, 1.0};
    GLfloat light_position2[] = {-400.0, 100.0, 0.0, 1.0};
    GLfloat light_position3[] = {0.0, 100.0, 400.0, 1.0};
    GLfloat light_position4[] = {0.0, 100.0, -400.0, 1.0};

    glutSolidSphere(300, 10, 10);
    
    // Sets light positions.
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
    glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
    glLightfv(GL_LIGHT4, GL_POSITION, light_position4);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draws the stars.
    glPushMatrix();
    {
        glDisable(GL_LIGHTING);
        glColor3f(1, 1, 1);
        glBegin(GL_POINTS);
        {
            for (int i = 0; i < stars.size(); i++) {
                glVertex3f(stars.at(i).at(0), stars.at(i).at(1), stars.at(i).at(2));
            }
        }
        glEnd();
        glEnable(GL_LIGHTING);
    }
    glPopMatrix();

    // Draws the game board.
    glPushMatrix();
    {
        GLfloat mat_diffuse[] = {0.426, 0.258, 0.129, 1.0};
        GLfloat mat_specular[] = {0.426, 0.258, 0.129, 1.0};
        GLfloat mat_shininess[] = {50.0};
        
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

        glCallList(boardObj);
    }
    glPopMatrix();

    // For each tile on the board...
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // Draws the white or black tile.
            glPushMatrix();
            {
                if ((i+j)%2 == 0) {
                    GLfloat mat_diffuse[] = {0.929, 0.929, 0.820, 1.0};
                    GLfloat mat_specular[] = {0.929, 0.929, 0.820, 1.0};
                    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
                }
                else {
                    GLfloat mat_diffuse[] = {0.090, 0.090, 0.090, 1.0};
                    GLfloat mat_specular[] = {0.090, 0.090, 0.090, 1.0};
                    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
                }

                GLfloat mat_shininess[] = {1000.0};
                glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

                glBegin(GL_TRIANGLE_STRIP);
                glNormal3f(0, 1, 0);
                glVertex3f(i*TILE_SIZE-299, 1, j*TILE_SIZE-299);
                glNormal3f(0, 1, 0);
                glVertex3f(i*TILE_SIZE-224, 1, j*TILE_SIZE-299);
                glNormal3f(0, 1, 0);
                glVertex3f(i*TILE_SIZE-299, 1, j*TILE_SIZE-224);
                glNormal3f(0, 1, 0);
                glVertex3f(i*TILE_SIZE-224, 1, j*TILE_SIZE-224);
                glEnd();
            }
            glPopMatrix();

            // Draws a game piece.
            char piece = board[i][j];
            if (piece != ' ') {
                glPushMatrix();
                {
                    glTranslatef(i*75-263, 0, (7-j)*75-263);

                    // White piece.
                    if (piece < 96) {
                        GLfloat mat_diffuse[] = {0.929, 0.929, 0.820, 1.0};
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
                        glRotatef(90, 0, 1, 0);

                        if (selectionI == i && selectionJ == j) {
                            GLfloat mat_diffuse[] = {0.500, 0.929, 0.820, 1.0};
                            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
                        }
                    }
                    // Black piece.
                    else {
                        GLfloat mat_diffuse[] = {0.090, 0.090, 0.090, 1.0};
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
                        glRotatef(270, 0, 1, 0);

                        if (selectionI == i && selectionJ == j) {
                            GLfloat mat_diffuse[] = {0.590, 0.090, 0.090, 1.0};
                            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
                        }
                    }

                    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
                    GLfloat mat_shininess[] = {50.0};
                    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
                    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

                    // Draws the correct piece.
                    if (piece == 'k' || piece == 'K')
                        glCallList(kingObj);
                    else if (piece == 'q' || piece == 'Q')
                        glCallList(queenObj);
                    else if (piece == 'b' || piece == 'B')
                        glCallList(bishopObj);
                    else if (piece == 'n' || piece == 'N')
                        glCallList(knightObj);
                    else if (piece == 'r' || piece == 'R')
                        glCallList(rookObj);
                    else if (piece == 'p' || piece == 'P')
                        glCallList(pawnObj);
                }
                glPopMatrix();
            }
        }
    }

    glDisable(GL_LIGHTING);

    // Draws the message to white player.
    glPushMatrix();
    {
        glColor3f(1, 1, 1);
        int z = glutStrokeWidth(GLUT_STROKE_MONO_ROMAN, '?')*(msgWhite.size()/2);
        glTranslatef(-500, 150, z);
        glRotatef(90, 0, 1, 0);
        for (int i = 0; i < msgWhite.size(); i++) {
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, msgWhite[i]);
        }
    }
    glPopMatrix();

    // Draws the message to black player.
    glPushMatrix();
    {
        glColor3f(1, 1, 1);
        int z = glutStrokeWidth(GLUT_STROKE_MONO_ROMAN, '?')*(-msgBlack.size()/2);
        glTranslatef(500, 150, z);
        glRotatef(270, 0, 1, 0);
        for (int i = 0; i < msgBlack.size(); i++) {
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, msgBlack[i]);
        }
    }
    glPopMatrix();

    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}

// Gets called when a player clicks on a new tile or piece.
void Game::handleNewSelection(int i, int j) {
    char piece = board[i][j];

    // Selecting own turn's piece.
    if (piece != ' ' && ((piece < 96 && turn == 'A') || (piece >= 96 && turn == 'a'))) {
        selectionI = i;
        selectionJ = j;
    }
    // Trying to move a selected piece.
    else if (selectionI >= 0 && selectionJ >= 0) {
        if (Rules::validMove(board, selectionI, selectionJ, i, j, turn)) {
            board[i][j] = board[selectionI][selectionJ];
            board[selectionI][selectionJ] = ' ';

            selectionI = -1;
            selectionJ = -1;
            
            if (turn == 'A') {
                if (Rules::isCheckMate(board, 'a')) {
                    msgWhite = MSG_WIN;
                    msgBlack = MSG_LOSE;
                }
                else {
                    turn = 'a';
                    msgWhite = "";
                    msgBlack = MSG_YOUR_TURN;
                }
            }
            else {
                if (Rules::isCheckMate(board, 'A')) {
                    msgWhite = MSG_LOSE;
                    msgBlack = MSG_WIN;
                }
                else {
                    turn = 'A';
                    msgWhite = MSG_YOUR_TURN;
                    msgBlack = "";
                }
            }
        }
        else {
            if (Rules::isCheck(board, turn)) {
                if (turn == 'A') {
                    msgWhite = MSG_CHECK;
                }
                else {
                    msgBlack = MSG_CHECK;
                }
            }
            else {
                if (turn == 'A') {
                    msgWhite = MSG_INVALID;
                }
                else {
                    msgBlack = MSG_INVALID;
                }
            }

            glutTimerFunc(500, resetMessageTimer, 0);
            selectionI = -1;
            selectionJ = -1;
        }
    }
}

// Handles mouse click inputs.
void Game::mouseInput(int button, int state, int x, int y) {
    // Scroll wheel down.
    if (button == 3) {
        cameraRadius -= 20;
    }
    // Scroll wheel up.
    else if (button == 4) {
        cameraRadius += 20;
    }
    // Left click down. Figures out which tile the mouse clicked and handles
    // potential game movemnets accordingly.
    else if (button == 0 && state == GLUT_DOWN) {
        GLint viewport[4];
        GLdouble modelview[16];
        GLdouble projection[16];
        GLfloat winX, winY, winZ;
        GLdouble posX, posY, posZ;
    
        // Get the current matrices.
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        glGetIntegerv(GL_VIEWPORT, viewport);
    
        // Convert display coordinates to model-view coordinates.
        winX = (float)x;
        winY = (float)viewport[3] - (float)y;
        glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
        gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

        // Figure out which tile based on x and z values.
        int i = (posX + 4*TILE_SIZE) / TILE_SIZE;
        int j = 8 - ((posZ + 4*TILE_SIZE) / TILE_SIZE);

        // Inside of the board.
        if (i >= 0 && i < 8 && j >= 0 && j < 8) {
            handleNewSelection(i, j);
        }
        // Outside of the board.
        else {
            selectionI = -1;
            selectionJ = -1;
        }
    }
    // Right click down event. Changes the delta motion for rotations.
    else if (button == 2 && state == GLUT_DOWN) {
        mouseRightClick = true;
        motionLastX = x;
        motionLastY = y;

        glutTimerFunc(0, motionTimer, 0);
    }
    // Right click up event.
    else if (button == 2 && state == GLUT_UP) {
        mouseRightClick = false;
    }

    display();
}

// Handles mouse movement changes.
void Game::mouseMotion(int x, int y) {
    if (mouseRightClick) {
        cameraAlpha += (float) (x-motionLastX) / 100;
        cameraBeta += (float) (y-motionLastY) / 100;

        if (cameraBeta < MIN_CAMERA_BETA) cameraBeta = MIN_CAMERA_BETA;
        if (cameraBeta > MAX_CAMERA_BETA) cameraBeta = MAX_CAMERA_BETA;

        motionLastX = x;
        motionLastY = y;
    }
}

// GL timer function to handle right click movements.
void Game::motionTimer(int value) {
    if (mouseRightClick) {
        display();
        glutTimerFunc(10, motionTimer, 0);
    }
}

// GL timer function to reset a message.
void Game::resetMessageTimer(int value) {
    if (turn == 'A') {
        msgWhite = MSG_YOUR_TURN;
    }
    else {
        msgBlack = MSG_YOUR_TURN;
    }

    display();
}

// Handles window size changes.
void Game::reshape(int newWidth, int newHeight) {
    double aspect = (double) newWidth / (double) newHeight;
    glViewport(0, 0, newWidth, newHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, aspect, 1, 4000);
}