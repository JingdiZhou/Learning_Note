# Linux 驱动开发

![](pictures/drive_structure.jpg)

应用程序运行在用户空间，而 Linux 驱动属于内核的一部分，因此驱动运行于内核空间

如open函数的流程：
![](pictures/dirve_open.jpg)

*__每一
个系统调用，在驱动中都有与之对应的一个驱动函数，在 Linux 内核文件 include/linux/fs.h 中
有个叫做 file_operations 的结构体，此结构体就是 Linux 内核驱动操作函数集合__*

### 驱动模块的加载和卸载
Linux 驱动有两种运行方式：
1. 第一种就是将驱动编译进 Linux 内核中，这样当 Linux 内核启动的时候就会自动运行驱动程序。
2. 第二种就是将驱动编译成模块(Linux 下模块扩展名为.ko)，在Linux 内核启动以后使用“insmod”命令加载驱动模块

__模块加载和卸载注册函数__
```
module_init(xxx_init); //注册模块加载函数
module_exit(xxx_exit); //注册模块卸载函数
```

`module_init` 函数用来向 `Linux 内核`注册一个模块加载函数，参数 `xxx_init` 就是需要注册的具体函数，当使用“`insmod`”命令加载驱动的时候，`xxx_init` 这个函数就会被调用

字符设备驱动模块加载和卸载模板如下:
```
/* 驱动入口函数 */
static int __init xxx_init(void)  //定义了个名为 xxx_init 的驱动入口函数，并且使用了“__init”来修饰
    {
/* 入口函数具体内容 */
return 0;
    }

/* 驱动出口函数 */
static void __exit xxx_exit(void)  //，定义了个名为 xxx_exit 驱动出口函数，并且使用了“__exit”来修饰
    {
 /* 出口函数具体内容 */
    }

 /* 将上面两个函数指定为驱动的入口和出口函数 */
 module_init(xxx_init);
 module_exit(xxx_exit);
```

- 驱动编译完成以后扩展名为.ko
- 有两种命令可以加载驱动模块：insmod和modprobe
  1. insmod是最简单的模块加载命令，此命令用于加载指定的.ko 模块,如
   ```
   insmod drv.ko
   ```
    2. insmod 命令不能解决模块的依赖关系，比如 drv.ko 依赖 first.ko 这个模块，就必须先使用insmod 命令加载 first.ko 这个模块，然后再加载 drv.ko 这个模块.但是 modprobe 就不会存在这个问题，modprobe 会分析模块的依赖关系，然后会将所有的依赖模块都加载到内核中
    3. modprobe 命令主要智能在提供了模块的依赖性分析、错误检查、错误报告等功能，推荐使用 modprobe 命令来加载驱动
    4. 使用 modprobe 命令可以卸载掉驱动模块所依赖的其他模块，前提是这些依赖模块已经没有被其他模块所使用，否则就不能使用 modprobe 来卸载驱动模块


### 字符设备注册与注销
![](pictures/drive_module_register.jpg)

##### 实现设备的具体操作函数
file_operations 结构体就是设备的具体操作函数，其中对字符设备有如下两个要求：
1. 能够对字符设备进行打开和关闭操作。设备打开和关闭是最基本的要求，几乎所有的设备都得提供打开和关闭的功能。因此我们需要实现 file_operations 中的 open 和 release 这两个函数
2. 对字符设备能实现读写操作

加入设备操作函数模块如下：
```
/* 打开设备 */
 static int chrtest_open(struct inode *inode, struct file *filp)
{
/* 用户实现具体功能 */
return 0;
}
 
 /* 从设备读取 */
static ssize_t chrtest_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
 {
 /* 用户实现具体功能 */
 return 0;
 }

 /* 向设备写数据 */
 static ssize_t chrtest_write(struct file *filp,const char __user *buf,size_t cnt, loff_t *offt)
 {
 /* 用户实现具体功能 */
 return 0;
 }

 /* 关闭/释放设备 */
 static int chrtest_release(struct inode *inode, struct file *filp)
 {
 /* 用户实现具体功能 */
 return 0;
 }

 static struct file_operations test_fops = {
 .owner = THIS_MODULE, 
 .open = chrtest_open,
 .read = chrtest_read,
 .write = chrtest_write,
 .release = chrtest_release,
 };

 /* 驱动入口函数 */
 static int __init xxx_init(void)
 {
 /* 入口函数具体内容 */
 int retvalue = 0;

 /* 注册字符设备驱动 */
 retvalue = register_chrdev(200, "chrtest", &test_fops);
 if(retvalue < 0){
 /* 字符设备注册失败,自行处理 */
 }
 return 0;
 }

 /* 驱动出口函数 */
 static void __exit xxx_exit(void)
 {
 /* 注销字符设备驱动 */
 unregister_chrdev(200, "chrtest");
 }

 /* 将上面两个函数指定为驱动的入口和出口函数 */
 module_init(xxx_init);
 module_exit(xxx_exit);

```

##### 添加LICENSE和作者信息
```
MODULE_LICENSE() //添加模块 LICENSE 信息，必要
MODULE_AUTHOR() //添加模块作者信息 ,非必要
```

### 设备号
__Linux上每个设备都有一个设备号，由`主设备号`和`次设备号`组成。主设备号表示某一个具体的驱动，次设备号表示使用这个驱动的各个设备。Linux 提供了一个名为 `dev_t` 的数据类型表示设备号，dev_t 定义在文件 `include/linux/types.h` 里面(dev_t是_u32类型，即unsigned int,其中32位里面高12位为主设备号，低20位为次设备号，因此 Linux系统中主设备号范围为 0~4095，所以在选择主设备号的时候一定不要超过这个范围)__

##### 设备号分配
1. 静态分配，可以通过`cat /proc/devices`来查看当前系统中所有已经使用了的设备号
2. 可以让系统自动给你分配一个设备号，使用以下函数：

```
int alloc_chrdev_region(dev_t *dev, unsigned baseminor,unsigned count, const char *name)
//dev：保存申请到的设备号
//baseminor：次设备号起始地址，alloc_chrdev_region 可以申请一段连续的多个设备号，这些设备号的主设备号一样，但是次设备号不同，次设备号以 baseminor 为起始地址地址开始递增。一般 baseminor 为 0，也就是说次设备号从 0 开始。
//count：要申请的设备号数量。
//name：设备名字


注销字符设备后要释放设备号，使用以下函数：
void unregister_chrdev_region(dev_t from, unsigned count)
//from：要释放的设备号。
//count：表示从 from 开始，要释放的设备号数量。


```

