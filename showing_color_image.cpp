#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <iostream>

GLuint textureID;

void loadImage(const char* imagePath) {
    int width, height, channels;
    unsigned char* image = SOIL_load_image(imagePath, &width, &height, &channels, SOIL_LOAD_RGBA);

    if (!image) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        exit(-1);
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);   // Bottom-left
    glVertex2f(-1.0f, -1.0f);

    glTexCoord2f(1.0f, 1.0f);   // Bottom-right
    glVertex2f(1.0f, -1.0f);

    glTexCoord2f(1.0f, 0.0f);   // Top-right
    glVertex2f(1.0f, 375.0f / 450.0f);

    glTexCoord2f(0.0f, 0.0f);   // Top-left
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

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("PNG Image in OpenGL");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    loadImage("capture/PNG/img_left0.png");  // Replace with the path to your PNG image

    glutMainLoop();

    return 0;
}

// g++ main.cpp -lGL -lGLU -lglut -lSOIL && ./a.out
