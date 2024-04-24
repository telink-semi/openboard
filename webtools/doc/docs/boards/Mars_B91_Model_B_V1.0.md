## 底板介绍

!!! note "Mars_B91_Model_B_V1.0"
    Mars_B91_Model_B_V1.0是为Mars_B91_v2.1提供的底板。在母板上核心板的接口引脚的功能如下表所示。
  核心板照片：
<div class="div_center" style="text-align:center"> 
 <img src="../../Resource/Mars_B91_Model_B_V1.0/images/Mars_B91_V2.1_Back_3D.png" class="image" style="width:30%">
</div>

!!! note "在母板上模组接口定义如下："

    |左侧接口   |功能|右侧接口|功能|下侧接口|功能|
    |:---      |:---:|:---|:---:|:---|:---:|
    |RESET     | 硬件复位  | PE5    | LED_B    |VBUS    | 电源5V    |
    |PE4       | WS2812B  | PE6    | LED_R    |GND     | 电源GND    |
    |PE3       | 中间按键  | PE7    | LED_G    |PC3     | DMIC_CLK2 |  
    |PE2       | I2C_SDA  | PA5_DM | USB_DM   |PC2     | DMIC_CLK1 | 
    |PE1       | 右侧按键  | PA6_DP | USB_DP   |PC1     | DMIC_DAT_I|
    |PE0       | I2C_SCK  | PA7_SWS | SWS     |PC0     | PSPI_CN   |
    |PD3       | PSPI_MOSI| PB0     | ADC_CH0 |PB5     | PWM1_LED  |
    |PD2       | PSPI_MISO| PB1     | 左侧按键 |PB4     | UART0_RTS |
    |PD1       | PSPI_CK  | PB2     | UART0_TX |AIPL1  | AIPL1     | 
    |PD0       | UART0_CTS|  PB3    | UART0_RTX|MICBIAS| MICBIAS   | 
    |GND       | 电源GND  | LN      | AONHPL    |       |          |
    |3.3V      | 电源3.3V | LP      | AOPHPL    |       |          |

-  PB1、PE1、PE3为GPIO功能，连接拨轮按键
-  PE0和PE2复用为IIC，IIC总线上挂载了触摸屏(驱动IC:CST816T)、SHT30（温湿度传感器）、LIS2DH（三轴加速度计）、BH1750（光照传感器）
-  PA5和PA6作为USB的DM和DP使用
-  PA7为SWS作下载调试使用
-  AIPL1和MICBIAS为模拟麦克风接口，母板并未使用到
-  LN和LP为耳机接口，连接了板载的3.5mm耳机座
-  PE5、PE6、PE7为GPIO功能，驱动板载的RGB_LED
-  PC1、PC2、PC3复用为DMIC功能，连接板载的数字麦克风
-  PB2、PB3、PD0、PB4复用为UART功能，连接板载的CH340E（UART_TTL转USB）使用
-  PD3、PD2、PD1、PC0复用为PSPI功能，连接板载的LCD屏（驱动IC:ST7789V）和TF卡接口
-  PE4复用为PWM功能，驱动板载的WS2812B彩灯
-  PB5复用为PWM功能，驱动板载的LED
-  PB0复用为ADC功能，采集光敏电阻处的电压值
-  左上侧引出GND、DOUT，其中DOUT为板载的WS2812B的串行输出DOUT引脚，该引脚可用来串联更多的WS2812B
-  右上侧引出3V3、SWS、GND用作调试使用
-  注意：PE4、PE5、PE6、PE7作为输出驱动WS2812B和RGB_LED，这两个器件不会对复用为JTAG功能进行干扰，因此用户可将这四个引脚复用为JTAG功能（TLSR9218B的JTAG只支持四线模式，不支持两线模式）


!!! note "母板照片（实物图中核心板为黑色，母板为蓝色）"
    <div class="div_center" style="text-align:center"> 
    <img src="../../Resource/Mars_B91_Model_B_V1.0/images/Mars_B91_Model_B_V1.0_Fornt_3D_2.png" class="image" style="width:100%">
    <iframe loading="lazy" src="../../Resource/Mars_B91_Model_B_V1.0/doc/Mars_B91_Model_B_V1.0_ibom.html" width="100%" height="800"></iframe>
    </div>

[模块板电路原理图](../Resource/Mars_B91_Model_B_V1.0/doc/Mars_B91_Model_B_V1.0_Sch.pdf)

可先下载驱动测试bin测试板子上器件是否正常: <a href="../../Resource/Mars_B91_Model_B_V1.0/bin/Mars_B91_Model_B_V1.0_Driver_Test1.bin" target="_blank">Mars_B91_Model_B_V1.0_Driver_Test1.bin</a> <br/>
开发环境截图：<img src="../../Resource/Mars_B91_Model_B_V1.0/images/Telink IoT Studio.png" class="image" style="width:100%">
 