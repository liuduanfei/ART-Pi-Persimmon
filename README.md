## 柿饼UI在ART-Pi上的移植

说是移植，实际上是添加柿饼库到工程，并完成**LCD驱动**和**触摸驱动**的对接。

### 说明

**1 柿饼UI:**

Persimmon 是一套运行在RT-Thread嵌入式实时操作系统上的图形用户组件界面，用于提供图形界面的用户交互。
它采用C++语言编写，基于C语言实现的底层图形像素引擎，为上层应用提供了控件、窗口、signal/slot，手势动画等机制。 为了提高UI的开发灵活性、降低用户开发时间和成本，Persimmon 提供了所见即所得的UI设计器。用户可通过设计器轻松实现UI界面设计，并利用**JavaScript**脚本实现对界面逻辑的控制。

**2 ART-Pi**

RT-Thread 团队精心准备推出的极具扩展性的 DIY 开源硬件。

### 准备工作

**工具**：

- RT-Studio           RT-Thread推出的一站式IDE。

- UI设计器             柿饼配套的设计器。

- ART-Pi                 本次移植需要的硬件平台。

- RGB屏幕             ART-Pi的lcd接口跟正点原子一样，其他屏幕请看原理图做专门的转接板。

- 多媒体扩展板     ART-Pi配套的多媒体扩展板。



本次移植的RGB屏幕为**正点原子 800x480**的七寸屏，触摸芯片为**FT5426**。

整个工程在`art_pi_blink_led` 基础上移植，SDK版本为 **v1.1.0**。

下面对整个过程进行说明。

**1.新建工程**

RT-Studio中安装ART-Pi的sdk。新建一个`art_pi_blink_led` 工程。

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\add_new_project.png)

**2.添加柿饼库**

在文件`art_pi_blink_led\libraries\Kconfig`中添加下面的代码。为方便复制，贴在图下面。

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\add_per_kconfig.png)



    config PKG_USING_PERSIMMON_SRC
        bool "Using Persimmon Library"
        select RT_USING_CPLUSPLUS
        select PKG_USING_FASTLZ
        select PKG_USING_EZXML
        select RT_USING_I2C
        default n
上面的配置时柿饼库所依赖的软件包和RTT的配置选型。

点击`RT-Thread Setting`

分别使能 **柿饼库**，**LCD驱动**，**文件系统（flash）**，**wifi**，**ftp库**。

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\enable_lcd.png)

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\enable_filesystem.png)

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\enable_p_ftp.png)

设置**lwip**，ftp库需要更多的资源配置，按照下图配置lwip。

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\config_lwip.png)

**添加文件**

在工程`applications`文件夹下添加启动柿饼UI所需要的文件。

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\add_p_file.png)

在`main.c`中添加使能`ftp`的相关代码。

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\add_ftp_code.png)

保存配置并编译

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\build.png)

可以看到，柿饼库成功链接，编译尺寸还是比较大的。

**下载**

此时下载之后屏幕点亮，还**不能显示界面**，这是因为柿饼的UI文件还没有加入到flash文件系统中，下面加入柿饼UI文件，采用ftp协议传输柿饼UI文件。

连接网络：

使用命令 `wifi join <ssid> <password>`连接网络。

待获取`IP`后连接`ftp`服务器，如下图所示：

其中IP为实际的IP地址，用户名和密码为 loogg，端口号为21。

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\add_UI.png)

将`test`文件夹上传到flash文件系统中。

传输完成后，执行命令`reboot`重启系统

执行命令`persim_start`启动柿饼UI，可以看到屏幕上有图像产生，但是显示不对，这是由于参数配置的问题。

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\para_err.jpg)

按照下图文件修改工程中的**屏幕参数配置**，本人用的正点原子屏幕的配置参数如下：

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\config_lcd.png)

重新编译下载：

执行命令 `persim_start`，可以看到界面已经正常显示了。

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\para_ok.jpg)

**添加触摸**

下载软件包，正点原子这块屏所实用的触摸IC为`ft5426`，该触摸芯片本人已制作成软件包。

添加添加**软件包**和`touch.c`文件，如下图：

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\add_touch.png)

使能触摸的初始化代码：

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\enable_touch_init.png)

该触摸芯片采用i2c控制，根据引脚设置配置使能i2c1，如下图：

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\enable_i2c1.png)

保存，编译下载：

启动柿饼UI，点击按钮，触摸OK

![](C:\Users\LIU\Desktop\ART-Pi_per\pic\touch_ok.jpg)