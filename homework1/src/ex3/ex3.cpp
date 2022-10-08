#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

bool judgeByRect(std::vector<cv::Point> contour);

void drawRotatedRect(cv::Mat &drawer, cv::RotatedRect &rect);

void drawCenterPoint(cv::Mat &drawer, cv::RotatedRect &rect);

void drawId(cv::Mat &drawer,int &cnt,cv::Point2f &lst_center_point,cv::RotatedRect &rect);

double dis(const cv::Point2f &p1, const cv::Point2f &p2);

int main() {
    cv::VideoCapture capture("../src/ex3/res/input.mp4");
    cv::Mat tmp;
    capture.read(tmp); // just for read size
    double fps=capture.get(cv::CAP_PROP_FPS);
    cv::VideoWriter writer("../src/ex3/output/output.avi",
                           cv::VideoWriter::fourcc('M','J','P','G'),
                           fps,cv::Size(tmp.cols,tmp.rows), true);
//    double fps=capture.get(cv::CAP_PROP_FPS)/2;
//    cv::VideoWriter writer("../src/ex3/output/output-slow.avi",
//                           cv::VideoWriter::fourcc('M','J','P','G'),
//                           fps,cv::Size(tmp.cols,tmp.rows), true);

    cv::Mat src;
    cv::Point2f lst_center_point;
    int cnt=0;

    while (capture.read(src)){
        cv::Mat grey;
        cv::cvtColor(src, grey, cv::COLOR_BGR2GRAY);

        cv::Mat binarization;
        cv::threshold(grey, binarization, 100, 255, cv::THRESH_BINARY);

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::Mat drawer = cv::Mat::zeros(cv::Size(src.cols, src.rows), CV_8UC3);
        cv::RotatedRect rect;
        cv::findContours(binarization, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
        for (int i = 0; i != -1; i = hierarchy[i][0]) {
            cv::drawContours(drawer, contours, i, {20, 20, 220}, 1);
            if (judgeByRect(contours[i])) {
                rect = cv::minAreaRect(contours[i]);
            }
        }
        drawRotatedRect(src, rect);
        drawCenterPoint(src, rect);
        drawId(src,cnt,lst_center_point,rect);
        cv::imshow("output",src);
        writer<<src;
        cv::waitKey(100);
    }
    writer.release();
    capture.release();
    return 0;
}

bool judgeByRect(std::vector<cv::Point> contour) {
    if (cv::arcLength(contour, true)<=50) return false;
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
    float x = center_point.x ;
    float y = center_point.y ;

    auto str_x=std::to_string(round(x*1e3)/1e3);
    auto str_y=std::to_string(round(y*1e3)/1e3);
    str_x=str_x.substr(0,str_x.find(".")+4);
    str_y=str_y.substr(0,str_y.find(".")+4);

    cv::circle(drawer, center_point, 5, {0, 255, 0}, 4);
    cv::putText(drawer, "TARGET(" + str_x + "," + str_y + ")",
                {static_cast<int>(x + 10), static_cast<int>(y - 10)},
                cv::FONT_HERSHEY_TRIPLEX, 0.4, {225, 225, 25}, 1);
}

void drawId(cv::Mat &drawer,int &cnt,cv::Point2f &lst_center_point,cv::RotatedRect &rect){
    const double MIN_TRACK_DISTANCE=200;
    cv::Point2f center_point=rect.center;
    float x = center_point.x ;
    float y = center_point.y ;

    if (!x&&!y) return;
    if(!cnt|| dis(lst_center_point,center_point)>MIN_TRACK_DISTANCE) {
        cnt++;
    }
    cv::putText(drawer,"ID="+ std::to_string(cnt),
                {static_cast<int>(x + 10), static_cast<int>(y + 10)},
                cv::FONT_HERSHEY_TRIPLEX, 0.4, {225, 225, 25}, 1);
    lst_center_point={x,y};
}

double dis(const cv::Point2f &p1, const cv::Point2f &p2){
    return sqrt(pow((p1.x-p2.x),2)+ pow((p1.y-p2.y),2));
}
