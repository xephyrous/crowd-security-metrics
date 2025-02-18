#include "MaskRCNN.h"

#include "preprocess.h"
#include "postprocess.h"

#include <iostream>

MaskRCNN::MaskRCNN(const std::string &model_path) {
    try {
        model = torch::jit::load(model_path);
    } catch (const c10::Error& e) {
        std::cerr << "TorchScript error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }
}

void MaskRCNN::analyzeVideo(const std::string &video_path, int targetSize, bool printFlag) {
     cv::VideoCapture cap(video_path);
     if (!cap.isOpened()) {
         std::cerr << "Error: Could not open video file: " << video_path << std::endl;
         return;
     }

     cv::Mat frame;
     while (cap.read(frame)) {
         // Preprocess frame
         torch::Tensor inputTensor = preprocessImage(frame, targetSize);

         if (printFlag) {
        	// Get C, H, W
         	int C = inputTensor.size(1);
         	int H = inputTensor.size(2);
      	    int W = inputTensor.size(3);

        // Loop over the tensor's elements and print
    	     for (int c = 0; c < C; ++c) {
    	        std::cout << "Channel " << c << ":\n";
     		    for (int h = 0; h < H; ++h) {
                	for (int w = 0; w < W; ++w) {
                    	// Print the value at position (c, h, w)
                    	std::cout << inputTensor[0][c][h][w].item<float>() << " ";
                	}
                std::cout << "\n";
            	}
            std::cout << "\n";
         	}
    	 }

         std::vector<torch::Tensor> inputs = {inputTensor};

         // Run inference
         auto output = model.forward({inputs}).toList();
         c10::Dict<c10::IValue, c10::IValue> raw_dict = output.get(0).toGenericDict();

         // Extract each tensor manually - Results
         torch::Tensor boxes = raw_dict.at("boxes").toTensor();
         torch::Tensor labels = raw_dict.at("labels").toTensor();
         torch::Tensor scores = raw_dict.at("scores").toTensor();
         torch::Tensor masks = raw_dict.at("masks").toTensor();

         // Pass extracted tensors to post-processing
         torch::Dict<std::string, torch::Tensor> result;
         result.insert("boxes", boxes);
         result.insert("labels", labels);
         result.insert("scores", scores);

         // Draw detections on frame
         drawDetections(frame, result);
     }

     cap.release();
     cv::destroyAllWindows();
}
