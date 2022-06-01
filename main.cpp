#include <opencv2/opencv.hpp>

int main(int, char**) {
    const int sizeX = 2;
    const int sizeY = 3;
    cv::Scalar White = cv::Scalar(255, 255, 255);
    cv::Scalar Grey  = cv::Scalar(127, 127, 127);
    cv::Scalar Black = cv::Scalar(  0,   0,   0);
    cv::Mat_<float> filter1(3, 3);
    filter1 <<  1,  0, -1,
                2,  0, -2,
                1,  0, -1;
    cv::Mat_<float> filter2(3, 3);
    filter2 <<  1,  2,  1,
                0,  0,  0,
               -1, -2, -1;
    cv::Point defaultAnchor = cv::Point(-1, -1);
    float delta = 0.0;

    const int rectSize = 150;
    const int imageSizeX = rectSize * 2;
    const int imageSizeY = rectSize * 3;
    cv::Rect squares[sizeY][sizeX] = {
        {
            cv::Rect(       0,            0, rectSize, rectSize),
            cv::Rect(rectSize,            0, rectSize, rectSize)
        },
        {
            cv::Rect(       0,     rectSize, rectSize, rectSize),
            cv::Rect(rectSize,     rectSize, rectSize, rectSize)
        },
        {
            cv::Rect(       0, 2 * rectSize, rectSize, rectSize),
            cv::Rect(rectSize, 2 * rectSize, rectSize, rectSize)
        }
    };
    cv::Scalar squareColors[sizeY][sizeX] = {
        { White, Grey }, { Black, White }, { Grey, Black }
    };

    const int circleShift = 75;
    const int circleRadius = 50;
    cv::Point circleCenters[sizeY][sizeX] = {
        {
            cv::Point(           circleShift,                circleShift),
            cv::Point(rectSize + circleShift,                circleShift)
        },
        {
            cv::Point(           circleShift,     rectSize + circleShift),
            cv::Point(rectSize + circleShift,     rectSize + circleShift)
        },
        {
            cv::Point(           circleShift, 2 * rectSize + circleShift),
            cv::Point(rectSize + circleShift, 2 * rectSize + circleShift)
        }
    };
    cv::Scalar circleColors[sizeY][sizeX] = {
        { Grey, White }, { White, Black }, { Black, Grey }
    };

    cv::Mat image1 = cv::Mat(imageSizeY, imageSizeX, CV_8UC1);

    for (int row = 0; row < sizeY; ++row) {
        for (int col = 0; col < sizeX; ++col) {
            cv::rectangle(image1, squares[row][col], squareColors[row][col], cv::FILLED);
            cv::circle(image1, circleCenters[row][col], circleRadius, circleColors[row][col], cv::FILLED);
        }
    }

    image1.convertTo(image1, CV_32FC1);

    cv::Mat image2;
    cv::filter2D(image1, image2, -1, filter1, defaultAnchor, delta);

    cv::Mat image3;
    cv::filter2D(image1, image3, -1, filter2, defaultAnchor, delta);

    cv::Mat image4 = cv::Mat(imageSizeY, imageSizeX, CV_32FC1);
    for (int row = 0; row < imageSizeY; ++row) {
        for (int col = 0; col < imageSizeX; ++col) {
            float from2 = image2.at<float>(row, col);
            float from3 = image3.at<float>(row, col);
            image4.at<float>(row, col) = sqrt(from2 * from2 + from3 * from3);
        }
    }
    
    image1.convertTo(image1, CV_8UC1);
    image2.convertTo(image2, CV_8UC1);
    image3.convertTo(image3, CV_8UC1);
    image4.convertTo(image4, CV_8UC1);

    // cv::namedWindow("image1", cv::WINDOW_AUTOSIZE);
    // cv::namedWindow("image2", cv::WINDOW_AUTOSIZE);
    // cv::namedWindow("image3", cv::WINDOW_AUTOSIZE);
    // cv::namedWindow("image4", cv::WINDOW_AUTOSIZE);
    // cv::imshow("image1", image1);
    // cv::imshow("image2", image2);
    // cv::imshow("image3", image3);
    // cv::imshow("image4", image4);

    cv::imwrite("image1.png", image1);
    cv::imwrite("image2.png", image2);
    cv::imwrite("image3.png", image3);
    cv::imwrite("image4.png", image4);

    cv::waitKey();
    return 0;
}
