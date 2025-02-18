#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <torch/torch.h>
#include <opencv2/opencv.hpp>

torch::Tensor preprocessImage(cv::Mat& frame, int targetSize);

#endif //PREPROCESS_H
