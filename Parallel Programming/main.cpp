#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void addNoise(Mat &image, int noise_num);

int main(void)
{
    VideoCapture open_origin_video("vtest.avi");
    if(!open_origin_video.isOpened())
    {
        cerr << "ERROR! Unable to open the Origin Video\n";
        open_origin_video.release();
        return -1;
    }
    
    double fps = open_origin_video.get(CAP_PROP_FPS);
    int width = open_origin_video.get(CAP_PROP_FRAME_WIDTH);
    int height = open_origin_video.get(CAP_PROP_FRAME_HEIGHT);
    int fourcc = VideoWriter::fourcc('m', 'p', '4', 'v');
    
    VideoWriter save_origin_video("save_origin_video.mov", fourcc, fps, Size(width, height));
    VideoWriter save_noise_video("save_noise_video.mov", fourcc, fps, Size(width, height));
    
    while(open_origin_video.isOpened())
    {
        Mat origin_frame;
        open_origin_video >> origin_frame;
        
        if(origin_frame.empty())
        {
            cerr << "ERROR! blank Origin Frame grabbed\n";
            return 0;
        }
        
        // 원본 영상 출력, 저장
        imshow("Origin Video", origin_frame);
        save_origin_video << origin_frame;


        Mat noise_frame;
        noise_frame = origin_frame.clone();
        
        addNoise(noise_frame, 5000);    // 노이즈 추가

        // 노이즈가 적용된 영상 출력, 저장
        imshow("Noise Video", noise_frame);
        save_noise_video << noise_frame;
        
        
        if(waitKey(1000 / fps) == 'q')
        {
            break;
        }
    }
    
    open_origin_video.release();
    save_origin_video.release();
    save_noise_video.release();
    
    
    destroyAllWindows();
    
    return 0;
}


void addNoise(Mat &image, int noise_num)
{
    srand((unsigned int)time(NULL));
    
    for(int k = 0; k < noise_num; k++)
    {
        int col_pixel = rand() % image.cols;    // noise를 추가할 열을 임의로 선택
        int row_pixel = rand() % image.rows;    // noise를 추가할 행을 임의로 선택
        int ch_num = image.channels();
        int noise_color = (rand() % 2) * 255;   // 0 or 255
        
        if(ch_num == 1) // 흑백 영상
        {
            image.at<uchar>(row_pixel, col_pixel) = noise_color;    // 임의 픽셀의 픽셀값을 검정색(0) 혹은 흰색(255)으로 교체
        }
        else if(ch_num == 3)    // 컬러 영상
        {
            image.at<Vec3b>(row_pixel, col_pixel)[0] = noise_color;  // B 채널
            image.at<Vec3b>(row_pixel, col_pixel)[1] = noise_color;  // G 채널
            image.at<Vec3b>(row_pixel, col_pixel)[2] = noise_color;  // R 채널
        }
    }
}
