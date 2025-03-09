#include <GL/glut.h>
#include <math.h>

float rotAngle = 15.0; // Sudut rotasi

void drawCircle(float x, float y, float radius) {
    const float PI = 3.1415926535898;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 100; i++) {
        float angle = 2 * PI * i / 100;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

void drawGround(int isInclined) {
    glColor3f(0.0, 0.5, 0.0);
    glBegin(GL_QUADS);
    if (isInclined) {
        glVertex2f(-1.0, -0.3);
        glVertex2f(1.0, -0.1);
    } else {
        glVertex2f(-1.0, -0.3);
        glVertex2f(1.0, -0.3);
    }
    glVertex2f(1.0, -1.0);
    glVertex2f(-1.0, -1.0);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Latar belakang
    glColor3f(0.68, 0.85, 0.9);
    glBegin(GL_QUADS);
    glVertex2f(-1.0, 1.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(1.0, -1.0);
    glVertex2f(-1.0, -1.0);
    glEnd();

	// Awan
    glColor3f(1.0, 1.0, 1.0);
    drawCircle(-0.7, 0.8, 0.1);
    drawCircle(-0.6, 0.85, 0.12);
    drawCircle(-0.5, 0.8, 0.1);
    drawCircle(0.3, 0.75, 0.1);
    drawCircle(0.4, 0.8, 0.12);
    drawCircle(0.5, 0.75, 0.1);

    // Jalan
    drawGround(1);

    // Transformasi mobil
    glPushMatrix();
    glTranslatef(0.0, -0.1, 0.0); 
    glRotatef(rotAngle, 0.0, 0.0, 1.0);
    
    // Badan mobil
    glColor3f(0.0, 0.0, 0.6);
    glBegin(GL_QUADS);
    glVertex2f(-0.6, -0.1);
    glVertex2f(0.6, -0.1);
    glVertex2f(0.6, 0.25);
    glVertex2f(-0.6, 0.25);
    glEnd();

    // Atap mobil
    glBegin(GL_QUADS);
    glVertex2f(-0.5, 0.25);
    glVertex2f(0.4, 0.25);
    glVertex2f(0.3, 0.45);
    glVertex2f(-0.5, 0.45);
    glEnd();

	// Jendela kiri & kanan
    glColor3f(0.4, 0.8, 1.0);
    glBegin(GL_QUADS);
    glVertex2f(-0.45, 0.28);
    glVertex2f(-0.2, 0.28);
    glVertex2f(-0.2, 0.42);
    glVertex2f(-0.45, 0.42);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(0.0, 0.28);
    glVertex2f(0.25, 0.28);
    glVertex2f(0.25, 0.42);
    glVertex2f(0.0, 0.42);
    glEnd();
	
	// Lampu depan
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(0.6, 0.05);
    glVertex2f(0.65, 0.07);
    glVertex2f(0.65, -0.02);
    glVertex2f(0.6, -0.04);
    glEnd();

    // Lampu belakang
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2f(-0.6, 0.05);
    glVertex2f(-0.65, 0.07);
    glVertex2f(-0.65, -0.02);
    glVertex2f(-0.6, -0.04);
    glEnd();

    // Ban kiri
    glPushMatrix();
    glTranslatef(-0.4, -0.2, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    drawCircle(0.0, 0.0, 0.12);
    glColor3f(0.5, 0.5, 0.5);
    drawCircle(0.0, 0.0, 0.06);
    glPopMatrix();

    // Ban kanan
    glPushMatrix();
    glTranslatef(0.3, -0.2, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    drawCircle(0.0, 0.0, 0.12);
    glColor3f(0.5, 0.5, 0.5);
    drawCircle(0.0, 0.0, 0.06);
    glPopMatrix();
    
    glPopMatrix(); 
    
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Mobil 2D");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -0.6, 1.0);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
