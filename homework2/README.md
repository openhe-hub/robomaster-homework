# 视觉部作业2 
## github仓库链接
[homework2](https://github.com/openhe-hub/robomaster-homework/tree/master/homework2)
## Ex1 透视
1. 先用传统视觉识别出车牌的轮廓（hsv提颜色）
2. 获取轮廓的外接矩形
3. 以轮廓外接矩形的四个顶点为映射点，然后直接套透视变换api，微调一下
* 效果  
![ex1-out](https://github.com/openhe-hub/robomaster-homework/blob/master/homework2/src/ex1/output/result.jpg)

## Ex2 拍摄
参考了教程中的程序（用opengl画的图，纯属测试）  
不过还是有一处有些不懂，就是旋转矩阵和平移矩阵这里，关于转置和去负号，不明白
实际的作用
```cpp
converter.block(0, 0, 3, 3) = rot_c_to_w.transpose().cast<double>();
converter.block(0, 3, 3, 1) = -rot_c_to_w.transpose().cast<double>() * cam_w;
```
* 效果  
![ex2-out](https://github.com/openhe-hub/robomaster-homework/blob/master/homework2/src/ex2/output/out.jpg)
## Ex3 推送
1. 主要思路是第二题的数据不动，然后模拟相机运动
2. 变动相机的世界坐标，没有动相机的欧拉角，让相机沿着$\cos(\theta),\sin(\theta),\sin(\theta)$ 
曲线运动至(2,2,2)
3. 关于速度的变化，只做了一个线性的降低
4. 其余点设置随机颜色和点半径，但是光晕没有做
* 效果  
[ex3-out](https://github.com/openhe-hub/robomaster-homework/blob/master/homework2/src/ex3/output/out.mp4)
