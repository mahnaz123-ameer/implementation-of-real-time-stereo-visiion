#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace cv;

const int IMAGE_WIDTH = 320;
const int IMAGE_HEIGHT = 240;
const int MAX_DISPARITY = 60;

#define dbg(a) cerr << #a << ": " << a << "\n"

typedef struct{
    int imagesize_x, imagesize_y;
    int **pixel;
}image_t;

int displayImages(string leftImagePath, string disparityImagePath, string rightImagesPath) {
    dbg("displayImages");
    // Load images with desired color treatments
    cv::Mat image1 = cv::imread(leftImagePath, cv::IMREAD_COLOR);
    cv::Mat image2 = cv::imread(disparityImagePath, cv::IMREAD_GRAYSCALE);
    cv::Mat image3 = cv::imread(rightImagesPath, cv::IMREAD_COLOR);

    // Check if images are loaded successfully
    if (image1.empty() || image2.empty() || image3.empty()) {
        std::cerr << "Failed to load one or more images." << std::endl;
        return -1;
    }

    // Ensure all images have the same number of rows (height)
    int maxRows = std::max({image1.rows, image2.rows, image3.rows});
    int maxCols = image1.cols + image2.cols + image3.cols;

    // Resize images to match the maximum height
    cv::resize(image1, image1, cv::Size(maxCols / 3, maxRows));
    cv::resize(image2, image2, cv::Size(maxCols / 3, maxRows));
    cv::resize(image3, image3, cv::Size(maxCols / 3, maxRows));

    // Convert grayscale image to three channels
    cv::Mat image2Color;
    cv::cvtColor(image2, image2Color, cv::COLOR_GRAY2BGR);

    // Concatenate the images horizontally
    cv::Mat result;
    cv::hconcat(image1, image2Color, result);
    cv::hconcat(result, image3, result);

    // Display the concatenated image
    cv::imshow("Images", result);

    // Wait for a short duration to allow the window to update
    cv::waitKey(1);

    // Wait for 5 seconds
    std::this_thread::sleep_for(std::chrono::seconds(20));

    return 0;
}

// Convert opencv image to .pgm file
void savePGM(const std::string& filename, const cv::Mat& image) {

    std::ofstream file(filename, std::ios::out);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Write PGM header
    file << "P2\n";
    file << "# Created by OpenCV\n";
    file << image.cols << " " << image.rows << "\n";
    file << "255\n";  // Maximum pixel value

    // Write pixel values
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            file << static_cast<int>(image.at<uchar>(i, j)) << " ";
        }
        file << "\n";
    }

    file.close();
}

// Convert .pgm file to image_t
void readPGMP2(image_t *image, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Read PGM header
    char magic[3];
    fscanf(file, "%2s\n", magic);
    if (magic[0] != 'P' || magic[1] != '2') {
        fprintf(stderr, "Not a PGM P2 file\n");
        exit(EXIT_FAILURE);
    }

    // Skip comments
    char c = getc(file);
    while (c == '#') {
        while (getc(file) != '\n');
        c = getc(file);
    }
    ungetc(c, file);

    // Read image dimensions
    fscanf(file, "%d %d\n", &(image->imagesize_x), &(image->imagesize_y));

    // Allocate memory for pixel array
    image->pixel = (int **)malloc(image->imagesize_x * sizeof(int *));
    for (int i = 0; i < image->imagesize_x; ++i) {
        image->pixel[i] = (int *)malloc(image->imagesize_y * sizeof(int));
    }

    // Read pixel data
    for (int y = 0; y < image->imagesize_y; ++y) {
        for (int x = 0; x < image->imagesize_x; ++x) {
            fscanf(file, "%d", &(image->pixel[x][y]));
        }
    }

    fclose(file);
}

// Compute Disparity
void computeDisparity(image_t *leftImage, image_t *rightImage, image_t *disparityMap) {
    // dbg(leftImage->imagesize_x);
    // dbg(rightImage->imagesize_x);

    disparityMap->imagesize_x = IMAGE_WIDTH;
    disparityMap->imagesize_y = IMAGE_HEIGHT;

    // Allocate memory for pixel array
    disparityMap->pixel = (int **)malloc(disparityMap->imagesize_x * sizeof(int *));
    for (int i = 0; i < disparityMap->imagesize_x; ++i) {
        disparityMap->pixel[i] = (int *)malloc(disparityMap->imagesize_y * sizeof(int));
    }

    for (int y = 0; y < IMAGE_HEIGHT; ++y) {
        for (int x = 0; x < IMAGE_WIDTH; ++x) {
            int minDifference = 256;  // Initialize with a value higher than possible
            int bestDisparity = 0;

            for (int d = 0; d < MAX_DISPARITY; ++d) {
                int leftPixel = leftImage->pixel[x][y];
                if (x - d < 0)  continue;
                int rightPixel = rightImage->pixel[x - d][y];

                int difference = abs(leftPixel - rightPixel);
                if (difference < minDifference) {
                    minDifference = difference;
                    bestDisparity = d;
                }
            }
            disparityMap->pixel[x][y] = bestDisparity;
        }
    }
}

// Convert image_t to .pgm file
void writePGMP2(const char *filename, const image_t *image) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Write PGM header
    fprintf(file, "P2\n");
    fprintf(file, "# Generated by your program\n");
    fprintf(file, "%d %d\n", image->imagesize_x, image->imagesize_y);
    fprintf(file, "255\n");

    // Write pixel data
    for (int y = 0; y < image->imagesize_y; ++y) {
        for (int x = 0; x < image->imagesize_x; ++x) {
            fprintf(file, "%d ", image->pixel[x][y]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}


int processImages(cv::Mat leftImage, cv::Mat rightImage) {
    dbg(leftImage.cols);
    dbg(leftImage.rows);
    dbg("processImages");
    // // Save in .png format
    imwrite("leftImage.png", leftImage);
    imwrite("rightImage.png", rightImage);

    dbg("ok");

    // // Save in .pgm format
    savePGM("leftImage.pgm", leftImage);
    savePGM("rightImage.pgm", rightImage);

    dbg("ok2");

    // Save in image struct
    image_t   image_left, image_right, image_disp;
    readPGMP2(&image_left, string("leftImage.pgm").c_str());
    readPGMP2(&image_right, string("rightImage.pgm").c_str());

    dbg("ok3");

    computeDisparity(&image_left, &image_right, &image_disp);

    dbg("ok4");

    writePGMP2(string("dispImage.pgm").c_str(), &image_left);
    // writePGMP2(rightImage.c_str(), image_right);


    displayImages("leftImage.png", "dispImage.pgm", "rightImage.png");
    return 0;
}

int capture() {
    dbg("capture");
    // Open the first camera
    cv::VideoCapture cap1(0, CAP_V4L2);
    if (!cap1.isOpened()) {
        cerr << "Error opening camera 1." << endl;
        return -1;
    }

    // Open the second camera
    cv::VideoCapture cap2(2, CAP_V4L2);
    if (!cap2.isOpened()) {
        cerr << "Error opening camera 2." << endl;
        return -1;
    }

    // int frameWidth = 450;
    // int frameHeight = 375;
    cap1.set(cv::CAP_PROP_FRAME_WIDTH, IMAGE_WIDTH);
    cap1.set(cv::CAP_PROP_FRAME_HEIGHT, IMAGE_HEIGHT);
    cap2.set(cv::CAP_PROP_FRAME_WIDTH, IMAGE_WIDTH);
    cap2.set(cv::CAP_PROP_FRAME_HEIGHT, IMAGE_HEIGHT);

    while (true) {
        cv::Mat frame1, frame2;


        // Capture frame from camera 1
        cap1 >> frame1;
        if (frame1.empty()) {
            cerr << "Error capturing frame from camera 1." << endl;
            return -1;
        }

        // Capture frame from camera 2
        cap2 >> frame2;
        if (frame2.empty()) {
            cerr << "Error capturing frame from camera 2." << endl;
            return -1;
        }

        processImages(frame1, frame2);

        break;
    }
    return 0;
}

int main() {
    capture();
}

// clear && g++ main.cpp -std=c++11 `pkg-config --cflags --libs opencv4` && ./a.out
