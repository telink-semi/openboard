/********************************************************************************************************
 * @file     ll_resolvlist.h
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

#ifndef LL_RESOLVLIST_H_
#define LL_RESOLVLIST_H_


#include <stack/ble/ble_common.h>




ble_sts_t  		ll_resolvingList_add(u8 peerIdAddrType, u8 *peerIdAddr, u8 *peer_irk, u8 *local_irk);
ble_sts_t  		ll_resolvingList_delete(u8 peerIdAddrType, u8 *peerIdAddr);
ble_sts_t  		ll_resolvingList_reset(void);
ble_sts_t  		ll_resolvingList_getSize(u8 *Size);
ble_sts_t  		ll_resolvingList_getPeerResolvableAddr (u8 peerIdAddrType, u8* peerIdAddr, u8* peerResolvableAddr); //not available now
ble_sts_t  		ll_resolvingList_getLocalResolvableAddr(u8 peerIdAddrType, u8* peerIdAddr, u8* LocalResolvableAddr); //not available now
ble_sts_t  		ll_resolvingList_setAddrResolutionEnable (u8 resolutionEn);
ble_sts_t  		ll_resolvingList_setResolvablePrivateAddrTimer (u16 timeout_s);   //not available now
ble_sts_t  		ll_resolvingList_setPrivcyMode(u8 peerIdAddrType, u8* peerIdAddr, u8 privMode);

u16				blc_ll_resolvGetRpaTmo(void);
int				blc_ll_resolvPeerRpaResolvedAny(const u8* rpa);
void			blc_ll_resolvSetPeerRpaByIdx(u8 idx, u8 *rpa);
void			blc_ll_resolvSetLocalRpaByIdx(u8 idx, u8 *rpa);
bool			blc_ll_resolvGetRpaByAddr(u8* peerIdAddr, u8 peerIdAddrType, u8* rpa, u8 local);
bool			blc_ll_resolvIsAddrResolved(const u8* irk, const u8* rpa);
bool			blc_ll_resolvIsAddrRlEnabled(void);
void			blc_ll_resolvListInit(void);
/******************************* User Interface  End  ******************************************************************/





#endif /* LL_RESOLVLIST_H_ */
