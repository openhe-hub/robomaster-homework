# Homework1
## Ex1-shell
### Description
1. 使用find和grep查找含有hello字段的.c/.h文件
2. 使用参数接受目标路径
3. 使用for循环遍历并计数
4. 使用date作差计算运行时间
### How to Use
```shell
chmod +x ex1.sh
./ex1.sh path
```
---
## Ex2-识别苹果
### Description
1. rgb => hsv
2. 使用hsv提取颜色空间，三部分：苹果主体（红，亮），叶子阴影（红，暗），
   苹果下半部分（黄，暗），并整合
3. 使用开运算进一步优化
4. 寻找轮廓，通过面积筛选苹果轮廓，并用外接矩形标出
### Output Link
[ex2-output](https://github.com/openhe-hub/robomaster-homework/tree/master/homework1/src/ex2/output)

## Ex3-识别小弹丸
### Description
1. rgb=>grey
2. 二值化
3. 查找轮廓，通过面积先排除静止的小弹丸干扰，再通过轮廓外接矩形的长宽比筛选出
   运动中的小弹丸
4. 用旋转外接矩形标识小弹丸
5. 用旋转外接矩形的中心标记曝光时间中点时的坐标位置
6. 记录上一次的中心，通过两次中心的距离判断是否是同一颗弹丸，并计数
7. 注意到在多弹丸的情况下，上述跟踪方式可能失效，或许可以通过运动角度和旋转矩阵的旋转角之差来进一步筛选？
### Output Link
[ex3-output](https://github.com/openhe-hub/robomaster-homework/tree/master/homework1/src/ex3/output)
* 注：output-slow是0.25倍速
