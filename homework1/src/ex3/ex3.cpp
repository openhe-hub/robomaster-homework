#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <queue>

struct Target {
    cv::Point2f lst_point;
    int id;
};

bool judgeByRect(std::vector<cv::Point> contour);

void drawRotatedRect(cv::Mat &drawer, cv::RotatedRect &rect);

void drawCenterPoint(cv::Mat &drawer, cv::RotatedRect &rect);

void drawId(cv::Mat &drawer, int &cnt, std::vector<Target> &targets, std::vector<cv::RotatedRect> &rects);

double dis(const cv::Point2f &p1, const cv::Point2f &p2);

int main() {
    cv::VideoCapture capture("../src/ex3/res/input.mp4");
    cv::Mat tmp;
    capture.read(tmp); // just for read size
//    double fps = capture.get(cv::CAP_PROP_FPS);
//    cv::VideoWriter writer("../src/ex3/output/output.avi",
//                           cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
//                           fps, cv::Size(tmp.cols, tmp.rows), true);
    double fps=capture.get(cv::CAP_PROP_FPS)/4;
    cv::VideoWriter writer("../src/ex3/output/output-slow.avi",
                           cv::VideoWriter::fourcc('M','J','P','G'),
                           fps,cv::Size(tmp.cols,tmp.rows), true);

    cv::Mat src;
    std::vector<Target> targets;
    int cnt = 0;

    while (capture.read(src)) {
        // rgb => grey
        cv::Mat grey;
        cv::cvtColor(src, grey, cv::COLOR_BGR2GRAY);
        // binarization
        cv::Mat binarization;
        cv::threshold(grey, binarization, 100, 255, cv::THRESH_BINARY);
        // find and filter contours
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::Mat drawer = cv::Mat::zeros(cv::Size(src.cols, src.rows), CV_8UC3);
        std::vector<cv::RotatedRect> rects;
        cv::findContours(binarization, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
        for (int i = 0; i != -1; i = hierarchy[i][0]) {
            cv::drawContours(drawer, contours, i, {20, 20, 220}, 1);
            if (judgeByRect(contours[i])) {
                rects.push_back(cv::minAreaRect(contours[i]));
            }
        }
        // recognize
        for (auto &rect: rects) {
            drawRotatedRect(src, rect);
            drawCenterPoint(src, rect);
        }
        // track targets & draw id
        drawId(src, cnt, targets, rects);
        cv::imshow("output", src);
        writer<<src;
        cv::waitKey(100);
    }
    writer.release();
    capture.release();
    return 0;
}

bool judgeByRect(std::vector<cv::Point> contour) {
    if (cv::arcLength(contour, true) <= 50) return false;
    cv::RotatedRect rect = cv::minAreaRect(contour);
    double height = rect.size.height;
    double width = rect.size.width;
    if (std::max(height, width) <= std::min(height, width) * 11) return false;
    return true;
}

void drawRotatedRect(cv::Mat &drawer, cv::RotatedRect &rect) {
    auto *vertices = new cv::Point2f[4];
    rect.points(vertices);
    for (int i = 0; i < 4; ++i) {
        cv::line(drawer, vertices[i], vertices[(i + 1) % 4], {0, 0, 255}, 2);
    }
}

void drawCenterPoint(cv::Mat &drawer, cv::RotatedRect &rect) {
    auto center_point = rect.center;
    float x = center_point.x;
    float y = center_point.y;

    auto str_x = std::to_string(round(x * 1e3) / 1e3);
    auto str_y = std::to_string(round(y * 1e3) / 1e3);
    str_x = str_x.substr(0, str_x.find(".") + 4);
    str_y = str_y.substr(0, str_y.find(".") + 4);

    cv::circle(drawer, center_point, 5, {0, 255, 0}, 4);
    cv::putText(drawer, "TARGET(" + str_x + "," + str_y + ")",
                {static_cast<int>(x + 10), static_cast<int>(y - 10)},
                cv::FONT_HERSHEY_TRIPLEX, 0.4, {225, 225, 25}, 1);
}

void drawId(cv::Mat &drawer, int &cnt, std::vector<Target> &targets, std::vector<cv::RotatedRect> &rects) {
    const double MIN_TRACK_DISTANCE = 200;
    //add new target
    if (rects.empty()) return;
    for (auto it = rects.begin(); it != rects.end(); it++) {
        bool is_tracked = false;
        for (auto &target: targets) {
            if (dis(it->center, target.lst_point) <= MIN_TRACK_DISTANCE) {
                is_tracked = true;
                break;
            }
        }
        if (!is_tracked) targets.push_back({it->center, ++cnt});
    }
    //remove old target
    for (auto it = targets.begin(); it != targets.end(); it++) {
        bool is_tracked = false;
        for (auto &rect: rects) {
            if (dis(it->lst_point, rect.center) <= MIN_TRACK_DISTANCE) {
                is_tracked = true;
                break;
            }
        }
        if (!is_tracked) it = targets.erase(it);
    }
//
//    std::cout << "targets:" << std::endl;
//    for (const auto &target: targets) {
//        std::cout << "(" << target.lst_point.x << "," << target.lst_point.y << ")\t" << target.id << std::endl;
//    }
//    std::cout << "rects:" << std::endl;
//    for (const auto &rect: rects) {
//        std::cout << "(" << rect.center.x << "," << rect.center.y << ")\t" << std::endl;
//    }
//    std::cout << "=======================" << std::endl;

    std::vector<bool> locked(rects.size(),false);
    for (auto &target: targets) {
        double min_dist = 1e5;
        int idx;
        for (int i=0;i<rects.size();i++) {
            double dist = dis(target.lst_point, rects[i].center);
            if (dist < min_dist && !locked[i]) {
                min_dist = dist;
                idx = i;
            }
        }
        cv::putText(drawer, "ID=" + std::to_string(target.id),
                    {static_cast<int>(rects[idx].center.x + 10), static_cast<int>(rects[idx].center.y + 10)},
                    cv::FONT_HERSHEY_TRIPLEX, 0.4, {225, 225, 25}, 1);
        target.lst_point = {rects[idx].center.x,rects[idx].center.y}; //refresh
        locked[idx]= true;
//        std::cout << "(" << target.lst_point.x << "," << target.lst_point.y << ")" << std::endl;
    }
//    std::cout << "=======================" << std::endl;
}


double dis(const cv::Point2f &p1, const cv::Point2f &p2) {
    return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}
