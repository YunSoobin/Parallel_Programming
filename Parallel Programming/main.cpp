#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

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
        
        
        if(waitKey(1000 / fps) == 'q')
        {
            break;
        }
    }
    
    open_origin_video.release();
    save_origin_video.release();
    
    
    destroyAllWindows();
    
    return 0;
}
