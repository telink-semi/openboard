# Web BLE

## 工具介绍
!!! note "Web BLE介绍"
    Web BLE是一款可与BLE设备通讯的网页的工具 (电脑端充当BLE Master Device)。模块板UART透传示意图如下所示
    <div class="div_center" style="text-align:center"> 
    <img src="../../Resource/images/Web_BLE0.png" class="image" style="width:70%">
    </div>

## 使用说明
  Mars_B91模块板上电后，即可被使用，通过对应的串口命令得到模块的信息，命令格式见：<a href="../../Resource/doc/TLSR9218_module说明.docx" target="_blank">TLSR9218_module说明.docx</a>。

!!! info "使用Web BLE与Mars_B91模块板通讯步骤如下："
    :octicons-heart-fill-24:{ .heart } 若使用旧版BDT工具进行下载，需先将下方固件下载到本地电脑端，若使用[Web BDT]，则可在线下载固件进芯片。 
    [Web BDT]: ../webtools/web_bdt.md#环境准备

     1. 下载  <a href="../../Resource/bin/Mars_B91/Mars_B91_BLE.bin" target="_blank">Mars_B91_BLE.bin</a>  到模块板中
     2. 打开网页 :octicons-heart-fill-24:{ .heart } <a href="https://debug.telink-semi.cn/web_ble/" target="_blank"> Web BLE </a>
     3. 点击"scan"，连接名为"Telink_Mars_B91"的设备
    <div class="div_center" style="text-align:center"> 
    <img src="../../Resource/images/Web_BLE1.jpg" class="image" style="width:50%">
    </div>
     4. 第一次连接时，需要系统给连接权限，设备连接后，模块上的LED会点亮，网页端会自动打印模块的 service、characteristic、properties，点击 "show" 也会再次打印这些信息
    <div class="div_center" style="text-align:center"> 
    <img src="../../Resource/images/Web_BLE2.jpg" class="image" style="width:50%">
    </div>
    <div class="div_center" style="text-align:center"> 
    <img src="../../Resource/images/Web_BLE4.jpg" class="image" style="width:100%">
    </div>
     5. 点击 "send" 发送左侧输入框内的16进制数据给模块板，模块板UART会打印收到的数据
   
        example: 

            Web BLE发送数据: 12 34 56 78

            UART接收到数据: FF 09 A0 07 52 15 00 12 34 56 78
 
            命令解析: 长度: 09  数据: 12 34 56 78

     6. 模块板会将UART收到的数据通过BLE发送给Web BLE，网页端会打印收到的数据以及characteristic的UUID
   
        example: 

            往模块板UART发送数据: 1c ff 05 00 11 22 33 44 55

            命令解析: 长度: 0x0005  数据: 01 02 03 04 05

            UART返回数据: FF 03 1C 07 00

            Web BLE打印数据: 00010203-0405-0607-0809-0a0b0c0d2b10  Received:  11  22  33  44  55

     6. 点击 "clear" 清空页面显示窗口内容，点击 "name" 打印模块板的名称，点击 "install" 可将网页以原生应用的方式安装在电脑端。
