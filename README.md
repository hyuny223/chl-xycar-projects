# 1. Overview
These projects are to precess images and control RC-Car.  
These is divided into three parts.  

## 1st - RC-Car perception and control
First project is for real perception and control of RC-Car.  
For perception, I use two ways. One is "Brightness Difference Lane Detection". The other is "Huff Transform Lane Detection"
For control, I use "PID control and Weighted Moving Average Filter" for stable driving.  

Brightness Difference Lane Detection
![image](https://user-images.githubusercontent.com/58837749/184784217-519109ea-05ce-4f5d-8e8d-1afc005f9bb9.png)  
<br>
Huff Transform Lane Detection
![image](https://user-images.githubusercontent.com/58837749/184784554-b62e7348-7a87-44a4-a792-76ec81773da5.png)



## 2nd - More sophisticated image processing
Second project is for mor sophisticated image processing. It is based "Brightness Difference Lane Detection".  
For Robustness against change of Brightness of driving environment, I use maintaining average brightness. And to prepare for lane loss, I use very basic Kalman filter, not WMVF above. However the performance is not good. It is necessary to adjust the value according to the environment.  

![image](https://user-images.githubusercontent.com/58837749/184783881-2eccbf1a-9a7f-4f65-a53e-1baa1ad652b8.png)  
(Green Box - original lane, Red Box - predicted lane using kalman filter)


## 3rd - Sliding Window Lane Detection with C++
Third project is using Sliding Window algorithm in order to detect lane.  
In terms of using mathematical formulas, it is a little bit systematic, but it is also weak to lane loss.  
Unlike before, I used c++.  
![Screenshot from 2022-07-22 09-39-36](https://user-images.githubusercontent.com/58837749/184784002-c265faf8-65fb-4031-bd6d-c6c8bf869239.png)

# 2. How to Use
## 1. 1st project
In order to run first project, you need to change topic of my RC-Car motor message type to yours. And this project needs ROS, you should use roslaunch.  

## 2. 2nd project
In order to run second project, please follow below command.
```bash
cd 2nd
python3 subProject_chl.py --video_path ${your video path}
```
Also, you can adjust value of binary image for better process using trackbar.  
![image](https://user-images.githubusercontent.com/58837749/184784310-a1e984c5-eed8-444f-bedc-5980a192a209.png)

## 3. 3rd project
In order to run third project, please follow below command.  

```bash
cd 3rd
mkdir build && cd build
cmake -G Ninja ..
cmake ninja
./sliding window ${your video path}
```

