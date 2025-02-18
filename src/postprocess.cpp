#include "postprocess.h"

void drawDetections(cv::Mat& frame, const torch::Dict<std::string, torch::Tensor>& result) {
    // Extract bounding boxes, labels, and scores from model output
    torch::Tensor boxes = result.at("boxes");
    torch::Tensor labels = result.at("labels");
    torch::Tensor scores = result.at("scores");

    for (int i = 0; i < boxes.size(0); ++i) {
        float score = scores[i].item<float>();

        // Only draw detections with confidence above a threshold
        if (score > 0.5) {
            float x1 = boxes[i][0].item<float>();
            float y1 = boxes[i][1].item<float>();
            float x2 = boxes[i][2].item<float>();
            float y2 = boxes[i][3].item<float>();

            // Draw bounding box
            cv::rectangle(frame, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 2);

            // Put label and confidence score
            std::string labelText = "Label " + std::to_string(labels[i].item<int>()) + " (" + std::to_string(score * 100).substr(0, 4) + "%)";
            cv::putText(frame, labelText, cv::Point(x1, y1 - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
        }
    }
}
