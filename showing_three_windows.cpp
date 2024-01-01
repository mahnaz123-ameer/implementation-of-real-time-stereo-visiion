#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

cv::Mat image1, image2, image3;
int iteration = 0;

void loadImage(const std::string& path, cv::Mat& image) {
    image = cv::imread(path, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Error loading image: " << path << std::endl;
        exit(-1);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Display image1
    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH) / 3, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRasterPos2i(-1, 1);
    glPixelZoom(1, -1);
    glDrawPixels(image1.cols, image1.rows, GL_BGR, GL_UNSIGNED_BYTE, image1.data);

    // Display image2
    glViewport(glutGet(GLUT_WINDOW_WIDTH) / 3, 0, glutGet(GLUT_WINDOW_WIDTH) / 3, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRasterPos2i(-1, 1);
    glPixelZoom(1, -1);
    glDrawPixels(image2.cols, image2.rows, GL_BGR, GL_UNSIGNED_BYTE, image2.data);

    // Display image3
    glViewport(2 * glutGet(GLUT_WINDOW_WIDTH) / 3, 0, glutGet(GLUT_WINDOW_WIDTH) / 3, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRasterPos2i(-1, 1);
    glPixelZoom(1, -1);
    glDrawPixels(image3.cols, image3.rows, GL_BGR, GL_UNSIGNED_BYTE, image3.data);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutReshapeWindow(3 * w, h);
}

void idle() {
    if (iteration < 100) {
        int j = iteration % 4;

        string img_left_src = "capture/PNG/img_left" + to_string(j) + ".png";
        loadImage(img_left_src, image1);

        string img_middle_src = "capture/PGM/img_left" + to_string(j) + ".pgm";
        loadImage(img_middle_src, image2);

        string img_right_src = "capture/PNG/img_right" + to_string(j) + ".png";
        loadImage(img_right_src, image3);

        std::this_thread::sleep_for(std::chrono::seconds(5));  // Sleep for 5 seconds

        iteration++;
    }
    else {
        exit(0);  // Exit after 100 iterations
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Calculating Disparity");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutMainLoop();

    return 0;
}

// g++ main.cpp $(pkg-config --cflags --libs opencv4) -lopencv_core -lopencv_highgui -lopencv_imgproc -lglut -lGLU -lGL
