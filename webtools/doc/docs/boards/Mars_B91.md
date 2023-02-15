---
icon: material/emoticon-happy
title: "Mars_B91"

subtitle: ""
author: "telink-semi.com"
author-url: 'xiaodong.zong@telink-semi.com'
date: "2022-08-04"
---

## 模组介绍

!!! note "Mars_B91"
    Mars_B91 是泰凌为推广芯片产品而生的一款模组板。其核心芯片为TLSR9218，当前版本为v1.1，支持ZigBee，BLE协议。只需下载现成的bin文件，即可通过uart来控制模组板。用户可以方便的进行芯片评估，也可把模组板集成到自己的电路系统中，为自己的系统扩展无线功能。

    模组板的原理图、PCB图以及相关的BLE和ZigBee SDK应用代码、网页工具代码均为开源。在开发工具上，电路图绘制软件使用的是流行且开源免费的KiCad，芯片软件开发环境可去泰凌wiki上获取。另外为了方便用户学习和二次开发，模组板配套的各种上位机工具也用开源免费的工具开发。

  模组板照片：

<div class="div_center" style="text-align:center"> 
<img src="../../Resource/images/m_top.png" class="image" style="width:25%"><img src="../../Resource/images/m_bottom.png" class="image" style="width:25%">
</div>

!!! note "从上图可以看出模组板只留出了必要的接口，让接口尽可能的简单，接口定义如下："

|左侧接口     |功能|右侧接口|功能|
|:---        |:---: |:---|:---:|
|RESET     | 硬件复位 | DM   | USB DM|  
|PD3       | GPIO/PWM | DP   | USB DP|  
|PD2       | GPIO     | SWS  | 下载调试 | 
|PD1       | GPIO     | TX   | uart tx  |
|PE0       | GPIO/PWM | RX   | uart rx  |
|PC2       | GPIO     | RTS  | uart rts/GPIO/PWM | 
|3.3V      | 电源3.3V | PB5  | GPIO/PWM |
|GND       | 电源GND  | GND  | 电源GND | 

下图为素颜照，从照片看，模组板尺寸和一枚1元硬币差不多.
<div class="div_center" style="text-align:center"> 
<img src="../../Resource/images/Mars_B91_vs_coin.png" class="image" style="width:50%">
</div>

一个模组板的典型应用见下图官方底板，其中PC2连接按键，PD2，PD1连接拨码开关，UART TX，RX通过USB转串口芯片转为USB接口，可以直接连接PC

<div class="div_center" style="text-align:center"> 
<img src="../../Resource/images/evb.gif" class="image" style="width:40%">
</div>

## 如何使用
- 使用Telink Buring Evk(调试下载器)，通过BDT或者Web BDT下载对应的bin文件到模组中
- 通过UART使用模组板

!!! Abstract "下载工具"

    === "WebBDT"
        为了工具能跨平台使用，同时用户不必下载和安装工具，可即开即用，Telink新推出了基于Web的下载调试工具：:octicons-heart-fill-24:{ .heart } <a href="https://debug.telink-semi.cn/web_bdt/" target="_blank"> Web BDT </a>
        
        用户可以通过网页来进行程序下载和调试，网页支持Windows，Linux，macOS，Android等平台，只需要一个兼容的网页浏览器即可(:octicons-heart-fill-24:{ .heart }推荐使用较新版本的Chrome或者Edge)。 <a href="../../webtools/web_bdt/" target="_blank"> 文档 </a>

    === "老工具"
        Telink原来的下载工具为<a href="http://wiki.telink-semi.cn/wiki/IDE-and-Tools/Burning-and-Debugging-Tools-for-all-Series/"  target="_blank"> BDT </a>，可在Windows平台下使用。

## 模组构建为ZigBee网络
<p>一个典型的ZigBee mesh网络如下所示，新设备入网时，网络中必须有一个coordinator设备：</p>
<div class="div_center" style="text-align:center"> 
<img src="../../Resource/images/zigbee.png" class="image" style="width:70%">
</div>

!!! info "Mars_B91模组板构建ZigBee网络步骤如下："
    :octicons-heart-fill-24:{ .heart } 若使用旧版BDT工具进行下载，需先将下方固件下载到本地电脑端，若使用[Web BDT]，则可在线下载固件进芯片。
    [Web BDT]: ../webtools/web_bdt.md#环境准备

     1. 下载  <a href="../../Resource/bin/Mars_B91/Mars_B91_ZigBee_C.bin" target="_blank">Mars_B91_ZigBee_C.bin</a>  到其中一个模块板(coordinator)
     2. 下载  <a href="../../Resource/bin/Mars_B91/Mars_B91_ZigBee_R.bin" target="_blank">Mars_B91_ZigBee_R.bin</a>  到其他多个模块板(router)
     3. coordinator 设备先上电，此时模块板上led灯亮，表示允许其他设备入网（3分钟内）
     4. router 设备上电，router设备led闪烁，表示入网成功
     5. 组网成功后，通过UART发送数据即可实现广播或者定点传输数据（UART发送一笔数据最长64字节）

固件默认相关配置说明:

模组板接口   |功能说明
:-         |:----
 TX/RX      | UART TX/RX，8数据位，1停止位，无奇偶检验，波特率默认为115200
 PC2        | 可接主控MCU，或者按键，上电初始化时需保持未按下状态<br><br>对于coordinator， 单击：限定时间内允许入网；双击：关闭入网许可<br><br>对于rounter， 单击：广播发送自身网络地址； 双击：广播用户自定义设备ID号；长按：恢复出厂设置
 PD2，PD1 | 模式配置控制IO（使用过程中可动态切换模式，切换间隔与按键长按的时间一致，默认为两秒）: <br><br> 00: 广播透传模式，uart收到任何数据都通过ZigBee协议广播出去，收到数据的设备通过uart打印出数据<br><br>01: command模式，功能码使用示例见后面的描述<br><br>10: 定位模式，router收集移动模组定位数据，定期发送给coordinator<br><br>11: 移动模式，router定期广播自定义ID号

!!! note "说明"
    + 设备广播自身的ID号时，广播数据中会带有头部 5A A5；
    + 设备广播自身短地址时，广播数据中会带有头部 A5 5A；
    + 设备广播自身长地址时，广播数据中会带有头部 A5 A5；
    + 打开设置命令应答（默认关闭）后，command模式下，发送设置相关的command命令，设备返回的应答中会带有尾部 5A 5A；

设备出厂时的默认设定：

功能 |默认设置 |功能 |默认设置
:-         |:---- |:---- |:----
UART回显|关闭|UART波特率|115200
设置指令的命令应答(后缀5A A5)|不添加|设备ID|0x00
设备广播自身ID的周期|2秒|PWM设定|不开启
短按检测时间|200毫秒|长按检测时间|2秒
单击按键后协调器限定的入网时间|180秒|/|/
入网时的触发IO|GPIO_PA5|入网时的触发IO的触发电平|低电平
死机时的触发IO|GPIO_PA6|死机时的触发IO的触发电平|低电平

透传模式系统框图如下所示
<div class="div_center" style="text-align:center"> 
<img src="../../Resource/images/zigbee_uart.png" class="image" style="width:70%">
</div>

!!! warning "提示"
    上方介绍的是ZigBee透传模式，其他模式下的使用请点击[ZigBee设备command模式下的配置]
    [ZigBee设备command模式下的配置]: ../webtools/web_uart.md#zigbeecommand
    
## 模组使用BLE功能

  Mars_B91模组板上电后，即可被使用，通过对应的串口命令得到模组的信息，命令格式见：<a href="../../Resource/doc/TLSR9218_module说明.docx" target="_blank">TLSR9218_module说明.docx</a>。

!!! note "Web BLE介绍"
    Web BLE是一款可与BLE设备通讯的网页的工具。模组板UART透传示意图如下所示
    <div class="div_center" style="text-align:center"> 
    <img src="../../Resource/images/Web_BLE0.png" class="image" style="width:70%">
    </div>

!!! info "使用Web BLE与Mars_B91模组板通讯步骤如下："
    :octicons-heart-fill-24:{ .heart } 若使用旧版BDT工具进行下载，需先将下方固件下载到本地电脑端，若使用[Web BDT]，则可在线下载固件进芯片。 
    [Web BDT]: ../webtools/web_bdt.md#环境准备

     1. 下载  <a href="../../Resource/bin/Mars_B91/Mars_B91_BLE.bin" target="_blank">B91_ble_module.bin</a>  到模组板中
     2. 打开网页 :octicons-heart-fill-24:{ .heart } <a href="https://debug.telink-semi.cn/web_ble/" target="_blank"> Web BLE </a>
     3. 点击"scan"，连接名为"Telink_Mars_B91"的设备
    <div class="div_center" style="text-align:center"> 
    <img src="../../Resource/images/Web_BLE1.jpg" class="image" style="width:50%">
    </div>
     4. 第一次连接时，需要系统给连接权限，设备连接后，模组上的LED会点亮，网页端会自动打印模组的 service、characteristic、properties，点击 "show" 也会再次打印这些信息
    <div class="div_center" style="text-align:center"> 
    <img src="../../Resource/images/Web_BLE2.jpg" class="image" style="width:50%">
    </div>
    <div class="div_center" style="text-align:center"> 
    <img src="../../Resource/images/Web_BLE4.jpg" class="image" style="width:100%">
    </div>
     5. 点击 "send" 发送左侧输入框内的16进制数据给模组板，模组板UART会打印收到的数据
   
        example: 

            Web BLE发送数据: 12 34 56 78

            UART接收到数据: FF 09 A0 07 52 15 00 12 34 56 78
 
            命令解析: 长度: 09  数据: 12 34 56 78

     6. 模组板会将UART收到的数据通过BLE发送给Web BLE，网页端会打印收到的数据以及characteristic的UUID
   
        example: 

            往模组板UART发送数据: 1c ff 05 00 11 22 33 44 55

            命令解析: 长度: 0x0005  数据: 01 02 03 04 05

            UART返回数据: FF 03 1C 07 00

            Web BLE打印数据: 00010203-0405-0607-0809-0a0b0c0d2b10  Received:  11  22  33  44  55

     6. 点击 "clear" 清空页面显示窗口内容，点击 "name" 打印模组板的名称，点击 "install" 可将网页以原生应用的方式安装在电脑端。

## Telink Web Tools演示视频

!!! info "<a href="https://www.bilibili.com/video/BV15P411n7Jb/?spm_id_from=333.337.search-card.all.click&vd_source=3c6169037f6b291cd7e68eff5dbaf15f" target="_blank"> Telink Web Tools演示视频(哔哩哔哩) </a>"
    <video  controls controlsList="nodownload" oncontextmenu="return(false);"> 
    <source src="../../Resource/video/Telink_Web_Tools_userguide.mp4" type="video/mp4">
    你的浏览器不支持video标签
    </video>
