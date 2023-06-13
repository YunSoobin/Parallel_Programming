#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void addNoise(Mat &image, int noise_num);   // 노이즈 추가
void applyFilter(Mat &image, int kernal_size);  // 필터 적용
void Bubble_Sort(vector<int> &pixels);   // 버블 정렬

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
    VideoWriter save_filter_video("save_filter_video.mov", fourcc, fps, Size(width, height));
    
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
        
        
        Mat filter_frame;
        filter_frame = noise_frame.clone();
        
        applyFilter(filter_frame, 3);  // 필터 적용
        
        // 필터가 적용된 영상 출력, 저장
        imshow("Filter Video", filter_frame);
        save_filter_video << filter_frame;
        

        if(waitKey(1000) == 'q')
        {
            break;
        }
    }
    
    open_origin_video.release();
    save_origin_video.release();
    save_noise_video.release();
    save_filter_video.release();
    
    
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

void applyFilter(Mat &image, int kernal_size)
{
    int row_size = image.rows;
    int col_size = image.cols;
    int ch_num = image.channels();
    
    int center_pixel = kernal_size / 2;
    
    if(ch_num == 1) // 흑백 영상
    {
        vector<int> pixels(kernal_size * kernal_size);
        for(int row = 0; row < row_size - kernal_size; row++)
        {
            for(int col = 0; col < col_size - kernal_size; col++)
            {
                pixels.clear();
                for(int r = 0; r < kernal_size; r++)
                {
                    for(int c = 0; c < kernal_size; c++)
                    {
                        pixels.push_back(image.at<uchar>(row + r, col + c));
                    }
                }
                
                Bubble_Sort(pixels);
                
                image.at<uchar>(row + center_pixel, col + center_pixel) = pixels.at(kernal_size + center_pixel);
            }
        }
    }
    else if(ch_num == 3)   // 컬러 영상
    {
        vector<int> pixels_B(kernal_size * kernal_size);
        vector<int> pixels_G(kernal_size * kernal_size);
        vector<int> pixels_R(kernal_size * kernal_size);
        for(int row = 0; row < row_size - kernal_size; row++)
        {
            for(int col = 0; col < col_size - kernal_size; col++)
            {
                pixels_B.clear();
                pixels_R.clear();
                pixels_G.clear();
                for(int r = 0; r < kernal_size; r++)
                {
                    for(int c = 0; c < kernal_size; c++)
                    {
                        pixels_B.push_back(image.at<Vec3b>(row + r, col + c)[0]);
                        pixels_G.push_back(image.at<Vec3b>(row + r, col + c)[1]);
                        pixels_R.push_back(image.at<Vec3b>(row + r, col + c)[2]);
                    }
                }
                
                Bubble_Sort(pixels_B);
                Bubble_Sort(pixels_G);
                Bubble_Sort(pixels_R);
                
                image.at<Vec3b>(row + center_pixel, col + center_pixel)[0] = pixels_B.at(kernal_size + center_pixel);
                image.at<Vec3b>(row + center_pixel, col + center_pixel)[1] = pixels_G.at(kernal_size + center_pixel);
                image.at<Vec3b>(row + center_pixel, col + center_pixel)[2] = pixels_R.at(kernal_size + center_pixel);
            }
        }
    }
}

void Bubble_Sort(vector<int> &pixels)
{
    for(int i = 0; i < pixels.size() - 1; i++)
    {
        for(int j = 0; j < pixels.size() - 1; j++)
        {
            if(pixels[j] > pixels[j + 1])
            {
                swap(pixels[j], pixels[j + 1]);
            }
        }
    }
}
