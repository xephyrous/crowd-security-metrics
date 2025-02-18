#pragma once

#include <torch/script.h>
#include <torch/torch.h>

class MaskRCNN {
public:
    explicit MaskRCNN(const std::string& model_path);

    void analyzeVideo(const std::string& video_path);

private:
    torch::jit::script::Module model;
};