## V2.2.0

### Version

* SDK version: telink_b91m_driver_sdk V2.2.0.
* This version of the SDK supports B91(A0/A1/A2),B92(A1/A2) chips.
* The default configuration of LEDs and KEYs match the following hardware revisions:
*   B91	 	C1T213A20_V1_3_20200727
*   B92	 	C1T266A20_V1_3_20220722

### Note

* telink_b91m_driver_sdk v2.0, this version corrects spelling errors and resolves compilation warnings, so there are some CHANGES that affect compatibility, please read the BREAKING CHANGES section for more information that affects compatibility.

### BREAKING CHANGES

* **watchdog**
  * (B92)charger_clear_vbus_detect_status()/charger_get_vbus_detect_status() were renamed wd_turn_off_vbus_timer()/usb_get_vbus_detect_status().
* **audio**
  * (B92)audio_set_codec_stream0_path()/audio_set_codec_stream1_path()/audio_codec_swap_stream0_data()/audio_codec_swap_stream1_d ata() has all new sampling rate parameters.
  * (B91)The structure aduio_i2s_clk_config_t change to audio_i2s_clk_config_t.
  * (B92)The interface aduio_i2s_set_mclk change to audio_i2s_set_mclk.
* **sys**
  * (B92)gpio_voltage_e added to sys_init function to configure the gpio voltage type(GPIO_VOLTAGE_3V3/GPIO_VOLTAGE_1V8).
  * (B92)Modify 1P2 to 1P4 in the enumeration type power_mode_e.
* **i2c** 
  * (B91)The register reg_i2c_slave_strech_en change it to reg_i2c_slave_stretch_en.
  * (B92)The interface i2c_slave_strech_en/i2c_slave_strech_dis/i2c_master_strech_en/i2c_master_strech_dis change them to i2c_slave_stretch_en/i2c_slave_stretch_dis/i2c_master_stretch_en/i2c_master_stretch_dis separately.
  * (B92)The enumeration member FLD_I2C_MASTER_STRECH_EN/FLD_I2C_SLAVE_STRECH_EN change them FLD_I2C_MASTER_STRETCH_EN/FLD_I2C_SLAVE_STRETCH_EN separately.
* **spi**
  * (B91)The enumeration spi_nomal_3line_mode_e and the enumeration member SPI_NOMAL change them spi_normal_3line_mode_e and SPI_NORMAL(the spi_master_config interface is associated with this enumeration).
  * (B92)Remove the spi_pin_config_t structure，add gspi_pin_config_t and lspi_pin_config_t,
     change the parameter of gspi_set_pin function to gspi_pin_config_t pointer type, and change the parameter of lspi_set_pin function to gspi_pin_config_t pointer type.
* **rf**
   * (B91)(B92)The enumeration member FLD_RF_R_NOACK_RECNT_EN/FLD_RF_R_CMD_SCHDULE_EN change them FLD_RF_R_NOACK_RETRY_CNT_EN/FLD_RF_R_CMD_SCHEDULE_EN separately.
   * (B91)(B92)The register reg_rf_modem_sync_thre_ble change to reg_rf_modem_sync_thres_ble.
   * (B92)The rf_set_channel_fpga frequency setting function in FPGA verification phase was deleted and the rf_set_chn interface was used instead.
   * (B92)Updated rf energy meters for the latest supply voltage configurations.
   * (B91/B92)Removed the const keyword from the rf_power_Level_list table.
* **flash**
  * (B92)The interface flash_quad_page_pragram_encrypt change to flash_quad_page_program_encrypt.
* **usb**
  * (B91)(B92)The register reg_usb_ep8_send_thre change to reg_usb_ep8_send_thres.
* **calibration**
  * (B92)The calibration value interface user_read_flash_value_calib() is changed to calibration_func().
* **pm**  
  * (B92)Occupies PM_ANA_REG_POWER_ON_CLR_BUF0[bit1] (0x3a[1]) to mark whether restart caused by abnormal start-up has occurred, the customer cannot use this bit.
* **uart**
  * (B92)uart_get_rx_buff_cnt is changed to uart_get_rxfifo_rem_cnt.
  * (B92)Delete the uart_rts_auto_mode/uart_rts_manual_mode interface.
* **ide**
  * (B91)The IDE for B91 is changed to TelinkIoTStudio, and the toolchain remains GCC7.

  
### Bug Fixes

* **gpio**
  * (B92)The digital Pull-up resistor of PF [5:0] and PG [5:0] is not available, and these pins are prohibited in the interface of gpio_set_pullup_res_30k(). [ad91c3e5](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/566/diffs?commit_id=ad91c3e57719db168a95846f8022a0c7c9bf8ec5)
* **rf**
  * (B92)Fixed the issue that the RSSI value did not match the actual received energy. [435541cf](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/444/diffs?commit_id=435541cf3d9776581b57883579b54cb4421f08fd)
  *	(B91/B92)Fixed the problem of incorrect setting of ctrim at some frequency points of rf_set_chn, and reduced the tx energy difference at different frequency points. [8ead48a7](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/541/diffs?commit_id=8ead48a7e05d90ee0cd64d68ef475f8275974b9f)
  *	(B91/B92)The SETTLE_Time setting error caused by 4bit settle_time not taking effect when the rf_set_tx_settle_time interface is set, the SETTLE_time setting range is extended to 0xfff. [8ead48a7](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/541/diffs?commit_id=8ead48a7e05d90ee0cd64d68ef475f8275974b9f)
* **emi**
  * (B91/B92)Fixed inconsistency between rx packet length and dma fifo size causing FIFO data overflow to overwrite other RAM information. [95a0c226](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/450/diffs?commit_id=95a0c22606473f33a8f5032cf0eeb7660bd94a16)
* **audio**
  * (B92)The channel data exchange problem is solved when the sampling rate of DMIC is 44.1k and 48k. [2928be92](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/453/diffs?commit_id=2928be92d4f9dbd3b52f2633ae13356b3e100c43) 
* **spi**
  * (B92)Fixed the slave gpsi rx dma receiving the first data error. [03fdb77b](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/452/diffs?commit_id=03fdb77bfb4840573ff6bad797a1deea9ebb32ed)
* **mspi**
  * (B92)The related function is modified with ramcode, to avoid the risk that the mspi-related function may be compiled into the text segment and cause crash. [7ce63726](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/465/diffs?commit_id=7ce63726d14f7135e8efd37d7b183a6e52577bd2)
* **sys**
  * (B92)sys_reboot: the definition of sys_reboot is changed to sys.c, and the sys_reboot_ram interface is added. the processing of sys_reboot is stored in ram to avoid incorrect operations on flash when sys_reboot is called. [7ce63726](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/465/diffs?commit_id=7ce63726d14f7135e8efd37d7b183a6e52577bd2)
  * (B91/B92)The sys_reboot interface was rewritten to avoid crash due to prefetch and interruption. [1cd3cc0e](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/479/diffs?commit_id=1cd3cc0e1c14780aad3f729d8db6917ba44c66c9)
  * (B92)The power supply of sram is about 1.2V. Adjusting the voltage to the ram in different states can be maintained at 1.2V. [1cd3cc0e](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/479/diffs?commit_id=1cd3cc0e1c14780aad3f729d8db6917ba44c66c9)
  * (B92)Solve the problem that the crystal oscillator stability flag fails to cause the crash. If the start-up is abnormal, it will restart. Use PM_ANA_REG_POWER_ON_CLR_BUF0[bit1] to check whether the restart caused by the abnormal start-up has occurred. (It has been implemented in the driver layer, and does not require special calls from the application layer.) [406f518d](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/540/diffs?commit_id=406f518d43f134209d4dc9c81bc40523208a93ad)
  * (B92)The clock sources for configuring cclk, hclk, pclk, and mspiclk during initialization are all from 24M rc. [2b85e90e](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/551/diffs?commit_id=2b85e90e6f30097f0f400212951fca7712666936)
  * (B92)The risk of digital core voltages greater than the maximum operating voltage of 1.32V is avoided. The digital core voltage is configured at 1.2V. [cb5916dc](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/573/diffs?commit_id=cb5916dc4b5503b19cc92744e4a7471c3b88205c)
* **clock**
  * (B91)clock_init: added clock_init_ram interface to store the processing of clock_init in the ram to avoid possible machine crashes caused by improper operations during clock configuration. [7ce63726](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/465/diffs?commit_id=7ce63726d14f7135e8efd37d7b183a6e52577bd2)
* **uart**
  * (B92)uart_receive_dma interface directly uses UART0 instead of parameter uart_num, modify it. [f7cebf4e](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/516/diffs?commit_id=f7cebf4e152151ca130c1c3861eff0f856985b10)
* **gpio**
  * (B92)Modify the gpio_set_src_irq() configuration to resolved the problem that the group src level interrupt could not work properly.[e9581b0a](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/511/commits?commit_id=e9581b0a098b9230269a5389a98a82c9e4a90bb3)
* **dma** 
  * (B92/B91) Fix the problem that if the buff parameter in audio_rx/tx_dma_chain_init interface is in IRAM area(DRAM is not affected), then calling audio_get_rx_dma_wptr/audio_get_tx_dma_rptr interface will cause error.  Reason: The return value of convert_ram_addr_cpu2bus is written to the dma-related register,The value of the register is read out as input parameter of convert_ram_addr_bus2cpu,it will be calculated incorrectly. [cfcb91ef](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/543/diffs?commit_id=cfcb91ef95289d327a78c014a401705c53d7118a)
* **pwm**
  * (B91)In the pwm_clr_irq_status interface, fix the problem that the FLD_PWM0_IR_FIFO_IRQ interrupt flag bit is not cleared correctly. [e0dfb211](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/532/diffs?commit_id=e0dfb21137f0e1396618ac246e10626d941093e0)
* **pm**
  * (B92)Solved the problem of high current in suspend sleep caused by leakage caused by the failure to connect the up and down pull of sws. [eb6c0261](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/466/diffs?commit_id=eb6c0261101c513f228a1a9dbf02dc9832f088d6)
* **I2C_Demo**
  * (B92)The received packet data is abnormal due to a code logic error. [106435f3](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/443/diffs?commit_id=106435f316711e1b27c9b7a095969ec2e1fca7aa)
* **USB_Demo**
  * (B92)Fixed the problem of not turn off the 8s vbus timer after unplugging and reinserting the vbus while the power is on(Vbat supply). [b36d04e1](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/451/diffs?commit_id=b36d04e10302fd0eb09c30d9a5e41fb1bc533096)
  * (B91/B92)Fixed an issue that may enter an unexpected endpoint interrupt due to the endpoint interrupt mask being fully turned on by default.[2a22e068](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/454/diffs?commit_id=2a22e0684b524a11c22db86fe932164d56642ed6)
  * (B91)Fixed mic_demo MIC_DMA_CHN undefined and spk_demo #ifdef #endif not used in pairs bug. [a2efbbb2](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/498/diffs?commit_id=a2efbbb2853f979e84fc2dc0464ff0b55c827cbb)
### Features

* **usb**
  * (B92)Add set config and get config support when disable auto config.
* **flash**
  * (B92)Add 2M flash(GD25LQ16E).
* **pm**
  * (B92)Add the function of clearing the pm interrupt flag bit.
* **audio**
  * (B91)Added audio adc/dac 24k sampling rate configuration option to the audio_set_i2s_clock() function interface..
* **rf**
  * (B91/B92) Added interfaces rf_ldot_ldo_rxtxlf_bypass_en(), rf_ldot_ldo_rxtxlf_bypass_dis(), rf_set_rxpara() related to improving the sensitivity performance of the emi/bqb program at 24M multiplier frequency points.
  * (B92) Update the energy meter to support energy setting when the GPIO output voltage is 1.8V.
* **aes**
  * (B91/B92) Added interfaces to prevent conflicting AES encryption and decryption - aes_encrypt_bt_en and aes_decrypt_bt_en.
* **clock**
  * (B91/B92)Added the configuration of CCLK to 96M. 
  * (B92)When the PLL is 192M, add the function interface cclk_hclk_pclk_config() for adjusting the CCLK/HCLK/PCLK frequency and the corresponding enumeration pll_div_cclk_hclk_pclk_e. 
* **flash**
  * (B91/B92)In each flash, add the interface flash_get_lock_block_midxxx to return which area is currently locked in flash (also in midxxx_lock_block_e, Note When flash write protection is used, only the midxxx_lock_block_e enumeration value can be selected, so that the values returned by the flash_get_lock_block_midxxx interface are in midxxx_lock_block_e).
  * (B91/B92)In flash.h, the enumeration flash_mid_e is added,can use the enumeration value instead of flash_mid to increase readability.
  * (B91/B92)flash.h, add FLASH_SIZE_16M to flash_capacity_e.
* **efuse**
  * (B92)Add the interface efuse_calib_adc_vref() to read adc calibration values from efuse.
* **adc**
  * (B92)Add adc calibration value configuration interfaces adc_set_gpio_calib_vref()、 adc_set_gpio_two_point_calib_offset()、adc_set_vbat_calib_vref()和adc_set_vbat_two_point_calib_offset(), and optimize some comments.
* **all**
  * (B92)Add calibration_func() for all demo.
* **i2c**
  * (B91)Add interfaces related to slave/matser stretch:i2c_slave_stretch_en/i2c_slave_stretch_dis/i2c_slave_manual_stretch_en/i2c_slave_manual_stretch_clr/i2c_master_stretch_en/i2c_master_stretch_dis.
  * (B91)In i2c_master_init, the master stretch function is enabled by default (i2c_master_stretch_en), no matter whether the slave is stretch or not, the master side will not be abnormal.
  * (B92)In i2c_master_init, the master stretch function is enabled by default (i2c_master_stretch_en), no matter whether the slave is stretch or not, the master side will not be abnormal.
  * (B92)In i2c_master_init, the i2c_master_detect_nack_en function is turned on by default,no matter whether the slave sends an nack, the master is normal.
* **PM_Demo**
  * (B92)Add PM demo.
* **USB_Demo**
  * (B91)Add msc demo based on tinyusb.
* **EMI_BQB_Demo**
  * (B91)Added EMI_Demo initial configuration function and PA configuration method.
  * (B92)Added EMI_BQB_Demo, and support RF current test function.
* **Flash_Demo**
  * (B91/B92)Maintain flash_lock/flash_unlock interfaces(to determine whether the flash is locked or unlocked), and list all flash interfaces to avoid omissions at the application layer due to subsequent flash additions.
* **TRAP_Demo**
  * (B92)Add machine timer interrupt, software interrupt and add corresponding demo to TRAP_Demo.

### Refactoring

* **aes**
  * (B91/B92)Update the comment of the function interface aes_set_em_base_addr.
* **dma**
  * (B91/B92)Add a note for peripherals using dma: the src and dst addresses of dma must be four-byte aligned,otherwise the program will enter an exception.
  * (B91/B92)Add comments about peripherals used in conjunction with dma.
* **rf**
  * (B91/B92)Set the rf_set_chn function to the _attribute_ram_code_sec_noinline_ type according to the application requirements.
  * (B91/B92)Simplified the implementation of rf_update_internal_cap.
  * (B91/B92)Define rf_set_power_level_index in ram.
* **all**
* (B91/B92)Normalizes the naming of content related to private schemas.
* (B92)Clear all warning errors from DriverSDK.
* **adc**
  * (B91/B92)Add comments to g_adc_vref and corrected some interface comments.
* **gpio**
* (B92)Peripheral some functions pin is not used and not set to GPIO_NONE_PIN, resulting in pin set to PA0 by mistake, modify GPIO_NONE_PIN to 0 to optimize the problem.
* **audio**
* (B92)Audio_set_codec_stream0/1_path interface internal optimization.
* **project**
  * (B92)Add related warning options in Telink IoT Studio: -Wall,-Wextra,-Wshadow,-Werror, and clear related warnings and errors.Clear coding warnings, 
  * (B92)Add functions, jump configurations for various data structures.
  * (B91/B92)Remove compilation options: -Wno-gnu-zero-variadic-macro-arguments(gcc7 and gcc10 no longer support this option).
* **uart**
  *  (B92)uart_get_dma_rev_data_len:simplify the function logic while ensuring the function remains unchanged.
* **adc**
  * (B91)Adjust the position of adc_set_gpio_two_point_calib_offset(), and optimize some comments.
* **gpio/i2c/spi/stimer/pm**
  * (B91/B92):The gpio.h,i2c.h,spi.h,stimer.h add stdbool header files,modified for pm_wakeup_status_e pm_get_wakeup_src function returns. 
* **keyscan/stimer/watchdog/aes/irlearn/adc/lpc/ctb/charger/pwm/timer/gpio**
  * (B91/B92)Correct spelling errors.
* **pm**
  * (B92)Add comments on some gpio pins that cannot be used as a pm wake source.
* **PLIC_Demo**
  * (B91/B92)PLIC_demo merge to TRAP_Demo and delete PLIC_Demo.


### Performance Improvements

* **rf**
  * (B92)Optimize the sensitivity of BLE2M, Private 2M, zigbee, hybee mode.
* **sys**
  * (B92)Adjust each ldo inside the chip to set it to normal voltage gear.
  * (B92)Adjust the VDDO3 output to 1.8V or 3.3V.
* **pm**
  * (B91)Modify the pm_sleep_wakeup function to make suspend sleep time more accurate during long sleep.
* **adc**
  * (B91) Stricter determination of illegal values for ADC calibration values.
* **EMI_BQB_Demo**
  * (B91/B92) Use rf_ldot_ldo_rxtxlf_bypass_en(), rf_ldot_ldo_rxtxlf_bypass_dis(), rf_set_rxpara()
interfaces to improve the sensitivity performance of emi/bqb programs at 24M multiplier frequency points.
### Note

* N/A

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">


### 版本

* SDK版本: telink_b91m_driver_sdk V2.2.0。
* 此版本SDK支持 B91(A0/A1/A2),B92(A1/A2) 芯片。
* LED和KEY的默认配置匹配以下硬件版本:
*   B91	 	C1T213A20_V1_3_20200727
*   B92	 	C1T266A20_V1_3_20220722

### Note

* telink_b91m_driver_sdk V2.2.0,该版本会纠正拼写错误并解决编译警告，因此会有一些影响兼容性的修改，影响兼容性的部分，请详细阅读BREAKING CHANGES部分。

### BREAKING CHANGES
* **watchdog**
  * (B92)charger_clear_vbus_detect_status()/charger_get_vbus_detect_status()分别改名为wd_turn_off_vbus_timer()/usb_get_vbus_detect_status()。
* **audio**
  * (B92)audio_set_codec_stream0_path()/audio_set_codec_stream1_path()/audio_codec_swap_stream0_data()/audio_codec_swap_stream1_data()全部新增加了采样率的形参。
  * (B91)结构体aduio_i2s_clk_config_t改为audio_i2s_clk_config_t。
  * (B92)接口aduio_i2s_set_mclk改为audio_i2s_set_mclk。
* **sys**
  * (B92)sys_init函数新增gpio_voltage_e参数，用于配置gpio电压类型（GPIO_VOLTAGE_3V3/GPIO_VOLTAGE_1V8）。
  * (B92)将枚举类型power_mode_e中的1P2修改为1P4
* **i2c** 
  * (B91)寄存器reg_i2c_slave_strech_en改为reg_i2c_slave_stretch_en。
  * (B92)接口i2c_slave_strech_en/i2c_slave_strech_dis/i2c_master_strech_en/i2c_master_strech_dis分别改为 i2c_slave_stretch_en/i2c_slave_stretch_dis/i2c_master_stretch_en/i2c_master_stretch_dis。
  * (B92)枚举成员FLD_I2C_MASTER_STRECH_EN/FLD_I2C_SLAVE_STRECH_EN分别改为FLD_I2C_MASTER_STRETCH_EN/FLD_I2C_SLAVE_STRETCH_EN。
* **spi**
  * (B91)枚举名spi_nomal_3line_mode_e以及枚举成员SPI_NOMAL分别改为spi_normal_3line_mode_e和SPI_NORMAL(spi_master_config接口和该枚举有关)。
  * (B92)去除spi_pin_config_t结构体,新增gspi_pin_config_t，lspi_pin_config_t，
    将gspi_set_pin函数形参改为gspi_pin_config_t指针类型，将lspi_set_pin函数形参改为gspi_pin_config_t指针类型。  
* **rf**
   * (B91)(B92)枚举成员FLD_RF_R_NOACK_RECNT_EN/FLD_RF_R_CMD_SCHDULE_EN分别改为FLD_RF_R_NOACK_RETRY_CNT_EN/FLD_RF_R_CMD_SCHEDULE_EN。
   * (B91)(B92)寄存器reg_rf_modem_sync_thre_ble改为reg_rf_modem_sync_thres_ble。
   * (B92)删除FPGA验证阶段的频点设置函数rf_set_channel_fpga，改为使用rf_set_chn接口。
   * (B92)针对最新的供电电压配置，更新rf能量表。
   * (B91/B92)删除了rf_power_Level_list表的const关键字。
* **flash**
  * (B92)接口flash_quad_page_pragram_encrypt改为flash_quad_page_program_encrypt。
* **usb**
  * (B91)(B92)寄存器reg_usb_ep8_send_thre改为reg_usb_ep8_send_thres。
* **calibration**
  * (B92)校准值接口user_read_flash_value_calib()改为calibration_func()。
* **pm**
  * (B92)占用PM_ANA_REG_POWER_ON_CLR_BUF0[bit1]（0x3a[1]）标志是否发生过起振异常导致的重启，客户不能使用这个bit。  
* **uart**
  * (B92)uart_get_rx_buff_cnt改为uart_get_rxfifo_rem_cnt。
  * (B92)(B92)删除接口uart_rts_auto_mode/uart_rts_manual_mode。
* **ide**
  * (B91)B91的IDE更改为TelinkIoTStudio，工具链仍然为GCC7。
  

### Bug Fixes

* **gpio**
  * (B92)PF[5:0]和PG[5:0]的数字上拉电阻不可用，这些引脚在接口gpio_set_pullup_res_30k()中被禁止。 [ad91c3e5](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/566/diffs?commit_id=ad91c3e57719db168a95846f8022a0c7c9bf8ec5)

* **rf**
  * (B92)修复了rssi值与实际接收能量不符的问题。[435541cf](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/444/diffs?commit_id=435541cf3d9776581b57883579b54cb4421f08fd)
  * (B91/B92)修复了rf_set_chn部分频点ctrim设置错误的问题，减小不同频点上tx能量差异。[8ead48a7](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/541/diffs?commit_id=8ead48a7e05d90ee0cd64d68ef475f8275974b9f)
  * (B91/B92)解决rf_set_tx_settle_time接口设置settle时间时高4bit未生效导致settle time设置错误的问题，将settle时间的设置范围扩展到0xfff。[8ead48a7](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/541/diffs?commit_id=8ead48a7e05d90ee0cd64d68ef475f8275974b9f)
* **emi**
  * (B91/B92)修复了rx包长度和dma fifo大小之间不一致导致FIFO数据溢出覆盖其他RAM信息。[95a0c226](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/450/diffs?commit_id=95a0c22606473f33a8f5032cf0eeb7660bd94a16)
* **audio**
  * (B92)解决了DMIC在采样率为44.1k,48k时通道数据相互调换的问题。[2928be92](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/453/diffs?commit_id=2928be92d4f9dbd3b52f2633ae13356b3e100c43) 
* **spi**
  * (B92)修复slave端 gpsi rx dma接收第一笔数据会出错问题。[03fdb77b](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/452/diffs?commit_id=03fdb77bfb4840573ff6bad797a1deea9ebb32ed)
* **mspi**
  * (B92)mspi相关的函数增加ramcode修饰，避免了mspi相关函数可能编译到text段，造成死机的风险。[7ce63726](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/465/diffs?commit_id=7ce63726d14f7135e8efd37d7b183a6e52577bd2)
* **sys**
  * (B92)sys_reboot:将sys_reboot的定义改到sys.c中，并新增sys_reboot_ram接口，将sys_reboot的处理放在ram中，避免调用sys_reboot时引发对flash错误操作。[7ce63726](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/465/diffs?commit_id=7ce63726d14f7135e8efd37d7b183a6e52577bd2)
  * (B91/B92)重新写了sys_reboot接口，避免了因预取址和中断等因素引发死机的隐患。[1cd3cc0e](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/479/diffs?commit_id=1cd3cc0e1c14780aad3f729d8db6917ba44c66c9)
  * (B92)sram供电要求1.2V左右。调整不同状态下给ram供电的电压都可以保持在1.2V。[1cd3cc0e](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/479/diffs?commit_id=1cd3cc0e1c14780aad3f729d8db6917ba44c66c9)
  * (B92)解决晶振稳定标志位失灵导致死机的问题。 起振异常则重启，占用PM_ANA_REG_POWER_ON_CLR_BUF0[bit1]查询是否发生过起振异常导致的重启。（已在驱动层实现，不需要应用层特殊调用。）[406f518d](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/540/diffs?commit_id=406f518d43f134209d4dc9c81bc40523208a93ad)
  * (B92)初始化时配置cclk/hclk/pclk/mspiclk的时钟源都来源于24M rc。[2b85e90e](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/551/diffs?commit_id=2b85e90e6f30097f0f400212951fca7712666936)
  * (B92)避免了digital core电压大于最高工作电压1.32V的可能带来的风险。digital core电压配置为1.2V。[cb5916dc](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/573/diffs?commit_id=cb5916dc4b5503b19cc92744e4a7471c3b88205c)
* **clock**
  * (B91)clock_init:新增clock_init_ram接口，将clock_init的处理放在ram中，避免配置clock时不当操作引起死机隐患。[7ce63726](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/465/diffs?commit_id=7ce63726d14f7135e8efd37d7b183a6e52577bd2)
* **uart**
  * (B92)uart_receive_dma接口中有直接使用UART0,而不是参数uart_num，将其修改。[f7cebf4e](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/516/diffs?commit_id=f7cebf4e152151ca130c1c3861eff0f856985b10)
* **gpio**
  * (B92)修改gpio_set_src_irq()配置，解决了group src level中断无法正常工作的问题。[e9581b0a](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/511/commits?commit_id=e9581b0a098b9230269a5389a98a82c9e4a90bb3)
* **dma** 
  * (B92/B91)修复audio_rx/tx_dma_chain_init接口中传入的buff参数分配在IRAM时(DRAM不受影响)，再调用audio_get_rx_dma_wptr/audio_get_tx_dma_rptr接口会出错问题，原因：先调用convert_ram_addr_cpu2bus，其返回值写到dma相关寄存器后，再读出寄存器的值输入到convert_ram_addr_bus2cpu 其计算结果会出错。[cfcb91ef](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/543/diffs?commit_id=cfcb91ef95289d327a78c014a401705c53d7118a)
* **pwm**
  * (B91)在pwm_clr_irq_status接口中，修复FLD_PWM0_IR_FIFO_IRQ中断标志位不能正确清除问题。[e0dfb211](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/532/diffs?commit_id=e0dfb21137f0e1396618ac246e10626d941093e0)
* **pm**
  * (B92)解决了因sws未接模拟上下拉而漏电导致的suspend睡眠电流大的问题。[eb6c0261](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/466/diffs?commit_id=eb6c0261101c513f228a1a9dbf02dc9832f088d6)
* **I2C_Demo**
  * (B92)代码逻辑错误导致收包数据异常。[106435f3](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/443/diffs?commit_id=106435f316711e1b27c9b7a095969ec2e1fca7aa)
* **USB_Demo**
  * (B92)修复了在不断电的情况下(Vbat供电),重复拔插Vbus，vbus 8s复位定时器无法再次清除的问题。[b36d04e1](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/451/diffs?commit_id=b36d04e10302fd0eb09c30d9a5e41fb1bc533096)
  * (B91/B92)修复端点中断mask默认全部打开，可能导致进入非预期端点中断问题。[2a22e068](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/454/diffs?commit_id=2a22e0684b524a11c22db86fe932164d56642ed6)
  * (B91)(B91)修复了 mic_demo MIC_DMA_CHN 未定义和 spk_demo #ifdef #endif未成对使用的错误。[a2efbbb2](http://192.168.48.36/src/driver/telink_b91m_driver_src/merge_requests/498/diffs?commit_id=a2efbbb2853f979e84fc2dc0464ff0b55c827cbb)
  

### Features

* **usb**
  * (B92)USB 手动配置模式下，支持获取配置命令和设置配置命令。  
* **flash**
  * (B92)增加2M flash(GD25LQ16E)。
* **pm**
  * (B92)增加清除pm中断标志位的函数接口。
* **audio**
  * (B91)在audio_set_i2s_clock()函数接口里增加audio adc/dac 24k采样率配置选项。
* **rf**
  * (B91/B92)添加了提升emi/bqb程序在24M倍数频点的sensitivity性能的相关接口rf_ldot_ldo_rxtxlf_bypass_en()、rf_ldot_ldo_rxtxlf_bypass_dis()、rf_set_rxpara()。
  * (B92)更新能量表以支持GPIO 输出电压为1.8V时的能量设置。
* **aes**
  * (B91/B92)新增防止冲突的AES加密和解密的接口-aes_encrypt_bt_en和aes_decrypt_bt_en。
* **clock**
  * (B91/B92)新增把CCLK为96M的配置。
  * (B92)PLL为192M时，增加调节CCLK/HCLK/PCLK频率的函数接口cclk_hclk_pclk_config() 和对应的枚举pll_div_cclk_hclk_pclk_e。  
* **flash**
  * (B91/B92)在每款flash中，添加接口flash_get_lock_block_midxxx，返回flash当前锁的是哪一块区域（同时在midxxx_lock_block_e中，添加注释说明flash写保护时只能选择。midxxx_lock_block_e的枚举值，使得flash_get_lock_block_midxxx接口返回的值都在midxxx_lock_block_e中）。
  * (B91/B92)flash.h中，新增枚举flash_mid_e，可以使用枚举值代替直接使用flash_mid，增加可读性。
  * (B91/B92)flash.h中，在flash_capacity_e中添加FLASH_SIZE_16M。
* **efuse**
  * (B92)新增从efuse读取adc校准值接口efuse_calib_adc_vref()。
* **adc**
  * (B92)新增adc校准值配置接口adc_set_gpio_calib_vref()、 adc_set_gpio_two_point_calib_offset()、adc_set_vbat_calib_vref()和adc_set_vbat_two_point_calib_offset()，并优化了部分注释。
* **all**
  * (B92)为所有demo添加calibration_func()功能。
* **i2c**
  * (B91) 新增slave/matser stretch功能相关接口 ：i2c_slave_stretch_en/i2c_slave_stretch_dis/i2c_slave_manual_stretch_en/i2c_slave_manual_stretch_clr/i2c_master_stretch_en/i2c_master_stretch_dis。
  * (B91) 在i2c_master_init中，默认将master stretch功能打开(i2c_master_stretch_en),无论对方slave是否stretch,master端都不会异常。
  * (B92) 在i2c_master_init中，默认将master stretch功能打开(i2c_master_stretch_en),无论对方slave是否stretch,master端都不会异常。
  * (B92) 在i2c_master_init中，默认打开i2c_master_detect_nack_en功能,无论对方slave是否发送nack,master端都不会异常。
* **PM_Demo**
  * (B92)添加 PM demo. 
* **USB_Demo**
  * (B91)基于 tinyusb 添加 U 盘 Demo。
* **EMI_BQB_Demo**
  * (B91)增加了EMI_Demo初始配置功能和PA配置方法。
  * (B92)增加了EMI_BQB_Demo,并支持RF电流测试功能。
* **Flash_Demo**
  * (B91/B92)给应用层维护两个接口flash_lock/flash_unlock（判断flash是否上锁还是解锁），将所有的flash列举出来，避免由于后续新增flash，应用层有遗漏。
* **TRAP_Demo**
  * (B92)添加 mtime 中断, 软件中断并在 TRAP_Demo 中添加相应的示例.

### Refactoring

* **aes**
  * (B91/B92)更新函数接口aes_set_em_base_addr的注释。
* **dma**
  * (B91/B92)添加使用dma的外设注释: dma的src和dst地址必须四字节对齐，否则程序进入异常。
  * (B91/B92)补充外设与dma结合使用的相关注释。
* **rf**
  * (B91/B92)根据应用需求将rf_set_chn函数设置为_attribute_ram_code_sec_noinline_类型。
  * (B91/B92)简化rf_update_internal_cap内部分实现。
  * (B91/B92)将rf_set_power_level_index定义在ram中。 
* **all**
  * (B91/B92)规范化与私有模式相关内容的命名。 
  * (B92)清除DriverSDK所有警告报错。
* **adc**
* (B91/B92)对g_adc_vref添加注释，并修正部分接口注释。
* **gpio**
* (B92)外设某些功能pin不用且未设为GPIO_NONE_PIN,导致pin误设置到PA0，修改GPIO_NONE_PIN为0优化该问题。
* **audio**
* (B92)audio_set_codec_stream0/1_path接口内部优化。
* **project**
  * (B92)工程配置下添加如下编译选项：-Wall,-Wextra,-Wshadow,-Werror，并清除警告报错。
  * (B92)解决特殊文件不能跳转问题。
  * (B91/B92)去除编译选项：-Wno-gnu-zero-variadic-macro-arguments(gcc7和gcc10已经不支持该选项)。
* **uart** 
  * (B92)uart_get_dma_rev_data_len:保证功能不变的情况下，简化功能逻辑。
* **adc**
  * (B91)调整adc_set_gpio_two_point_calib_offset()的位置，并优化了部分注释。
* **gpio/i2c/spi/stimer/pm**
  * (B91/B92)将gpio.h,i2c.h,spi.h,stimer.h添加stdbool头文件，pm_get_wakeup_src函数返回修改为pm_wakeup_status_e。
* **keyscan/stimer/watchdog/aes/irlearn/adc/lpc/ctb/charger/pwm/timer/gpio**
  * (B91/B92)纠正拼写错误。
* **pm**
  * (B92)添加注释说明不能用作pm唤醒源的一些gpio引脚
* **PLIC_Demo**
  * (B91/B92)将 PLIC_Demo 合并到 TRAP_Demo 并删除 PLIC_Demo。

### Performance Improvements

* **rf**
  * (B92)优化 BLE2M，Private 2M，zigbee，hybee模式的sensitivity。
* **sys**
  * (B92)调节芯片内部各个ldo，使其设置成正常电压档位。
  * (B92)调节VDDO3输出1.8V或者3.3V。
* **pm**
  * (B91)修改pm_sleep_wakeup函数，使长睡眠时suspend睡眠时间更准确。
* **adc**
  * (B91) 加严了ADC校准值的非法值判断。
* **EMI_BQB_Demo**
  * (B91/B92)使用rf_ldot_ldo_rxtxlf_bypass_en()、rf_ldot_ldo_rxtxlf_bypass_dis()、rf_set_rxpara()
接口，提升emi/bqb程序在24M倍数频点的sensitivity性能。


<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">


## V2.1.0-Beta

### Version

* SDK version: telink_b91m_driver_sdk V2.1.0-Beta.
* This version of the SDK supports B91(A0,A1,A2),B92(A1) chips.
* B92(added) supports USB/AUDIO/I2C/s7816//UART/QDEC/PWM/Timer/Stimer/PLIC/GPIO/SPI/PKE/AES/ANALOG/TRNG
* The default configuration of LEDs and KEYs match the following hardware revisions:
*   B91	 	C1T213A20_V1_3_20200727
*   B92	 	C1T266A20_V1_3_20220722

### BREAKING CHANGES

* **flash** 
  * (B91)Modify midxxxx_otp_block_e and midxxxx_lock_otp_e element name to change the unit K to B to avoid misleading OTP size.
* **watchdog**
  * (B91)The wd_clear interface only clears the watchdog count, but not the reset state of the watchdog.

### Bug Fixes

* **EMI_Demo**
  * (B91)Fixed the problem that the carrier signal cannot be amplified by the PA module under certain PA pin combinations.[e01a08e2](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/e01a08e240e16476a87cb7bdf618a77c18a331a5)
* **PM**
  * (B91)Solved the probabilistic crash problem caused by abnormal flash instruction fetch after suspend wakeup (must appear under certain software logic).[c3e12945](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/c3e12945fa99b7a73d98a0af96360e330c9b9ac9)
  * (B91)Solved the problem that the sleep time is incorrect due to a small probability that the total interruption time of the shutdown is not timely.[e66aca1a](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/e66aca1a1c3fa87d80bba50828437bc767c16088)
* **pwm**
  * (B91)pwm_clr_irq_status: when the pwm_clr_irq_status function is used to clear some pwm interrupts, other pwm interrupts may be cleared by mistake.[c796ae51](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/c796ae51712642ba0c17c2846a30b1669d54843d)
  * (B91)pwm_clr_irq_mask: the function of this interface is to clear mask. this interface does not take effect in the old version and has been modified in the new version.[c796ae51](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/c796ae51712642ba0c17c2846a30b1669d54843d)
* **dma**
  * (B91)struct dma_chain_config_t variable member added volatile modified: the interface pwm_set_tx_dma_add_list_element/audio_rx_dma_chain_init/audio_tx_dma_chain_init chain configuration is affected by compilation optimization, may cause the pwm/audio module can not work properly(In the use of the chain function scenario).[c796ae51](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/c796ae51712642ba0c17c2846a30b1669d54843d)
* **swire**
  * (B91)swire_master_read:fixed an issue with older versions of the swire_master_read interface that could get stuck when the slave did not respond.[a44b6ca6](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/a44b6ca6429a58d863099af8199720079a014c88)
* **uart**
  * (B91)uart_reset：in no_dma, the old version needs to call uart_clr_tx_done before setting the uart interrupt to clear the tx_done interrupt state, otherwise the call of uart_reset will cause tx_done interrupt by mistake, the new version changes the interface of uart_reset. there is no need to call uart_clr_tx_done in no_dma.[cd80a1b7](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/cd80a1b79975bcbe1ff91660f1c230a7fa1544d9)
* **lpc**
  * (B91)solves the problem that the LPC POWER DOWN configuration is incorrect.[74c85d0d](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/74c85d0d8a9f95a6152fd9910d086402ed5d5693)
* **rf**
  * (B91)Turn on the continue mode to reduce the probability of missynchronization and solve the packet loss problem caused by missynchronization[6108aba3](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/6108aba3868a18322396e74c25106bb2d631d121)
  * (B91)Fix per floor issue.[6108aba3](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/6108aba3868a18322396e74c25106bb2d631d121)
  
### Features

* **PLIC_Demo**
  * (B91)add WFI(Wait-For-Interrupt Mode) demo
* **GPIO_Demo**
  * (B91)Added the function of triggering interrupts by using the buttons that come with the development board.
* **vendor/common**
  * (B91)add 4M Flash Calibration.
* **flash**
  * (B91)add 2M flash(P25Q16SU)/4M flash(P25Q32SH).
  * (B91)delete 4M flash (P25Q32SH) and add 4M wide press flash (P25Q32SU).
* **RF**
  * (B91)Added PTA related interfaces and demos.
* **swire**
  * (B91)swire_read_set_timeout_tick/swire_read_get_timeout_tick: the default value of 300,000 stimer ticks is set for abnormal read timeout in swire_master_read. can be configured according to the read timing.
* **dp_through_swire**   
  * (B91)Add "DP_THROUGH_SWIRE_DIS" setting to enable and disable "dp_through_swire" function in startup file. 
* **watchdog**
  * (B91)Add the interface for the watchdog to clear the reset state - wd_clear_status.
  * (B91)Add the interface for reading the reset status of the watchdog -wd_get_status.
* **version** 
  * (B91)Add version section for SDK name and version info.
* **uart**
  * (B91)uart_set_pin: the original version set the uart tx/rx function, will be set at the same time, but the application will have a separate use of uart rx or tx function, add enum can choose to separate rx or tx function.
* **uart(demo)**
  * (B91)Add uart and pm combined with the sample code.
* **emi**
  * (B91)Added relevant processing for frequency offset calibration of 4M flash.
* **bqb**
  * (B91)use the uart_send_byte() function replaces bqb_uart_send_Byte() function.
  * (B91)Added relevant processing for frequency offset calibration of 4M flash.
 * **DUT_Demo** 
  * (B91)Users can develop fixture test programs according to their own needs. 
  
### Refactoring

* **plic**
  * (B91)adjusted the location of the plic_isr.c file
* **ALL_Demo**
  * (B91)All demos use the led and key of the telink development board by default, and delete the useless definitions of SW1 and SW2.
* **BQB_Demo**
  * (B91)Modified the writing method of obtaining the size of the flash capacity, which is consistent with the description of the flash capacity in flash.h.
* **EMI_Demo**
  * (B91)Adjusted the interface read_calibration_flash for reading calibration parameters.
* **Vendor/common/calibration**
  * (B91)Adjust user_calib_adc_vref and user_calib_freq_offset to be externally callable interfaces.
* **Swire**
  * (B91)Removed Swire_Demo and Swire.h files.

### Performance Improvements

* N/A

### Note

* (B92) There is a problem with PB0,A1 does not work  
* (B92) watchdoge for 32K is turned on by default and can be turned off by calling wd_32k_stop()  
* (B92) Vbus power supply (not Vbat), there will be muc reset after 8s, USB demo call charger_clear_vbus_detect_status() can be turned off 

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">


### 版本

* SDK版本: telink_b91m_driver_sdk V2.1.0-Beta。
* 此版本SDK支持 B91(A0,A1,A2),B92(A1) 芯片。
* B92(新增) 支持 USB/AUDIO/I2C/s7816//UART/QDEC/PWM/Timer/Stimer/PLIC/GPIO/SPI/PKE/AES/ANALOG/TRNG
* LED和KEY的默认配置匹配以下硬件版本:
*   B91	 	C1T213A20_V1_3_20200727
*   B92	 	C1T266A20_V1_3_20220722

### BREAKING CHANGES

* **flash**
  * (B91)修改modify midxxxx_otp_block_e 和 midxxxx_lock_otp_e元素名,将单位K改为B,避免误导OTP的大小。
* **watchdog**
  * (B91)wd_clear接口只清看门狗计数，不清看门狗的复位状态。

### Bug Fixes

* **EMI_Demo**
  * (B91)修复了某些PA pin脚组合下，Carrier信号不能被PA模块放大的问题。[e01a08e2](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/e01a08e240e16476a87cb7bdf618a77c18a331a5)
* **PM**
  * (B91)解决了suspend唤醒后，flash取指异常导致的概率性的死机问题（某种软件逻辑下必现）。[c3e12945](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/c3e12945fa99b7a73d98a0af96360e330c9b9ac9)
  * (B91)解决了因关总中断时间不及时，小概率地导致的睡眠时间不对的问题。[e66aca1a](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/e66aca1a1c3fa87d80bba50828437bc767c16088)
* **pwm**
  * (B91)pwm_clr_irq_status:在使用函数pwm_clr_irq_status清除pwm某个中断状态时，有可能会误清除其他pwm中断。[c796ae51](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/c796ae51712642ba0c17c2846a30b1669d54843d)
  * (B91)pwm_clr_irq_mask: 该接口功能是清mask，旧版本调用该接口不能生效，新版本已修正。[c796ae51](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/c796ae51712642ba0c17c2846a30b1669d54843d)
* **dma**
  * (B91)结构体dma_chain_config_t变量成员添加volatile修改：由于编译优化影响接口pwm_set_tx_dma_add_list_element/audio_rx_dma_chain_init/audio_tx_dma_chain_init链表的配置，可能导致pwm/audio模块不能正常工作（在使用了链表功能的场景下）。[c796ae51](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/c796ae51712642ba0c17c2846a30b1669d54843d)
* **swire**
  * (B91)swire_master_read接口：修复了旧版本，当slave无响应时，可能会卡死在swire_master_read接口的问题。[a44b6ca6](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/a44b6ca6429a58d863099af8199720079a014c88)
* **uart**
  * (B91)uart_reset接口：no_dma下，旧版本需要在设置uart中断之前调用uart_clr_tx_done清除tx_done中断状态，否则调用uart_reset导致误触发tx_done中断，新版本对uart_reset接口进行修改，无需在no_dma下调用uart_clr_tx_done.[cd80a1b7](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/cd80a1b79975bcbe1ff91660f1c230a7fa1544d9)
* **lpc** 
  * (B91)解决了LPC POWER DOWN配置错误的问题。[74c85d0d](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/74c85d0d8a9f95a6152fd9910d086402ed5d5693)
* **rf**
  * (B91)使能continue mode降低误同步概率，解决误同步造成的丢包问题。[6108aba3](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/6108aba3868a18322396e74c25106bb2d631d121)
  * (B91)修复per floor问题。[6108aba3](http://192.168.48.36/src/driver/telink_b91m_driver_src/commit/6108aba3868a18322396e74c25106bb2d631d121)
  
### Features

* **PLIC_Demo**
  * (B91)添加了WFI(Wait-For-Interrupt Mode) demo
* **GPIO_Demo**
  * (B91)新增使用开发板自带的按键来触发中断的功能。 
* **vendor/common**
  * (B91)添加4M flash calibration相关处理。
* **flash**
  * (B91)增加2M flash(P25Q16SU)/4M flash(P25Q32SH)。
  * (B91)删掉了4M flash(P25Q32SH)，添加了4M 宽压flash(P25Q32SU)。
* **RF**
  * (B91)增加PTA相关接口及demo。
* **swire**
  * (B91)swire_read_set_timeout_tick/swire_read_get_timeout_tick:在swire_master_read中的读异常超时时间默认设置300000个stimer tick值，改为可以根据读时序进行配置。
* **dp_through_swire**  
  * (B91)启动文件中新增"DP_THROUGH_SWIRE_DIS" 配置用于打开或关闭"dp_through_swire"功能。 
* **watchdog**
  * (B91)增加看门狗清复位状态的接口-wd_clear_status。
  * (B91)增加读看门狗复位状态的接口-wd_get_status。
* **version** 
  * (B91)新增version段用于存放SDK name和version信息。
* **uart**
  * (B91)uart_set_pin:原有的版本设置uart tx/rx功能时，会同时进行设置，但应用中会有单独使用uart的rx或者tx功能，新增枚举可以选择单独设置rx或者tx功能。
* **uart(demo)**
  * (B91)添加uart和pm结合示例代码。
* **emi**
  * (B91)增加了对4M flash 频偏校准的相关处理。
* **bqb**
  * (B91)使用uart_send_byte()函数代替了bqb_uart_send_byte()函数。
  * (B91)增加了对4M flash 频偏校准的相关处理。
* **DUT_Demo** 
  * (B91)用户可根据自身需要开发夹具测试程序。
  
### Refactoring

* **plic**
  * (B91)调整了plic_isr.c文件的位置。
* **ALL_Demo**
  * (B91)所有的demo默认使用telink开发板的led和key，同时删掉了无用的SW1、SW2的定义。
* **BQB_Demo**
  * (B91)修改了获取flash容量大小的写法，与flash.h中关于flash容量的描述保持一致。
* **EMI_Demo**
  * (B91)调整了读校准参数的接口read_calibration_flash。
* **Vendor/common/calibration**
  * (B91)调整user_calib_adc_vref和user_calib_freq_offset为可被外部调用的接口。
* **Swire**
  * (B91)删除了Swire_Demo和Swire.h文件。

### Performance Improvements

* N/A

### Note

* (B92) PB0有问题,A1不能用
* (B92) 32K的watchdoge默认打开，调用wd_32k_stop()可以将其关掉
* (B92) Vbus供电(不是Vbat),8s后会有muc reset ，USB demo中调用charger_clear_vbus_detect_status()可以关掉

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">


## V2.0.0

### Version

* SDK version: telink_b91m_driver_sdk V2.0.0.
* This version of the SDK supports B91 A0, A1 and A2 chips.

### BREAKING CHANGES

* **project**
    * Modified the folder structure, using virtual folders in the project and linking to local folders through relative paths.
* **pke**
    * B91 adds the cases where the curves are 160, 192 and 224.
    * pke_get_irq_status, pke_clr_irq_status, pke_set_irq_mask, pke_clr_irq_mask, pke_set_microcode, pke_set_exe_cfg, pke_opr_start, pke_check_rt_code, pke_set_operand_width_pre, pke_set_operand_width_pre_mont, pke_calc_pre_width and pke_mont_calc_pre
* **swire**
    * A new parameter is added to the swire_sync interface and swire_set_swm_en interface to set swm gpio.
* **uart**
    * The uart_set_dma_rx_timeout interface is changed to uart_set_rx_timeout.
* **dma**
    * dma_mask is turned on by default, and it is cleared in sys_init, and there is no need to clear it again in the upper application.
* **i2c**
    * Before the i2c_master_write_read function, there is no stop between sending data and receiving data by default. After modification, you can choose whether there is a stop by using the interface i2c_master_send_stop.
* **pwm**
    * Before pwm_start can only open one at the same time by default, redefine the enumeration type, after modification, you can open multiple at the same time.
* **flash**
    * In order to be compatible with different Flash models, the Flash driver structure has been adjusted, and some Flash interface calling methods have been modified.
    * In order to be compatible with different types of Flash, flash_read_status and flash_write_status are not provided for external use. If you need to use the corresponding interface, you need to select the interface under the flash directory according to the mid (for example: flash_write_status_midxxxxxx).
    * The first parameter flash_mid of the interface int flash_read_mid_uid_with_check( unsigned int *flash_mid ,unsigned char *flash_uid) requires 4 bytes of space, and the second parameter requires 16 bytes of space.
    * Interface flash_lock, flash_unlock, flash_read_otp, flash_write_otp, flash_erase_otp, flash_lock_otp are replaced by flash_lock_midxxxxxx, flash_unlock_midxxxxxx, flash_read_otp_midxxxxxx, flash_write_otp_midxxxxxx, flash_miderase_otxxxxxx, flash_miderase_otxxx, xxx
    * Delete unused function interfaces: flash_erase_page, flash_erase_32kblock, flash_erase_64kblock, flash_erase_chip, flash_deep_powerdown, flash_release_deep_powerdown.
* **s7816**
    * Modify the meaning of the s7816_set_clk parameter, and change the parameter passed in to represent the frequency division number.
* **rf**
    * The previous function interface rf_set_tx_settle_time, rf_set_rx_settle_time set the time value to the parameter + 1, now the set value is consistent with the parameter.
* **pm**
    * pm_set_suspend_power() add a argument to select power down or power on usb/npe/baseband module before suspend.if power down,th sleep current will decrease,but need to re-init the module after wake up.three module all power down default.

### Bug Fixes

* **freertos**
    * Fixed the problem that the system would crash after being interrupted by a task while reading and writing the flash. Because const variables also access flash.
* **swire**
    * Modify the driving time of the swire_sync function interface.
    * Modify the swire_set_slave_id interface to prevent writing 0 first and then assigning the value when configuring the register. When the configuration is 0, abnormal signals may be generated and some risks may be generated.
* **flash**
    * Solve the problem that the unlock interface of flash will reset the four-wire configuration.
* **aes**
    * Modify the pointer of the aes data address to the offset address based on I-RAM minus the offset address of embase to I-RAM.
* **clock**
    * Modify the value of the corresponding position in the sys_pll_clk_e enumeration to the correct value of 60.
* **audio**
    * In mono mode, fixed the problem that the analog input (amic or line-in) does not work when switching channels.
    * When the channel is switched, the adc channel (adc1, adc2) of the corresponding codec also needs to be switched.
* **rf**
    * Fix the definition error of FLD_RF_IRQ_ALL in the enumeration of rf interrupt in B91.
    * Fixed the problem that the function rf_set_access_code_len only takes effect when the access code length is set to 4.
    * Fixed the function rf_set_rffe_pin that may cause the set pin to be in an unstable state during the setting process.
* **uart**
    * Fixed the problem caused by shared variables when UART0 and UART1 are working at the same time.
    * Modify the uart_set_pin interface and adjust the configuration sequence of gpio multiplexing function to avoid the generation of a signal from high to low to high when the uart pin multiplexing function is configured by the old version of the code.
    * Fixed the problem of incorrect calculation of dma receiving length in uart dma mode.
    * Modify the uart_reset interface to solve the problem that when uart is reset, the tx_done mask has been opened, and the tx_done interrupt will continue to enter.
    * Modify the uart_set_rx_timeout/uart_init interface to prevent writing 0 first and then assigning values ​​when configuring certain registers. When the configuration is 0, abnormal signals may be generated and some risks may be generated.
* **i2c**
    * Modify the internal implementation mechanism of the i2c_slave_read function, add the fifo software pointer, record the current fifo position, and avoid the slave always fetching data from the first address of the fifo when reading data, which may cause the software pointer and the hardware pointer to not correspond.
    * Modify the i2c_set_pin interface and adjust the configuration sequence of gpio multiplexing functions to avoid the generation of a signal from high to low to high when the i2c pin multiplexing function is configured by the old version of the code.
    * Modify the implementation method of the i2c_rx_irq_trig_cnt interface to avoid writing 0 first and then assigning the value when configuring the register, because when the configuration is 0, an error interrupt will be generated.
* **gpio**
    * Solved the problem of false interruption caused by incorrect configuration sequence when the rising edge triggers the interrupt when combined with the pull-up resistor.
    * After configuring IO as peripheral functions (such as I2C / SPI, etc.),then call the gpio_shutdown function to make the GPIO enter the shutdown mode. At this time, the function does not work.
* **spi**
    * Fixed the problem that the mixed use of SPI DMA mode and non-DMA mode will cause abnormal communication in DMA mode.
    * Fixed the problem that the SPI slave was powered on first and the CSN was floating, which caused an end interrupt.
    * The operation of clearing 0 and then writing the corresponding value is modified to a write to avoid possible abnormalities caused by the state when the register is written to 0.
    * The hspi_set_pin_mux() interface and pspi_set_pin_mux() interface modified part of the code sequence.
* **S7816**
    * Modify the s7816_set_clk interface to prevent writing 0 first and then assigning the value when configuring the register. When the configuration is 0, abnormal signals may be generated and some risks may be generated.
* **charger**
    * Fixed the problem that the charger repeatedly switched on and off during the USB charging process.
* **stimer**
    * Solve the problem that the stimer_disable interface cannot stop the stimer because the auto/manual mode is not distinguished.
* **dma**
    * The operation of clearing the reg_dma_ctrl register to 0 and then writing the target value is modified as a write to avoid abnormalities caused by writing 0 to the register.
    * Fixed the problem that any dma channel cannot be written to the dma register again when the transmission has not finished. Need to disable dma before performing the corresponding operation.
* **pm**
    * Fixed the problem that the program freezes during the suspend wakeup process when cclk is configured to 96M and pclk is configured to cclk's quarter-frequency 24M.
    * In DCDC_1P4_DCDC_1P8 mode, set the correct voltage level to avoid the risk of low voltage level of 2.8V which was used in the previous version. ( the voltage of some chips may be lower than the 2.5V required by the codec, which will affect the performance of the codec)
* **pwm(demo)**
    * In app_pwm_ir_dma.c, in pwm_ir_fifo_dma mode, the array subscript set 0 for each interrupt,otherwise, multiple interrupts will result in buff out-of-bounds access.
 * **plic** 
    * fix the problem that the plic_set_threshold() may not take effect.  
* **spi**
	* Solved the problem that sometimes compilation optimization will store the table of switch case statement in rodata segment. Encoding options add -fno-jump-tables.
* **sws_usb**
	* fix the issue that dp dm use as GPIO may cause unexpected error.firmware will disable swire through usb function,and you need to call usb_set_pin_en() function to enable it.

### Refactoring

* **spi**
    * Move spi_tx_dma_en and spi_rx_dma_en operations from demo to interface.
* **plic**
    * To facilitate application, move all interrupt handling functions to vendor/common/plic_isr.c.
* **gpio**
    * Turn on the jtag function in the gpio_init() function.
* **project**
    * The suffix of Debug file is uniformly revised to elf.
* **rf**
    * Normalizes the private schema keyword name.
    * Modify the internal implementation of rf_set_rx_timeout.
    * Separate the rx settle time setting from the rf_start_srx function and use it as the rf_set_rx_settle_time function.
* **audio**
    * Refactored audio_set_i2s_clock() function.
* **dma**
    * Refactored the function of setting DMA source address and destination address to make it compatible with addresses other than sram.
* **emi**
    * Delete mode3 in continue mode, which is debug mode. Does not affect the use, the function has been verified to be normal.
* **pke**
    * Adjust the structure of pke_demo (package the algorithm calculation part into a library), and adjust the structure of the files to be sealed into a library.
* **adc**
    * Modify the interface adc_set_ref_voltage and delete the repeated operation of setting the default reference voltage.
* **plic**
    * adjust core_interrupt_disable and core_restore_interrupt interfaces.
* **pwm(demo)**
    * In app_pwm_ir_dma.c, in pwm_chain_dma mode, modify the source addresses pointed to by nodes 1 and 2 so that the output waveform starts from buff0 and then buff1 alternately outputs.
* **license**
    * update license of related files from TELINK_BSD to TELINK_APACHE


### Features

* **emi**
    * Add test 0xaa and 0xf0 two data packets in tx burst mode, and control these two modes through bdt.
* **audio**
    * Added 192K sampling rate.
* **rf**
    * Added setting rf tx/rx mi modulation interface: rf_set_tx_modulation_index and rf_set_rx_modulation_index.
* **plic_demo**
    * Add the hardware stack protection (HSP) demo in the PLIC demo.
* **spi**
    * Add interfaces pspi_csn_pin_def_e pspi_change_csn_pin() and hspi_csn_pin_def_e hspi_change_csn_pin() for switching hardware CSN in multi-slave mode.
    * Add interface spi_master_write_read_full_duplex(), which is used for SPI full-duplex communication in MCU data transmission mode.
    * Add interface spi_master_read() for master read-only data.
    * Add macro definition SPI_SLAVE_READY in Demo. 1 means the slave_ready bit is used. The master will first ask if the slave is ready, and the slave will set this bit to 1 during initialization.
    * Add the spi_master_get_slave_ready() interface in the Demo, and the master sends instructions to read the ready status of the slave.
* **flash**
    * Add a four-wire read interface, a two-wire read interface, and a four-wire write interface.
* **adc**
    * Add the interface adc_get_gpio_calib_vref to obtain the reference voltage for single-point calibration.
    * Increase the value ADC_SAMPLE_FREQ_192K of the enumeration variable adc_sample_freq_e to select the ADC sampling rate of 192Khz.
    * Add interface adc_set_gpio_two_point_calib_offset to set the offset of two-point calibration.
* **vendor/common**
    * Added interface user_read_flash_value_calib for chip calibration (including ADC calibration and RF frequency offset calibration).
* **EMI_BQB_Demo**
    * Added the macro CLOSE_INTERNAL_CAP_EN to choose whether to close the internal capacitor.
    * Removed some unnecessary codes: Code for writing frequency offset value in EMI initialization;Temporary PA setting Code.
* **spi(demo)**
    * Added macro definition SPI_SLAVE_READY_TEST to test slave_ready function.

### Performance Improvements
* **IDE**
     * Add the compile command make -j${NUMBER_OF_PROCESSORS} to speed up the compilation.
* **plic**
     * Add the always_inline modification of the function plic_isr to reduce the size of the compiled bin file.
<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">


### 版本

* SDK版本: telink_b91m_driver_sdk V2.0.0。
* 此版本SDK支持B91 A0、A1和A2芯片。

### BREAKING CHANGES

* **project**
    * 修改了文件夹结构，工程中采用虚拟文件夹，并通过相对路径链接到本地文件夹。
* **pke**
    * B91增加了曲线为160，192和224的情况。
    * pke_get_irq_status、pke_clr_irq_status、pke_set_irq_mask、pke_clr_irq_mask、pke_set_microcode、pke_set_exe_cfg、pke_opr_start、pke_check_rt_code、pke_set_operand_width和pke_calc_pre_mont、pke_pre_calc_mont等接口删除。
* **swire**
    * swire_sync接口、swire_set_swm_en接口新添加一个参数，设置swm gpio。
* **uart**
    * uart_set_dma_rx_timeout接口改为uart_set_rx_timeout。
* **dma**
    * dma_mask默认是打开的，在sys_init中统一进行了清除，在上层的应用中不需要再次清除。
* **i2c**
    * i2c_master_write_read函数之前默认发送数据与接收数据中间是没有stop，修改之后，通过使用接口i2c_master_send_stop，可以选择是否有stop。
* **pwm**
    * pwm_start之前默认只能同时打开一个，重新定义枚举类型，修改之后，可以同时打开多个。
* **flash**
    * 为兼容不同的Flash型号，Flash驱动结构做了调整，修改了部分Flash接口调用方式。
    * 为兼容不同型号的Flash,flash_read_status、flash_write_status不提供给外部使用，需要使用对应接口时，需要根据mid去选择flash目录下的接口(例如：flash_write_status_midxxxxxx)。
    * 接口int flash_read_mid_uid_with_check( unsigned int *flash_mid ,unsigned char *flash_uid)的第一个参数flash_mid需要4个字节空间，第二个参数需要16byte空间。
    * 接口flash_lock、flash_unlock、flash_read_otp、flash_write_otp、flash_erase_otp、flash_lock_otp由flash_lock_midxxxxxx、flash_unlock_midxxxxxx、flash_read_otp_midxxxxxx、flash_write_otp_midxxxxxx、flash_erase_otp_midxxxxxx、flash_lock_otp_midxxxxxx替代。
    * 删除不用函数接口：flash_erase_page、flash_erase_32kblock、flash_erase_64kblock、flash_erase_chip、flash_deep_powerdown、flash_release_deep_powerdown。
* **s7816**
    * 修改s7816_set_clk参数含义，改为传入的参数就代表分频数。
* **rf**
    * 之前函数接口rf_set_tx_settle_time，rf_set_rx_settle_time设置时间值为参数 + 1，现在改为设置值与参数保持一致。
* **pm**
    * pm_set_suspend_power()接口添加一个参数用于选择进入suspend前是power down还是power on usb/npe/baseband。关闭模块时睡眠电流会减小，但是唤醒后需要重新初始化对应模块。默认全关。

### Bug Fixes

* **freertos**
    * 修复了在读写flash，被任务中断后会死机的问题。因为const 变量也访问 flash。
* **swire**
    * 修改swire_sync函数接口的驱动时间。
    * 修改swire_set_slave_id接口，防止配置寄存器时先写0，后赋值，由于配置为0时，可能产生异常信号导致一些风险。
* **flash**
    * 解决了flash的解锁接口会把四线配置复位的问题。
* **aes**
    * 修改aes数据地址的指针为基于I-RAM的偏移地址减去embase对I-RAM的偏移地址。
* **clock**
    * 修改sys_pll_clk_e枚举中对应位置的值为正确的60。
* **audio**
    * mono模式下，修复了模拟输入（amic或line-in）切换通道时不起作用问题。
	* 通道切换时，对应的codec的adc通道（adc1,adc2）也需作切换。
* **rf**
    * 修复B91中rf中断的枚举中关于FLD_RF_IRQ_ALL的定义错误。
    * 修复了函数rf_set_access_code_len只有在设置access code长度为4的时候才生效的问题。
    * 修复了函数rf_set_rffe_pin可能会导致设置的pin脚在设置过程中处于不定态。
* **uart**
    * 修复了UART0和UART1同时工作时，由于共享变量造成的问题。
    * 修改uart_set_pin接口，调整gpio复用功能配置顺序,避免旧版本代码写法配置uart pin脚复用功能时，产生一个由高到低再到高的信号。
    * 修复了uart dma模式下，计算dma接收长度错误的问题。
    * 修改uart_reset接口，解决当reset uart时，tx_done mask已经打开，会一直进入tx_done中断的问题。
    * 修改uart_set_rx_timeout/uart_init接口，防止在配置某些寄存器时先写0，后赋值，由于配置为0时，可能产生异常信号导致一些风险。
* **i2c**
    * 修改i2c_slave_read函数的内部实现机制，添加fifo软件指针，记录当前fifo的位置，避免slave读取数据时，总是从fifo的首地址取数据，可能导致软件指针与硬件指针没有对应。
    * 修改i2c_set_pin接口，调整gpio复用功能配置顺序,避免旧版本代码写法配置i2c pin脚复用功能时，产生一个由高到低再到高的信号。
    * 修改i2c_rx_irq_trig_cnt接口的实现方法，避免配置寄存器时先写0，后赋值，由于配置为0时，产生一个误中断。
* **gpio**
    * 解决了当配合上拉电阻，上升沿触发中断，由于配置顺序不对，导致误中断的问题。
    * 将IO配置为外设功能（比如I2c/SPI等）后，再调用gpio_shutdown函数让该GPIO进入shutdown模式，此时该函数不起作用。
* **spi**
    * 修复了SPI DMA模式和非DMA模式混用会造成DMA模式通信异常的问题。
    * 修复了SPI slave先上电，CSN浮空导致产生end中断的问题。
    * 先清0再写相应值的操作，修改为一次写入，避免寄存器写0时状态可能造成异常。
    * hspi_set_pin_mux()接口和pspi_set_pin_mux()接口修改了部分代码顺序。
* **S7816**
    * 修改s7816_set_clk接口，防止配置寄存器时先写0，后赋值，由于配置为0时，可能产生异常信号导致一些风险。
* **charger**
    * 修复了USB在充电过程中，charger反复开关的问题。
* **stimer**
    * 解决因为没有区分auto/manual模式而导致stimer_disable接口不能停止stimer的问题。
* **dma**
    * reg_dma_ctrl寄存器先清0再写目标值的操作，修改成一次写入，避免寄存器写0造成异常。
    * 修复了任一dma通道在传输还未结束时，不能再次写入dma寄存器的问题，需要先disable dma再进行相应的操作。
* **pm**
    * 修复了cclk配置为96M，pclk配置为cclk的四分频24M时，程序在suspend唤醒过程中，程序卡死的问题。
    * DCDC_1P4_DCDC_1P8模式下，设置正确的电压档位，规避之前版本使用电压低档位，可能造成2.8V电压偏低的风险。（某些芯片该电压可能会低于codec要求的2.5V，会影响codec性能）
* **pwm(demo)**
    * 在app_pwm_ir_dma.c中，模式PWM_IR_FIFO_DMA下，每次进中断需将数组下标归0，否则多次中断，导致buff越界访问;
* **plic** 
  * 修复 plic_set_threshold() 可能不生效的问题。
* **spi** 
  * 解决了有时编译优化会将switch case语句的表存放在rodata段的问题。编码选项添加-fno-jump-tables。
* **sws_usb** 
  * 解决当dp dm做GPIO使用时可能会导致异常错误的问题。firmware默认会disable swire through usb功能，当需要使用该功能时需要调用usb_set_pin_en接口。

### Refactoring
* **spi**
    * 将spi_tx_dma_en和spi_rx_dma_en操作从demo中移到了接口中。
* **plic**
    * 为了方便应用，将所有的中断处理函数移动到vendor/common/plic_isr.c。
* **gpio**
    * 在gpio_init()函数中打开jtag功能。
* **project**
    * Debug文件后缀统一修改为elf。
* **rf**
    * 规范化private模式关键字名称。
    * 修改rf_set_rx_timeout内部实现方式。
    * 将rx settle时间设置从rf_start_srx函数中分离出来作为 rf_set_rx_settle_time函数使用。
* **audio**
    * 重构audio_set_i2s_clock()函数。
* **dma**
    * 重构了设置dma源地址、目的地址函数，使其可以兼容除sram以外的其他地址。
* **emi**
    * 删除continue模式下的mode3，该模式为debug模式。不影响使用，已验证功能正常。
* **pke**
    * 将pke_demo结构进行调整（算法运算部分封装成库），对要封成库的文件进行结构调整。
* **adc**
    * 修改接口adc_set_ref_voltage，删除设置默认参考电压的重复操作。
* **plic**
    * 调整core_interrupt_disable和core_restore_interrupt接口。
* **pwm(demo)**
    * 在app_pwm_ir_dma.c中，模式PWM_CHAIN_DMA下，修改节点1和节点2指向的源地址，使得输出的波形从buff0开始再到buff1交替输出;
* **license**
    * 将相关文件的许可证从 TELINK_BSD 更新为 TELINK_APACHE


### Features
* **emi**
    * tx burst模式下添加测试0xaa和0xf0两种数据包，通过bdt控制这两种模式。
* **audio**
    * 增加了192K采样率。
* **rf**
    * 新增设置rf tx/rx mi调制接口：rf_set_tx_modulation_index和rf_set_rx_modulation_index。
* **plic_demo**
    * 在PLIC demo里添加硬件栈保护（HSP）demo。
* **spi**
    * 添加接口pspi_csn_pin_def_e pspi_change_csn_pin()和hspi_csn_pin_def_e hspi_change_csn_pin()，用于多slave模式下切换硬件CSN。
    * 添加接口spi_master_write_read_full_duplex(),用于MCU传输数据方式的SPI全双工通信。
    * 添加接口spi_master_read(),用于master只读数据。
    * 在Demo中添加宏定义SPI_SLAVE_READY。1表示使用slave_ready位，master会先询问slave是否准备好，slave会在初始化时将该位设置为1。
    * 在Demo中添加spi_master_get_slave_ready()接口，master发送指令读取slave的ready状态。
* **flash**
    * 增加四线读接口，增加两线读接口，增加四线写接口。
* **adc**
    * 增加接口adc_get_gpio_calib_vref，获取单点校准的参考电压。
    * 增加枚举变量adc_sample_freq_e的值ADC_SAMPLE_FREQ_192K，用于选择ADC采样率192Khz。
    * 增加接口adc_set_gpio_two_point_calib_offset，设置两点校准的offset。
* **vendor/common**
    * 增加接口user_read_flash_value_calib，用于芯片校准(包括ADC校准和RF频偏校准)。
* **EMI_BQB_Demo**
    * 增加了宏CLOSE_INTERNAL_CAP_EN，用来选择是否关闭内部电容。
    * 去除了一些不必要的代码：EMI初始化中写频偏值的代码；用于PA设置的临时代码。
* **spi(demo)**
    * 增加宏定义SPI_SLAVE_READY_TEST，用于测试slave_ready功能。
	
### Performance Improvements
* **IDE**
    * 添加编译命令make -j${NUMBER_OF_PROCESSORS}，增快编译速度。
* **plic**
    * 添加函数plic_isr的always_inline修饰，减少编译的bin文件大小。
	
---
## V1.5.0-Beta

### Version

* SDK version : telink_b91_driver_sdk v1.5.0-Beta.
* This version sdk support B91 A0，A1 and A2 chip.

### Bug Fixes

* **usb**
  * Fix the problem that usb_spk could not work when only USB_SPEAKER_ENABLE is turned on in USB_SPK Demo.
* **audio**
  * Correct spelling errors in "audio_buff" in AUDIO Demo.
  * Correct the data type definition error in the app_sin_data.h file in AUDIO_Demo.
* **spi**
  * Interface hspi_change_csn_pin()/pspi_change_csn_pin() have logic error, would configure the unused csn pin, the error has been fixed.And hspi_change_csn_pin()/pspi_change_csn_pin() has been deleted from the driver and moved to SPI_Demo.
  * Interface spi_slave_set_pin() have a pin configuration bug, the bug has been fixed.
  * Fixed the comment error of the hspi address length of the hspi_set_addr_len function.
* **rf**
  * Fixed the following problem: When using the function rf_clr_irq_status () to clear a certain rf interrupt status, other rf interrupts may  be cleared by mistake.
* **uart** 
  * add parameter of uart_num in function "uart_get_dma_rev_data_len() to distinguish UART0 and UART1".

### Features

* **freertos**
  * add freertos Demo

### Known issues

* N/A

### BREAKING CHANGES

* **flash**
  * In order to be compatible with other sdk, the flash_read_mid function used to pass in a buf pointer, and instead returns an unsigned int variable.
  * Mid is changed from 3byte to 4byte, and the highest byte is used to distinguish flashes with the same mid but different functions.

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">


### 版本

* SDK版本: telink_b91_driver_sdk v1.5.0-Beta。
* 此版本SDK支持B91 A0、A1和A2芯片。

### Bug Fixes

* **usb**
  * 修复在USB_SPK Demo中仅打开USB_SPEAKER_ENABLE时，usb_spk无法工作的问题。
* **audio**
  * 更正在AUDIO_DEMO中“audio_buff”中的拼写错误。
  * 更正在AUDIO_DEMO中app_sin_data.h文件中数据类型定义不符的错误。
* **spi**
  * Hspi_change_csn_pin()/Pspi_change_csn_pin() 接口有逻辑错误，会配置没有用到的csn引脚，该问题已经修复。并且Hspi_change_csn_pin()/Pspi_change_csn_pin()接口已经从Drvier中删除并移到了SPI_Demo中。
  * spi_slave_set_pin()接口引脚配置有错误，该问题已经修复。
  * 修复了hspi_set_addr_len函数的hspi地址长度的注释错误。
* **rf**
  * 修复了如下问题：在使用函数rf_clr_irq_status ()清除rf某个中断状态时，有可能会误清除其他rf中断。
* **uart**
  * 函数uart_get_dma_rev_data_len()增加参数uart_num区分UART0和UART1。

### Features

* **freertos**
  * 增加 freertos Demo

### Known issues

* N/A

### BREAKING CHANGES

* **flash**
  * 为了兼容其他sdk，flash_read_mid函数以前是传入一个buf指针，改为返回unsigned int型变量。
  * mid由3byte改为4byte,最高byte用于区分mid相同但是功能存在差异的flash。

---
## V1.4.0-Beta

### Version
* SDK version : telink_b91_driver_sdk v1.4.0-Beta.
* This version sdk support B91 A0，A1 and A2 chip.

### Bug Fixes

* **pm**
  * solve the problem that pragrem can not run when VBAT power supply is greater than 4.1V,and sys_int configure power mode as DCDC mode.
  * During the stability test, the program freezes after running for a period of time.
* **spi** 
  * spi enum SPI_MODE1、SPI_MODE2 definition as error value, fixed as correct value.
* **uart**
  * when close the irq in dma mode,the receive buffer maybe overflow if dma receive data continuous.

### Features

* **pwm**
  * add function interface "pwm_32k_chn_dis".
* **audio** 
  * Add the interface for selecting DAC output channel (audio_set_output_chn).
* **usb**
  * Add software to handle set_configuration control transmission.
  * Support usb cdc to send wMaxPacketSize (64bytes) length data.
* **uart** 
  * Add function uart_get_dma_rev_data_len() in DMA mode.
    

### Refactoring

* **spi** 
  * adjusted the initial process of LED in SPI_Demo.
* **audio**
  * Adjust the calls of i2c (control codec) and pwm (provide external clock) interfaces in audio applications.
* **usb/audio**
  * set the sampling rate of the DAC to 16k by default,In the mic_spk demo.

### Known issues

* N/A
### BREAKING CHANGES
* **audio**
  * Modify the audio_i2s_config function parameters, add audio_i2s_invert_config structure parameters to select i2s_lr_clk_invert and i2s_data_invert.

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">

### 版本

* SDK版本: telink_b91_driver_sdk v1.4.0-Beta。
* 此版本SDK支持B91 A0、A1和A2芯片。

### Bug Fixes

* **pm**
  * 解决VBAT供电高于4.1V且sys_int配置供电模式为DCDC时程序不能运行的问题。
  * 稳定性测试时，运行一段时间程序卡死的问题。
* **spi**
  * spi 的SPI_MODE1、SPI_MODE2枚举值定义错了，修复为了正确值。
* **uart**
  * DMA模式下关闭总中断时，如果UART仍在接收数据，有可能导致接收数组溢出。

### Features

* **pwm**
  * 添加函数接口pwm_32k_chn_dis。
* **audio**
  * 增加invert I2S CLK接口（audio_invert_i2s_lr_clk）。
* **usb**
  * 添加软件处理set_configuration控制传输。
  * 支持USB CDC发送wMaxPacketSize（64bytes）长度数据。
* **uart**
  * DMA模式增加函数uart_get_dma_rev_data_len()。


### Refactoring

* **spi** 
  * 调整了SPI_Demo中LED的初始化过程。
* **audio**
  * 调整AUDIO应用中i2c（控制codec）和pwm(提供外部时钟)接口的调用。
* **usb/audio**
  * 在mic_spk demo中将DAC的采样率默认设为16k。

### Known issues

* N/A

### BREAKING CHANGES

* **audio**
  * 修改audio_i2s_config function函数参数，新增audio_i2s_invert_config 结构体形参，设置i2s_lr_clk_invert 和i2s_data_invert。


---
## V1.3.1-Beta

### Version
* SDK version : telink_b91_driver_sdk V1.3.1-Beta.
* This version sdk support B91 A0，A1 and A2 chip.

### Bug Fixes
* **pm**:

 * Use digital register way to get 32k tick may read error tick,cause the wakeup time is incorrect with the setting time,the sleep time will very little or very big,will not wakeup ontime.

  + The previous configuration will cause the voltage on the VDD1V8 pin to be generally high during normal operation, about 3.0V~3.1V (the reason is that the previous configuration will cause the two power supplies to be powered at the same time), which may cause abnormal phenomena. The version is modified to the correct configuration. In the default configuration, the voltage on the VDD1V8 pin will be about 2.8V.

* **USB**:
  + The header file "stdDescriptors.h" is not case sensitive when included, so it is uniformly changed to "StdDescriptors.h"

* **AUDIO**:
      + In the function audio_set_dmic_pin(), fix the problem of possible errors in setting the dmic pin

### Known issues
* N/A
### BREAKING CHANGES
* **pwm**
   + Detailed description: add 32k clock calibration,the interface function(pwm_32k_chn_en) changes" =" to |=，to prevent reset other channels.

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">

### 版本

* SDK版本: telink_b91_driver_sdk V1.3.1-Beta。
* 此版本SDK支持B91 A0、A1和A2芯片。

### Bug Fixes

* **pm**:
  * 使用数字方式获取32k tick出错会导致设置的唤醒tick与预期不符，从而导致唤醒时间与设置的不相符，睡眠时间可能过小或者过大，可能会表现为不唤醒。
 + 之前的配置会导致正常工作时，VDD1V8引脚上的电压会普遍偏高，大概为3.0V~3.1V（原因是之前的配置会导致两个电源会同时供电），可能会造成异常现象，此版本修改为正确配置，默认配置下，VDD1V8引脚上的电压会是2.8V左右。

* **USB**:
    + 头文件"stdDescriptors.h",include时没有区分大小写，统一改成"StdDescriptors.h"


* **AUDIO**:
      + 在函数audio_set_dmic_pin()中，修复设置dmic pin可能出错问题

### Known issues
* N/A

### BREAKING CHANGES
* **pwm**
 + 添加32k时钟源校准，接口函数pwm_32k_chn_en修改，=改为|=，防止重置其他通道.

---

### Version



## V1.3.0-Beta

### Version
* SDK version : telink_b91_driver_sdk V1.3.0-Beta.
* This version sdk support B91 A0，A1 and A2 chip.

### Bug Fixes

* **pm**:
  * Solved the problem that the reboot state cannot be read after the reboot comes back, and the deep state is read only. The solution is to add a status "MCU_STATUS_REBOOT_DEEP_BACK" to the enumeration type "pm_mcu_status".
  * Solved the problem that the timer could not wake up after waking up several times when the function clock_cal_32k_rc() was not called.
  * Solved the problem that when the flash is configured with four wires, it cannot work normally after waking up from suspend mode.
* **clock**: 
  * Solved the problem of invalid kick with PD0 port when 32k xtal starts to vibrate.
* **adc**: 
  * Fix the sort bug of function adc_sort_and_get_average_code() in ADC_Demo.
  * Fix the problem that the adc_get_temperature()/adc_get_voltage() function in ADC_Demo may not get data for the first time.

### Features

* **swire**
  * add swire driver and demo.
* **cproject**: 
  * delete B91 keyword.
* **adc**
  * Added the 1/4 pre_scale parameter enum of the adc_set_scale_factor() function, and deleted the 1, 1/2, 1/8 pre_scale parameter enum.
* **pm** 
  * change the variable name g_pm_multi_addr ->g_pm_mspi_cfg.
    
  * Improved the problem of incorrect LED configuration during the suspend sleep current test in pm_demo.
* **audio** 
  * add audio_set_codec_wnf api.
  * add input_mode_select at line_in.
  * set all gain to 0db default in audio.c.
* **flash** 
  * add flash_set_xip_config function.
* **usb** 
  * add host speaker stop playback detection function in spk demo.
* **pwm**
  * add function interface pwm_32k_chn_update_duty_cycle.


### Known issues
* N/A
### BREAKING CHANGES
* **s7816**
  * modify the s7816_set_time(),delete the atr wait time in the function of coldreset and warmreset.

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">

### 版本

* SDK版本: telink_b91_driver_sdk V1.3.0-Beta。
* 此版本SDK支持B91 A0、A1和A2芯片。

### Bug Fixes

* **pm**:
  * 解决了reboot回来后不能读到reboot状态，只读到deep状态的问题。解决方案是在枚举类型"pm_mcu_status"中新增一种状态"MCU_STATUS_REBOOT_DEEP_BACK"。 
  * 解决了不调用函数clock_cal_32k_rc()时，timer唤醒几次后不能再唤醒的问题。
  * 解决了设置flash四线时，suspend唤醒后flash不能正常工作的问题。
* **clock**:
  * 解决了32k xtal起振时用PD0口kick无效的问题。
* **adc**:
  * 修复了ADC_Demo中的adc_sort_and_get_average_code()函数的排序问题。
  * 修复了ADC_Demo中的adc_get_temperature()/adc_get_voltage()函数第一次可能会采不到数据的问题。

### Features

* **swire**
  * 增加swire的驱动和demo。
* **cproject**:
  * 删除关键字B91。
* **adc**
  * 添加了adc_set_scale_factor（）函数的1/4 pre_scale参数枚举，并删除了1、1 / 2、1 / 8 pre_scale参数枚举。


* **pm**

  * 更改变量名称g_pm_multi_addr-> g_pm_mspi_cfg。
  * 改进了在pm_demo中的suspend睡眠电流测试过程中，指示灯配置错误的问题。

* **audio** 
  * 添加接口audio_set_codec_wnf。
  * 在line_in添加input_mode_select。
  * 在audio.c中将所有增益设置为0db默认值。

* **flash** 

  * 添加函数flash_set_xip_config 。

* **usb** 

  * 在spk_demo中添加主机扬声器停止播放的检测功能。

* **pwm**

  * 添加功能接口pwm_32k_chn_update_duty_cycle。




### Known issues
* N/A

### BREAKING CHANGES
* **s7816**

  * 修改函数s7816_set_time()，在coldreset和warmreset函数中删除atr等待时间。


---
## V1.2.0-Beta

### Version
* SDK version : telink_b91_driver_sdk V1.2.0-Beta.
* This version sdk support B91 A0 and A1 chip.

### Bug Fixes

* **uart** 

  * Fix the problem that the configuration function uart\_receive_dma cannot continue to receive data after UART suspend or reset UART

### Features
* **sys**
  * Modify the member name of the parameter enumeration vbat\_type\_e variable in the sys\_init function
* **watchdog**
	* Simplify the parameters of the wd\_set\_interval\_ms function, and no longer retain the parameter tick\_per\_ms
* **timer**
   * In the timer\_set\_mode function, the initial tick and capture tick are no longer configured and are encapsulated into two interfaces (timer\_set\_init\_tick, timer\_set\_cap_tick)
* **gpio** 
  * Modify default macro definition in gpio_default.h 
  * The corresponding enable function gpio\_irq\_en,gpio\_gpio2risc0\_irq\_en,gpio\_gpio2risc1\_irq\_en is no longer called in the gpio\_set\_irq,gpio\_set\_gpio2risc0\_irq,gpio\_set\_gpio2risc1\_irq function, which is flexibly called in the application layer
* **plic**
  * Delete the parameter of the function plic\_interrupt\_claim
* **other**  
  * Optimize the include structure of the header file, add Include paths in the project configuration options

### Known issues
* **32k xtal** 
  - 32k xtal can't be used on the A1 version chip
* **gpio** 
  - GPIO_PD0 can't be used on the A1 version chip
* **other**
  - Note (A1 version chip): Use 2.8V voltage to power the chip. When the power supply voltage is higher than 2.8V, the program may not run.
### BREAKING CHANGES
* N/A

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">

### Bug Fixes

* **uart** 

  * 修复UART suspend起来或reset UART后，配置函数uart\_receive\_dma不能继续接收数据问题

### 版本
* SDK版本: telink_b91_driver_sdk v1.2.0-Beta.
* 此版本SDK支持B91 A0和A1芯片.
### Features
* **sys**
  * 在sys\_init函数中修改枚举vbat\_type\_e类型的成员名称。
* **watchdog**
	* 简化wd\_set\_interval_ms函数的参数,参数period\_ms为间隔时间，不再保留参数tick\_per\_ms
* **timer**
  * 在timer\_set\_mode函数中,不再配置初始tick和capture tick，分别封装成两个接口（timer\_set\_init_tick,timer\_set\_cap\_tick）
* **gpio** 
  * 修改gpio_default.h默认宏定义
  * 在gpio\_set\_irq,gpio\_set\_gpio2risc0_irq/gpio\_set\_gpio2risc1\_irq函数中不再调用对应使能函数gpio\_irq\_en,gpio\_gpio2risc0\_irq\_en,gpio\_gpio2risc1\_irq\_en,在应用层里灵活调用
* **plic**
  * 删去函数plic\_interrupt\_claim的参数
* **other**  
  * 优化头文件包含结构，在工程配置选项里添加Include paths
### Known issues
* **32k xtal** 
   - 32k xtal 在A1版本芯片上不能使用。
* **gpio** 
  - GPIO_PD0在A1版本芯片上不能使用
* **其他**
  - 注意事项（A1版本芯片）：使用2.8V电压给芯片供电，供电电压高于2.8V时，程序有可能会跑不起来。

### BREAKING CHANGES
* N/A

---
## V1.1.0-Beta
### Version

* SDK version : telink_b91_driver_sdk v1.1.0-Beta.
* This version sdk support B91 A0 and A1 chip.

### Bug Fixes

* **pm**

  * Solved the problem of not being able to wake up when the sleep time is less than 950us.

  * Solved the problem that the sleep function cannot be awakened when the sleep function is called immediately within 500us after power on.

* **mdec**: 

  * Solved the problem of first interruption after initialization.

* **clock**: 

  * Solve the problem that the program sometimes stops in the clock initialization function (clock_init).

* **lpc**: 

  * Solved the problem of abnormal operation of lpc.

* **flash**: 

  * Remove the declaration of the function flash_read_mid_uid_with_check_ram. Added declaration of function flash_read_mid_uid_with_check.

  * Solved the problem of not being able to cross pages when writing flash across pages.

* **bqb:** 

  * fix the issue that could not send packets through instructions in initialization state.

* **rf:** 

  * Fixed an abnormal function of rf_set_rffe_pin function.

  * Modify the function rf_set_rx_dma and rf_set_tx_dma to adapt to sending and receiving long packets(up to 255 byte)

  * Modified the usage example of rx　interrupt. 

  * fix the issue that rf_set_pipe_access_code can not set 5-byte access code.

  * Fix the register setting error in the rf_set_preamble_len function.

  * Modify the access code of S2 and S8 mode to manual trigger.

* **emi** 

  * Fix the problem of zigbee mode 2480Mhz band edge can not pass the spec.
  * Fixed single carrier frequency offset in emi.

* **.s** 

  * Modified cstartup_b91_flash.S and cstartup_b91_ram.S file to reserve space. 

* **reboot&clock**

  * fix the issue that reboot will crash when hclk is half of cclk in pll_clk and pad24M_clk,need add a 

* **analog** 

  * fix a compile error in analog demo

* **spi** 

  * Simplify structure and macro definition naming, optimize SPI slave demo structure，fix compilation error

* **uart** 

  * Fix the issue that the mask might be overwritten by the set of bit width when the uart-init.

### Features
* **sys**

  * Add parameters vbat_v in the sys_init function.

* **sys_time**

  * Change CLOCK_16M_SYS_TIMER_CLK_XXX to SYSTEM_TIMER_TICK_XX.

* **pm** 

  * Increase the convergence calibration function of sleep time.

  * Increase USB wake-up function.

  * Increase the suspend sleep function.
  * add long sleep wakeup api.

* **bqb** 

  * add a macro to switch new/old setting ways to configure uart/cap/flash.

* **license** 

  * change the author to "Driver Group" and modify the license template to meet the requiements CI checking.

* **rf** 

  * Add ANT mode to rf demo.

  * update the function rf_set_chn and rf_set_ble_chn to support out-of-band frequency point setting

  * In order to facilitate the development of SDK, the DMA setting function related to RF is divided into several functions with single function, and added to the driver.

* **AES** 

  * optimize AES_set_mode apiuart:add Single line transceiver function

* **uart**

  * add UART-TXDONE/RXDONE interrupt in DMA mode in the chip version of A1 .

  * data of variable length is acceptable in uart-dma mode in the chip version of A1, the length of receive-date can be work out.

* **s7816**

  * add s7816 demo, can communicate with the equipment that support IS07816 protocol.

* **gpio**

  * add 30k-digital pull-up。

* **plic**

  * Optimized interrupt handling function structure in PLIC

* **i2c**

  * add function that the master sends a stop after detecting a NACK in the ID phase，then stops reading and writing。

  * Add tx_done interrupt in dma demo

* **audio**

  * In MONO mode, the input channel (left/right) can be selected

  * Add input channel acquisition mode selection (single-ended/differential)

  * Add configuration for ADC and DAC to work at different sampling rates（ADC=16K，DAC=48K）

* **spi**

  * Add 3line mode and master to multiple-slave function

* **adc**

  * Add ADC module  in the chip version of A1 which support DMA sampling mode and manual sampling mode, support Vbat\GPIO\Temperature sensor sampling

### Known issues
* **32k xtal** 
  - 32k xtal can't be used on the A1 version chip
* **gpio** 
  - GPIO_PD0 can't be used on the A1 version chip
* **other**
  - Note (A1 version chip): Use 2.8V voltage to power the chip. When the power supply voltage is higher than 2.8V, the program may not run.
### BREAKING CHANGES
* N/A

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">

### Bug Fixes

* **pm**  

  * 解决了睡眠时间小于950us时，不能唤醒的问题。 

  * 解决了上电500us内立即调用睡眠函数时无法唤醒的问题。

* **mdec**  

  * 解决了初始化后先进一次中断的问题。

* **clock**  

  * 解决了程序有时会停在时钟初始化函数（clock_init）中的问题。

* **lpc**  

  * 解决了lpc工作异常的问题。

* **flash**  

  * 去掉函数flash_read_mid_uid_with_check_ram的声明。新增函数flash_read_mid_uid_with_check的声明。

  * 解决了跨页写flash时，不能跨页的问题。

* **bqb** 

  * 修复了在初始化状态下，不能通过指令发包的问题。

* **rf** 

  * 修复了函数rf_set_rffe_pin功能异常 。

  * 修改函数参数rf_set_rx_dma ,rf_set_tx_dma以适应长包（最大长度255byte）

  * 修改rx interrupt使用示例

  * 修复了rf_set_pipe_access_code不能设置5byte长度的access code

  * 修复了rf_set_preamble_len函数中的寄存器设置错误

  *  修改S2,S8模式access code为手动触发

* **emi** 

  * 修复zigbee 2480Mhz边带不能过spec的问题
  * 修复单通频偏的问题

* **.s** 

  * 更新s文件以预留空间

* **reboot&clock** 

  * 修复当hclk等于1/2cclk时reboot出现宕机的问题。

* **analog** 

  * 修复analog_demo中的错误

* **spi** 

  * 精简结构体和宏定义命名，优化SPI slave demo结构，修复编译错误

* **uart** 

  * 修复UART初始化设置位宽时可能会覆盖掉mask的问题

### 版本
* SDK版本: telink_b91_driver_sdk v1.0.0-Beta.
* 此版本SDK支持B91 A0和A1芯片.
### Features
* **sys**

  * 在sys_init函数中添加参数vbat_v。

* **sys_time**

  * 将CLOCK_16M_SYS_TIMER_CLK_XXX更改为SYSTEM_TIMER_TICK_XX。

* **pm**

  * 增加睡眠时间的收敛校准功能。

  * 增加USB唤醒功能。 

  * 增加suspend睡眠功能。    
  * 新增长睡眠接口。

* **bqb** 

  * 增加了一个宏，用来选择新的或旧的方式来配置串口、CAP值以及适配的Flash大小。

* **license** 

  * 将License的作者统一改成了“Driver Group”以及统一调整了License的模板。    

* **rf** 

  * 添加ANT模式到rf demo中.

  * 更新设置channel的函数rf_set_chn和rf_set_ble_chn以支持带外频点设置

  * 为了方便SDK的开发将DMA设置函数拆分成几个功能单一的函数并添加到驱动中。

* **AES** 

  * 优化AES_set_mode接口uart:新增单线收发模式

* **uart**

  * A1版本芯片新增DMA模式UART-TXDONE/RXDONE中断。

  * A1版本芯片UART-DMA模式可以接收不定长的数据，并能计算出接收数据的长度

* **s7816**

  * 新增s7816模块，可与支持ISO7816协议的设备进行单线半双工通信。

* **gpio**

  * 新增30k数字上拉。

* **plic**

  * 在PLIC中优化了中断处理函数结构

* **i2c**

  * 增加了master在发ID阶段检测到NACK后发stop，并停止读写动作

  * 在dma demo中增加了tx_done中断。

* **audio**

  * MONO模式下，可选择输入通道（左/右)，默认只有左通道输出；

  * 增加了输入通道采集模式选择（单端/差分）

  * 新增ADC和DAC在不同采样率工作的配置（ADC=16K，DAC=48K）。

* **spi**

  * 新增3line模式和master对多slave功能。

* **adc**

  * A1版本芯片新增ADC模块，支持DMA采样模式和手动采样模式，支持Vbat\GPIO\Temperature sensor采样


### Known issues
* **32k xtal** 
   - 32k xtal 在A1版本芯片上不能使用。
* **gpio** 
  - GPIO_PD0在A1版本芯片上不能使用
* **其他**
  - 注意事项（A1版本芯片）：使用2.8V电压给芯片供电，供电电压高于2.8V时，程序有可能会跑不起来。

### BREAKING CHANGES
* N/A

---
## V1.0.0-Beta
### Version
* SDK version : telink_b91_driver_sdk v1.0.0-Beta.
* This version sdk support B91 A0 chip.
### Features
* AES/AUDIO/FLASH/LPC/MDEC/PKE/PM/PWM/RF/STIMER/TIMER/TRNG
* Interface:GPIO/I2C/SPI/UART/USB
### Known issues
* **pm** 
  - The suspend mode can't be used on the A0 version chip
  - In order to avoid the problem of abnormal overwrite sram in the A0 chip, a special treatment is added to the sys_init function 
    (if it is the A0 chip and it is the first power-on, it will enter deep, wake up after 100ms, and the program will be executed normally after wake-up)
* **gpio** 
  - GPIO_PD can't be used on the A0 version chip
* **uart** 
  - It is recommended to use the non-DMA mode. In the A0 version, the function of the DMA mode is defective. When receiving, the interrupt can be generated normally 
    only when the received data length is known.
* **clock**
  - The use of clock_init function needs to pay attention to two points:
   - CCLK_DIV2_TO_HCLK and HCLK_DIV4_TO_PCLK can’t be set at the same time
   - If the pm or reboot function is used, the parameter hclk_div cannot be set to CCLK_DIV2_TO_HCLK
* **other**
  - Precautions for the use of the development board (A0 version chip):
  If USB power supply, you need to unplug the USBVBUS-VBUS jumper (that is, you can't use +5V directly to the Vbus of the chip), otherwise it will cause USB/Audio 
  instability.
### BREAKING CHANGES
* N/A

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">

### 版本
* SDK版本: telink_b91_driver_sdk v1.0.0-Beta.
* 此版本SDK支持B91 A0芯片.
### Features
* AES/AUDIO/FLASH/LPC/MDEC/PKE/PM/PWM/RF/STIMER/TIMER/TRNG
* Interface:GPIO/I2C/SPI/UART/USB
### Known issues
* **pm** 
   - suspend模式在A0版本芯片上不可以使用
   - 为了规避A0芯片存在的异常改写sram问题，在sys_init函数中加了一段特殊处理（如果是A0芯片，并且是第一次上电，则会进入deep，100ms后唤醒，唤醒后会正常执行程序）
* **gpio** 
  - GPIO_PD在A0版本芯片上不能使用
* **uart** 
  - 建议使用非DMA模式，在A0版本，DMA模式的功能有缺陷，接收时，只有已知接收数据长度时，才能正常的产生中断。
* **clock**
  - clock_init函数的使用需要注意一下两点:
   - CCLK_DIV2_TO_HCLK和HCLK_DIV4_TO_PCLK不能同时设置 
   - 如果有使用pm或者reboot功能，则参数hclk_div不能设置为CCLK_DIV2_TO_HCLK
* **其他**
  - 开发板使用注意事项（A0版本芯片）：如果USB供电时，需要拔掉USBVBUS-VBUS跳帽（即不能用+5V直接给芯片的Vbus），否则会导致USB/Audio不稳定

### BREAKING CHANGES
* N/A