# Crowd Security Metrics

## Overview
Most large corporations and businesses already have some sort of security system implemented, typically a setup of security cameras at the very least.
Boid Brother (BB) aims to provide metrics and security with the least impact, cost-wise and time-wise, to the existing system. BB is a system composed of various tracking, 
classification, mapping, detection, alerting, and other related systems to analyze and process security feed data.

## Environment Setup
This project was built with Visual Studio 2022, it is heavily suggested that it be built using this to guarantee a successful build.

**Dependencies**
- [OpenCV](https://opencv.org/releases/)
	- Install to `C:\opencv`
- [LibTorch](https://pytorch.org/)
	- Use the onsite generator with the following settings
		- _Stable (X.X.X)_
		- _Windows_
		- _LibTorch_
		- _C++/Java_
		- _CPU_
	- Install to `C:\libtorch`
	- Once a build folder is generated, copy all `.dll` files from `C:\libtorch\lib` to `PROJECT_DIRECTORY/bin/Debug`
- [Boost](https://www.boost.org/users/download/)
	- Install to `C:\boost`

**Model Location**  
- Create  a new folder `CrowdSecurityMetrics\model`
- Download the model `mask_rcnn.pt` file and place it in the model folder

## File Structure

- CrowdSecurityMetrics
  - Assets
  - fltk
  - include
    - Views
    - .h files
  - model
    - mask\_rcnn.pt 
  - src
    - Views
    - .cpp files
  - CMakeLists.txt
  - run.bat
