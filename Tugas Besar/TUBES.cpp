#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float deltaAngle = 0.0; 
int deltaMove = 0;      // Untuk gerakan maju/mundur
int h_global, w_global;

float balloonX = 0.0f;
float balloonY = 10.0f;
float balloonZ = 0.0f;
float balloonVerticalSpeed = 0.0f;
float balloonRadius = 2.5f;

// Orientasi Kamera terkait dengan balon
float camLookAngle = 0.0f;
float camLookOffsetX = 0.0f;
float camLookOffsetZ = -1.0f;
float cameraDistance = 12.0f;
float cameraHeightOffset = 5.0f;


// Variabel Skor dan Koin
#define MAX_COINS 20
#define COINS_TO_WIN 5
struct Coin {
    float x, y, z;
    float radius;
    bool isCollected;
    float rotation;
};
Coin coins[MAX_COINS];
int score = 0;
int collectedCoinsCount = 0;
bool isGameOver = false;
bool enableShadows = false;
GLuint barkTextureID; 
GLuint leavesTextureID;
const float JALAN_LEBAR_TOTAL = 6.0f;
const float JALAN_LEBAR = JALAN_LEBAR_TOTAL / 2.0f;
const GLfloat ORIGINAL_global_light_ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
const GLfloat ORIGINAL_global_light_diffuse[] = {1.0f, 1.0f, 0.9f, 1.0f};
const GLfloat ORIGINAL_global_light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat SHADOW_global_light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
const GLfloat global_light_position[] = {100.0f, 100.0f, 50.0f, 1.0f};
const GLfloat global_mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
const GLfloat global_mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
const GLfloat global_mat_specular[] = {0.6f, 0.6f, 0.6f, 1.0f};
const GLfloat global_high_shininess[] = {60.0f};


void Reshape(int width, int height);
void initOpenGL();
void setupLighting();
void renderBitmapString(float x_pos, float y_pos, void *font, const char *string);
GLuint createProceduralTexture(int type);
void initTextures();
void initializeCoins();
void checkCoinCollision();
bool checkSphereBoxCollision(float sX, float sY, float sZ, float sR, float bMinX, float bMaxX, float bMinY, float bMaxY, float bMinZ, float bMaxZ);
void checkBuildingCollisions();
void checkEnvironmentObjectCollisions();
void displayScore();
void displayGameOverMessage();
void resetGame();
void updateGameLogic(int value);
void pressKey(int key, int x_coord, int y_coord);
void releaseKey(int key, int x_coord, int y_coord);
void processNormalKeys(unsigned char key, int x_coord, int y_coord);
void releaseNormalKeys(unsigned char key, int x_coord, int y_coord);
void Grid();
void orientCamera(float ang);
void moveBalloon(int i);
void drawCube(float x, float y, float z, float sx, float sy, float sz, float r, float g, float b);
void drawCubeTwo(float cx, float cy, float cz, float sizeX, float sizeY, float sizeZ);
void drawFace();
void drawSteve(float steve_x, float steve_y, float steve_z, float scale);
void drawBalloonBasket();
void drawSquareRoof();
void drawRopes();
void drawBalloon();
void drawCoin3D(float radius, float coinX, float coinY, float coinZ, float rotation_angle);
void drawDashedLine(float x1, float z1, float x2, float z2, float line_thickness);
void drawStreetLight(float x, float z, float height);
void drawTrafficLight(float x, float z);
void drawCar(float car_x, float car_z, float r, float g, float b);
void drawPohon(float px, float pz); // Menggunakan tekstur
void drawCylinder(float base, float top, float height, int slices, int stacks);
void drawWindow(float x, float y, float z, float width, float height, float r, float g, float b);
void drawBuilding(float x, float z, float base_width, float base_depth, float height1, float top_width, float top_depth, float height2);
void drawWindowsUniversitas();
void drawPillarsUniversitas();
void drawStairsUniversitas();
void drawRoofUniversitas();
void drawLampsUniversitas();
void drawLabelUniversitas();
void drawUniversitasBuilding();
void drawTallPurpleBuilding(float tpx_local, float tpz_local, float size);
void drawYellowBuilding(float yx_local, float yz_local, float size);
void drawGlassBuilding(float gx_local, float gz_local, float size);
void drawRedRoofBuilding(float rrx_local, float rrz_local, float size);
void drawNewOfficeBuilding(float ox_local, float oz_local, float width, float height, float depth, float r, float g, float b);
void renderScene(void);


void Reshape(int width, int height) {
    if (height == 0) height = 1;
    w_global = width; h_global = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(45.0f, (float)width / (float)height, 0.1f, 500.0f);
    glMatrixMode(GL_MODELVIEW);
}

void orientCamera(float ang) {
    camLookOffsetX = sin(ang);
    camLookOffsetZ = -cos(ang);
}

void moveBalloon(int i) {
    float speed = 0.2f;
    balloonX += i * camLookOffsetX * speed;
    balloonZ += i * camLookOffsetZ * speed;
}

void drawCubeTwo(float cx, float cy, float cz, float sizeX, float sizeY, float sizeZ) {
    glPushMatrix();
    glTranslatef(cx, cy, cz);
    glScalef(sizeX, sizeY, sizeZ);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawFace() {
    glColor3f(1.0f, 0.8f, 0.6f); drawCubeTwo(0.0f, 1.0f, 0.21f, 0.38f, 0.38f, 0.02f);
    glColor3f(0.0f, 0.0f, 1.0f);
    drawCubeTwo(-0.08f, 1.05f, 0.22f, 0.05f, 0.05f, 0.01f);
    drawCubeTwo(0.08f, 1.05f, 0.22f, 0.05f, 0.05f, 0.01f);
}

void drawSteve(float steve_x, float steve_y, float steve_z, float scale) {
    glPushMatrix();
    glTranslatef(steve_x, steve_y, steve_z); glScalef(scale, scale, scale);
    glColor3f(0.4f, 0.2f, 0.1f); drawCubeTwo(0.0f, 1.0f, 0.0f, 0.4f, 0.4f, 0.4f);
    drawFace();
    glColor3f(0.0f, 1.0f, 1.0f); drawCubeTwo(0.0f, 0.5f, 0.0f, 0.5f, 0.6f, 0.3f);
    glColor3f(1.0f, 0.8f, 0.6f);
    drawCubeTwo(-0.3f, 0.5f, 0.0f, 0.2f, 0.6f, 0.2f);
    drawCubeTwo(0.3f, 0.5f, 0.0f, 0.2f, 0.6f, 0.2f);
    glColor3f(0.0f, 0.0f, 1.0f); drawCubeTwo(-0.15f, 0.0f, 0.0f, 0.2f, 0.6f, 0.2f);
    drawCubeTwo(0.15f, 0.0f, 0.0f, 0.2f, 0.6f, 0.2f);
    glPopMatrix();
}

void drawBalloonBasket() {
    glColor3f(0.5f, 0.25f, 0.0f); drawCubeTwo(0.0f, 0.0f, 0.0f, 0.8f, 0.1f, 0.8f);
    glColor3f(1.0f, 1.0f, 0.8f);
    drawCubeTwo(0.0f, 0.3f, 0.4f, 0.8f, 0.4f, 0.02f);
    drawCubeTwo(0.0f, 0.3f, -0.4f, 0.8f, 0.4f, 0.02f);
    drawCubeTwo(0.4f, 0.3f, 0.0f, 0.02f, 0.4f, 0.8f);
    drawCubeTwo(-0.4f, 0.3f, 0.0f, 0.02f, 0.4f, 0.8f);
    glColor3f(0.5f, 0.25f, 0.0f);
    drawCubeTwo(0.0f, 0.12f, 0.4f, 0.8f, 0.04f, 0.04f); drawCubeTwo(0.0f, 0.12f, -0.4f, 0.8f, 0.04f, 0.04f);
    drawCubeTwo(0.4f, 0.12f, 0.0f, 0.04f, 0.04f, 0.8f); drawCubeTwo(-0.4f, 0.12f, 0.0f, 0.04f, 0.04f, 0.8f);
    drawCubeTwo(0.0f, 0.5f, 0.4f, 0.8f, 0.04f, 0.04f); drawCubeTwo(0.0f, 0.5f, -0.4f, 0.8f, 0.04f, 0.04f);
    drawCubeTwo(0.4f, 0.5f, 0.0f, 0.04f, 0.04f, 0.8f); drawCubeTwo(-0.4f, 0.5f, 0.0f, 0.04f, 0.04f, 0.8f);
}

void drawSquareRoof() {
    glColor3f(0.5f, 0.25f, 0.0f); drawCubeTwo(0.0f, 0.8f, 0.0f, 0.8f, 0.1f, 0.8f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 1.1f, 0.0f);
    glScalef(0.1f, 0.3f, 0.1f);
    glutSolidCone(1.0f, 1.0f, 16, 16);
    glPopMatrix();
}

void drawRopes() {
    glColor3f(0.0f, 0.0f, 0.0f); glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex3f(-0.4f, 0.5f, 0.4f); glVertex3f(-0.5f, 2.0f, 0.5f);
    glVertex3f(0.4f, 0.5f, 0.4f);  glVertex3f(0.5f, 2.0f, 0.5f);
    glVertex3f(-0.4f, 0.5f, -0.4f);glVertex3f(-0.5f, 2.0f, -0.5f);
    glVertex3f(0.4f, 0.5f, -0.4f); glVertex3f(0.5f, 2.0f, -0.5f);
    glEnd();
}

void drawBalloon() {
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, 0.0f);
    glColor3f(1.0f, 0.5f, 0.0f);
    glPushMatrix();
    glScalef(1.0f, 1.2f, 1.0f);
    glutSolidSphere(1.5f, 32, 32);
    glPopMatrix();
    glColor3f(0.4f, 0.2f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, -1.2f, 0.0f);
    glScalef(0.8f, 0.0f, 0.8f);
    glutSolidSphere(1.0f, 30, 30);
    glPopMatrix();
    glColor3f(0.6f, 0.3f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, -1.5f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidTorus(0.08, 0.75, 30, 30);
    glPopMatrix();
    glPopMatrix();
}

void drawCoin3D(float radius_val, float coinX, float coinY, float coinZ, float rotation_angle) {
    glColor3f(1.0f, 0.843f, 0.0f);
    glPushMatrix();
    glTranslatef(coinX, coinY, coinZ);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation_angle, 0.0f, 1.0f, 0.0f);

    float thickness = 0.05f;
    GLUquadric* quad = gluNewQuadric();
    gluDisk(quad, 0, radius_val, 32, 1);
    glPushMatrix();
    glTranslatef(0,0, -thickness);
    gluDisk(quad, 0, radius_val, 32, 1);
    glPopMatrix();
    gluCylinder(quad, radius_val, radius_val, thickness, 32, 1);
    gluDeleteQuadric(quad);
    glPopMatrix();
}

void initializeCoins() {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < MAX_COINS; ++i) {
        coins[i].x = (rand() % 80 - 40) * 0.5f;
        coins[i].y = (rand() % 20 + 5) * 0.1f + 3.0f;
        coins[i].z = (rand() % 80 - 40) * 0.5f;
        coins[i].radius = 0.4f;
        coins[i].isCollected = false;
        coins[i].rotation = (float)(rand() % 360);
    }
}

void checkCoinCollision() {
    if (isGameOver) return;
    float keranjangRadius = 0.8f;
    for (int i = 0; i < MAX_COINS; ++i) {
        if (!coins[i].isCollected) {
            float dx = balloonX - coins[i].x;
            float dy = balloonY - coins[i].y;
            float dz = balloonZ - coins[i].z;
            float distance = sqrt(dx*dx + dy*dy + dz*dz);
            if (distance < (keranjangRadius + coins[i].radius)) {
                coins[i].isCollected = true;
                score += 10;
                collectedCoinsCount++;
            }
        }
    }
}

bool checkSphereBoxCollision(float sX, float sY, float sZ, float sR, float bMinX, float bMaxX, float bMinY, float bMaxY, float bMinZ, float bMaxZ) {
    float closestX = fmax(bMinX, fmin(sX, bMaxX));
    float closestY = fmax(bMinY, fmin(sY, bMaxY));
    float closestZ = fmax(bMinZ, fmin(sZ, bMaxZ));
    float distanceX = sX - closestX;
    float distanceY = sY - closestY;
    float distanceZ = sZ - closestZ;
    float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY) + (distanceZ * distanceZ);
    return distanceSquared < (sR * sR);
}

void checkBuildingCollisions() {
    if (isGameOver) return;
    float base_width = JALAN_LEBAR_TOTAL * 1.5f;
    float base_depth = JALAN_LEBAR_TOTAL * 1.5f;
    float height1 = 10.0f;
    float build1_approx_height = height1 * 1.5f;
    float build1_approx_width = base_width;
    float build1_approx_depth = base_depth;

    float building_coords[][3] = {
        {-40.0f, 0.0f, -40.0f}, {0.0f, 0.0f, -40.0f}, {40.0f, 0.0f, -40.0f},
        {-40.0f, 0.0f, 40.0f}, {0.0f, 0.0f, 40.0f}, {40.0f, 0.0f, 40.0f},
        {-20.0f, 0.0f, 30.0f}, {20.0f, 0.0f, 30.0f},
        {-30.0f, 0.0f, 10.0f}, {30.0f, 0.0f, 10.0f},
        {-30.0f, 0.0f, -10.0f}, {-20.0f, 0.0f, -10.0f}, {20.0f, 0.0f, -10.0f},
        {-20.0f, 0.0f, -30.0f}, {20.0f, 0.0f, -30.0f}
    };
    int num_building_coords = sizeof(building_coords) / sizeof(building_coords[0]);

    for (int i = 0; i < num_building_coords; ++i) {
        float bx = building_coords[i][0];
        float bz = building_coords[i][2];
        float minX = bx - build1_approx_width / 2.0f;
        float maxX = bx + build1_approx_width / 2.0f;
        float minY = 0.0f;
        float maxY = build1_approx_height;
        float minZ = bz - build1_approx_depth / 2.0f;
        float maxZ = bz + build1_approx_depth / 2.0f;
        if (checkSphereBoxCollision(balloonX, balloonY, balloonZ, balloonRadius, minX, maxX, minY, maxY, minZ, maxZ)) {
            isGameOver = true; return;
        }
    }

    float univ_scale = 0.8f;
    float univ_x = -10.0f;
    float univ_z = -10.0f;
    float univ_width = 10.0f * univ_scale;
    if (checkSphereBoxCollision(balloonX, balloonY, balloonZ, balloonRadius,
        univ_x - univ_width / 2.0f, univ_x + univ_width / 2.0f,
        0.0f, 6.0f,
        univ_z - (4.0f*univ_scale)/2.0f, univ_z + (2.4f+0.2f)*univ_scale )) {
        isGameOver = true; return;
    }

    float tpx = 19.0f, tpz = 10.0f, tps = 1.5f;
    if (checkSphereBoxCollision(balloonX, balloonY, balloonZ, balloonRadius,
        tpx - (tps * 2.5f / 2.0f), tpx + (tps * 2.5f / 2.0f),
        0.0f, tps * 8.0f,
        tpz - (tps * 2.5f / 2.0f), tpz + (tps * 2.5f / 2.0f))) {
        isGameOver = true; return;
    }
    tpx = 30.0f, tpz = -9.0f, tps = 2.0f;
    if (checkSphereBoxCollision(balloonX, balloonY, balloonZ, balloonRadius,
        tpx - (tps * 2.5f / 2.0f), tpx + (tps * 2.5f / 2.0f),
        0.0f, tps * 8.0f,
        tpz - (tps * 2.5f / 2.0f), tpz + (tps * 2.5f / 2.0f))) {
        isGameOver = true; return;
    }

    float yx = 10.0f, yz = -9.0f, ys = 2.0f;
    if (checkSphereBoxCollision(balloonX, balloonY, balloonZ, balloonRadius,
        yx - (ys * 3 / 2.0f), yx + (ys * 3 / 2.0f),
        0.0f, ys*5.0f,
        yz - (ys * 3 / 2.0f), yz + (ys * 3 / 2.0f))) {
        isGameOver = true; return;
    }
    yx = 23.0f, yz = 10.0f, ys = 1.2f;
    if (checkSphereBoxCollision(balloonX, balloonY, balloonZ, balloonRadius,
        yx - (ys * 3 / 2.0f), yx + (ys * 3 / 2.0f),
        0.0f, ys*5.0f,
        yz - (ys * 3 / 2.0f), yz + (ys * 3 / 2.0f))) {
        isGameOver = true; return;
    }

    float ox = -10.0f, oz = 10.0f, ow = 12.0f, oh_nob = 10.0f, od = 8.0f;
    if (checkSphereBoxCollision(balloonX, balloonY, balloonZ, balloonRadius,
        ox - ow / 2.0f, ox + ow / 2.0f, 0.0f, oh_nob, oz - od / 2.0f, oz + od / 2.0f)) {
        isGameOver = true; return;
    }
    ox = 10.0f, oz = -30.0f, ow = 10.0f, oh_nob = 15.0f, od = 8.0f;
    if (checkSphereBoxCollision(balloonX, balloonY, balloonZ, balloonRadius,
        ox - ow / 2.0f, ox + ow / 2.0f, 0.0f, oh_nob, oz - od / 2.0f, oz + od / 2.0f)) {
        isGameOver = true; return;
    }

    float gx = 10.0f, gz = 10.0f, gs = 1.8f;
    if (checkSphereBoxCollision(balloonX, balloonY, balloonZ, balloonRadius,
        gx - (gs * 3 / 2.0f), gx + (gs * 3 / 2.0f),
        0.0f, gs*6.0f + gs*0.4f,
        gz - (gs * 3 / 2.0f), gz + (gs * 3 / 2.0f))) {
        isGameOver = true; return;
    }

    float rrx = -30.0f, rrz = -30.0f, rrs = 1.5f;
    if (checkSphereBoxCollision(balloonX, balloonY, balloonZ, balloonRadius,
        rrx - (rrs * 3 / 2.0f), rrx + (rrs * 3 / 2.0f),
        0.0f, rrs*5.0f + rrs*0.5f,
        rrz - (rrs * 3 / 2.0f), rrz + (rrs * 3 / 2.0f))) {
        isGameOver = true; return;
    }
}

void checkEnvironmentObjectCollisions() {
    if (isGameOver) return;
    float sl_height = 6.0f; float sl_radius_approx = 0.8f;
    float sl_minY = 0.0f; float sl_maxY = sl_height;
    float sl_coords[][2] = {
        {-35.0f, -23.0f}, {-15.0f, -23.0f}, {-35.0f, -3.0f}, {-15.0f, -3.0f},
        {15.0f, -3.0f}, {35.0f, -3.0f}, {-35.0f, 17.0f}, {-15.0f, 17.0f},
        {15.0f, 17.0f}, {35.0f, 17.0f}, {-43.0f, -35.0f}, {-43.0f, -15.0f},
        {-3.0f, -35.0f}, {-3.0f, -15.0f}, {-3.0f, 15.0f}, {-3.0f, 35.0f}
    };
    int num_sl_coords = sizeof(sl_coords) / sizeof(sl_coords[0]);
    for (int i = 0; i < num_sl_coords; ++i) {
        float sx = sl_coords[i][0]; float sz = sl_coords[i][1];
        if (checkSphereBoxCollision(balloonX, balloonY, balloonZ, balloonRadius,
            sx - sl_radius_approx, sx + sl_radius_approx, sl_minY, sl_maxY, sz - sl_radius_approx, sz + sl_radius_approx)) {
            isGameOver = true; return;
        }
    }

    float tl_minY = 0.0f; float tl_maxY = 6.0f + (2.5f/2.0f + 0.3f);
    float tl_radius_approx = 1.2f / 2.0f;
    float tl_coords[][2] = {
        {-40.0f - JALAN_LEBAR - 1.0f, 0.0f - JALAN_LEBAR - 1.0f}, {0.0f - JALAN_LEBAR - 1.0f, 0.0f - JALAN_LEBAR - 1.0f},
        {40.0f - JALAN_LEBAR - 1.0f, 0.0f - JALAN_LEBAR - 1.0f}, {0.0f - JALAN_LEBAR - 1.0f, 20.0f - JALAN_LEBAR - 1.0f},
        {0.0f - JALAN_LEBAR - 1.0f, -20.0f - JALAN_LEBAR - 1.0f}
    };
    int num_tl_coords = sizeof(tl_coords) / sizeof(tl_coords[0]);
    for (int i = 0; i < num_tl_coords; ++i) {
        float tx = tl_coords[i][0]; float tz = tl_coords[i][1];
        if (checkSphereBoxCollision(balloonX, balloonY, balloonZ, balloonRadius,
            tx - tl_radius_approx, tx + tl_radius_approx, tl_minY, tl_maxY, tz - tl_radius_approx, tz + tl_radius_approx)) {
            isGameOver = true; return;
        }
    }

    float car_height_approx = 0.25f + 0.2f/2.0f + 0.3f;
    float car_width_approx = 1.2f;
    float car_depth_approx = 0.7f;
    float car_minY = 0.0f; float car_maxY = car_height_approx;
    float car_coords[][2] = {
        {-30.0f, 20.0f + JALAN_LEBAR / 2.0f}, {-40.0f + JALAN_LEBAR / 2.0f, 30.0f},
        {-10.0f, -20.0f + JALAN_LEBAR / 2.0f}, {10.0f, -20.0f + JALAN_LEBAR / 2.0f},
        {-20.0f, 0.0f + JALAN_LEBAR / 2.0f}, {20.0f, 0.0f + JALAN_LEBAR / 2.0f},
        {10.0f, 20.0f + JALAN_LEBAR / 2.0f}, {-40.0f + JALAN_LEBAR / 2.0f, -10.0f},
        {-40.0f + JALAN_LEBAR / 2.0f, 10.0f}, {0.0f + JALAN_LEBAR / 2.0f, -30.0f},
        {0.0f + JALAN_LEBAR / 2.0f, -10.0f}, {0.0f + JALAN_LEBAR / 2.0f, 10.0f},
        {0.0f + JALAN_LEBAR / 2.0f, 30.0f}, {40.0f + JALAN_LEBAR / 2.0f, -20.0f},
        {40.0f + JALAN_LEBAR / 2.0f, 20.0f}
    };
    int num_car_coords = sizeof(car_coords) / sizeof(car_coords[0]);
    for (int i = 0; i < num_car_coords; ++i) {
        float cx = car_coords[i][0]; float cz = car_coords[i][1];
        if (checkSphereBoxCollision(balloonX, balloonY, balloonZ, balloonRadius,
            cx - car_width_approx / 2.0f, cx + car_width_approx / 2.0f, car_minY, car_maxY,
            cz - car_depth_approx / 2.0f, cz + car_depth_approx / 2.0f)) {
            isGameOver = true; return;
        }
    }

    float tree_leaves_max_y = 2.5f + 0.6f;
    float tree_leaves_radius_xz = 0.7f + 0.4f;
    float tree_minY = 0.0f; float tree_maxY = tree_leaves_max_y;
    float tree_coords[][2] = {
        {-30.0f - 2.0f, 0.0f - (JALAN_LEBAR + 0.5f)},
        {-25.0f, -45.0f}, {-5.0f, -45.0f}, {15.0f, -45.0f}, {35.0f, -45.0f},
        {-45.0f, -25.0f}, {-45.0f, -5.0f}, {-45.0f, 15.0f}, {-45.0f, 35.0f},
        {-25.0f, 45.0f}, {-5.0f, 45.0f}, {15.0f, 45.0f}, {35.0f, 45.0f},
        {45.0f, -25.0f}, {45.0f, -5.0f}, {45.0f, 15.0f}, {45.0f, 35.0f}
    };
    int num_tree_coords = sizeof(tree_coords) / sizeof(tree_coords[0]);
    for (int i = 0; i < num_tree_coords; ++i) {
        float pohon_x = tree_coords[i][0];
        float pohon_z = tree_coords[i][1];
        if (checkSphereBoxCollision(balloonX, balloonY, balloonZ, balloonRadius,
            pohon_x - tree_leaves_radius_xz, pohon_x + tree_leaves_radius_xz,
            tree_minY, tree_maxY,
            pohon_z - tree_leaves_radius_xz, pohon_z + tree_leaves_radius_xz)) {
            isGameOver = true; return;
        }
    }
}

void renderBitmapString(float x_pos, float y_pos, void *font, const char *string) {
    const char *c;
    glRasterPos2f(x_pos, y_pos);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void displayScore() {
    char scoreText[50], coinText[50], winText[100] = "", shadowText[50];
    sprintf(scoreText, "Skor: %d", score);
    sprintf(coinText, "Koin Target: %d / %d", collectedCoinsCount, COINS_TO_WIN);
    sprintf(shadowText, "Shadows: %s (Key 'G')", enableShadows ? "ON" : "OFF");
    if (collectedCoinsCount >= COINS_TO_WIN && !isGameOver) {
        sprintf(winText, "SELAMAT! TARGET %d KOIN TERCAPAI!", COINS_TO_WIN);
    }
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, w_global, 0, h_global);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
    glColor3f(1.0f, 1.0f, 0.0f);
    renderBitmapString(10, h_global - 20, GLUT_BITMAP_HELVETICA_18, scoreText);
    renderBitmapString(10, h_global - 40, GLUT_BITMAP_HELVETICA_18, coinText);
    glColor3f(0.7f, 0.7f, 1.0f);
    renderBitmapString(10, h_global - 60, GLUT_BITMAP_HELVETICA_10, shadowText);
    if (strlen(winText) > 0) {
        glColor3f(0.0f, 1.0f, 0.0f);
        int textWidth = 0;
        for(const char* c_char = winText; *c_char != '\0'; ++c_char) {
            textWidth += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c_char);
        }
        renderBitmapString((w_global - textWidth) / 2.0f, h_global / 2.0f + 50, GLUT_BITMAP_HELVETICA_18, winText);
    }
    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW); glPopMatrix();
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void displayGameOverMessage() {
    char msg1[] = "GAME OVER", msg2[] = "Anda menabrak objek!", msg3[] = "Tekan 'R' untuk Restart", msg4[] = "Tekan ESC untuk Keluar";
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, w_global, 0, h_global);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
    glColor3f(1.0f, 0.0f, 0.0f);
    int tw1 = 0; for(const char* c_char = msg1; *c_char != '\0'; ++c_char) tw1 += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, *c_char);
    renderBitmapString((w_global - tw1) / 2.0f, h_global / 2.0f + 60, GLUT_BITMAP_TIMES_ROMAN_24, msg1);
    glColor3f(1.0f, 1.0f, 1.0f);
    int tw2 = 0; for(const char* c_char = msg2; *c_char != '\0'; ++c_char) tw2 += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c_char);
    renderBitmapString((w_global - tw2) / 2.0f, h_global / 2.0f + 30, GLUT_BITMAP_HELVETICA_18, msg2);
    int tw3 = 0; for(const char* c_char = msg3; *c_char != '\0'; ++c_char) tw3 += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c_char);
    renderBitmapString((w_global - tw3) / 2.0f, h_global / 2.0f - 0, GLUT_BITMAP_HELVETICA_18, msg3);
    int tw4 = 0; for(const char* c_char = msg4; *c_char != '\0'; ++c_char) tw4 += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c_char);
    renderBitmapString((w_global - tw4) / 2.0f, h_global / 2.0f - 30, GLUT_BITMAP_HELVETICA_18, msg4);
    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW); glPopMatrix();
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void resetGame() {
    isGameOver = false; score = 0; collectedCoinsCount = 0;
    balloonX = 0.0f; balloonY = 10.0f; balloonZ = 0.0f; balloonVerticalSpeed = 0.0f;
    camLookAngle = 0.0f; orientCamera(camLookAngle);
    deltaMove = 0; deltaAngle = 0.0f;
    initializeCoins();
}

void pressKey(int key, int x_coord, int y_coord) {
    (void)x_coord; (void)y_coord; if (isGameOver) return;
    switch (key) {
        case GLUT_KEY_LEFT:  deltaAngle = -0.02f; break;
        case GLUT_KEY_RIGHT: deltaAngle = 0.02f;  break;
        case GLUT_KEY_UP:    deltaMove = 1;        break;
        case GLUT_KEY_DOWN:  deltaMove = -1;       break;
    }
}

void releaseKey(int key, int x_coord, int y_coord) {
    (void)x_coord; (void)y_coord;
    switch (key) {
        case GLUT_KEY_LEFT: case GLUT_KEY_RIGHT:
            if ((deltaAngle < 0.0f && key == GLUT_KEY_LEFT) || (deltaAngle > 0.0f && key == GLUT_KEY_RIGHT)) deltaAngle = 0.0f;
            break;
        case GLUT_KEY_UP: case GLUT_KEY_DOWN:
            if ((deltaMove > 0 && key == GLUT_KEY_UP) || (deltaMove < 0 && key == GLUT_KEY_DOWN)) deltaMove = 0;
            break;
    }
}

void processNormalKeys(unsigned char key, int x_coord, int y_coord) {
    (void)x_coord; (void)y_coord;
    if (isGameOver) { if (key == 'r' || key == 'R') resetGame(); else if (key == 27) exit(0); return; }
    switch (key) {
        case 'w': case 'W': balloonVerticalSpeed = 0.05f; break;
        case 's': case 'S': balloonVerticalSpeed = -0.05f; break;
        case 'g': case 'G': enableShadows = !enableShadows; setupLighting(); break;
        case 27: exit(0); break;
        case 'r': case 'R': resetGame(); break;
    }
}

void releaseNormalKeys(unsigned char key, int x_coord, int y_coord) {
    (void)x_coord; (void)y_coord; if (isGameOver) return;
    switch (key) {
        case 'w': case 'W': if(balloonVerticalSpeed > 0) balloonVerticalSpeed = 0.0f; break;
        case 's': case 'S': if(balloonVerticalSpeed < 0) balloonVerticalSpeed = 0.0f; break;
    }
}

void setupLighting() {
    glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LESS);
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE); glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    if (enableShadows) glLightfv(GL_LIGHT0, GL_AMBIENT, SHADOW_global_light_ambient);
    else glLightfv(GL_LIGHT0, GL_AMBIENT, ORIGINAL_global_light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, ORIGINAL_global_light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, ORIGINAL_global_light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, global_light_position);
    glMaterialfv(GL_FRONT, GL_AMBIENT, global_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, global_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, global_mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, global_high_shininess);
}

void initOpenGL() {
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
}

GLuint createProceduralTexture(int type) {
    const int texWidth = 64; 
    const int texHeight = 64;
    GLubyte textureImage[texHeight][texWidth][3];

    for (int i = 0; i < texHeight; i++) { 
        for (int j = 0; j < texWidth; j++) { 
            GLubyte r_val, g_val, b_val;
            float fi = (float)i / texHeight;
            float fj = (float)j / texWidth;

            if (type == 0) { 
                float pattern1 = sin(fj * 15.0f + fi * 5.0f + sin(fi * 10.0f) * 2.0f) * 0.5f + 0.5f; 
                float pattern2 = sin(fi * 30.0f + fj * 2.0f) * 0.5f + 0.5f; 

                r_val = (GLubyte)(100 + pattern1 * 50 + pattern2 * 20);
                g_val = (GLubyte)(70  + pattern1 * 30 + pattern2 * 10); 
                b_val = (GLubyte)(40  + pattern1 * 20 + pattern2 * 5);
                if ( (int)(fj * texWidth) % (8 + (int)(sin(fi*texHeight*0.1)*2.0)) < 2) {
                    r_val = (GLubyte)(r_val * 0.7f);
                    g_val = (GLubyte)(g_val * 0.7f);
                    b_val = (GLubyte)(b_val * 0.7f);
                }

            } else {
                float noise = (sin(fj * 25.0f + fi*15.0f) * cos(fi * 20.0f - fj*30.0f) + 1.0f) * 0.5f; // noise 0-1

                float r_base1 = 30.0f, g_base1 = 100.0f, b_base1 = 30.0f;
                float r_base2 = 60.0f, g_base2 = 150.0f, b_base2 = 60.0f;

                r_val = (GLubyte)(r_base1 * (1.0f - noise) + r_base2 * noise);
                g_val = (GLubyte)(g_base1 * (1.0f - noise) + g_base2 * noise);
                b_val = (GLubyte)(b_base1 * (1.0f - noise) + b_base2 * noise);
                float speckle_chance = (sin(fj*texWidth*0.7f + i) * cos(fi*texHeight*0.7f + j) + 1.0f) * 0.5f;
                if ( ((int)(fi*texHeight*1.1f + fj*texWidth*1.3f + i*0.3f) % 13 == 0 && speckle_chance > 0.8f) ||
                     ((int)(fi*texHeight*1.2f - fj*texWidth*0.8f + j*0.2f) % 17 == 0 && speckle_chance > 0.85f) ) {
                    r_val = (GLubyte)fmin(255.0f, r_val * 1.4f + 20.0f);
                    g_val = (GLubyte)fmin(255.0f, g_val * 1.4f + 20.0f);
                    b_val = (GLubyte)fmax(0.0f, b_val * 0.8f - 10.0f);
                }
            }
            textureImage[i][j][0] = r_val > 255 ? 255 : (r_val < 0 ? 0 : r_val);
            textureImage[i][j][1] = g_val > 255 ? 255 : (g_val < 0 ? 0 : g_val);
            textureImage[i][j][2] = b_val > 255 ? 255 : (b_val < 0 ? 0 : b_val);
        }
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage);

    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}


void initTextures() {
    barkTextureID = createProceduralTexture(0);
    leavesTextureID = createProceduralTexture(1);
}

void Grid() {
    const float Z_MIN_GRID = -50, Z_MAX_GRID = 50, X_MIN_GRID = -50, X_MAX_GRID = 50, gap = 2.5;
    const float NETWORK_X_START = -40.0f, NETWORK_X_END = 40.0f, NETWORK_Z_START = -40.0f, NETWORK_Z_END = 40.0f;
    glBegin(GL_QUADS);
    for (float x_tile = X_MIN_GRID; x_tile < X_MAX_GRID; x_tile += gap) {
        for (float z_tile = Z_MIN_GRID; z_tile < Z_MAX_GRID; z_tile += gap) {
            bool isJalan = false;
            if (x_tile >= NETWORK_X_START - JALAN_LEBAR && x_tile <= NETWORK_X_END + JALAN_LEBAR &&
                z_tile >= NETWORK_Z_START - JALAN_LEBAR && z_tile <= NETWORK_Z_END + JALAN_LEBAR) {
                if ((z_tile >= -20.0f - JALAN_LEBAR && z_tile <= -20.0f + JALAN_LEBAR) ||
                    (z_tile >= 0.0f - JALAN_LEBAR && z_tile <= 0.0f + JALAN_LEBAR) ||
                    (z_tile >= 20.0f - JALAN_LEBAR && z_tile <= 20.0f + JALAN_LEBAR) ||
                    (x_tile >= -40.0f - JALAN_LEBAR && x_tile <= -40.0f + JALAN_LEBAR) ||
                    (x_tile >= 0.0f - JALAN_LEBAR && x_tile <= 0.0f + JALAN_LEBAR) ||
                    (x_tile >= 40.0f - JALAN_LEBAR && x_tile <= 40.0f + JALAN_LEBAR)) isJalan = true;
            }
            if (isJalan) glColor3f(0.2f, 0.2f, 0.2f); else glColor3f(0.1f, 0.6f, 0.1f);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(x_tile, 0, z_tile); glVertex3f(x_tile + gap, 0, z_tile);
            glVertex3f(x_tile + gap, 0, z_tile + gap); glVertex3f(x_tile, 0, z_tile + gap);
        }
    }
    glEnd();
    glLineWidth(2.0); glColor3f(1.0f, 1.0f, 1.0f); float LINE_THICKNESS = 0.005;
    drawDashedLine(NETWORK_X_START, -20.0f, NETWORK_X_END, -20.0f, LINE_THICKNESS);
    drawDashedLine(NETWORK_X_START, 0.0f, NETWORK_X_END, 0.0f, LINE_THICKNESS);
    drawDashedLine(NETWORK_X_START, 20.0f, NETWORK_X_END, 20.0f, LINE_THICKNESS);
    drawDashedLine(-40.0f, NETWORK_Z_START, -40.0f, NETWORK_Z_END, LINE_THICKNESS);
    drawDashedLine(0.0f, NETWORK_Z_START, 0.0f, NETWORK_Z_END, LINE_THICKNESS);
    drawDashedLine(40.0f, NETWORK_Z_START, 40.0f, NETWORK_Z_END, LINE_THICKNESS);
}

void drawDashedLine(float x1, float z1, float x2, float z2, float line_thickness) {
    float dx = x2 - x1, dz = z2 - z1, length = sqrt(dx*dx + dz*dz);
    if (length == 0) return; float segment_length = 2.0, gap_length = 2.0;
    glBegin(GL_LINES);
    for (float current_length = 0; current_length < length; current_length += (segment_length + gap_length)) {
        float start_x = x1 + (dx / length) * current_length, start_z = z1 + (dz / length) * current_length;
        float end_x = x1 + (dx / length) * fmin(current_length + segment_length, length);
        float end_z = z1 + (dz / length) * fmin(current_length + segment_length, length);
        glVertex3f(start_x, 0.01 + line_thickness, start_z); glVertex3f(end_x, 0.01 + line_thickness, end_z);
    }
    glEnd();
}

void drawStreetLight(float x, float z, float height_val) {
    glPushMatrix(); glTranslatef(x, 0, z);
    glPushMatrix(); glTranslatef(0, height_val/2, 0); glColor3f(0.4f, 0.4f, 0.4f);
    glScalef(0.2f, height_val, 0.2f); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0, height_val - 0.5f, 0); glColor3f(0.9f, 0.9f, 0.7f);
    glutSolidSphere(0.8f, 12, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.2f, 0); glColor3f(0.3f, 0.3f, 0.3f);
    glScalef(0.6f, 0.4f, 0.6f); glutSolidCube(1.0); glPopMatrix();
    glPopMatrix();
}

void drawTrafficLight(float x, float z) {
    glPushMatrix(); glTranslatef(x, 0, z);
    glPushMatrix(); glTranslatef(0, 3.0f, 0); glColor3f(0.3f, 0.3f, 0.3f);
    glScalef(0.3f, 6.0f, 0.3f); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 5.5f, 0); glColor3f(0.2f, 0.2f, 0.2f);
    glScalef(1.2f, 2.5f, 0.8f); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 6.2f, 0.41f); glColor3f(1.0f, 0.0f, 0.0f); glutSolidSphere(0.3f, 8, 8); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 5.5f, 0.41f); glColor3f(1.0f, 1.0f, 0.0f); glutSolidSphere(0.3f, 8, 8); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 4.8f, 0.41f); glColor3f(0.0f, 0.8f, 0.0f); glutSolidSphere(0.3f, 8, 8); glPopMatrix();
    glPushMatrix(); glTranslatef(0, 0.3f, 0); glColor3f(0.2f, 0.2f, 0.2f);
    glScalef(0.8f, 0.6f, 0.8f); glutSolidCube(1.0); glPopMatrix();
    glPopMatrix();
}

void drawCube(float x, float y, float z, float sx, float sy, float sz, float r_color, float g_color, float b_color) {
    glPushMatrix(); glTranslatef(x, y, z); glColor3f(r_color, g_color, b_color);
    glScalef(sx, sy, sz); glutSolidCube(1.0); glPopMatrix();
}

void drawCar(float car_x, float car_z, float r_color, float g_color, float b_color) {
    drawCube(car_x, 0.25f, car_z, 1.2f, 0.2f, 0.7f, r_color, g_color, b_color);
    drawCube(car_x, 0.55f, car_z, 0.8f, 0.3f, 0.6f, r_color * 0.8f, g_color * 0.8f, b_color * 0.8f);
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix(); glTranslatef(car_x - 0.4f, 0.15f, car_z - 0.35f); glutSolidTorus(0.05, 0.15, 8, 8); glPopMatrix();
    glPushMatrix(); glTranslatef(car_x + 0.4f, 0.15f, car_z - 0.35f); glutSolidTorus(0.05, 0.15, 8, 8); glPopMatrix();
    glPushMatrix(); glTranslatef(car_x - 0.4f, 0.15f, car_z + 0.35f); glutSolidTorus(0.05, 0.15, 8, 8); glPopMatrix();
    glPushMatrix(); glTranslatef(car_x + 0.4f, 0.15f, car_z + 0.35f); glutSolidTorus(0.05, 0.15, 8, 8); glPopMatrix();
}

void drawPohon(float px, float pz) {
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT | GL_LIGHTING_BIT);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    GLUquadric* trunkQuad = gluNewQuadric();
    gluQuadricTexture(trunkQuad, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, barkTextureID);
    glPushMatrix(); glTranslatef(px, 0.0f, pz); glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(trunkQuad, 0.2f, 0.2f, 2.0f, 12, 3); glPopMatrix();
    gluDeleteQuadric(trunkQuad);
    GLUquadric* leavesQuad = gluNewQuadric();
    gluQuadricTexture(leavesQuad, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, leavesTextureID);
    glPushMatrix(); glTranslatef(px, 2.2f, pz); gluSphere(leavesQuad, 0.7f, 16, 16); glPopMatrix();
    glPushMatrix(); glTranslatef(px + 0.4f, 2.5f, pz + 0.4f); gluSphere(leavesQuad, 0.6f, 16, 16); glPopMatrix();
    glPushMatrix(); glTranslatef(px - 0.4f, 2.5f, pz); gluSphere(leavesQuad, 0.6f, 16, 16); glPopMatrix();
    gluDeleteQuadric(leavesQuad);
    glPopAttrib();
}

void drawCylinder(float base, float top, float height_val, int slices, int stacks) {
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, base, top, height_val, slices, stacks);
    gluDeleteQuadric(quad);
}

void drawWindow(float x, float y, float z, float width_val, float height_val, float r_color, float g_color, float b_color) {
    glPushMatrix(); glTranslatef(x, y, z); glColor3f(r_color, g_color, b_color);
    glScalef(width_val, height_val, 0.1f); glutSolidCube(1.0); glPopMatrix();
}

void drawBuilding(float x, float z, float base_width, float base_depth, float height1, float top_width, float top_depth, float height2) {
    glPushMatrix(); glTranslatef(x, 0, z);
    float building_colors[][3] = {{0.9f,0.7f,0.5f},{0.8f,0.6f,0.4f},{0.7f,0.5f,0.3f},{0.95f,0.8f,0.6f}};
    glPushMatrix();glTranslatef(0,height1*0.4f,0);glColor3f(building_colors[0][0],building_colors[0][1],building_colors[0][2]);glScalef(base_width*0.8f,height1*0.8f,base_depth*0.6f);glutSolidCube(1.0);glPopMatrix();
    glPushMatrix();glTranslatef(-base_width*0.25f,height1*0.7f,0);glColor3f(building_colors[1][0],building_colors[1][1],building_colors[1][2]);glScalef(base_width*0.4f,height1*1.2f,base_depth*0.5f);glutSolidCube(1.0);glPopMatrix();
    glPushMatrix();glTranslatef(base_width*0.3f,height1*0.5f,0);glColor3f(building_colors[2][0],building_colors[2][1],building_colors[2][2]);glScalef(base_width*0.5f,height1*1.0f,base_depth*0.7f);glutSolidCube(1.0);glPopMatrix();
    glPushMatrix();glTranslatef(0,height1*1.1f,0);glColor3f(building_colors[3][0],building_colors[3][1],building_colors[3][2]);glScalef(base_width*0.6f,height1*0.3f,base_depth*0.4f);glutSolidCube(1.0);glPopMatrix();
    float win_r=0.3f, win_g=0.6f, win_b=0.9f;
    for (int floor_idx=1; floor_idx<=4; floor_idx++) {
        for (int window_col=0; window_col<3; window_col++) {
            float win_base_w=base_width*0.8f, win_base_d=base_depth*0.6f, win_h=height1*0.8f;
            float window_x=-win_base_w*0.25f+(window_col*win_base_w*0.15f);
            float window_y=floor_idx*(win_h*0.15f)+height1*0.4f-win_h/2.0f;
            drawWindow(window_x,window_y,win_base_d/2.0f+0.01f,0.8f,0.6f,win_r,win_g,win_b);
            drawWindow(window_x,window_y,-win_base_d/2.0f-0.01f,0.8f,0.6f,win_r,win_g,win_b);
        }
    }
    glPopMatrix();
}

void drawWindowsUniversitas() {
    for (float y_coord = -0.5f; y_coord <= 3.0f; y_coord += 1.2f) {
        for (float x_coord = -4.5f; x_coord <= 4.5f; x_coord += 1.8f) {
            glPushMatrix(); glTranslatef(x_coord, y_coord, 2.05f);
            drawWindow(0, 0, 0, 0.9f, 1.0f, 0.2f, 0.6f, 0.7f); glPopMatrix();
        }
    }
}

void drawPillarsUniversitas() {
    glColor3f(0.7f, 0.9f, 0.8f);
    for (float x_coord = -4.5f; x_coord <= 4.5f; x_coord += 3.0f) {
        glPushMatrix(); glTranslatef(x_coord, -2.5f, 2.1f);
        glScalef(0.3f, 4.5f, 0.3f); glutSolidCube(1.0); glPopMatrix();
    }
}

void drawStairsUniversitas() {
    for (int i = 0; i < 4; i++)
        drawCube(0, -3.0f-i*0.2f, 2.4f+i*0.2f, 6.0f-i*0.3f, 0.2f, 1.5f, 0.5f,0.8f,0.6f);
}

void drawRoofUniversitas() {
    glColor3f(0.0f, 0.4f, 0.3f);
    glBegin(GL_TRIANGLES);
    glVertex3f(-4.5f,6.0f,2.0f);glVertex3f(4.5f,6.0f,2.0f);glVertex3f(0.0f,7.5f,2.0f);
    glVertex3f(-4.5f,6.0f,-2.0f);glVertex3f(4.5f,6.0f,-2.0f);glVertex3f(0.0f,7.5f,-2.0f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(-4.5f,6.0f,2.0f);glVertex3f(-4.5f,6.0f,-2.0f);glVertex3f(0.0f,7.5f,-2.0f);glVertex3f(0.0f,7.5f,2.0f);
    glVertex3f(4.5f,6.0f,2.0f);glVertex3f(4.5f,6.0f,-2.0f);glVertex3f(0.0f,7.5f,-2.0f);glVertex3f(0.0f,7.5f,2.0f);
    glEnd();
}

void drawLampsUniversitas() {
    for (float x_coord = -10.0f; x_coord <= 10.0f; x_coord += 5.0f) {
        glColor3f(0.2f,0.2f,0.2f);
        glPushMatrix(); glTranslatef(x_coord,-2.5f,6.0f); glRotatef(-90,1,0,0);
        drawCylinder(0.1,0.1,2.5,10,10); glPopMatrix();
        glColor3f(1.0f,1.0f,0.5f);
        glPushMatrix(); glTranslatef(x_coord,0.2f,6.0f); glutSolidSphere(0.2,10,10); glPopMatrix();
    }
}

void drawLabelUniversitas() { }

void drawUniversitasBuilding() {
    glPushMatrix(); glTranslatef(0.0f, 3.0f, 0.0f);
    drawCube(0,0,0,10,6,4,0.8f,1.0f,0.9f);
    drawCube(0,4.2f,0,9,2.5,3.5,0.6f,1.0f,1.0f);
    drawCube(0,-2.5f,2.05f,1.5,2.5,0.1,0.2f,0.3f,0.2f);
    drawWindowsUniversitas(); drawPillarsUniversitas(); drawStairsUniversitas();
    drawRoofUniversitas(); drawLampsUniversitas();
    glPopMatrix();
}

void drawTallPurpleBuilding(float tpx_local, float tpz_local, float size_val) {
    glPushMatrix(); glTranslatef(tpx_local, 0.0f, tpz_local);
    drawCube(0,size_val*4,0,size_val*2.5f,size_val*8,size_val*2.5f,0.7f,0.6f,0.9f);
    for(int row=0;row<4;row++){for(int col=0;col<3;col++){
        float wx_pos=-size_val*0.9f+col*size_val*0.9f; float wy_pos=size_val*1.5f+row*size_val*1.5f;
        drawWindow(wx_pos,wy_pos,size_val*1.25f+0.01f,size_val*0.7f,size_val*0.7f,0.3f,0.6f,0.9f);
        drawWindow(wx_pos,wy_pos,-size_val*1.25f-0.01f,size_val*0.7f,size_val*0.7f,0.3f,0.6f,0.9f);}}
    glPopMatrix();
}

void drawYellowBuilding(float yx_local, float yz_local, float size_val) {
    glPushMatrix(); glTranslatef(yx_local, 0.0f, yz_local);
    drawCube(0,size_val*2.5f,0,size_val*3,size_val*5,size_val*3,0.9f,0.7f,0.3f);
    for(int row=0;row<4;row++){for(int col=0;col<4;col++){
        float wx_pos=-size_val*1.1f+col*size_val*0.75f; float wy_pos=size_val*1+row*size_val*1;
        drawWindow(wx_pos,wy_pos,size_val*1.5f+0.01f,size_val*0.6f,size_val*0.6f,0.3f,0.6f,0.9f);
        drawWindow(wx_pos,wy_pos,-size_val*1.5f-0.01f,size_val*0.6f,size_val*0.6f,0.3f,0.6f,0.9f);}}
    drawCube(0,size_val*0.6f,size_val*1.5f+0.01f,size_val*1.2f,size_val*1.2f,0.1f,0.4f,0.3f,0.1f);
    glPopMatrix();
}

void drawRedRoofBuilding(float rrx_local, float rrz_local, float size_val) {
    glPushMatrix(); glTranslatef(rrx_local, 0.0f, rrz_local);
    drawCube(0,size_val*2.5f,0,size_val*3,size_val*5,size_val*3,0.9f,0.85f,0.7f);
    glColor3f(0.8f,0.2f,0.1f); glPushMatrix();glTranslatef(0,size_val*5,0);glScalef(size_val*3.2f,size_val*0.5f,size_val*3.2f);glutSolidCube(1.0f);glPopMatrix();
    float windowSize=size_val*0.4f,gapSize=size_val*0.3f;
    for(int row=0;row<5;row++){for(int col=0;col<5;col++){
        float wx_pos=-size_val*1.2f+col*(windowSize+gapSize*0.3f);float wy_pos=size_val*1+row*(windowSize+gapSize*0.3f);
        drawWindow(wx_pos,wy_pos,size_val*1.5f+0.01f,windowSize,windowSize,0.1f,0.1f,0.2f);
        drawWindow(wx_pos,wy_pos,-size_val*1.5f-0.01f,windowSize,windowSize,0.1f,0.1f,0.2f);}}
    glPopMatrix();
}

void drawNewOfficeBuilding(float ox_local,float oz_local,float width_val,float height_val,float depth_val,float r_color,float g_color,float b_color) {
    glPushMatrix(); glTranslatef(ox_local,0,oz_local);
    drawCube(0,height_val/2,0,width_val,height_val,depth_val,r_color,g_color,b_color);
    float windowWidth=width_val*0.15f,windowHeight=height_val*0.1f,windowGap=height_val*0.05f,startY=windowGap;
    int rows=(int)((height_val-windowGap)/(windowHeight+windowGap)),cols=4;
    for(int row=0;row<rows;row++){for(int col=0;col<cols;col++){
        float wx_pos=-(width_val/2)+(width_val/(cols+1))*(col+1);float wy_pos=startY+row*(windowHeight+windowGap);
        drawWindow(wx_pos,wy_pos,depth_val/2+0.01f,windowWidth,windowHeight,0.1f,0.1f,0.3f);
        drawWindow(wx_pos,wy_pos,-depth_val/2-0.01f,windowWidth,windowHeight,0.1f,0.1f,0.3f);}}
    glPopMatrix();
}

void drawGlassBuilding(float gx_local, float gz_local, float size_val) {
    glPushMatrix(); glTranslatef(gx_local,0,gz_local);
    drawCube(0,size_val*3,0,size_val*3,size_val*6,size_val*3,0.7f,0.9f,0.8f);
    for(int i=0;i<3;i++){
        drawCube(0,size_val*(i*1.5f+1.5f),size_val*1.5f+0.01f,size_val*2.5f,size_val*1.2f,0.1f,0.1f,0.6f,0.5f);
        drawCube(0,size_val*(i*1.5f+1.5f),-(size_val*1.5f+0.01f),size_val*2.5f,size_val*1.2f,0.6f,0.5f,0.5f,0.5f);}
    drawCube(0,size_val*6.2f,0,size_val*3.2f,size_val*0.4f,size_val*3.2f,0.5f,0.8f,0.7f);
    glPopMatrix();
}

void updateGameLogic(int value) {
    if (!isGameOver) {
        if (deltaAngle!=0.0f){camLookAngle+=deltaAngle;orientCamera(camLookAngle);}
        if (deltaMove!=0)moveBalloon(deltaMove);
        if (balloonVerticalSpeed!=0.0f){balloonY+=balloonVerticalSpeed;
            if(balloonY<2.0f)balloonY=2.0f; if(balloonY>40.0f)balloonY=40.0f;}
        for(int i=0;i<MAX_COINS;++i){if(!coins[i].isCollected){
            coins[i].rotation+=2.5f;if(coins[i].rotation>360.0f)coins[i].rotation-=360.0f;}}
        checkCoinCollision(); checkBuildingCollisions();
        if (!isGameOver) checkEnvironmentObjectCollisions();
        if (collectedCoinsCount>=COINS_TO_WIN){/* Game Won - Tampilkan pesan */}
    }
    glutPostRedisplay(); glutTimerFunc(16,updateGameLogic,0);
}

void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (isGameOver) { displayGameOverMessage(); glutSwapBuffers(); return; }
    glLoadIdentity();
    float camActualX = balloonX - camLookOffsetX * cameraDistance;
    float camActualY = balloonY + cameraHeightOffset;
    float camActualZ = balloonZ - camLookOffsetZ * cameraDistance;
    gluLookAt(camActualX,camActualY,camActualZ,balloonX,balloonY+1.5f,balloonZ,0.0f,1.0f,0.0f);
    Grid();
    float buildingBaseWidth=JALAN_LEBAR_TOTAL*1.5f,buildingBaseDepth=JALAN_LEBAR_TOTAL*1.5f,buildingHeight1=10.0f;
    drawBuilding(-40.0f,-40.0f,buildingBaseWidth,buildingBaseDepth,buildingHeight1,0,0,0);drawBuilding(0.0f,-40.0f,buildingBaseWidth,buildingBaseDepth,buildingHeight1,0,0,0);
    drawBuilding(40.0f,-40.0f,buildingBaseWidth,buildingBaseDepth,buildingHeight1,0,0,0);drawBuilding(-40.0f,40.0f,buildingBaseWidth,buildingBaseDepth,buildingHeight1,0,0,0);
    drawBuilding(0.0f,40.0f,buildingBaseWidth,buildingBaseDepth,buildingHeight1,0,0,0);drawBuilding(40.0f,40.0f,buildingBaseWidth,buildingBaseDepth,buildingHeight1,0,0,0);
    drawBuilding(-20.0f,30.0f,buildingBaseWidth,buildingBaseDepth,buildingHeight1,0,0,0);drawBuilding(20.0f,30.0f,buildingBaseWidth,buildingBaseDepth,buildingHeight1,0,0,0);
    drawBuilding(-30.0f,10.0f,buildingBaseWidth,buildingBaseDepth,buildingHeight1,0,0,0);drawBuilding(30.0f,10.0f,buildingBaseWidth,buildingBaseDepth,buildingHeight1,0,0,0);
    drawBuilding(-30.0f,-10.0f,buildingBaseWidth,buildingBaseDepth,buildingHeight1,0,0,0);drawBuilding(-20.0f,-10.0f,buildingBaseWidth,buildingBaseDepth,buildingHeight1,0,0,0);
    drawBuilding(20.0f,-10.0f,buildingBaseWidth,buildingBaseDepth,buildingHeight1,0,0,0);drawBuilding(-20.0f,-30.0f,buildingBaseWidth,buildingBaseDepth,buildingHeight1,0,0,0);
    drawBuilding(20.0f,-30.0f,buildingBaseWidth,buildingBaseDepth,buildingHeight1,0,0,0);
    glPushMatrix();glTranslatef(-10.0f,0.0f,-10.0f);glScalef(0.8f,0.8f,0.8f);drawUniversitasBuilding();glPopMatrix();
    drawTallPurpleBuilding(19.0f,10.0f,1.5f);drawYellowBuilding(10.0f,-9.0f,2.0f);drawYellowBuilding(23.0f,10.0f,1.2f);
    drawTallPurpleBuilding(30.0f,-9.0f,2.0f);drawNewOfficeBuilding(-10.0f,10.0f,12.0f,10.0f,8.0f,0.4f,0.4f,0.7f);
    drawGlassBuilding(10.0f,10.0f,1.8f);drawRedRoofBuilding(-30.0f,-30.0f,1.5f);drawNewOfficeBuilding(10.0f,-30.0f,10.0f,15.0f,8.0f,0.5f,0.5f,0.8f);
    drawStreetLight(-35.0f,-23.0f,6.0f);drawStreetLight(-15.0f,-23.0f,6.0f);drawStreetLight(-35.0f,-3.0f,6.0f);drawStreetLight(-15.0f,-3.0f,6.0f);
    drawStreetLight(15.0f,-3.0f,6.0f);drawStreetLight(35.0f,-3.0f,6.0f);drawStreetLight(-35.0f,17.0f,6.0f);drawStreetLight(-15.0f,17.0f,6.0f);
    drawStreetLight(15.0f,17.0f,6.0f);drawStreetLight(35.0f,17.0f,6.0f);drawStreetLight(-43.0f,-35.0f,6.0f);drawStreetLight(-43.0f,-15.0f,6.0f);
    drawStreetLight(-3.0f,-35.0f,6.0f);drawStreetLight(-3.0f,-15.0f,6.0f);drawStreetLight(-3.0f,15.0f,6.0f);drawStreetLight(-3.0f,35.0f,6.0f);
    drawTrafficLight(-40.0f-JALAN_LEBAR-1.0f,0.0f-JALAN_LEBAR-1.0f);drawTrafficLight(0.0f-JALAN_LEBAR-1.0f,0.0f-JALAN_LEBAR-1.0f);
    drawTrafficLight(40.0f-JALAN_LEBAR-1.0f,0.0f-JALAN_LEBAR-1.0f);drawTrafficLight(0.0f-JALAN_LEBAR-1.0f,20.0f-JALAN_LEBAR-1.0f);
    drawTrafficLight(0.0f-JALAN_LEBAR-1.0f,-20.0f-JALAN_LEBAR-1.0f);
    drawCar(-30.0f,20.0f+JALAN_LEBAR/2.0f,1.0f,0.5f,0.0f);drawCar(-40.0f+JALAN_LEBAR/2.0f,30.0f,0.0f,0.0f,1.0f);
    drawCar(-10.0f,-20.0f+JALAN_LEBAR/2.0f,0.8f,0.2f,0.2f);drawCar(10.0f,-20.0f+JALAN_LEBAR/2.0f,0.2f,0.8f,0.2f);
    drawCar(-20.0f,0.0f+JALAN_LEBAR/2.0f,0.2f,0.2f,0.8f);drawCar(20.0f,0.0f+JALAN_LEBAR/2.0f,0.8f,0.8f,0.2f);
    drawCar(10.0f,20.0f+JALAN_LEBAR/2.0f,0.5f,0.0f,0.5f);drawCar(-40.0f+JALAN_LEBAR/2.0f,-10.0f,0.0f,0.5f,0.5f);
    drawCar(-40.0f+JALAN_LEBAR/2.0f,10.0f,0.5f,0.5f,0.0f);drawCar(0.0f+JALAN_LEBAR/2.0f,-30.0f,0.7f,0.3f,0.0f);
    drawCar(0.0f+JALAN_LEBAR/2.0f,-10.0f,0.3f,0.7f,0.0f);drawCar(0.0f+JALAN_LEBAR/2.0f,10.0f,0.0f,0.3f,0.7f);
    drawCar(0.0f+JALAN_LEBAR/2.0f,30.0f,0.7f,0.0f,0.3f);drawCar(40.0f+JALAN_LEBAR/2.0f,-20.0f,0.4f,0.2f,0.6f);
    drawCar(40.0f+JALAN_LEBAR/2.0f,20.0f,0.6f,0.4f,0.2f);
    float tree_offset_from_building = JALAN_LEBAR + 0.5f;
    drawPohon(-30.0f-2.0f,0.0f-tree_offset_from_building);drawPohon(-25.0f,-45.0f);drawPohon(-5.0f,-45.0f);drawPohon(15.0f,-45.0f);
    drawPohon(35.0f,-45.0f);drawPohon(-45.0f,-25.0f);drawPohon(-45.0f,-5.0f);drawPohon(-45.0f,15.0f);drawPohon(-45.0f,35.0f);
    drawPohon(-25.0f,45.0f);drawPohon(-5.0f,45.0f);drawPohon(15.0f,45.0f);drawPohon(35.0f,45.0f);drawPohon(45.0f,-25.0f);
    drawPohon(45.0f,-5.0f);drawPohon(45.0f,15.0f);drawPohon(45.0f,35.0f);
    for (int i=0;i<MAX_COINS;++i){if(!coins[i].isCollected)drawCoin3D(coins[i].radius,coins[i].x,coins[i].y,coins[i].z,coins[i].rotation);}
    glPushMatrix();
    glTranslatef(balloonX,balloonY,balloonZ);glRotatef(camLookAngle*(180.0f/M_PI)+90.0f,0.0f,1.0f,0.0f);
    drawBalloon();drawSquareRoof();drawRopes();drawBalloonBasket();drawSteve(0.0f,0.4f,0.0f,0.3f);
    glPopMatrix();
    displayScore();
    glutSwapBuffers();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50,50); glutInitWindowSize(1280,720);
    glutCreateWindow("Balon Udara Koin - Gedung Kota");
    initOpenGL(); setupLighting(); initTextures(); // <<<< Tekstur diinisialisasi di sini
    resetGame();
    glutIgnoreKeyRepeat(0);
    glutSpecialFunc(pressKey); glutSpecialUpFunc(releaseKey);
    glutKeyboardFunc(processNormalKeys); glutKeyboardUpFunc(releaseNormalKeys);
    glutDisplayFunc(renderScene);
    glutTimerFunc(16, updateGameLogic, 0); // Missing semicolon was here
    glutReshapeFunc(Reshape);
    glutMainLoop();
    return 0;
}