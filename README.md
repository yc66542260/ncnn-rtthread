# ncnn-rtthread

NCNN针对RT-Thread OS移植版本，当前仅支持arm-v8板卡，如树莓派4B。

ncnn is a high-performance neural network inference computing framework optimized for mobile platforms. ncnn is deeply considerate about deployment and uses on mobile phones from the beginning of design. ncnn does not have third party dependencies. It is cross-platform, and runs faster than all known open source frameworks on mobile phone cpu. Developers can easily deploy deep learning algorithm models to the mobile platform by using efficient ncnn implementation, create intelligent APPs, and bring the artificial intelligence to your fingertips. ncnn is currently being used in many Tencent applications, such as QQ, Qzone, WeChat, Pitu and so on.

## 树莓派4B测试例程

### 1.树莓派4B设置

因为AI需要大内存，因此按照树莓派4B编译系统时，需要修改设计内存相关的参数。请参考以下参数修改。

1.修改文件：`./drivers/board.h`

```c
#define HEAP_END        (KERNEL_VADDR_START + 500 * 1024 * 1024)
#define PAGE_START      HEAP_END
#define PAGE_END        ((size_t)PAGE_START + 500 * 1024 * 1024)
```

2.修改文件：`./driver/board.c`

```c
struct mem_desc platform_mem_desc[] = {
    {0x00200000, (512ul << 21) - 1, 0x00200000, NORMAL_MEM},
    {0xFC000000, 0x000100000000 - 1, 0xFC000000, DEVICE_MEM},
};
```

3.修改文件：`./rtconfig.h`

```c
#define RT_MAIN_THREAD_STACK_SIZE 245760

#define FINSH_THREAD_STACK_SIZE 245760
```

### 2.树莓派4B例程

1.将main.c文件删除，将examples文件夹中代码拷贝出来，修改文件名为main.cpp；

2.修改menuconfig，设置支持C++特性；

3.编译源码。

### 3.运行效果

以mobilenet-yolov3为例，测试结果如下所示。

```bash
heap: 0x0056b530 - 0x1f400000

 \ | /
- RT -     Thread Operating System
 / | \     5.0.0 build Apr 18 2023 22:12:57
 2006 - 2022 Copyright by RT-Thread team
lwIP-2.1.2 initialized!
EMMC: assuming clock rate to be 100MHz
[I/sal.skt] Socket Abstraction Layer initialize success.
[I/utest] utest is initialize success.
[I/utest] total utest testcase num: (0)
[I/DBG] version is B1

[I/SDIO] SD card capacity 31178752 KB.
found part[0], begin: 4194304, size: 256.0MB
found part[1], begin: 272629760, size: 29.492GB
file system initialization done!
Helcpu 1 boot success
cpu 2 boot success
cpu 3 boot success
1sh /> success
link disconnected
Support link mode Speed 1000M

msh />
msh />
msh />mn
mnet_yolov3_test
msh />mnet_yolov3_test
Hello RT-Thread NCNN
15 = 0.99720 at 48.90 389.97 159.19 x 525.88
6 = 0.96088 at 18.52 260.82 783.92 x 512.58
15 = 0.95704 at 207.39 407.80 150.12 x 444.35
```

查看sd卡中的测试图片（该图片可以任意，注意图片命名和代码中一致即可）

![](./examples/test.png)

### 注意事项

当前`concat`算子有问题，待修正。

##  联系方式

- 维护：YE Chang
- 邮箱：yechang163@163.com