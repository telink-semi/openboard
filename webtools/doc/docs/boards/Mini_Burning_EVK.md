## 离线下载器介绍

!!! note "Mini_Burning_EVK"
    Mini_Burning_EVK 是一款专为泰凌芯片提供调试下载功能的下载器，当前版本为v1.1。可配合<a href="http://wiki.telink-semi.cn/wiki/IDE-and-Tools/Burning-and-Debugging-Tools-for-all-Series/" target="_blank"> BDT </a>或者<a href="https://debug.telink-semi.cn/web_bdt/" target="_blank"> Web BDT </a>工具使用。
    与普通Burning EVK在功能上有两点最明显的不同：其可支持离线下载功能且不支持OTP烧录。

  模块板照片：

<div class="div_center" style="text-align:center"> 
<img src="../../Resource/images/mini_burning_evk.jpg" class="image" style="width:85%">
</div>
下载板左侧为USB Type-C接口，通过USB线与电脑连接。左上角为FLASH复位按键，下方分别为一件下载按钮、模式选择按钮、
菜单选择拨轮按钮，右侧为连接目标板的下载接口：GND、SWM、3V3

!!! note "离线下载功能使用步骤"
本文档主要介绍使用Web BDT和Mini_Burning_EVK实现离线下载功能，其他功能的使用说明可参考BDT文档和Web BDT文档。

1、将<a href="../../Resource/bin/Mini_Burning_EVK/Mini_Burning_EVK.bin" target="_blank">固件</a>下载到Mini_Burning_EVK的FLASH 0地址处。更新固件的操作，Web BDT在Online选项卡处，BDT在Help->Upgrade处。正确更新后会有对应的提示，更新后需要插拔USB线对设备进行重连。

<img src="../../Resource/images/web_bdt_online.jpg" class="image" style="width:80%">

<img src="../../Resource/images/bdt_upgrade.png" class="image" style="width:80%">

2、参考<a href="../../webtools/web_bdt/" target="_blank">web_bdt文档</a>，准备好环境。

3、点击connect连接上设备并打开offline选项卡，点击DownloadToolBin按钮等待提示下载成功后需插拔USB线对设备进行重连

4、点击Add按钮添加bin文件，可根据实际情况修改bin文件的目标芯片和固件类型以及固件名称(小于或等于十个字符)

5、点击Download可以将所选中的文件下载到Mini_Burning_EVK对应FLASH处，等待提示下载成功

6、给Mini_Burning_EVK重新上电后，根据屏幕提示，结合按键使用即可进行离线下载等操作。

注意：前三步如果先前做过，且固件不需要进行更新，则直接从第四步执行即可