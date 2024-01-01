#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

using namespace std;

#define dbg(a)  cerr << #a << ": " << a << "\n"

const int targetWidth = 450;  // Set your desired width
const int targetHeight = 375; // Set your desired height

void savePGM(const cv::Mat& image, const std::string& filename) {

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


int main() {
    for (int i = 0; i < 4; ++i) {
        string img_left_src = "capture/PNG/img_left" + to_string(i) + ".png";
        string img_left_des = "capture/PGM/img_left" + to_string(i) + ".pgm";
        cv::Mat img_left_cv = cv::imread(img_left_src, cv::IMREAD_UNCHANGED);

        // Resize image
        cv::resize(img_left_cv, img_left_cv, cv::Size(targetWidth, targetHeight));
        cv::imwrite(img_left_src, img_left_cv);
        cv::cvtColor(img_left_cv, img_left_cv, cv::COLOR_BGR2GRAY);
        savePGM(img_left_cv, img_left_des);


        string img_right_src = "capture/PNG/img_right" + to_string(i) + ".png";
        string img_right_des = "capture/PGM/img_right" + to_string(i) + ".pgm";
        cv::Mat img_right_cv = cv::imread(img_right_src, cv::IMREAD_UNCHANGED);

        // Resize image
        resize(img_right_cv, img_right_cv, cv::Size(targetWidth, targetHeight));
        cv::imwrite(img_right_src, img_right_cv);
        cv::cvtColor(img_right_cv, img_right_cv, cv::COLOR_BGR2GRAY);
        savePGM(img_right_cv, img_right_des);
    }
}

// clear && g++ main.cpp -std=c++11 `pkg-config --cflags --libs opencv4` && ./a.out
