#include "GuiSettings.hpp"
#include <gtkmm/application.h>
#include <gtkmm.h>
#include <opencv2/opencv.hpp>
#include <thread>
cv::Mat frame, outImage;

volatile bool threadRun;
std::mutex myMutex;
std::thread myThread;

Glib::Dispatcher dispatcher;

void run(GuiSettings *h) {

    threadRun = true;
    cv::VideoCapture capture(0);
    int a=0;
    while (a==0) {
        // a++;
        // myMutex.lock();
        capture >> frame;
        cv::cvtColor(frame, outImage, cv::COLOR_BGR2RGB);
        // h->img.set(Gdk::Pixbuf::create_from_data(outImage.data, Gdk::COLORSPACE_RGB, false, 8, outImage.cols, outImage.rows, outImage.step));
        // h->img.queue_draw();
        // std::cout<<"Hello";
        // myMutex.unlock();
        // cv::waitKey(0);
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
        hello.img.set(Gdk::Pixbuf::create_from_data(outImage.data, Gdk::COLORSPACE_RGB, false, 8, outImage.cols, outImage.rows, outImage.step));
        hello.img.queue_draw();
        myMutex.unlock();
    });
    // gdk_threads_leave();

    //Shows the window and returns when it is closed.
    return app->run(hello);
}