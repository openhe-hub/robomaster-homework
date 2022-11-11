#include <iostream>
#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define MAX_FEATURE_POINTS_NUM 1000
#define MAX_DELTA_DIS 20
#define MIN_FEATURE_POINTS 30

void track(cv::Mat &gray, cv::Mat &prev_gray,
           std::vector<cv::Point2f> &features, std::vector<cv::Point2f> &prev_features,
           std::vector<uchar> &status, std::vector<float> &err,
           cv::Mat &canvas);

int main() {
    cv::VideoCapture capture(R"(../src/ex4/res/lk.mp4)");

    cv::Mat src, gray, prev_gray, feature_canvas, track_canvas;
    std::vector<cv::Point2f> features, prev_features;
    std::vector<uchar> status;
    std::vector<float> err;
    int frame = 0;
    capture>>src;

    cv::VideoWriter writer(R"(../src/ex4/out/out.avi)",
                           cv::VideoWriter::fourcc('M','J','P','G'),
                           50,cv::Size(src.cols,src.rows), true);


    while (!src.empty()) {
        frame++;
        std::cout << prev_features.size() << std::endl;

        feature_canvas = src.clone();
        track_canvas = src.clone();
        // 1. detect feature points
        cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY); // covert from rgb to gray
        cv::goodFeaturesToTrack(gray, features, MAX_FEATURE_POINTS_NUM, 0.01, MAX_DELTA_DIS); // extract feature points
        for (int i = 0; i < features.size(); ++i) {
            cv::circle(feature_canvas, features[i], 2, cv::Scalar(25, 25, 255), 2);
        }
        // 2. Lucas-Kanade optical flow algorithm
        if (prev_features.size() <= MIN_FEATURE_POINTS) {
            // MIND: num of feature points may be too small because of the drastic swift, thus refreshing the prev fpts
            prev_features.clear();
            prev_features.assign(features.begin(), features.end());
            prev_gray = gray.clone();
        } else {
            track(gray, prev_gray, features, prev_features, status, err, track_canvas);
        }

//        cv::imshow("src", src);
        cv::imshow("feature", feature_canvas);
        cv::imshow("track", track_canvas);
        cv::waitKey(100);

        capture>>src;
        writer<<track_canvas;
    }

    capture.release();
    writer.release();
    return 0;
}

void track(cv::Mat &gray, cv::Mat &prev_gray,
           std::vector<cv::Point2f> &features, std::vector<cv::Point2f> &prev_features,
           std::vector<uchar> &status, std::vector<float> &err,
           cv::Mat &canvas) {
    cv::calcOpticalFlowPyrLK(prev_gray, gray, prev_features, features, status, err);
    int cnt = 0;
    for (int i = 0; i < features.size(); ++i) {
        float dist = sqrt(pow(features[i].x - prev_features[i].x, 2) +
                          pow(features[i].y - prev_features[i].y, 2));
        if (status[i] && dist <= MAX_DELTA_DIS) {
            cv::circle(canvas, prev_features[i], 2, cv::Scalar(25, 225, 25), 2);
            cv::circle(canvas, features[i], 2, cv::Scalar(25, 25, 225), 2);
            cv::line(canvas, prev_features[i], features[i], cv::Scalar(225, 25, 25), 2);
            prev_features[cnt++] = features[i]; // update
        }
    }

    prev_features.resize(cnt); // set as old feature points
    features.clear(); // clear current feature points
    prev_gray = gray.clone();
    gray.release();

}

