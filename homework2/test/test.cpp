#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <cmath>

#define N 80
#define R 0.01f

std::ifstream ifs("../src/ex2/output/out.txt");

void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    // draw a circle
    glColor3f(1.0, 0.0, 0.0);

    for (int i = 0; i < 3193; ++i) {
        double x, y;
        ifs >> x >> y;
        glBegin(GL_POLYGON);
        for (int j = 0; j < N; ++j) {
            glVertex2d(x/1000 + R * cos(2 * M_PI / N * j), y/1000 + R * sin(2 * M_PI / N * j));
        }
        glEnd();
    }

    glFlush();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("result");
    glutDisplayFunc(&myDisplay);
    glutMainLoop();
    return 0;
}
