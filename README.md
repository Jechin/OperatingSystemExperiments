# OperatingSystemExperiments
Some operating system experiments

## content

1. [进程调度与资源管理](#进程调度与资源管理)
2. [存储器管理](#存储器管理)
3. [处理机管理与死锁](#处理机管理与死锁)
4. [磁盘调度算法与优化](#磁盘调度算法与优化)

## [进程调度与资源管理](https://github.com/Jechin/OperatingSystemExperiments/blob/main/ProcessScheduling/ProcessScheduling.cpp)

### 算法

* 进程调度算法
  * 采用非抢断的算法
  * 使用优先级策略
  * 增了等待时间，在优先级相同时，进程调度有限选择等待时间长的
* 资源管理
  * 对于资源未设定数量
  * 创建进程时也会规定所需资源的数量
  * 在此实验中每个进程规定仅需要一种资源

### 初始化

初始化流程图

![image](https://github.com/Jechin/OperatingSystemExperiments/blob/main/src/ProcessInitial.png)

### 进程调度

进程调度流程图

![image](https://github.com/Jechin/OperatingSystemExperiments/blob/main/src/ProcessScheduling.png)



## [存储器管理](https://github.com/Jechin/OperatingSystemExperiments/blob/main/MemoryAllocation/MemoryAllocation.cpp)

#### 算法

- 最先适应算法FF（First-Fit）
  - 按空闲块的起始地址升序排序
- 最佳适应算法BF（Best-Fit）
  - 按空闲块的大小升序排序
- 最坏适应算法WF（Worst_Fit）
  - 按空闲块的大小降序排序

#### 代码实现

* 对空闲块链表进行排序
  * 不采用指针操作
  * 只交换节点中的值（大小、起始地址）
* 合并空闲块
  * 首先将空闲块链表按起始地址排序
  * 合并连续的空闲块
  * 再将空闲块按分配算法所需进行排序

## 处理机管理与死锁

#### 算法

* 银行家算法的代码实现
* 银行家算法流程图
![image](https://github.com/Jechin/OperatingSystemExperiments/blob/main/src/Banker’sAlgorithm.png)

## 磁盘调度算法与优化

#### 算法

* 先来先服务FCFS（First Come First Serve）
* 最短寻道时间优先SSTF（Shortest Seek Time First）
* 电梯算法SCAN

#### 流程图

![image](https://github.com/Jechin/OperatingSystemExperiments/blob/main/src/DiskScheduling.png)

