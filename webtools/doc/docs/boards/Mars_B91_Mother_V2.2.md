## 底板介绍

!!! note "Mars_B91_Mother_V2.2"
    Mars_B91_Mother_V2.2是为Mars_B91提供的底板。Mars_B91接口引脚的功能如下表所示。

    |左侧接口   |功能|右侧接口|功能|
    |:---      |:---:|:---|:---:|
    |RESET     | 硬件复位 | PA5/DM | USB_DM|  
    |PD3       | 右侧按键 | PA6/DP | USB_DP|  
    |PD2       | 中间按键 | PA7/SWS | 下载调试 | 
    |PD1       | 左侧按键 | PB2/TX | UART0_TX |
    |PE0       | IIC_SCL  | PB3/RX | UART0_RX |
    |PC2       | IIC_SDA  | PB4/RTS | PWM0 |
    |3.3V      | 电源3.3V | PB5  | ADC |
    |GND       | 电源GND  | GND  | 电源GND | 
-  PD3、PD2、PD1为拨轮按键所使用
-  PE0和PC2复用为IIC，IIC总线上挂载了0.96寸的OLED、SHT30（温湿度传感器）、LIS2DH（三轴加速度计）
-  PA5和PA6作为USB的DM和DP使用
-  PA7为SWS作下载调试使用
-  PB2和PB3复用为UART功能，连接板载的CH340E（UART_TTL转USB）使用
-  PB4复用为PWM功能，驱动板载的WS2812B彩灯
-  PB5复用为ADC功能，采集光敏电阻处的电压值
-  左下侧引出3V3、SWS、GND、DOUT，其中DOUT为板载的WS2812B的串行输出DOUT引脚，该引脚可用来串联更多的WS2812B
  
  模组板照片：
<div class="div_center" style="text-align:center"> 
<img src="../../Resource/images/Mars_B91_Mother_V2.2/Mars_B91_Mother_1.bmp" class="image" style="width:100%">
<img src="../../Resource/images/Mars_B91_Mother_V2.2/Mars_B91_Mother_2.png" class="image" style="width:100%">
<iframe loading="lazy" src="../../Resource/html/Mars_B91_Mother_V2_ibom.html" width="100%" height="800"></iframe>
</div>
[模块板电路原理图](../Resource/doc/Mars_B91_Mother_V2.2/Mars_B91_Mother_V2.2_Sch.pdf)

可先下载驱动测试bin测试板子上器件是否正常: <a href="../../Resource/bin/Mars_B91_Mother_V2.2/Mars_B91_Mother_V2.2_Driver_Test1.bin" target="_blank">Mars_B91_Mother_V2.2_Driver_Test1.bin</a> <br/>
开发环境截图：<img src="../../Resource/images/Mars_B91_Mother_V2.2/Telink IoT Studio.png" class="image" style="width:100%">
