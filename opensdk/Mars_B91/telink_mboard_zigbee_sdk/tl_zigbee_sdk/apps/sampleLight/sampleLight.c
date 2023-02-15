/********************************************************************************************************
 * @file    sampleLight.c
 *
 * @brief   This is the source file for sampleLight
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *******************************************************************************************************/

#if (__PROJECT_TL_DIMMABLE_LIGHT__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "bdb.h"
#include "ota.h"
#include "gp.h"
#include "sampleLight.h"
#include "sampleLightCtrl.h"
#include "app_ui.h"
#include "factory_reset.h"
#if ZBHCI_EN
#include "zbhci.h"
#endif
#if ZCL_WWAH_SUPPORT
#include "wwah.h"
#endif

app_ctx_t gLightCtx;

#ifdef ZCL_OTA
extern ota_callBack_t sampleLight_otaCb;

//running code firmware information
ota_preamble_t sampleLight_otaInfo = {
	.fileVer 			= FILE_VERSION,
	.imageType 			= IMAGE_TYPE,
	.manufacturerCode 	= MANUFACTURER_CODE_TELINK,
};
#endif

//Must declare the application call back function which used by ZDO layer
const zdo_appIndCb_t appCbLst = {
	bdb_zdoStartDevCnf,//start device cnf cb
	NULL,//reset cnf cb
	NULL,//device announce indication cb
	sampleLight_leaveIndHandler,//leave ind cb
	sampleLight_leaveCnfHandler,//leave cnf cb
	sampleLight_nwkUpdateIndicateHandler,//nwk update ind cb
	NULL,//permit join ind cb
	NULL,//nlme sync cnf cb
	NULL,//tc join ind cb
	NULL,//tc detects that the frame counter is near limit
};

/**
 *  @brief Definition for bdb commissioning setting
 */
bdb_commissionSetting_t g_bdbCommissionSetting = {
	.linkKey.tcLinkKey.keyType = SS_GLOBAL_LINK_KEY,
	.linkKey.tcLinkKey.key = (u8 *)tcLinkKeyCentralDefault,       		//can use unique link key stored in NV

	.linkKey.distributeLinkKey.keyType = MASTER_KEY,
	.linkKey.distributeLinkKey.key = (u8 *)linkKeyDistributedMaster,  	//use linkKeyDistributedCertification before testing

	.linkKey.touchLinkKey.keyType = MASTER_KEY,
	.linkKey.touchLinkKey.key = (u8 *)touchLinkKeyMaster,   			//use touchLinkKeyCertification before testing

#if TOUCHLINK_SUPPORT
	.touchlinkEnable = 1,												/* enable touch-link */
#else
	.touchlinkEnable = 0,												/* disable touch-link */
#endif
	.touchlinkChannel = DEFAULT_CHANNEL, 								/* touch-link default operation channel for target */
	.touchlinkLqiThreshold = 0xA0,			   							/* threshold for touch-link scan req/resp command */
};

/**********************************************************************
 * LOCAL VARIABLES
 */
ev_timer_event_t *sampleLightAttrsStoreTimerEvt = NULL;


/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      stack_init
 *
 * @brief   This function initialize the ZigBee stack and related profile. If HA/ZLL profile is
 *          enabled in this application, related cluster should be registered here.
 *
 * @param   None
 *
 * @return  None
 */
void stack_init(void)
{
	/* Initialize ZB stack */
	zb_init();

	/* Register stack CB */
    zb_zdoCbRegister((zdo_appIndCb_t *)&appCbLst);
}

/*********************************************************************
 * @fn      user_app_init
 *
 * @brief   This function initialize the application(Endpoint) information for this node.
 *
 * @param   None
 *
 * @return  None
 */
void user_app_init(void)
{
	af_nodeDescManuCodeUpdate(MANUFACTURER_CODE_TELINK);

    /* Initialize ZCL layer */
	/* Register Incoming ZCL Foundation command/response messages */
	//xiaodong
   // zcl_init(sampleLight_zclProcessIncomingMsg);

	/* Register endPoint */
	//af_endpointRegister(SAMPLE_LIGHT_ENDPOINT, (af_simple_descriptor_t *)&sampleLight_simpleDesc, zcl_rx_handler, NULL);
#if AF_TEST_ENABLE
	/* A sample of AF data handler. */
	af_endpointRegister(SAMPLE_TEST_ENDPOINT, (af_simple_descriptor_t *)&sampleTestDesc, afTest_rx_handler, afTest_dataSendConfirm);
#endif

	/* Initialize or restore attributes, this must before 'zcl_register()' */
	//zcl_sampleLightAttrsInit();
	//zcl_reportingTabInit();

	/* Register ZCL specific cluster information */
	//zcl_register(SAMPLE_LIGHT_ENDPOINT, SAMPLELIGHT_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)g_sampleLightClusterList);

#if ZCL_GP_SUPPORT
	/* Initialize GP */
	gp_init();
#endif

#if ZCL_OTA_SUPPORT
	/* Initialize OTA */
    ota_init(OTA_TYPE_CLIENT, (af_simple_descriptor_t *)&sampleLight_simpleDesc, &sampleLight_otaInfo, &sampleLight_otaCb);
#endif

#if ZCL_WWAH_SUPPORT
    /* Initialize WWAH server */
    wwah_init(WWAH_TYPE_SERVER, (af_simple_descriptor_t *)&sampleLight_simpleDesc);
#endif
}

s32 sampleLightAttrsStoreTimerCb(void *arg)
{
	zcl_onOffAttr_save();
	zcl_levelAttr_save();
	zcl_colorCtrlAttr_save();

	sampleLightAttrsStoreTimerEvt = NULL;
	return -1;
}

void sampleLightAttrsStoreTimerStart(void)
{
	if(sampleLightAttrsStoreTimerEvt){
		TL_ZB_TIMER_CANCEL(&sampleLightAttrsStoreTimerEvt);
	}
	sampleLightAttrsStoreTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLightAttrsStoreTimerCb, NULL, 200);
}

void sampleLightAttrsChk(void)
{
	if(gLightCtx.lightAttrsChanged){
		gLightCtx.lightAttrsChanged = FALSE;
		if(zb_isDeviceJoinedNwk()){
			sampleLightAttrsStoreTimerStart();
		}
	}
}

void report_handler(void)
{
	if(zb_isDeviceJoinedNwk()){
		if(zcl_reportingEntryActiveNumGet()){
			u16 second = 1;//TODO: fix me

			reportNoMinLimit();

			//start report timer
			reportAttrTimerStart(second);
		}else{
			//stop report timer
			reportAttrTimerStop();
		}
	}
}

void app_task(void){
	app_handler();
	localPermitJoinState();
	if(BDB_STATE_GET() == BDB_STATE_IDLE){
		//factroyRst_handler();
		report_handler();
#if 0/* NOTE: If set to '1', the latest status of lighting will be stored. */
		sampleLightAttrsChk();
#endif
	}
}

static void sampleLightSysException(void){
	zcl_onOffAttr_save();
	zcl_levelAttr_save();
	zcl_colorCtrlAttr_save();
	my_set_gpio_level(g_config_st.sys_exception_io,g_config_st.sys_exception_io_level);
	//SYSTEM_RESET();
	//led_on(LED_POWER);
	//while(1);
}

#define SAMPLE_GW_ENDPOINT      				 0x01
#define SAMPLE_TEST_ENDPOINT					 0x02
#define ZCL_CLUSTER_TELINK_SDK_TEST				 0xFF00
#define ZCL_CLUSTER_TELINK_SDK_TEST_REQ			 0xFF01

void my_data_broadcast(unsigned char *dat,int len){
	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);
	dstEp.dstEp = SAMPLE_TEST_ENDPOINT;
	dstEp.profileId = HA_PROFILE_ID ;
	dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEp.dstAddr.shortAddr = 0xfffc;
	u8 apsCnt = 0;
	af_dataSend(SAMPLE_TEST_ENDPOINT, &dstEp, ZCL_CLUSTER_TELINK_SDK_TEST_REQ, len, dat, &apsCnt);
}

void my_data_unicast(unsigned char *dat,unsigned short addr,int len){
	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);
	dstEp.dstEp = SAMPLE_TEST_ENDPOINT;
	dstEp.profileId = HA_PROFILE_ID ;
	dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEp.dstAddr.shortAddr = addr;
	u8 apsCnt = 0;
	af_dataSend(SAMPLE_TEST_ENDPOINT, &dstEp, ZCL_CLUSTER_TELINK_SDK_TEST_REQ, len, dat, &apsCnt);
}

#define MY_UART_RX_BUF_SIZE  	64
unsigned char my_uartRxBuf[MY_UART_RX_BUF_SIZE];
void my_uart_data_handler(void *arg);

typedef struct{
	u32 dataLen;
	u8 dataPayload[0];
}uart_rxData_t;

void my_uartRcvHandler(void){
	TL_SCHEDULE_TASK(my_uart_data_handler, my_uartRxBuf);
}

void my_uart_init(unsigned int baudrate){
    drv_uart_pin_set(UART0_TX_PB2, UART0_RX_PB3);
	drv_uart_init(baudrate, my_uartRxBuf, MY_UART_RX_BUF_SIZE, my_uartRcvHandler);
}

void cmd_rec_success(void){
	unsigned char arrary[2];
	arrary[0] = 0x5A;
	arrary[1] = 0x5A;
	if(1 == g_config_st.feedback_open){
		drv_uart_tx_start(arrary,2);
	}
}

void my_uart_data_handler(void *arg){
	// the format of the uart rx data: length(4 Bytes) + payload
	unsigned char  cmd;
	unsigned char  ret;
	uart_rxData_t *rxData = arg;
	unsigned char  u8_array_temp[64];
	unsigned int frequency;
	if(g_config_st.pwm_info.tick_max == 0) frequency = 0;
	else frequency = (sys_clk.pclk * 1000 * 1000)/g_config_st.pwm_info.tick_max;
	if(((rxData->dataLen)&0xff) > MY_UART_RX_BUF_SIZE){
		drv_uart_tx_start(rxData->dataPayload,MY_UART_RX_BUF_SIZE);
		return;
	}

	if(g_config_st.echo_open){
		drv_uart_tx_start(rxData->dataPayload,rxData->dataLen);
	}
	/*
		模式配置控制IO（使用过程中可动态切换模式，切换间隔与按键长按的时间一致，默认为两秒）:
		00: 广播透传模式，uart收到任何数据都通过zigbee广播出去，zigbee收到的数据通过uart发送
		01: command模式，功能码使用示例见后面的描述
		10: 定位模式，router收集移动模块定位数据，定期发送给coordinator
		11: 移动模式，router定期广播自定义ID号跳包
	*/
	if(0 == g_config_st.module_function){
		my_data_broadcast(rxData->dataPayload,rxData->dataLen);
	}
	else if(1 == g_config_st.module_function){
		cmd = rxData->dataPayload[0];
	    switch(cmd){
	    	case SEND_DATA			 : // 指定短地址发送数据
	    		if(rxData->dataLen > 3){
	    			unsigned short short_addr =  ((rxData->dataPayload[1])<<8) + rxData->dataPayload[2];
	    			my_data_unicast(&(rxData->dataPayload[3]),short_addr,rxData->dataLen - 3);
	    			cmd_rec_success();
	    		}
	    		break;
	    	case  OPEN_ECHO          :		  // 打开回显
	    		if(rxData->dataLen == 1){
	    			g_config_st.echo_open = 1;
	    			CFG_Save();
	    			cmd_rec_success();
	    		}
	    		break;
	    	case  CLOSE_ECHO         :        // 关闭回显
	    		if(rxData->dataLen == 1){
	    			g_config_st.echo_open = 0;
	    			CFG_Save();
	    			cmd_rec_success();
	    		}
	    		break;
	    	case  SET_BAUDRATE       :        // 设置波特率 固定几个
	    		if(rxData->dataLen == 4){
	    			// 9600    03 00 25 80
	    			// 19200   03 00 4B 00
	    			// 115200  03 01 C2 00
	    			g_config_st.baudrate = ((rxData->dataPayload[1])<<16) + ((rxData->dataPayload[2])<<8) + rxData->dataPayload[3];
	    			CFG_Save();
	    			cmd_rec_success();
	    		}
	    		break;
	    		/*case  SET_PERMIT_TIME    :        // 限定入网时间设定
	    		if(rxData->dataLen == 2){
	    			g_config_st.permit_time = rxData->dataPayload[1];
	    			CFG_Save();
	    			cmd_rec_success();
	    		}
	    		break;*/
	    	case  SET_ID             :        // 设定ID
	    		if(rxData->dataLen == 2){
	    			g_config_st.device_id = rxData->dataPayload[1];
	    			CFG_Save();
	    			cmd_rec_success();
	    		}
	    		break;
	    	case  SET_PER_BRO_TIME   :        // 设定广播ID的周期
	    		if(rxData->dataLen == 2){
	    			g_config_st.per_bro_time = rxData->dataPayload[1];
	    			CFG_Save();
	    			cmd_rec_success();
	    		}
	    		break;
	    	case  SET_SHORT_TIME     :        // 设置短按检测时间 ms
	    		if(rxData->dataLen == 2){
	    			g_config_st.short_ticks = (rxData->dataPayload[1] /TICKS_INTERVAL);
	    			CFG_Save();
	    			cmd_rec_success();
	    		}
	    		break;
	    	case  SET_LONG_TIME      :        // 设置长按检测时间 ms
	    		if(rxData->dataLen == 3){
	    			g_config_st.long_ticks = ((rxData->dataPayload[1]<<8 | rxData->dataPayload[2])/TICKS_INTERVAL);
	    			CFG_Save();
	    			cmd_rec_success();
	    		}
	    		break;

	    	case  SEND_SHORT_ADDR    :        // RF发送自身短地址
	    		if(rxData->dataLen == 1){
					u8_array_temp[0] = 0xA5;
					u8_array_temp[1] = 0x5A;
					u8_array_temp[2] = (g_zbMacPib.shortAddress>>8)&0xff;
					u8_array_temp[3] = g_zbMacPib.shortAddress&0xff;
					my_data_broadcast(u8_array_temp,4);
					cmd_rec_success();
	    		}
	    		break;
	    	case  SEND_LONG_ADDR     :        // RF发送自身长地址
	    		if(rxData->dataLen == 1){
					u8_array_temp[0] = 0xA5;
					u8_array_temp[1] = 0xA5;
					u8_array_temp[2] = g_zbMacPib.extAddress[7];
					u8_array_temp[3] = g_zbMacPib.extAddress[6];
					u8_array_temp[4] = g_zbMacPib.extAddress[5];
					u8_array_temp[5] = g_zbMacPib.extAddress[4];
					u8_array_temp[6] = g_zbMacPib.extAddress[3];
					u8_array_temp[7] = g_zbMacPib.extAddress[2];
					u8_array_temp[8] = g_zbMacPib.extAddress[1];
					u8_array_temp[9] = g_zbMacPib.extAddress[0];
					my_data_broadcast(u8_array_temp,10);
					cmd_rec_success();
	    		}
	    		break;
	    	case  SEND_ID            :        // RF发送自身ID
	    		if(rxData->dataLen == 1){
					u8_array_temp[0] = 0x5A;
					u8_array_temp[1] = 0xA5;
					u8_array_temp[2] = g_config_st.device_id;
					u8_array_temp[3] = g_config_st.device_id;
					my_data_broadcast(u8_array_temp,4);
					cmd_rec_success();
	    		}
	    		break;
	    	case  SET_GPIO_LEVEL     :        // 设置某个GPIO引脚高低电平状态
	    		if(rxData->dataLen == 3 && rxData->dataPayload[1] < g_config_st.gpio_group_num){
	    			ret = my_set_gpio_level(rxData->dataPayload[1],rxData->dataPayload[2]?1:0);
	    			if(0 == ret)  cmd_rec_success();
	    		}
	    		break;
	    	case  READ_GPIO_LEVEL    :        // 读某个GPIO引脚高低电平状态
	    		if(rxData->dataLen == 2 && rxData->dataPayload[1] < g_config_st.gpio_group_num){
	    			u8_array_temp[0] = my_read_gpio_level(rxData->dataPayload[1]);
	    			if(0 == u8_array_temp[0] || 1 == u8_array_temp[0]){
	    				drv_uart_tx_start(u8_array_temp,1);
	    			}
	    		}
	    		break;
	    	case  SET_GPIO_STATE     :        // 设置设备入网/允许其他设备入网时的某个IO电平状态
	    		if(rxData->dataLen == 3 && rxData->dataPayload[1] < g_config_st.gpio_group_num){
	    		    g_config_st.join_net_io	 = rxData->dataPayload[1]; // 入网触发IO
	    		    g_config_st.join_net_io_level  = rxData->dataPayload[2]?1:0;  // 入网IO触发电平
	    		    ret = my_set_gpio_level(g_config_st.join_net_io,!g_config_st.join_net_io_level);
	    		    CFG_Save();
	    		    if(0 == ret)  cmd_rec_success();
	    		}
	    		break;

	    	case  SET_GPIO_SYS_EXP       :        // 设置设备死机时的某个IO电平状态
	    		if(rxData->dataLen == 3 && rxData->dataPayload[1] < g_config_st.gpio_group_num){
	    		    g_config_st.sys_exception_io = rxData->dataPayload[1];  // 入网触发IO
	    		    g_config_st.sys_exception_io_level  = rxData->dataPayload[2]?1:0;  // 入网IO触发电平
	    		    ret = my_set_gpio_level(g_config_st.sys_exception_io,!g_config_st.sys_exception_io_level);
	    		    CFG_Save();
	    		    if(0 == ret)  cmd_rec_success();
	    		}
	    		break;

	    	case  SET_GPIO_PWM_OPEN  :        // 设置GPIO  PWM频率4B   占空比
	    		if(rxData->dataLen == 7 && rxData->dataPayload[1] < g_config_st.pwm_group_num){
	    			// 10 00 00 00 27 10 32
	    			ret = my_set_pwm_run(rxData->dataPayload[1],
	    					rxData->dataPayload[2]<<24|rxData->dataPayload[3]<<16|rxData->dataPayload[4]<<8 | rxData->dataPayload[5], rxData->dataPayload[6]);
	    			if(0 == ret)  cmd_rec_success();
	    		}
	    		break;
	    	case  SET_GPIO_PWM_CLOSE  :       // PWM 关闭所有
	    		if(rxData->dataLen == 1){
	    			ret = my_set_pwm_stop();
	    			if(0 == ret)  cmd_rec_success();
	    		}
	    		break;
	    	case  SET_GPIO_PWM_RESTART :       // PWM 重启上一次设定的端口
	    		if(rxData->dataLen == 1){
	    			ret = my_set_pwm_restart();
	    			if(0 == ret)  cmd_rec_success();
	    		}
	    		break;


	    	case  SHOW_MODE          :        // 显示当前模式
	    		if(rxData->dataLen == 1){
	    			u8_array_temp[0] = g_config_st.module_function;
	    			drv_uart_tx_start(u8_array_temp,1);
	    		}
	    		break;
	    	case  SHOW_DEVICE_ID     :        // 显示DEVICE_ID
	    		if(rxData->dataLen == 1){
	    			u8_array_temp[0] = g_config_st.device_id;
	    			drv_uart_tx_start(u8_array_temp,1);
	    		}
	    		break;
	    	case  SHOW_PER_BRO_TIME  :        // 显示广播DEVICE_ID的周期
	    		if(rxData->dataLen == 1){
	    			u8_array_temp[0] = g_config_st.per_bro_time;
	    			drv_uart_tx_start(u8_array_temp,1);
	    		}
	    		break;
	    		/*	case  SHOW_PERMIT_TIME   :        // 显示限定入网时间
	    		if(rxData->dataLen == 1){
	    			u8_array_temp[0] = g_config_st.permit_time;
	    			drv_uart_tx_start(u8_array_temp,1);
	    		}
	    		break;*/
	    	case  SHOW_ECHO          :        // 显示是否有回显
	    		if(rxData->dataLen == 1){
	    			u8_array_temp[0] = g_config_st.echo_open;
	    			drv_uart_tx_start(u8_array_temp,1);
	    		}
	    		break;

	    	case  SHOW_PAN_ID        :        // 显示PAN_ID
	    		if(rxData->dataLen == 1){
	    			u8_array_temp[0] = (g_zbMacPib.panId>>8)&0xff;
	    			u8_array_temp[1] =  g_zbMacPib.panId&0xff;
	    			drv_uart_tx_start(u8_array_temp,2);
	    		}
	    		break;
	    	case  SHOW_C_LONG_ADDR   :        // 显示coordinator长地址
	    		if(rxData->dataLen == 1){
					u8_array_temp[0] = g_zbMacPib.coordExtAddress[7];
					u8_array_temp[1] = g_zbMacPib.coordExtAddress[6];
					u8_array_temp[2] = g_zbMacPib.coordExtAddress[5];
					u8_array_temp[3] = g_zbMacPib.coordExtAddress[4];
					u8_array_temp[4] = g_zbMacPib.coordExtAddress[3];
					u8_array_temp[5] = g_zbMacPib.coordExtAddress[2];
					u8_array_temp[6] = g_zbMacPib.coordExtAddress[1];
					u8_array_temp[7] = g_zbMacPib.coordExtAddress[0];
	    			drv_uart_tx_start(u8_array_temp,8);
	    		}
	    		break;
	    	case  SHOW_LONG_ADDR     :        // 显示长地址
	    		if(rxData->dataLen == 1){
					u8_array_temp[0] = g_zbMacPib.extAddress[7];
					u8_array_temp[1] = g_zbMacPib.extAddress[6];
					u8_array_temp[2] = g_zbMacPib.extAddress[5];
					u8_array_temp[3] = g_zbMacPib.extAddress[4];
					u8_array_temp[4] = g_zbMacPib.extAddress[3];
					u8_array_temp[5] = g_zbMacPib.extAddress[2];
					u8_array_temp[6] = g_zbMacPib.extAddress[1];
					u8_array_temp[7] = g_zbMacPib.extAddress[0];
	    			drv_uart_tx_start(u8_array_temp,8);
	    		}
	    		break;
	    	case  SHOW_SHORT_ADDR        :        // 显示短地址
	    		if(rxData->dataLen == 1){
					u8_array_temp[0] = (g_zbMacPib.shortAddress>>8)&0xff;
					u8_array_temp[1] =  g_zbMacPib.shortAddress&0xff;
	    			drv_uart_tx_start(u8_array_temp,2);
	    		}
	    		break;

	    	case  SHOW_DEVICE_KIND   :        // 显示设备类型
	    		if(rxData->dataLen == 1){
	    			u8_array_temp[0] =  D_ROUTER;
	    			drv_uart_tx_start(u8_array_temp,1);
	    		}
	    		break;
	    	case  SHOW_BUTTON_TICK           :        // 显示长按和短按时间 ms
	    		if(rxData->dataLen == 1){
		    		u8_array_temp[0] =   (g_config_st.short_ticks*TICKS_INTERVAL)&0xff;
		    		u8_array_temp[1] =  ((g_config_st.long_ticks *TICKS_INTERVAL)>>8)&0xff;
		    		u8_array_temp[2] =   (g_config_st.long_ticks *TICKS_INTERVAL)&0xff;
		    		drv_uart_tx_start(u8_array_temp,3);
	    		}
	    		break;

	    	case  SHOW_BUTTON_SELECT           :        // 设备入网/允许其他设备入网时的某个IO 和 触发电平  设备死机时的某个IO 和 触发电平
	    		if(rxData->dataLen == 1){
					u8_array_temp[0] =  g_config_st.join_net_io;
					u8_array_temp[1] =  g_config_st.join_net_io_level;
					u8_array_temp[2] =  g_config_st.sys_exception_io;
					u8_array_temp[3] =  g_config_st.sys_exception_io_level;
		    		drv_uart_tx_start(u8_array_temp,4);
	    		}
	    		break;

	    	case  SHOW_PWM_SELECT           :        // pwm_group[]引脚下标 频率 占空比
	    		if(rxData->dataLen == 1){
					u8_array_temp[0] =  g_config_st.pwm_info.num;
					u8_array_temp[1] = (g_config_st.pwm_info.tick_max>>24)&0xff;
					u8_array_temp[2] = (g_config_st.pwm_info.tick_max>>16)&0xff;
					u8_array_temp[3] = (g_config_st.pwm_info.tick_max>>8)&0xff;
					u8_array_temp[4] = (g_config_st.pwm_info.tick_max)&0xff;
					u8_array_temp[5] =  g_config_st.pwm_info.dutycycle;
		    		drv_uart_tx_start(u8_array_temp,6);
	    		}
	    		break;
	    	case  SHOW_ALL           :        // 显示所有数据
	    		if(rxData->dataLen == 1){
					u8_array_temp[0] =  g_config_st.module_function;
					u8_array_temp[1] = (g_config_st.baudrate>>16)&0xff;
					u8_array_temp[2] = (g_config_st.baudrate>>8)&0xff;
					u8_array_temp[3] =  g_config_st.baudrate&0xff;

					u8_array_temp[4] = (g_zbMacPib.panId>>8)&0xff;
					u8_array_temp[5] =  g_zbMacPib.panId&0xff;
					u8_array_temp[6]  = g_zbMacPib.coordExtAddress[7];
					u8_array_temp[7]  = g_zbMacPib.coordExtAddress[6];
					u8_array_temp[8]  = g_zbMacPib.coordExtAddress[5];
					u8_array_temp[9]  = g_zbMacPib.coordExtAddress[4];
					u8_array_temp[10] = g_zbMacPib.coordExtAddress[3];
					u8_array_temp[11] = g_zbMacPib.coordExtAddress[2];
					u8_array_temp[12] = g_zbMacPib.coordExtAddress[1];
					u8_array_temp[13] = g_zbMacPib.coordExtAddress[0];
					u8_array_temp[14] = g_zbMacPib.extAddress[7];
					u8_array_temp[15] = g_zbMacPib.extAddress[6];
					u8_array_temp[16] = g_zbMacPib.extAddress[5];
					u8_array_temp[17] = g_zbMacPib.extAddress[4];
					u8_array_temp[18] = g_zbMacPib.extAddress[3];
					u8_array_temp[19] = g_zbMacPib.extAddress[2];
					u8_array_temp[20] = g_zbMacPib.extAddress[1];
					u8_array_temp[21] = g_zbMacPib.extAddress[0];
					u8_array_temp[22] = (g_zbMacPib.shortAddress>>8)&0xff;
					u8_array_temp[23] =  g_zbMacPib.shortAddress&0xff;

					u8_array_temp[24] = g_config_st.permit_time;
					u8_array_temp[25] = g_config_st.echo_open;
					u8_array_temp[26] = g_config_st.device_id;
					u8_array_temp[27] = g_config_st.per_bro_time;

					u8_array_temp[28] =   (g_config_st.short_ticks*TICKS_INTERVAL)&0xff;
					u8_array_temp[29] =  ((g_config_st.long_ticks *TICKS_INTERVAL)>>8)&0xff;
					u8_array_temp[30] =   (g_config_st.long_ticks *TICKS_INTERVAL)&0xff;
					u8_array_temp[31] =  g_config_st.join_net_io;
					u8_array_temp[32] =  g_config_st.join_net_io_level;
					u8_array_temp[33] =  g_config_st.sys_exception_io;
					u8_array_temp[34] =  g_config_st.sys_exception_io_level;
					u8_array_temp[35] =  g_config_st.pwm_info.num;

					u8_array_temp[36] = (frequency>>24)&0xff;
					u8_array_temp[37] = (frequency>>16)&0xff;
					u8_array_temp[38] = (frequency>>8)&0xff;
					u8_array_temp[39] = (frequency)&0xff;

					u8_array_temp[40] =  g_config_st.pwm_info.dutycycle;
					u8_array_temp[41] =  g_config_st.feedback_open;
					u8_array_temp[42] =  D_ROUTER;
					drv_uart_tx_start(u8_array_temp,43);
	    		}
	    		break;

	    	case  OPEN_FEEDBACK        :         // 打开设置后的反馈
	    		if(rxData->dataLen == 1){
	    			g_config_st.feedback_open = 1;
	    			cmd_rec_success();
	    			CFG_Save();
	    		}
	    		break;
	    	case  CLOSE_FEEDBACK        :        // 关闭设置后的反馈
	    		if(rxData->dataLen == 1){
	    			g_config_st.feedback_open = 0;
	    			cmd_rec_success();
	    			CFG_Save();
	    		}
	    		break;

	    	case  SET_DEFAULT        :        // 设置默认
	    		if(rxData->dataLen == 1){
	    			CFG_Default();
	    			CFG_Save();
	    			cmd_rec_success();
	    		}
	    		break;
	    	case  SET_RESET          :        // 复位芯片
	    		if(rxData->dataLen == 1){
	    			cmd_rec_success();
	    			//while(uart_tx_is_busy(UART0));
	    			delay_ms(10);
	    			SYSTEM_RESET();
	    		}
	    		break;
	    	case  SET_FACTORY        :        // 恢复出场设置
	    		if(rxData->dataLen == 1){
	    			cmd_rec_success();
	    			//while(uart_tx_is_busy(UART0));
	    			delay_ms(10);
	    			zb_factoryReset();
	    		}
	    		break;
	    	default:break;
		}
	}
}

void user_init(bool isRetention){
	(void)isRetention;
	/* Initialize LEDs*/
	led_init();
	//hwLight_init();
	CFG_Load();
	my_uart_init(g_config_st.baudrate);
	Key_Init();
	//factroyRst_init();

	/* Initialize Stack */
	stack_init();

	/* Initialize user application */
	user_app_init();

	/* Register except handler for test */
	sys_exceptHandlerRegister(sampleLightSysException);

	/* Adjust light state to default attributes*/
	light_adjust();
	/* User's Task */
#if ZBHCI_EN
	zbhciInit();
	ev_on_poll(EV_POLL_HCI, zbhciTask);
#endif
	ev_on_poll(EV_POLL_IDLE, app_task);

    /* Read the pre-install code from NV */
	if(bdb_preInstallCodeLoad(&gLightCtx.tcLinkKey.keyType, gLightCtx.tcLinkKey.key) == RET_OK){
		g_bdbCommissionSetting.linkKey.tcLinkKey.keyType = gLightCtx.tcLinkKey.keyType;
		g_bdbCommissionSetting.linkKey.tcLinkKey.key = gLightCtx.tcLinkKey.key;
	}

    /* Set default reporting configuration */
    u8 reportableChange = 0x00;
    bdb_defaultReportingCfg(SAMPLE_LIGHT_ENDPOINT, HA_PROFILE_ID, ZCL_CLUSTER_GEN_ON_OFF, ZCL_ATTRID_ONOFF,
    						0x0000, 0x003c, (u8 *)&reportableChange);

    /* Initialize BDB */
	bdb_init((af_simple_descriptor_t *)&sampleLight_simpleDesc, &g_bdbCommissionSetting, &g_zbDemoBdbCb, 1);
}

#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */

