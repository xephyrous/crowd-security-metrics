#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include <torch/torch.h>
#include <opencv2/opencv.hpp>

void drawDetections(cv::Mat& frame, const torch::Dict<std::string, torch::Tensor>& result);

#endif //POSTPROCESS_H
