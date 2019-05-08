#include "GuiSettings.hpp"
#include <gtkmm/application.h>
#include <gtkmm.h>
#include <opencv2/opencv.hpp>
#include <thread>
cv::Mat frame, outImage;

#define H_RANGE 30
#define S_RANGE 70
#define V_RANGE 30

volatile bool threadRun;
std::mutex myMutex;
std::thread myThread;

Glib::Dispatcher dispatcher;

void run(GuiSettings *h) {

    threadRun = true;
    cv::VideoCapture capture(0);
    int a=0;
    while (a==0) {
        capture >> frame;
        if(h->set_frame){
                int iLowH, iHighH, iLowS, iHighS, iLowV, iHighV;
                int rectsize=30;
                cv::Rect Rec(frame.rows/2 - rectsize, frame.cols/2 - rectsize, rectsize *2, rectsize*2);
                cv::rectangle(frame, Rec, cv::Scalar(255), 1, 8, 0);
                if(h->get_color){
                    int rectsize=30;
                    // cv::Rect Rec(image.rows/2 - rectsize, image.cols/2 - rectsize, rectsize *2, rectsize*2);
                    cv::Mat frame1 = frame;
                    cv::Mat roi=frame1(Rec);
                    cv::cvtColor(roi, frame1, cv::COLOR_BGR2HSV);
                    std::vector<cv::Mat> channels;
                    double minval,maxval;
                    cv::Point minidx, maxidx;
                    cv::split(frame1, channels);
                    // minMaxLoc( channels[0], &minval, &maxval, &minidx, &maxidx);
                    
                    cv::Scalar temp= cv::mean(channels[0]);
                    minval=maxval=temp[0];

                    
                    iLowH=std::max((int)minval-H_RANGE,0);
                    std::cout<<minval-H_RANGE<<" ";
                    iHighH=std::min((int)maxval+H_RANGE,179);
                    std::cout<<iHighH<<std::endl;

                    h->hsv_values['h']=std::pair<int,int>(iLowH, iHighH);

                    temp= cv::mean(channels[1]);
                    minval=maxval=temp[0];
                    iLowS=std::max((int)minval-S_RANGE,0);
                    iHighS=std::min((int)maxval+S_RANGE,255);

                    h->hsv_values['s']=std::pair<int,int>(iLowS, iHighS);
                    
                    temp= cv::mean(channels[2]);
                    minval=maxval=temp[0];
                    iLowV=std::max(0,(int)minval-V_RANGE);
                    iHighV=std::min(255,(int)maxval+V_RANGE);

                    h->hsv_values['v']=std::pair<int,int>(iLowV, iHighV);

                    h->set_frame = FALSE;
                    h->get_color = FALSE;
                    h->call.set_label("CALLIBRATE");
                    h->set.hide();
                    h->callibration->draw();
                }
        }
        cv::cvtColor(frame, outImage, cv::COLOR_BGR2RGB);
        dispatcher.emit();
    }
}

int main (int argc, char *argv[])
{
//   gdk_threads_init();
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    GuiSettings hello;

    myThread = std::thread(&run, &hello);
    dispatcher.connect([&](){
        myMutex.lock();
        // cv::cvtColor(frame, outImage, cv::COLOR_BGR2RGB);
        hello.img.img.set(Gdk::Pixbuf::create_from_data(outImage.data, Gdk::COLORSPACE_RGB, false, 8, outImage.cols, outImage.rows, outImage.step));
        hello.img.img.queue_draw();
        myMutex.unlock();
    });
    // gdk_threads_leave();

    //Shows the window and returns when it is closed.
    return app->run(hello);
}