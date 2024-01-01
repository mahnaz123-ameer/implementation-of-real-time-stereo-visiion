#include <GL/glut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

const int IMG_WIDTH = 450;
const int IMG_HEIGHT = 375;
const int GAP = 40;

int iteration = 0;

GLuint textureID1, textureID2, textureID3;

void loadImage(const char* imagePath, GLuint& textureID) {
    int width, height, channels;
    unsigned char* image = stbi_load(imagePath, &width, &height, &channels, STBI_rgb_alpha);

    if (!image) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        exit(-1);
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    stbi_image_free(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void changeImages() {
    int idx = iteration % 4;
    std::string imagePath1 = "capture/PNG/img_left" + std::to_string(idx) + ".png";
    std::string imagePath2 = "capture/PNG/img_left" + std::to_string(idx) + ".png";
    std::string imagePath3 = "capture/PNG/img_right" + std::to_string(idx) + ".png";

    loadImage(imagePath1.c_str(), textureID1);
    loadImage(imagePath2.c_str(), textureID2);
    loadImage(imagePath3.c_str(), textureID3);

    iteration++;
}

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);

    // Display image 1
    glBindTexture(GL_TEXTURE_2D, textureID1);
    glViewport(0, 0, IMG_WIDTH, IMG_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(1.0f, 375.0f / 450.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-1.0f, 375.0f / 450.0f);
    glEnd();

    // Display image 2
    glBindTexture(GL_TEXTURE_2D, textureID2);
    glViewport(GAP + IMG_WIDTH, 0, IMG_WIDTH, IMG_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(1.0f, 375.0f / 450.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-1.0f, 375.0f / 450.0f);
    glEnd();

    // Display image 3
    glBindTexture(GL_TEXTURE_2D, textureID3);
    glViewport(2 * (GAP + IMG_WIDTH), 0, IMG_WIDTH, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(1.0f, 375.0f / 450.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-1.0f, 375.0f / 450.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void idle(int value) {
    changeImages();
    glutPostRedisplay();
    glutTimerFunc(5000, idle, 0);  // Set the next timer to 5 seconds later
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Set the window size here (width, height)
    glutInitWindowSize(3 * IMG_WIDTH + 2 * GAP, IMG_HEIGHT);  // Adjust the height as needed

    glutCreateWindow("Disparity");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(0, idle, 0);  // Set the timer to start immediately

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    changeImages();  // Load initial set of images

    glutMainLoop();

    return 0;
}
