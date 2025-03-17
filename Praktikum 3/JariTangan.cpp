#include <GL/glut.h>
//membuat lengan atas, lengan bawah dan telapak tangan
static int shoulder = 0, elbow = 0, wrist = 0;
// membuat jari
static int thumbBase = 0, thumbMid = 0;
static int indexBase = 0, indexMid = 0;
static int middleBase = 0, middleMid = 0;
static int ringBase = 0, ringMid = 0;
static int pinkyBase = 0, pinkyMid = 0;

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0); 
    glShadeModel(GL_FLAT);
}

void drawFingerJoint(float length, float width, float height) {
    glPushMatrix();
    glScalef(length, width, height);
    glutWireCube(1.0);
    glPopMatrix();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT); 
    glPushMatrix();
    glTranslatef(-0.95, 0.0, 0.0);
    
    // Shoulder
    glTranslatef(-0.6, 0.0, 0.0);
    glRotatef((GLfloat) shoulder, 0.0, 0.0, 1.0); 
    glTranslatef(0.6, 0.0, 0.0); 
    
    glPushMatrix();
    glScalef(1.2, 0.5, 1.0); 
    glutWireCube(1.0); 
    glPopMatrix();
    
    // Elbow
    glTranslatef(0.6, 0.0, 0.0);
    glRotatef((GLfloat) elbow, 0.0, 0.0, 1.0); 
    glTranslatef(0.6, 0.0, 0.0); 
    
    glPushMatrix();
    glScalef(1.2, 0.5, 1.0); 
    glutWireCube(1.0); 
    glPopMatrix(); 
    
    // Wrist
    glTranslatef(0.6, 0.0, 0.0);
    glRotatef((GLfloat) wrist, 0.0, 0.0, 1.0);
    glTranslatef(0.4, 0.0, 0.0);
 
    glPushMatrix();
    glScalef(0.8, 1.2, 0.5); 
    glutWireCube(1.0); 
    glPopMatrix();
    
    // Thumb (ibu jari)
    glPushMatrix();
    glTranslatef(0.0, 0.65, 0.0);
    glRotatef(-50.0, 0.0, 0.0, 1.0);
    glRotatef(15.0, 0.0, 1.0, 0.0);
    glRotatef((GLfloat) thumbBase, 0.0, 1.0, 0.0);
  
    glTranslatef(0.25, 0.0, 0.0);
    drawFingerJoint(0.5, 0.2, 0.2);
    
    // Thumb middle
    glTranslatef(0.25, 0.0, 0.0);
    glRotatef((GLfloat) thumbMid, 0.0, 0.0, 1.0);
    glTranslatef(0.125, 0.0, 0.0);

    drawFingerJoint(0.25, 0.2, 0.2);
    glPopMatrix();
    
    // Index finger(jari telunjuk)
    glPushMatrix();
    glTranslatef(0.4, 0.5, 0.0);
    glRotatef((GLfloat) indexBase, 0.0, 0.0, 1.0);

    glTranslatef(0.2, 0.0, 0.0);
    drawFingerJoint(0.4, 0.18, 0.18);
    
    // Index middle
    glTranslatef(0.2, 0.0, 0.0);
    glRotatef((GLfloat) indexMid, 0.0, 0.0, 1.0);
    glTranslatef(0.2, 0.0, 0.0);

    drawFingerJoint(0.4, 0.18, 0.18);
    glPopMatrix();
    
    // Middle finger(jari tengah)
    glPushMatrix();
    glTranslatef(0.4, 0.2, 0.0);
    glRotatef((GLfloat) middleBase, 0.0, 0.0, 1.0);
 
    glTranslatef(0.2, 0.0, 0.0);
    drawFingerJoint(0.4, 0.18, 0.18);
    
    // Middle middle
    glTranslatef(0.2, 0.0, 0.0);
    glRotatef((GLfloat) middleMid, 0.0, 0.0, 1.0);
    glTranslatef(0.225, 0.0, 0.0);

    drawFingerJoint(0.45, 0.18, 0.18);
    glPopMatrix();
    
    // Ring finger(jari manis)
    glPushMatrix();
    glTranslatef(0.4, -0.15, 0.0);
    glRotatef((GLfloat) ringBase, 0.0, 0.0, 1.0);

    glTranslatef(0.2, 0.0, 0.0);
    drawFingerJoint(0.4, 0.18, 0.18);
    
    // Ring middle
    glTranslatef(0.2, 0.0, 0.0);
    glRotatef((GLfloat) ringMid, 0.0, 0.0, 1.0);
    glTranslatef(0.2, 0.0, 0.0);

    drawFingerJoint(0.4, 0.18, 0.18);
    glPopMatrix();
    
    // Pinky finger(jari kelingking)
    glPushMatrix();
    glTranslatef(0.4, -0.45, 0.0);
    glRotatef((GLfloat) pinkyBase, 0.0, 0.0, 1.0);
  
    glTranslatef(0.15, 0.0, 0.0);
    drawFingerJoint(0.3, 0.16, 0.16);
    
    // Pinky middle
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat) pinkyMid, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);

    drawFingerJoint(0.3, 0.16, 0.16);
    glPopMatrix();
    
    glPopMatrix(); 
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0); 
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 's': shoulder = (shoulder + 5) % 360; glutPostRedisplay(); break;
        case 'S': shoulder = (shoulder - 5) % 360; glutPostRedisplay(); break;
        
        case 'e': elbow = (elbow + 5) % 360; glutPostRedisplay(); break;
        case 'E': elbow = (elbow - 5) % 360; glutPostRedisplay(); break;

        case 'w': wrist = (wrist + 5) % 360; glutPostRedisplay(); break;
        case 'W': wrist = (wrist - 5) % 360; glutPostRedisplay(); break;

        case 't': thumbBase = (thumbBase + 5) % 90; glutPostRedisplay(); break;
        case 'T': thumbBase = (thumbBase - 5) % 90; glutPostRedisplay(); break;
        case 'y': thumbMid = (thumbMid + 5) % 90; glutPostRedisplay(); break;
        case 'Y': thumbMid = (thumbMid - 5) % 90; glutPostRedisplay(); break;

        case 'i': indexBase = (indexBase + 5) % 90; glutPostRedisplay(); break;
        case 'I': indexBase = (indexBase - 5) % 90; glutPostRedisplay(); break;
        case 'o': indexMid = (indexMid + 5) % 90; glutPostRedisplay(); break;
        case 'O': indexMid = (indexMid - 5) % 90; glutPostRedisplay(); break;

        case 'm': middleBase = (middleBase + 5) % 90; glutPostRedisplay(); break;
        case 'M': middleBase = (middleBase - 5) % 90; glutPostRedisplay(); break;
        case 'n': middleMid = (middleMid + 5) % 90; glutPostRedisplay(); break;
        case 'N': middleMid = (middleMid - 5) % 90; glutPostRedisplay(); break;

        case 'r': ringBase = (ringBase + 5) % 90; glutPostRedisplay(); break;
        case 'R': ringBase = (ringBase - 5) % 90; glutPostRedisplay(); break;
        case 'f': ringMid = (ringMid + 5) % 90; glutPostRedisplay(); break;
        case 'F': ringMid = (ringMid - 5) % 90; glutPostRedisplay(); break;

        case 'p': pinkyBase = (pinkyBase + 5) % 90; glutPostRedisplay(); break;
        case 'P': pinkyBase = (pinkyBase - 5) % 90; glutPostRedisplay(); break;
        case 'l': pinkyMid = (pinkyMid + 5) % 90; glutPostRedisplay(); break;
        case 'L': pinkyMid = (pinkyMid - 5) % 90; glutPostRedisplay(); break;

        case 27: exit(0); break;
        default: break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
    glutInitWindowSize(700, 600); 
    glutInitWindowPosition(100, 100); 
    glutCreateWindow(argv[0]); 
    init();
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape); 
    glutKeyboardFunc(keyboard); 
    glutMainLoop(); 
    return 0;
}
