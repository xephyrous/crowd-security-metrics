#include "preprocess.h"

torch::Tensor preprocessImage(cv::Mat& frame, int targetSize) { // set to 0 for no resize
    // Convert BGR to RGB (since OpenCV loads images in BGR format)
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

    // Convert to float and normalize to [0,1]
    frame.convertTo(frame, CV_32F, 1.0f / 255.0f);

    // Resize image to expected input size if needed?
    if (targetSize > 0) {
     cv::resize(frame, frame, cv::Size(targetSize, targetSize));
    }

    // Convert OpenCV Mat to Torch Tensor (H, W, C) → (C, H, W)
    torch::Tensor imgTensor = torch::from_blob(frame.data, {frame.rows, frame.cols, 3}, torch::kFloat);
    imgTensor = imgTensor.permute({2, 0, 1}); // Change to (C, H, W)

    // Add batch dimension (1, C, H, W) before passing to the model
    imgTensor = imgTensor.unsqueeze(0);

    return imgTensor.clone(); // Clone ensures the tensor owns its memory
}