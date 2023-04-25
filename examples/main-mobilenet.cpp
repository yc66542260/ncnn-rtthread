#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "layer.h"
#include "net.h"

#include "simpleocv.h"
#include <float.h>
#include <stdio.h>
#include <vector>

#include <stdio.h>
#include <vector>

struct Object
{
    cv::Rect_<float> rect;
    int label;
    float prob;
};

static int detect_mobilenet(const cv::Mat& bgr, std::vector<Object>& objects)
{
    ncnn::Net mobilenet;

    mobilenet.opt.use_vulkan_compute = true;

    // model is converted from https://github.com/chuanqi305/MobileNet-SSD
    // and can be downloaded from https://drive.google.com/open?id=0ByaKLD9QaPtucWk0Y0dha1VVY0U
    // the ncnn model https://github.com/nihui/ncnn-assets/tree/master/models
    if (mobilenet.load_param("mobilenet_ssd_voc_ncnn.param"))
        exit(-1);
    if (mobilenet.load_model("mobilenet_ssd_voc_ncnn.bin"))
        exit(-1);

    const int target_size = 300;

    int img_w = bgr.cols;
    int img_h = bgr.rows;

    ncnn::Mat in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_BGR, bgr.cols, bgr.rows, target_size, target_size);

    const float mean_vals[3] = {127.5f, 127.5f, 127.5f};
    const float norm_vals[3] = {1.0 / 127.5, 1.0 / 127.5, 1.0 / 127.5};
    in.substract_mean_normalize(mean_vals, norm_vals);

    ncnn::Extractor ex = mobilenet.create_extractor();

    ex.input("data", in);

    ncnn::Mat out;
    ex.extract("detection_out", out);

    //     printf("%d %d %d\n", out.w, out.h, out.c);
    objects.clear();
    for (int i = 0; i < out.h; i++)
    {
        const float* values = out.row(i);

        Object object;
        object.label = values[0];
        object.prob = values[1];
        object.rect.x = values[2] * img_w;
        object.rect.y = values[3] * img_h;
        object.rect.width = values[4] * img_w - object.rect.x;
        object.rect.height = values[5] * img_h - object.rect.y;

        objects.push_back(object);
    }

    return 0;
}

static void draw_objects(const cv::Mat& bgr, const std::vector<Object>& objects)
{
    static const char* class_names[] = {"background",
                                        "aeroplane", "bicycle", "bird", "boat",
                                        "bottle", "bus", "car", "cat", "chair",
                                        "cow", "diningtable", "dog", "horse",
                                        "motorbike", "person", "pottedplant",
                                        "sheep", "sofa", "train", "tvmonitor"
                                       };

    cv::Mat image = bgr.clone();

    for (size_t i = 0; i < objects.size(); i++)
    {
        const Object& obj = objects[i];

        fprintf(stderr, "%d = %.5f at %.2f %.2f %.2f x %.2f\n", obj.label, obj.prob,
                obj.rect.x, obj.rect.y, obj.rect.width, obj.rect.height);

        cv::rectangle(image, obj.rect, cv::Scalar(255, 0, 0));

        char text[256];
        sprintf(text, "%s %.1f%%", class_names[obj.label], obj.prob * 100);

        int baseLine = 0;
        cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

        int x = obj.rect.x;
        int y = obj.rect.y - label_size.height - baseLine;
        if (y < 0)
            y = 0;
        if (x + label_size.width > image.cols)
            x = image.cols - label_size.width;

        cv::rectangle(image, cv::Rect(cv::Point(x, y), cv::Size(label_size.width, label_size.height + baseLine)),
                      cv::Scalar(255, 255, 255), -1);

        cv::putText(image, text, cv::Point(x, y + label_size.height),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
    }
    cv::imwrite("image.png", image);
    // cv::imshow("image", image);
    // cv::waitKey(0);
}

void mnet_test()
{
    rt_kprintf("Hello RT-Thread NCNN\n");
    cv::Mat m = cv::imread("bus.jpg", 1);
    if (m.empty())
    {
        rt_kprintf("cv::imread failed\n");
    }

    std::vector<Object> objects;
    detect_mobilenet(m, objects);

    draw_objects(m, objects);
}
MSH_CMD_EXPORT(mnet_test, test yolo);  



int main(int argc, char** argv)
{
    rt_kprintf("Hello RT-Thread NCNN\n");

    return 0;
}