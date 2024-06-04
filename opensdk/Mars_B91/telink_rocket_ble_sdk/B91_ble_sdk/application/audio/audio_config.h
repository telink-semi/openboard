/********************************************************************************************************
 * @file     audio_config.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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

 
#include  "application/audio/audio_common.h"
#include  "tl_common.h"


#ifndef		TL_AUDIO_MODE
#define		TL_AUDIO_MODE									AUDIO_DISABLE
#endif

#if (TL_AUDIO_MODE & RCU_PROJECT)						//RCU
	#if (TL_AUDIO_MODE == TL_AUDIO_RCU_ADPCM_GATT_TLEINK)
		#define	ADPCM_PACKET_LEN				128
		#define TL_MIC_ADPCM_UNIT_SIZE			248
		#define	TL_MIC_BUFFER_SIZE				992
	#elif (TL_AUDIO_MODE == TL_AUDIO_RCU_ADPCM_GATT_GOOGLE)
		#define	ADPCM_PACKET_LEN				136		//(128+6+2)
		#define TL_MIC_ADPCM_UNIT_SIZE			256
		#define	TL_MIC_BUFFER_SIZE				1024
	#elif (TL_AUDIO_MODE == TL_AUDIO_RCU_ADPCM_HID_DONGLE_TO_STB)
		#define	ADPCM_PACKET_LEN				120
		#define TL_MIC_ADPCM_UNIT_SIZE			240
		#define	TL_MIC_BUFFER_SIZE				960
	#elif (TL_AUDIO_MODE == TL_AUDIO_RCU_ADPCM_HID)
		#define	ADPCM_PACKET_LEN				120
		#define TL_MIC_ADPCM_UNIT_SIZE			240
		#define	TL_MIC_BUFFER_SIZE				960
	#elif (TL_AUDIO_MODE == TL_AUDIO_RCU_SBC_HID_DONGLE_TO_STB)
		#define	ADPCM_PACKET_LEN				20
		#define MIC_SHORT_DEC_SIZE				80
		#define	TL_MIC_BUFFER_SIZE				320
	#elif (TL_AUDIO_MODE == TL_AUDIO_RCU_SBC_HID)
		#define	ADPCM_PACKET_LEN				20
		#define MIC_SHORT_DEC_SIZE				80
		#define	TL_MIC_BUFFER_SIZE				320
	#elif (TL_AUDIO_MODE == TL_AUDIO_RCU_MSBC_HID)
		#define	ADPCM_PACKET_LEN				57
		#define MIC_SHORT_DEC_SIZE				120
		#define	TL_MIC_BUFFER_SIZE				480
	#else

	#endif

#elif (TL_AUDIO_MODE & DONGLE_PROJECT)					//Dongle

	#if (TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_GATT_TELINK)
		#define	MIC_ADPCM_FRAME_SIZE		128
		#define	MIC_SHORT_DEC_SIZE			248
	#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_GATT_GOOGLE)
		#define	MIC_ADPCM_FRAME_SIZE		136 		//128+6+2
		#define	MIC_SHORT_DEC_SIZE			256
	#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_HID_DONGLE_TO_STB)
		#define	MIC_ADPCM_FRAME_SIZE		120
		#define	MIC_SHORT_DEC_SIZE			240
	#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_ADPCM_HID)
		#define	MIC_ADPCM_FRAME_SIZE		120
		#define	MIC_SHORT_DEC_SIZE			240
	#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_SBC_HID_DONGLE_TO_STB)
		#define	MIC_ADPCM_FRAME_SIZE		20
		#define	MIC_SHORT_DEC_SIZE			80

	#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_SBC_HID)
		#define	MIC_ADPCM_FRAME_SIZE		20
		#define	MIC_SHORT_DEC_SIZE			80
	#elif (TL_AUDIO_MODE == TL_AUDIO_DONGLE_MSBC_HID)
		#define	MIC_ADPCM_FRAME_SIZE		57
		#define	MIC_SHORT_DEC_SIZE			120
	#else

	#endif

	#if ((TL_AUDIO_MODE == TL_AUDIO_DONGLE_SBC_HID) || (TL_AUDIO_MODE == TL_AUDIO_DONGLE_SBC_HID_DONGLE_TO_STB) || (TL_AUDIO_MODE == TL_AUDIO_DONGLE_MSBC_HID))
		#define 	MIC_OPEN_FROM_RCU		0x31999999
		#define 	MIC_OPEN_TO_STB			0x32999999
		#define 	MIC_CLOSE_FROM_RCU		0x34999999
		#define 	MIC_CLOSE_TO_STB		0x35999999
	#else
		#define 	MIC_OPEN_FROM_RCU		0x21999999
		#define 	MIC_OPEN_TO_STB			0x22999999
		#define 	MIC_CLOSE_FROM_RCU		0x24999999
		#define 	MIC_CLOSE_TO_STB		0x25999999
	#endif
#else

#endif
