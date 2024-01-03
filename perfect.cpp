#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <thread>

using namespace std;

#define dbg(a) cerr << #a << ": " << a << "\n"

const int TOTAL_IMAGES = 4;

vector<string> leftImagePaths(TOTAL_IMAGES);
vector<string> disparityImagePaths(TOTAL_IMAGES);
vector<string> rightImagesPaths(TOTAL_IMAGES);

int main() {

    for (int i = 0; i < TOTAL_IMAGES; ++i) {
        leftImagePaths[i] = "capture/PNG/img_left" + to_string(i) + ".png";
        disparityImagePaths[i] = "capture/PGM/img_left" + to_string(i) + ".pgm";
        rightImagesPaths[i] = "capture/PNG/img_right" + to_string(i) + ".png";
    }

    // Create a window to display the images
    cv::namedWindow("Images", cv::WINDOW_NORMAL);

    int iteration = 0;
    while (true) {
        dbg(iteration);
        // Load images with desired color treatments
        cv::Mat image1 = cv::imread(leftImagePaths[iteration], cv::IMREAD_COLOR);
        cv::Mat image2 = cv::imread(disparityImagePaths[iteration], cv::IMREAD_GRAYSCALE);
        cv::Mat image3 = cv::imread(rightImagesPaths[iteration], cv::IMREAD_COLOR);

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
        std::this_thread::sleep_for(std::chrono::seconds(5));

        iteration++;

        if (iteration == 4) {
            iteration = 0;
        }
    }


    return 0;
}

// clear && g++ main.cpp -std=c++11 `pkg-config --cflags --libs opencv4` && ./a.out
