[toc]

# 实例一、配置一个DTS节点 + 字符设备驱动

### `1、配置DTS`
```bash
文件: ROO_mt6765.dts
位置: android/kernel-4.9/arch/arm64/boot/dts/mediatek/ROO_mt6765.dts
内容: 
    fih_fp: fingerprint {
        compatible = "mediatek, fih-fp";
    };

    mydemo: demo_test {
        compatible = "ts,hello";
        status = "okay";
        value = /bits/ 8 <88>;
        value16 = /bits/ 16 <166>;
        value32 = <251>;
        value32array = <1 2 3 4 5>;
    }

备注: 如何判断项目用的是哪个 dts 文件呢? 从项目内核 defconfig 文件可以查到:
文件: ROO_debug_defconfig
位置: android/kernel-4.9/arch/arm64/configs/ROO_debug_defconfig
内容: 
    CONFIG_BUILD_ARM64_APPENDED_DTB_IMAGE=y
    CONFIG_BUILD_RAM64_APPENDED_DTB_IMAGE_NAME="mediatek/ROO_mt6765"
    COMFIG_BUILD_ARM64_DTB_OVERLAY_IMAGE_NAME="mediatek/ROO_k62v1_64_bsp"
```

### `2、配置驱动代码`

```bash
说明: 基于一个字符设备驱动, 添加设备树节点和对应的驱动, 编写源代码hello.c
文件: hello.c
操作: 编写完驱动代码后, 将 hello.c 复制到 LINUX/android/kernel-4.9/drivers/char/ 目录下
内容: 
```

### `3、添加编译`

```bash
1.添加源码编译
文件: Makefile
位置: LINUX/android/kernel-4.9/drivers/char/Makefile
内容: 
    obj-$(CONFIG_DEMO_TEST) += hello.o
说明: 把上面编写的 hello.c 复制到 LINUX/android/kernel-4.9/drivers/char/ 目录下


2.添加 Kconfig
文件: Kconfig
位置: LINUX/android/kernel-4.9/drivers/char/Kconfig
内容: 添加
    config DEMO_TEST
        bool "DEMO test"
        default y
        help
            Say Y here if you want to support the /dev/demo_test device.


3.开启 Kconfig 配置
文件: ROO_debug_defconfig
位置: kernel-4.9/arch/arm64/configs/ROO_debug_defconfig
内容:
    CONFIG_DEMO_TEST = y
    
说明: 可以从项目的 .mk 配置文件, 找到当前项目对应的是哪个defconfig
文件: LINUX/android/device/xxx/ROO/ROO.mk
内容: 
    ifeq($TARGET_BUILD_VARIANT), eng)
    KERNEL_DEFCONFIG ?= ROO_debug_defconfig     # 指明 debug 版本用的配置文件
    KERNEL_DEFCONFIG ?= ROO_defconfig           # 指明了 user 版本用的配置文件
```

### `4、编译验证`


```bash
1.编译 boot image

$ rm -rf LINUX/android/out/target/product/ROO/obj/KERNEL_OBJ
$ cd LINUX/scripts
$ ./bld -p ROO --model-code=0UWW --major-version=1 --minor-version=000 boot
或者
$ cd LINUX/scripts
$ ./bld -p ROO --model-code=0UWW --major-version=1 --minor-version=000 and
source build/env_setup.sh
lunch 96
export FIH_PROJ_ROO=1
cd LINUX/android/kernel-4.9
make mrproper
cd LINUX/android
make bootimage


2. 刷机
$ adb reboot bootloader                 # 进入fastboot模式 
$ sudo fastboot flash boot boot.img     # 将boot.img文件刷入(push)手机
$ sudo fastboot reboot


3.验证
$ adb devices
$ adb shell

# 确认设备树是否正常
android:/ $ su
android:/ # ls /proc/device-tree/demo_test/ -lrt
android:/ # cat /proc/device-tree/demo_test1/name
android:/ # cat /proc/device-tree/deom_test1/compatible ts,hello

# 确认字符设备驱动是否正常
$ adb shell 
android:/ $ su
android:/ # mknod /dev/demo_test c 236 0
android:/ # cat /dev/demo_test
android:/ # dmesg | grep DEMO

```

# 实例二、配置两个DTS节点 + 字符设备驱动

```bash
验证

验证字符设备驱动是否正常
$ adb shell
android:/ $ su
android:/ # mknod /dev/demo_test c 236 0
android:/ # cat /dev/demo_test
android:/ # dmesg | grep DEMO
```