
#include <GLUT/glut.h>

GLfloat direct1=0;

void init()
{
    // 添加光照等效果
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    double aspect = (double)viewport[2] / (double)viewport[3];
    gluPerspective(60, aspect, 1, 100);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // move back a bit
    glTranslatef( direct1, 0, -35 );
    
    static float angle = 0;
    angle += 1.0f;
    
    // 画一个3D实体Cube
    glPushMatrix();
    glTranslatef(direct1,0,0);
    glRotatef(angle, 0.1, 0.2, 0.5);
    glColor3ub(255,0,255);
    glutSolidCube(5);
    glPopMatrix();
    
//    画一个3D实体圆锥
    glPushMatrix();
    glTranslatef(10,-10,0);
    glRotatef(angle, 0.1, 0.2, 0.5);
    glColor3ub(255,0,0);
    glutSolidCone(4.0, 5, 3, 2);
    glPopMatrix();
    
    // 画一个3D实体茶壶
    glPushMatrix();
    glTranslatef(10,10,0);
    glRotatef(angle, 0.1, 0.2, 0.5);
    glColor3ub(0,255,0);
    glutSolidTeapot(5);
    glPopMatrix();
    
    //画一个3D实体的球
    glPushMatrix();
    glTranslatef(-10,10,0);
    glRotatef(angle, 0.1, 0.2, 0.5);
    glColor3ub(0,0,255);
    glutSolidSphere(8, 8, 8);
    glPopMatrix();
    
    //画一个3D的Cube线框
    glPushMatrix();
    glTranslatef(-10,-10,0);
    glRotatef(angle, 0.1, 0.2, 0.5);
    glColor3ub(255,255,0);
    glutWireCube(5);
    glPopMatrix();
    
    
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

void timer(int extra)
{
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyPressed (unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            direct1+=0.2;
            break;
        case 's':
            direct1-= 0.2;
            break;
    }

}
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("CUBES");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
      glutKeyboardFunc(keyPressed); 
    init();
    
    glutMainLoop();
    return 0;
}
