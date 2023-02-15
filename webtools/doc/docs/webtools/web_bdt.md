---
icon: :octicons-heart-fill-24: 
---

# Web BDT


## 环境准备

!!! note "Web BDT"
    Telink原来的下载工具为<a href="http://wiki.telink-semi.cn/wiki/IDE-and-Tools/Burning-and-Debugging-Tools-for-all-Series/"  target="_blank"> BDT </a>，可在Windows平台下使用，如果用原来的BDT工具下载bin，可以跳过本文，但还是建议大家能了解一下新的下载工具。

    为了工具能跨平台使用，同时用户可即开即用，而不必去下载和安装工具，Telink新推出了基于Web的下载调试工具<a href="https://debug.telink-semi.cn/web_bdt/" target="_blank"> Web BDT </a>(开源，可二次开发)。用户可以通过网页来进行程序的下载和调试，网页支持Windows，Linux，macOS，Android等平台，只需要一个兼容的网页浏览器即可(:octicons-heart-fill-24:{ .heart }推荐使用较新版本的Chrome或者Edge)。
    
!!! info "Burning EVK libusb驱动安装"
    Web BDT通过Web USB控制Telink burning evk(一款下载调试器)，为了能使用Web BDT，需要给电脑安装libusb驱动。而传统BDT工具通过Windows自带的usb printer驱动去操控Telink burning evk。为了避免Windows平台同时使用Web BDT和传统的BDT时需要频繁的切换驱动，需要更新Telink burning evk本身的固件程序。更新之后，通过Telink burning evk上的按键SW2来切换USB PID。

    1. 如果上电后Telink Burning EVK单个LED常亮，则不需要更新固件，否则就需要下载对应的 <a href="../../Resource/bin/8266_tlink_20211118.bin" target="_blank">固件</a>  到模块板中，通过旧的BDT工具更新固件后重新上电。
    <div class="div_center" style="text-align:center"> 
     <img src="../../Resource/images/update.gif" class="image" style="width:85%">
     </div>

    2. BDT和Web BDT对应的LED状态如下图，传统BDT用绿灯指示，Web BDT用蓝灯指示，通过按键SW2可以切换USB PID并改变LED状态( :octicons-heart-fill-24:{ .heart } 切换状态后，需要重新上电)。
     <div class="div_center" style="text-align:center">  
     <img src="../../Resource/images/state_change.png" class="image" style="width:85%">
     </div>

    3. 使用Web BDT前需要先安装一次libusb驱动，方法如下，两步即可：
         + 下载<a href="https://zadig.akeo.ie/" target="_blank"> Zadig </a>
         + Telink burning evk设置为蓝灯状态（若上电后改变了LED状态，需要重新上电后生效设置），通过USB连接PC，安装libusb驱动，过程见下图，驱动安装需要几十秒甚至更长时间，需要耐心等待。
    <div class="div_center" style="text-align:center">
    <img src="../../Resource/images/zadig.gif" class="image" style="width:75%">
    </div>

## 常规操作说明

!!! info "硬件连接"
    + 通过USB线缆连接Burning EVK和电脑，以mboard为例，把Burning EVK的SWM，3V3，GND 分别和 mboard 底板的 SWS，3V3，GND连接
    + 通过Burning EVK上的SW2切换为LED蓝灯状态 (如果上一次连接的时候已经切换为该状态则可跳过这一步和下一步)。
    + :octicons-heart-fill-24:{ .heart } 重新连接USB
<div class="div_center" style="text-align:center">
    <img src="../../Resource/images/burning_evk_connect.jpg" class="image" style="width:75%">
    </div>

!!! info ":octicons-heart-fill-24:{ .heart } 打开<a href="https://debug.telink-semi.cn/web_bdt/" target="_blank"> Web BDT </a>"
    点击"install"可将网页以原生应用的方式安装在电脑/手机上，并自动添加桌面快捷方式，用户也可将应用固定任务栏处。
    同时采用离线存储和后台同步技术，网页可在没有网络的条件下使用(从服务器上下载程序功能除外)。
    <div class="div_center" style="text-align:center">
    <img src="../../Resource/images/webbdt1.png" class="image" style="width:75%">
    </div>

!!! info "下载本地bin文件"
    1. 选择主芯片型号或开源板子型号
    2. 点击connect，在弹出的对话框中选择"Telink Web Debugger"， 点击"连接"
    3. 点击File Import，选择本地bin文件（在此之前可以点击"SWS"测试调试接口连接是否正常)
    4. 点击Download（之后可以重新上电复位，也可以点击"Reset")
   
    <div class="div_center" style="text-align:center">
    <img src="../../Resource/images/webbdt2.jpg" class="image" style="width:75%">
    </div>  

    <div class="div_center" style="text-align:center">
    <img src="../../Resource/images/webbdt3.jpg" class="image" style="width:75%">
    </div>

!!! info "下载网络bin文件"
    以 mboard模块板对应的ZigBee coordinator固件为例子

       1. 选择板子型号为mboard，点击connect
       2. 选择Multi Addr Download标签
       3. 选择右边一列的服务器上的bin文件，这里选择 zigbee_coordinator
       4. 点击"MultiDownload" (之后重新上电，或者点击"Rest")

    <div class="div_center" style="text-align:center">
    <img src="../../Resource/images/webbdt4.jpg" class="image" style="width:75%">
    </div>


!!! warning "异常处理"
    如果下载程序出现异常，可尝试刷新页面并重新连接设备后点击 "SWS"，若返回OK则接着点击 "Activate"等待返回OK后可进行正常下载，若返回fail则检查硬件连接。

## 高级操作说明

!!! note "下载详细pdf使用手册"
    在手册里有详细的看变量，调试程序，以及有代码休眠情况下难以连接SWS调试接口的解决办法。
    <a href="../../Resource/doc/web_bdt/README.html" target="_blank"> Web BDT详细文档 </a>

