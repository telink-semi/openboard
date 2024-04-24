---
icon: :octicons-heart-fill-24: 
---

# telink script tool

## 固件构建

!!! info "固件构建"
    telink script tool与我们主控芯片通讯，用于脚本文件的收发等功能。首先我们需要将符合交互规范的固件下载到我们的芯片中。下面介绍三种构建固件的办法，使用2、3两种方式时，用户可在my_lua_reg.c中添加自己封装给脚本使用的接口。

    1. 若想快速体验，可下载[B91快速体验固件](../Resource/telink_script_tool/B91_test.bin)，此固件封装了一些驱动接口供LUA调用，具体可看[my_lua_reg.c](../Resource/telink_script_tool/my_lua_reg.c)文件。
   
    2. 源码放在<a href="http://192.168.48.36/sdk_app/openboard/opensdk"target="_blank">仓库</a>的 telink_mars_b91_driver_sdk_v2.2.0/Mars_B91_Model_B_V1.0下，可以编译此工程后得到固件。当然也可以直接将源码里的代码直接移植到其他平台中。目前仅内部测试使用，主要待改进地方：内存管理功能修改，文件系统存储介质层驱动改为注册制。

    3. 利用源码生成的lib库加载到自己的工程里进行编译产生固件，参考<a href="http://192.168.48.36/sdk_app/openboard/opensdk/tree/master/Mars_B91/telink_script/driver_sdk_V2.3.0.7z"target="_blank">B91_demo工程</a>。在Telink IoT Studio的设置中将tlua_lib.a和数学库加进去，并将文件夹lua_interface和telink_lua_lib加入工程，初始化和主循环中调用lib库中的两个接口即可。<img src="../../Resource/telink_script_tool/tlua_lib.jpg" class="image" style="width:100%"><img src="../../Resource/telink_script_tool/math_lib.jpg" class="image" style="width:100%">
    
    ``` c
        #include "lua_interface/my_lua_reg.h"
        #include "lua_interface/my_lua_interface.h"
        #include "telink_lua_lib/lua/lua_extend.h"

        void user_init()
        {
            telink_lua_lib_init(lua_lib,&lua_interface);
        }

        void main_loop(void)
        {
            while(1){
                telink_lua_main_loop();
            }
        }
    ```
## telink script tool
!!! note "Windows APP"
    目前只提供了桌面端的APP，WEB端APP计划后续推出。
    首先下载工具[telink_script_tool](../Resource/telink_script_tool/telink_script_tool.7z)，工具主界面如下图所示，侧边栏是功能按钮，中间为文件列表，右侧为文件编辑区，下方为log显示窗口。
    <div class="div_center" style="text-align:center"> 
     <img src="../../Resource/telink_script_tool/telink_script_tool_home.jpg" class="image" style="width:100%">
    </div>

    project文件夹下，send为发送区的文件夹，receive为接收区文件夹，config.ini为系统配置文件，stytle.qss为用户自定义样式表。设置选项的状态是可以保存的，正常使用时设置为默认模式即可。连接好串口后，点击下载/删除/读取按钮后需在规定时间内复位芯片。
    <div class="div_center" style="text-align:center"> 
     <img src="../../Resource/telink_script_tool/telink_script_tool_setting.png" class="image" style="width:80%">
    </div>

!!! note "lua script"
    用户需在send区编辑main.lua脚本，并实现setup和loop函数。用户可以下载其他名称的lua脚本到芯片中，并在main.lua中引入这些文件并使用其中的功能。