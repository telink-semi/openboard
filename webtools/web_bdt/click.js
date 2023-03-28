const SWSButton = document.getElementById("SWSButton");
const StallButton = document.getElementById("StallButton");
const StartButton = document.getElementById("StartButton");
const usbConnectButton = document.getElementById("usbConnectButton");
const DownloadButton = document.getElementById("DownloadButton");
const EraseButton = document.getElementById("EraseButton");
const ActivateButton = document.getElementById("ActivateButton");
const RunButton = document.getElementById("RunButton");
const PauseButton = document.getElementById("PauseButton");
const StepButton = document.getElementById("StepButton");
const PCButton = document.getElementById("PCButton");
const ResetButton = document.getElementById("ResetButton");
const ClearButton = document.getElementById("ClearButton");
const usbRWAddr = document.getElementById("usbRWAddr");
const usbRWLength = document.getElementById("usbRWLength");
const usbRWData = document.getElementById("usbRWData");
const FwDownloadButton1 = document.getElementById("FwDownloadButton1");
const FwDownloadButton2 = document.getElementById("FwDownloadButton2");
const MultiDownloadButton = document.getElementById("MultiDownloadButton");

const input1 = document.getElementById("input1");
const input2 = document.getElementById("input2");
const input3 = document.getElementById("input3");
const input4 = document.getElementById("input4");
const input5 = document.getElementById("input5");
const input6 = document.getElementById("input6");
const input7 = document.getElementById("input7");
const input8 = document.getElementById("input8");

/********************************mini_burning_evk_offline_download*********************************************/
const fw_bin_table = document.getElementById('UserFwFileTable');
const offline_del_user_bin = document.getElementById("offline_del_user_bin");
const offline_download_user_bin = document.getElementById("offline_download_user_bin");
$("#OffLineFWBinFileImport").click(function() {
	$("#OffLineFWBinFileInput").click();
})

const tool_bin_table = document.getElementById('RamToolFileTable');
const offline_list_ram_tool_bin = document.getElementById("offline_list_ram_tool_bin");
const offline_download_ram_tool_bin = document.getElementById("offline_download_ram_tool_bin");
///////////////////////////////////////////////////////////////////////////////////////////////////
const FW_INFO_ADDR				     =  0x80000;
const FW_START_ADDR 				 =  (FW_INFO_ADDR + 0x1000);
const FW_FLASH_MAX 					 =  2*1024*1024;
const g_offline_fw_data_max  	  	 =  FW_FLASH_MAX - FW_START_ADDR;     // burning evk maximum user_fw capacity 

const OffLineDownloadNumMax		  	 = 	32;
var OffLineChipSelect  	   		  	 = 	new Array(OffLineDownloadNumMax); // 芯片类型 默认B91
let OffLineChipType   	  	 	  	 = 	new Array(OffLineDownloadNumMax); // 固件适配的目标芯片 1B
let OffLineChip_RAM_Type   		  	 = 	new Array(OffLineDownloadNumMax); // 工具固件适配的目标芯片 1B
let OffLineFirmware_Type   		  	 = 	new Array(OffLineDownloadNumMax); // 固件类型 1B
let OffLineFirmware_Version   	  	 = 	new Array(OffLineDownloadNumMax); // 固件版本 1
let OffLineFirmware_Name  		  	 = 	new Array(OffLineDownloadNumMax); // 固件名称 11B /0
let OffLineFirmware_Flag   		  	 = 	new Array(OffLineDownloadNumMax); // 固件是否有效标志位 1B
let OffLineFirmware_Store_Addr    	 = 	new Array(OffLineDownloadNumMax); // 固件存放在EVK FLASH处的的起始地址（相对位移） 4B
let OffLineFirmware_Size   		  	 = 	new Array(OffLineDownloadNumMax); // 固件大小 4B
let OffLineFirmware_Download_Addr 	 = 	new Array(OffLineDownloadNumMax); // 固件下载到目标芯片处的地址 相对地址 4B
let OffLineFirmware_CRC   		  	 = 	new Array(OffLineDownloadNumMax); // 固件CRC值 4B

let FW_INFO_DATA 			 	  	 =  new Uint8Array(1024*4);			  // 4KB FW_INFO
let g_offline_fw_data       	  	 =  new Array(OffLineDownloadNumMax); // 固件缓存
let next_fw_store_addr 		 	  	 =  0; 								  // 固件存放在EVK FLASH处的的起始地址（相对位移）4B
/*
const RAM_TOOL_INFO_ADDR 		     =  0x20000;
const RAM_TOOL_START_ADDR 			 =  (RAM_TOOL_INFO_ADDR + 0x1000);
const g_offline_ram_tool_data_max    =  FW_INFO_ADDR-RAM_TOOL_START_ADDR; // burning evk maximum ram tool fw capacity 
let RAM_TOOL_INFO_DATA 		 		 =  new Uint8Array(1024*4);			  // 4KB RAM_TOOL_INFO
let g_offline_ram_tool_data  		 =  []; 							  // 固件缓存
let OffLine_TOOL_Firmware_Store_Addr = 	new Array(ChipNumber); 			  // 固件存放在EVK FLASH处的的起始地址（相对位移） 4B
let OffLine_TOOL_Firmware_Size 		 = 	new Array(ChipNumber); 			  // 固件大小 4B
let OnceTimeLoadToolBin				 =  true;							  // 只加载一次toolbin
let OnceTimeShowToolBinTable		 =  true;							  // 列表只显示一次
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
const C_8366   = 0x01;
const C_8368   = 0x02;
const C_8367_i = 0x03;
const C_8367_e = 0x04;
const C_8369_i = 0x05;
const C_8369_e = 0x06;
const C_8232   = 0x07;
const C_8266   = 0x08;
const C_8267   = 0x09;
const C_8269   = 0x0a;
const C_B80    = 0x0b;
const C_B85    = 0x0c;
const C_B87    = 0x0d;
const C_B89    = 0x0e;
const C_B91    = 0x0f;
const C_B92    = 0x10;
var C_Number   = 0x11;
var C_Name   = new Array(C_Number);

C_Name[C_8366]   = "8366";     C_Name[C_8368]   = "8368";     
C_Name[C_8367_i] = "8367_i";   C_Name[C_8367_e] = "8367_e";   
C_Name[C_8369_i] = "8369_i";   C_Name[C_8369_e] = "8369_e"; 
C_Name[C_8232]   = "8232";     C_Name[C_8266]   = "8266";		
C_Name[C_8267]   = "8267";     C_Name[C_8269]   = "8269";     
C_Name[C_B80]    = "B80";      C_Name[C_B85]    = "B85";
C_Name[C_B87]    = "B87";      C_Name[C_B89]    = "B89";   
C_Name[C_B91]	 = "B91";      C_Name[C_B92]	= "B92";

// 根据 chip_select 确定 Chip_Type 和 Chip_RAM_Type
function get_Offline_ChipType_Chip_RAM_Type(chip_select){
	if(typeof chip_select != "number" || chip_select >= C_Number){
		console.log("typeof chip_select != number or chip_select >= ChipNumber");
		return [0xff,0xff];
	}
	switch(chip_select){
		case C_8366:
			return [0x01,C_8366];

		case C_8368:
			return [0x01,C_8368];

		case C_8367_i:
			return [0x01,C_8367_i];

		case C_8367_e:
			return [0x01,C_8367_e];

		case C_8369_i:
			return [0x01,C_8369_i];

		case C_8369_e:
			return [0x01,C_8369_e];

		case C_8232:
			return [0x11,C_8232];

		case C_8266:
			return [0x12,C_8266];

		case C_8267:
			return [0x12,C_8267];

		case C_8269:
			return [0x12,C_8269];

		case C_B80:
			return [0x22,C_B80];

		case C_B85:
			return [0x22,C_B85];
			
		case C_B87:
			return [0x23,C_B87];

		case C_B89:
			return [0x24,C_B89];

		case C_B91:
			return [0x31,C_B91];

	    case C_B92:
			return [0x32,C_B92];

		default:
			return [0xff,0xff];
	}
}
// burning evk flash解锁
async function unlock_flash_of_evk(){
	let i = 0;
	let size = unlock_flash_buffer.length;
	let buffer = new Uint8Array(2); 
	let temp;
	let sbyte_num;
	let temp_data;
	let aign_256byte_num;
	let ret;
	//*********    reset mcu    *********************
	buffer[0] = 0x05;
	buffer[1] = 0;
	g_rcv_data[0] = 0;
	while((g_rcv_data[0]&0x05) != 0x05) {
		await usb_write(0x602,buffer,1);
		await usb_read(0x602,1);
		i++;
		if(i > 3) {
			console.log(" TC32 USB : USB Err! ");
 			return ERROR_Typdef.EPERM;
		}
	}
	//******  Disable watch dog, Disable interrupt,download file to ram and start MCU    *********
	buffer[0] = 0x00;
	aign_256byte_num = Math.floor(size/256);
	sbyte_num = Math.floor(size%256);
	console.log("size: " + size + " aign_256byte_num: " + aign_256byte_num + " sbyte_num: " + sbyte_num);
	await usb_write(0x622,buffer,1);
	await usb_write(0x643,buffer,1);
	buffer[0] = 0x40;
	await usb_write(0x60c,buffer,1);
	await usb_write(0x60d,buffer,1);
	buffer[0] = 0xff;
	await usb_write(0x104,buffer,1);
	
	for(i = 0; i < aign_256byte_num; i++) {
		temp_data = unlock_flash_buffer.slice(i*256,i*256+256);
		await usb_write3(0x8000+i*256,temp_data,256);
	}
	if(sbyte_num != 0) {
		temp_data = unlock_flash_buffer.slice(aign_256byte_num*256,size);
		await usb_write3(0x8000+aign_256byte_num*256,temp_data,sbyte_num);
	}
	//check bin
	await crc32_check_look_table(0xffffffff,unlock_flash_buffer,size);
	temp = crc_ret;
	for(i = 0; i < size; i++) {
		unlock_flash_buffer[i] = 0;
	}
	await usb_read(0x8000,size);
	await crc32_check_look_table(0xffffffff,g_rcv_data,size);
	if(temp != crc_ret) {
		console.log(" crc check err! ");
		console.log("temp: " + temp + " crc_ret: " + crc_ret);
		return ERROR_Typdef.ECRC;
	}
	else {
		console.log(" crc check success! ");
	}
	// program run
	buffer[0] = 0x88;
	await usb_write(0x602,buffer,1);
	await sleep(10);
	//2.erase flash of evk to confirm whether unlock flash is  succeeded
	ret = await MCU_Init_None_Log(CHIP_8266,TL_ModeTypdef.USB);
	console.log(" succeed! ");
	//b = await usb_evk_dut_cmd(CHIP_8266,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_ERASE,0,4,TL_ModeTypdef.USB);
	console.log(" unlock flash succeed!");
	return 0;
}
// burning evk flash上锁
async function lock_flash_of_evk(){
	let i = 0;
	let size = lock_flash_buffer.length;
	let buffer = new Uint8Array(2); 
	let temp;
	let sbyte_num;
	let temp_data;
	let aign_256byte_num;
	let a,b;
	//4.lock flash
	size = lock_flash_buffer.length;
	//*********    reset mcu    *********************
	buffer[0] = 0x05;
	buffer[1] = 0;
	i = 0;
	g_rcv_data[0] = 0;
	while((g_rcv_data[0]&0x05) != 0x05) {
		await usb_write(0x602,buffer,1);
		await usb_read(0x602,1);
		i++;
		if(i > 3) {
			console.log(" TC32 USB : USB Err! ");
			return 0;
		}
	}
//********************   Disable watch dog, Disable interrupt,download file to ram and start MCU   *********************
	aign_256byte_num = Math.floor(size/256);
	sbyte_num = Math.floor(size%256);
	console.log("size: " + size + " aign_256byte_num: " + aign_256byte_num + " sbyte_num: " + sbyte_num);
	buffer[0] = 0x00;
	await usb_write(0x622,buffer,1);
	await usb_write(0x643,buffer,1);
	buffer[0] = 0x40;
	await usb_write(0x60c,buffer,1);
	await usb_write(0x60d,buffer,1);
	buffer[0] = 0xff;
	await usb_write(0x104,buffer,1);
	for(i = 0; i < aign_256byte_num; i++) {
		temp_data = lock_flash_buffer.slice(i*256,i*256+256);
		await usb_write3(0x8000+i*256,temp_data,256);
	}
	if(sbyte_num != 0) {
		temp_data = lock_flash_buffer.slice(aign_256byte_num*256,size);
		await usb_write3(0x8000+aign_256byte_num*256,temp_data,sbyte_num);
	} 
	//check bin
	await crc32_check_look_table(0xffffffff,lock_flash_buffer,size);
	temp = crc_ret;
	for(i = 0; i < size; i++) {
		lock_flash_buffer[i] = 0;
	}
	await usb_read(0x8000,size);
	await crc32_check_look_table(0xffffffff,g_rcv_data,size);
	if(temp != crc_ret) {
		console.log(" crc check err! ");
		console.log("temp: " + temp + " crc_ret: " + crc_ret);
		return ERROR_Typdef.ECRC;
	}
	else {
		console.log(" crc check success! ");
	}
	// ram program run
	buffer[0] = 0x88;
	await usb_write(0x602,buffer,1);
	await sleep(10);
	//2.erase flash of evk to confirm whether lock flash is succeeded
	a = await MCU_Init_None_Log(CHIP_8266,TL_ModeTypdef.USB);
	b = await TL_Dut_Flash_cmd_Process_None_Log(CHIP_8266,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_ERASE,0,4,TL_ModeTypdef.USB);
	//console.log(" --------------- upgrade firmware succeed, please power on again ------------------ 	 ");
	return 0;
}
// download bin to  burning evk flash
async function download_bin_to_evk_flash(data,length,statr_addr,index){
	let i = 0;
	let msecond1;
	let msecond2;
	let EraseSector_Num;
	let PageWrite_Num;
	let temp_data;
	let temp_len;
	let callback_en = 0;
	let n=0;
	let progress_cb = false;
	let k = 0;
	await MCU_Init_None_Log(CHIP_8266,TL_ModeTypdef.USB);
	EraseSector_Num = (0 == (length%0x1000)) ? Math.floor(length/0x1000) : Math.floor((length/0x1000)+1);
	PageWrite_Num = (0 == (length%0x100)) ? Math.floor(length/0x100) : Math.floor((length/0x100)+1);
	msecond1 = new Date();
	console.log("length: "+length +"		EraseSector_Num: "+EraseSector_Num+ "		PageWrite_Num: "+PageWrite_Num);
	await usb_evk_dut_cmd(CHIP_8266,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_ASK,0,0,TL_ModeTypdef.USB);
	
	console.log("  BinLoadAdr:  " + BinLoadAdr); // 45392
	for(i = 0; i < EraseSector_Num; i++) {
		await usb_evk_dut_cmd(CHIP_8266,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_ERASE,statr_addr + (i*0x1000),4,TL_ModeTypdef.USB);
	}
	for(k = 0; k < PageWrite_Num; k++) {
		if(k < PageWrite_Num - 1) {
			temp_data = data.slice(k*256, k*256+256);
			temp_len = 256;
			callback_en = 1;
			n++;
			if(16 == n) {
			    n = 0;
				progress_cb = true;
			}
		}
		else {
			temp_data = data.slice(k*256,length);
			temp_len = length - (k*256);
			if(temp_len > 256) {
				temp_len = 256;
			}
			callback_en = 1;
			progress_cb = true;
		}
		await usb_write3(BinLoadAdr,temp_data,temp_len);
		await usb_evk_dut_cmd(CHIP_8266,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_WRITE,(k*256)+statr_addr,temp_len,TL_ModeTypdef.USB);
		if(1 == callback_en) {
			console.log("Flash Page Program at address: 0x"+dec2hex2(statr_addr+k*256));
			if(progress_cb && index !=undefined){
				progress_cb = false;
				if(index < OffLineDownloadNumMax){
					if(k == PageWrite_Num - 1)
					document.getElementById(`offline_fw_download_progress_${index}`).value = 100;
					else
					document.getElementById(`offline_fw_download_progress_${index}`).value = Math.floor(((k+1)*25600)/(length));
				}
				else{
					if(k == PageWrite_Num - 1)
					layer.msg("Download Progress: " +  "100%"); 
					else
					layer.msg("Download Progress: " +  Math.floor(((k+1)*25600)/(length))+"%");
				}
			}
		}
	}
	msecond2 = new Date();
	console.log("Total Time:  "+ parseInt(msecond2-msecond1) +"  ms");

	await crc32_check_look_table(0xffffffff,data,length);
	await usb_evk_dut_cmd(CHIP_8266,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_CRC ,statr_addr,length,TL_ModeTypdef.USB);
	if(crc_ret != crc_check) {
		console.log(" file check err!");
		console.log("crc_ret:  " + crc_ret.toString(16));
		console.log("crc_check:  " + crc_check.toString(16));
		return ERROR_Typdef.ECRC;
	}
	console.log(" Update FW succeed! ");
	return 0;
}
// fw_name_to_array   取前10个字符，不够补0 第十一个写0
function fw_name_to_array(name_str) {
    let arr = [];
    for (let i = 0; i < 10; ++i) {
		if(i < name_str.length)
        	arr.push(name_str.charCodeAt(i));
		else arr.push(0);
    }
	arr.push(0);
    let tmpUint8Array = new Uint8Array(arr);
    return tmpUint8Array;
}

function offline_fw_table_add_cb(rowId){
	var offline_chip_select_name_ = document.getElementById(`offline_chip_select_name_${rowId}`);
	layui.use('dropdown', function() {
		var dropdown = layui.dropdown
		dropdown.render({
		elem: `#offline_chip_select_${rowId}`
		,data: [
			{
				title: '83xx'
				,id: 8300
				,child: [{
					title: '8366'
					,id: 8366
				},{
					title: '8368'
					,id: 8368
				},{
					title: '8367_i'
					,id: 83671
				},{
					title: '8367_e'
					,id: 83672
				},{
					title: '8369_i'
					,id: 83691
				},{
					title: '8369_e'
					,id: 83692
				}]
			},
			{
				title: '82xx'
				,id: 8200
				,child: [{
				title: '8232'
				,id: 8232
				},{
					title: '8266'
					,id: 8266
				},{
					title: '8267'
					,id: 8267
				},{
					title: '8269'
					,id: 8269
				}]
			},
			{
				title: 'B80'
				,id: 80
			},
			{
				title: 'B85'
				,id: 85
			},
			{
				title: 'B87'
				,id: 87
			},
			{
				title: 'B89'
				,id: 89
			},
			{
				title: 'B91'
				,id: 91
			},
			// {
			// 	title: 'B92'
			// 	,id: 92
			// },
		]
		,id: `offline_chip_select_${rowId}`
		//菜单被点击的事件
		,click: function(obj) {
			LogConsole(obj,"backstage");
			switch(obj.id) {
				case 8366:    
					offline_chip_select_name_.innerHTML = '8366';
					OffLineChipSelect[rowId] = C_8366;  
				break;
				case 8368:
					//htmlStr += '<i class = "layui-icon layui-icon-down layui-font-12"></i>'; 
					offline_chip_select_name_.innerHTML = '8368';
					OffLineChipSelect[rowId] = C_8368;  	
				break;
				case 83671:
					offline_chip_select_name_.innerHTML = '8367_i';
					OffLineChipSelect[rowId] = C_8367_i;   
				break;
				case 83672: 	
					offline_chip_select_name_.innerHTML = '8367_e';
					OffLineChipSelect[rowId] = C_8367_e;  
				break;
				case 83691: 	
					offline_chip_select_name_.innerHTML = '8369_i';
					OffLineChipSelect[rowId] = C_8369_i;  
				break;
				case 83692:
					offline_chip_select_name_.innerHTML = '8369_e';
					OffLineChipSelect[rowId] = C_8369_e;  
				break;
				case 8232:  	
					offline_chip_select_name_.innerHTML = '8232';
					OffLineChipSelect[rowId] = C_8232; 	
				break;

				case 8266:    
					offline_chip_select_name_.innerHTML = '8266';
					OffLineChipSelect[rowId] = C_8266; 
				break;
				case 8267:
					offline_chip_select_name_.innerHTML = '8267'; 
					OffLineChipSelect[rowId] = C_8267; 
				break;
				case 8269:
					offline_chip_select_name_.innerHTML = '8269';
					OffLineChipSelect[rowId] = C_8269; 
				break;

				case 80:    
					offline_chip_select_name_.innerHTML = 'B80';
					OffLineChipSelect[rowId] = C_B80; 
				break;
				case 85:    
					offline_chip_select_name_.innerHTML = 'B85'; 
					OffLineChipSelect[rowId] = C_B85;
				break;
				case 87:  	
					offline_chip_select_name_.innerHTML = 'B87'; 
					OffLineChipSelect[rowId] = C_B87; 
				break;
				case 89:  	
					offline_chip_select_name_.innerHTML = 'B89'; 
					OffLineChipSelect[rowId] = C_B89; 
				break;

				case 91:    
					offline_chip_select_name_.innerHTML = 'B91';  
					OffLineChipSelect[rowId] = C_B91; 	
				break;
				case 92:    
					offline_chip_select_name_.innerHTML = 'B92';  
					OffLineChipSelect[rowId] = C_B92; 	
				break;
				default: 
					LogConsole("Wrong chip model selection"); 
				break;
			}
			layer.msg("The selected chip type is: " + C_Name[OffLineChipSelect[rowId]] ); 
			var Type = get_Offline_ChipType_Chip_RAM_Type(OffLineChipSelect[rowId]);
			OffLineChipType[rowId] = Type[0]; 	 // 固件适配的目标芯片 1B
			OffLineChip_RAM_Type[rowId] = Type[1]; // 工具固件适配的目标芯片 1B
		}});
	});

	layui.use('dropdown', function() {
		var dropdown = layui.dropdown
		dropdown.render({
			elem: `#offline_fw_type_select_${rowId}`
			,data: [
				{
					title: 'FW_RAM'
					,id: 2
				},
				{
					title: 'FW_FLASH'
					,id: 3
				},
			]
			,id: `offline_fw_type_select_${rowId}`
			//菜单被点击的事件
			,click: function(obj) {
				LogConsole(obj,"backstage");
				document.getElementById(`offline_fw_type_select_name_${rowId}`).innerHTML = obj.title; 
				OffLineFirmware_Type[rowId] = obj.id; // 固件类型 1B	
				layer.msg("The selected offline_fw_type is: " + obj.title); 
			}
		});
	});
}

function removeRow(row) {
    const rowIndex = Array.from(fw_bin_table.rows).indexOf(row);
    fw_bin_table.deleteRow(rowIndex);
	var rowCount = fw_bin_table.rows.length;
	console.log("removeRow rowCount:"+rowCount);
	OffLineFirmware_Flag[rowCount-1] = 0x00;// 固件是否有效标志位 1B
	next_fw_store_addr = OffLineFirmware_Store_Addr[rowCount - 1];// 固件存放在EVK FLASH处的的起始地址（相对位移）
}

function offline_get_input(){
	var rowCount = fw_bin_table.rows.length;
	var offline_fw_name;
	var offline_fw_store_addr;
	var offline_fw_size;
	var offline_fw_download_addr;
	console.log("rowCount: " + rowCount);
	if(rowCount > 1){
		for (var i = 0; i < rowCount -1; i++) {
			offline_fw_name = document.getElementById(`offline_fw_name_${i}`).value;
			if(offline_fw_name=="")offline_fw_name="no name";
			offline_fw_store_addr = document.getElementById(`offline_fw_store_addr_${i}`).innerHTML;
			offline_fw_size = document.getElementById(`offline_fw_size_${i}`).innerHTML;
			offline_fw_download_addr = document.getElementById(`offline_fw_download_addr_${i}`).value;
			console.log("==============================");
			console.log("name:"+offline_fw_name+"  store:"+offline_fw_store_addr + " size:"+offline_fw_size + "  addr:"+offline_fw_download_addr);
			OffLineFirmware_Name[i] = fw_name_to_array(offline_fw_name); // 固件名称 11B /0
			OffLineFirmware_Download_Addr[i] = hex2int(offline_fw_download_addr); // 固件下载到目标芯片处的地址 相对地址 4B
		}
	}
}

function print_offline_fw_info(index){
	var fw_name = document.getElementById(`offline_fw_name_${index}`).value;
	var Type = get_Offline_ChipType_Chip_RAM_Type(OffLineChipSelect[index]);
	OffLineChipType[index] = Type[0]; // 固件适配的目标芯片 1B
	OffLineChip_RAM_Type[index] = Type[1]; // 工具固件适配的目标芯片 1B
	OffLineFirmware_Name[index] = fw_name_to_array(fw_name); // 固件名称 11B /0
	OffLineFirmware_Download_Addr[index] = hex2int(document.getElementById(`offline_fw_download_addr_${index}`).value);// 固件下载到目标芯片处的地址 相对地址 4B

	console.log("==============================");
	console.log("OffLineChipType: "+ OffLineChipType[index]); // 固件适配的目标芯片 1B
	console.log("OffLineChip_RAM_Type: "+ OffLineChip_RAM_Type[index]); // 工具固件适配的目标芯片 1B
	console.log("OffLineFirmware_Type: "+ OffLineFirmware_Type[index]); // 固件类型 1B		
	console.log("OffLineFirmware_Version: "+ OffLineFirmware_Version[index]);// 固件版本 1
	console.log("OffLineFirmware_Name: "+ OffLineFirmware_Name[index]);// 固件名称 11B /0
	console.log("OffLineFirmware_Flag: "+ OffLineFirmware_Flag[index]);// 固件是否有效标志位 1B bit15
	console.log("OffLineFirmware_Store_Addr: "+ OffLineFirmware_Store_Addr[index]);// 固件存放在EVK FLASH处的的起始地址（相对位移） 4B
	console.log("OffLineFirmware_Size: "+ OffLineFirmware_Size[index]);// 固件大小 4B bit20-bit23
	console.log("OffLineFirmware_Download_Addr: 0x"+ OffLineFirmware_Download_Addr[index]);// 固件下载到目标芯片处的地址 相对地址 4B
	console.log("OffLineFirmware_CRC: "+ OffLineFirmware_CRC[index]);// 固件CRC值 4B bit28-bit31
	console.log("==============================");

	let arr = [];
	arr.push(OffLineChipType[index]);
	arr.push(OffLineChip_RAM_Type[index]);
	arr.push(OffLineFirmware_Type[index]);
	arr.push(OffLineFirmware_Version[index]);
    for (let i = 0; i < 11; ++i) {
        arr.push(OffLineFirmware_Name[index][i]);
    }
	arr.push(OffLineFirmware_Flag[index]); // 15

	arr.push((OffLineFirmware_Store_Addr[index])&0xff);//16
	arr.push((OffLineFirmware_Store_Addr[index]>>8)&0xff);
	arr.push((OffLineFirmware_Store_Addr[index]>>16)&0xff);
	arr.push((OffLineFirmware_Store_Addr[index]>>24)&0xff);

	arr.push((OffLineFirmware_Size[index])&0xff);//20
	arr.push((OffLineFirmware_Size[index]>>8)&0xff);
	arr.push((OffLineFirmware_Size[index]>>16)&0xff);
	arr.push((OffLineFirmware_Size[index]>>24)&0xff);
	
	arr.push((OffLineFirmware_Download_Addr[index])&0xff);//24
	arr.push((OffLineFirmware_Download_Addr[index]>>8)&0xff);
	arr.push((OffLineFirmware_Download_Addr[index]>>16)&0xff);
	arr.push((OffLineFirmware_Download_Addr[index]>>24)&0xff);
	
	arr.push((OffLineFirmware_CRC[index])&0xff);//28 
	arr.push((OffLineFirmware_CRC[index]>>8)&0xff);
	arr.push((OffLineFirmware_CRC[index]>>16)&0xff);
	arr.push((OffLineFirmware_CRC[index]>>24)&0xff);

    let tmpUint8Array = new Uint8Array(arr);
    return tmpUint8Array;
}

async function OffLineFWBinFileRead() {
//  document.getElementById('UserFwFileTable').rows.length
	var rowCount = fw_bin_table.rows.length;
	console.log("OffLineFWBinFileRead");	
	console.log("rowCount:" + rowCount);
	if(rowCount > OffLineDownloadNumMax){
		layer.msg("The number of firmware loaded exceeds the maximum limit of " + OffLineDownloadNumMax); 
		return ERROR_Typdef.ENOMEM;
	}
    var row = fw_bin_table.insertRow(rowCount);
	rowCount = rowCount - 1;
	// document.getElementById('offline_chip_select_1')
    var cell0 = row.insertCell(0);
	cell0.innerHTML = `
	<button class = "layui-btn layui-btn-sm layui-btn-radius" id = "offline_chip_select_${rowCount}">
		<span class = "iconfont icon-xinpian"></span>
		<i id = "offline_chip_select_name_${rowCount}">B91</i> 
		<i class = "layui-icon layui-icon-down layui-font-12"></i> 
	</button>`;
	// document.getElementById('offline_fw_type_select_0')
    var cell1 = row.insertCell(1);
	cell1.innerHTML = `
	<button class = "layui-btn layui-btn-sm layui-btn-radius" id = "offline_fw_type_select_${rowCount}">
		<span class = "iconfont icon-xinpian"></span>
		<i id = "offline_fw_type_select_name_${rowCount}">FW_FLASH</i> 
		<i class = "layui-icon layui-icon-down layui-font-12"></i> 
	</button>`;
	// document.getElementById('offline_fw_download_addr_0')
    var cell2 = row.insertCell(2);
	cell2.innerHTML = `<input class = "offline_fw_name_input" type = "text" id = "offline_fw_name_${rowCount}" placeholder = "Max length:10" />`;
	//  document.getElementById('offline_fw_store_addr_0').innerHTML
	var cell3 = row.insertCell(3);
	cell3.innerHTML = `<span id = "offline_fw_store_addr_${rowCount}">0x0</span>`;
 	//  document.getElementById('offline_fw_size_0').innerHTML
	var cell4 = row.insertCell(4);
	cell4.innerHTML = `<span id = "offline_fw_size_${rowCount}">0</span>`;
	// document.getElementById('offline_fw_download_addr_0')
	var cell5 = row.insertCell(5);
	cell5.innerHTML = `0x<input class = "offline_fw_name_input" type = "text" id = "offline_fw_download_addr_${rowCount}" value = "0"/>`;
	
	// document.getElementById('offline_fw_download_progress_0').value  style="background-color:'red';"
	var cell6 = row.insertCell(6);
	cell6.innerHTML = `<progress value="0" max="100" id = "offline_fw_download_progress_${rowCount}"></progress>`;
	offline_fw_table_add_cb(rowCount);
/***************************************************************************************************** */
	var fw_name;
	var localFile = document.getElementById("OffLineFWBinFileInput").files[0];
	console.log(localFile);
	var reader = new FileReader();
	reader.readAsArrayBuffer(localFile);
	reader.onload = function() {
		var bytes = new Uint8Array(reader.result);  
 		var date = new Date(localFile.lastModified);
		dateTime = date.toLocaleString();
		console.log(localFile.name + "	:  " + dateTime);
		g_offline_fw_data[rowCount] = bytes;
		OffLineFirmware_Size[rowCount] = bytes.length;
		console.log("file read end...","backstage");
		document.getElementById('OffLineFWBinFileInput').value = null;
 		document.getElementById('OffLineFWBinFileImport').innerHTML = '<i class = "layui-icon layui-icon-export" ></i>'  + ' Add ';
		if(localFile.name.length > 14){
			fw_name = localFile.name.substr(0,10);
		}
		else{
			fw_name = localFile.name.substr(0,localFile.name.length-4);
		}
		console.log('fw_name: ' + fw_name + "  fileSize:" + OffLineFirmware_Size[rowCount]);

		document.getElementById(`offline_fw_name_${rowCount}`).value = fw_name;
		document.getElementById(`offline_fw_store_addr_${rowCount}`).innerHTML = "0x" + dec2hex2(next_fw_store_addr);
		document.getElementById(`offline_fw_size_${rowCount}`).innerHTML = OffLineFirmware_Size[rowCount];

		var Type = get_Offline_ChipType_Chip_RAM_Type(C_B91); 	    // 默认芯片型号为B91 FLASH固件
		OffLineChipType[rowCount] = Type[0]; 	   					// 固件适配的目标芯片 1B
		OffLineChip_RAM_Type[rowCount] = Type[1];  					// 工具固件适配的目标芯片 1B
		OffLineFirmware_Type[rowCount] = 3; 	   					// 固件类型 1B		
		OffLineFirmware_Version[rowCount] = 0x12;  					// 固件版本 1B 可以根据编译时间定
		OffLineFirmware_Name[rowCount] = fw_name_to_array(fw_name); // 固件名称 11B /0
		OffLineFirmware_Flag[rowCount] = 0x01;	   		    		// 固件是否有效标志位 1B
		OffLineFirmware_Store_Addr[rowCount] = next_fw_store_addr;	// 固件存放在EVK FLASH处的的起始地址（相对位移） 4B
		//OffLineFirmware_Size[rowCount] = localFile.size;  		// 固件大小 4B  g_offline_fw_len[rowCount]
		OffLineFirmware_Download_Addr[rowCount] =  hex2int(document.getElementById(`offline_fw_download_addr_${rowCount}`).value); // 固件下载到目标芯片处的地址 相对地址 4B
		crc32_check_look_table(0xffffffff,g_offline_fw_data[rowCount],OffLineFirmware_Size[rowCount]);// 固件CRC值 4B
		OffLineFirmware_CRC[rowCount] = crc_ret;
		// LogConsole("rowCount:  " + rowCount);
		// LogConsole("OffLineFirmware_CRC:  " + OffLineFirmware_CRC[rowCount]);
		// LogConsole("OffLineFirmware_CRC:  0x" + OffLineFirmware_CRC[rowCount].toString(16));
		next_fw_store_addr = next_fw_store_addr + OffLineFirmware_Size[rowCount];
		if(next_fw_store_addr%0x1000){
			next_fw_store_addr = (Math.floor(next_fw_store_addr/0x1000)+1)*0x1000;
		}
		if(next_fw_store_addr >= g_offline_fw_data_max){ // 加载的固件总大小超过了FLASH范围了
			layer.msg("The total size of firmware loaded exceeds the FLASH range"); 
			removeRow(fw_bin_table.rows.length);
			return EFBIG;
		}
	}
	return 0;
}

offline_del_user_bin.onclick = async () => {
	console.log("offline_del_user_bin");
	var rowCount = fw_bin_table.rows.length;
	if(rowCount > 1) removeRow(rowCount);
}

offline_download_user_bin.onclick = async () => {
	console.log("offline_download_user_bin");
	let rowCount = fw_bin_table.rows.length;
	for (let m = 0; m < 4096; m++) {
		FW_INFO_DATA[m] = 0;
	}
	
	let ret = await unlock_flash_of_evk();
	console.log("unlock_flash_of_evk ret:",ret);
	if (ret != 0) {
		layer.msg("Download Failed"); 
		return ERROR_Typdef.ELOCKFLASH;
	}

	offline_get_input();
	console.log("==========");

	FW_INFO_DATA[31] = rowCount - 1;
	for(var k = 0; k < rowCount - 1; k++){
		let showlog = print_offline_fw_info(k);
		console.log(showlog);
		console.log("==========");
		for(var j = 0; j < 32; j++){
			FW_INFO_DATA[k*32+j+32] =  showlog[j];
		}
	}

	await crc32_check_look_table(0xffffffff,FW_INFO_DATA.slice(4),4092);
	FW_INFO_DATA[0] = (crc_ret)&0xff;
	FW_INFO_DATA[1] = (crc_ret>>8)&0xff;
	FW_INFO_DATA[2] = (crc_ret>>16)&0xff;
	FW_INFO_DATA[3] = (crc_ret>>24)&0xff;
	console.warn("crc_ret:  " + crc_ret);
	console.warn("crc_ret:  " + crc_ret.toString(16));
 
	ret = await download_bin_to_evk_flash(FW_INFO_DATA,4096,FW_INFO_ADDR);
	if (ret != 0) {
		await lock_flash_of_evk();
		layer.msg("Download Failed"); 
		return ERROR_Typdef.ELOCKFLASH;
	}

	for(var i = 0; i < rowCount - 1; i++){
		layer.msg("Downloading NO."+ (i+1) + "firmware"); 
		ret = await download_bin_to_evk_flash(g_offline_fw_data[i],OffLineFirmware_Size[i],FW_START_ADDR+OffLineFirmware_Store_Addr[i],i);
		if (ret != 0) {
			await lock_flash_of_evk();
			layer.msg("Download Failed"); 
			document.getElementById(`offline_fw_download_progress_${i}`).value = 0;
			return ERROR_Typdef.ELOCKFLASH;
		}
	}

	ret = await lock_flash_of_evk();
	console.log("lock_flash_of_evk ret:",ret);
	if (ret != 0) {
		layer.msg("Download Failed"); 
		return ERROR_Typdef.ELOCKFLASH;
	}
	layer.msg("Download Successed"); 
	return 0;
}

/*
function print_offline_tool_bin_info(index){
	let arr = [];
	for (let i = 0; i < 16; i++) {
		arr.push(0);
	}
	arr.push((OffLine_TOOL_Firmware_Store_Addr[index])&0xff);//16
	arr.push((OffLine_TOOL_Firmware_Store_Addr[index]>>8)&0xff);
	arr.push((OffLine_TOOL_Firmware_Store_Addr[index]>>16)&0xff);
	arr.push((OffLine_TOOL_Firmware_Store_Addr[index]>>24)&0xff);
	arr.push((OffLine_TOOL_Firmware_Size[index])&0xff);//20
	arr.push((OffLine_TOOL_Firmware_Size[index]>>8)&0xff);
	arr.push((OffLine_TOOL_Firmware_Size[index]>>16)&0xff);
	arr.push((OffLine_TOOL_Firmware_Size[index]>>24)&0xff);
	arr.push(0);arr.push(0);arr.push(0);arr.push(0);
	arr.push((OffLineFirmware_CRC[index])&0xff);//28
	arr.push((OffLineFirmware_CRC[index]>>8)&0xff);
	arr.push((OffLineFirmware_CRC[index]>>16)&0xff);
	arr.push((OffLineFirmware_CRC[index]>>24)&0xff);
	let tmpUint8Array = new Uint8Array(arr);
    return tmpUint8Array;
}

function list_offline_tool_firmware(showtable){
	if(OnceTimeLoadToolBin){
		OnceTimeLoadToolBin = false;
		for (let m = 0; m < 4096; m++) {
			RAM_TOOL_INFO_DATA[m] = 0;
		}
		RAM_TOOL_INFO_DATA[31] = ChipNumber - 1;

		for(let i = 1; i < ChipNumber; i++){
			console.log("Name:"+C_Name[i]+"  Size:"+LoadBinary[i].length);
			OffLine_TOOL_Firmware_Size[i] = LoadBinary[i].length;
			if(1==i) OffLine_TOOL_Firmware_Store_Addr[i] = 0;
			else OffLine_TOOL_Firmware_Store_Addr[i] = OffLine_TOOL_Firmware_Store_Addr[i-1] + OffLine_TOOL_Firmware_Size[i-1];// + 1;
			g_offline_ram_tool_data.push(...LoadBinary[i]);

			let showlog = print_offline_tool_bin_info(i);
			console.log(showlog);
			console.log("==========");
			for(let k = 0; k < 32; k++){
				RAM_TOOL_INFO_DATA[i*32+k] =  showlog[k];
			}
		}

		if(OffLine_TOOL_Firmware_Store_Addr[ChipNumber-1] + OffLine_TOOL_Firmware_Size[ChipNumber-1] >= g_offline_ram_tool_data_max){ // 加载的固件总大小超过了给定的FLASH范围了
			layer.msg("The total firmware size exceeds the given space size"); 
			return ERROR_Typdef.ENOMEM;
		}
	}
	
	if(showtable){
		if(OnceTimeShowToolBinTable){
			OnceTimeShowToolBinTable = false;
			for(let j = 1; j < ChipNumber; j++){
				var row = tool_bin_table.insertRow(j);

				let value_hex = dec2hex2(OffLine_TOOL_Firmware_Store_Addr[j]);

				var cell0 = row.insertCell(0);
				cell0.innerHTML = `<span">${j}</span>`;

				var cell1 = row.insertCell(1); 
				cell1.innerHTML = `<span>${C_Name[`${j}`]}</span>`;

				var cell2 = row.insertCell(2);// "0x" + dec2hex2(next_fw_store_addr);
				cell2.innerHTML = `<span>${OffLine_TOOL_Firmware_Store_Addr[`${j}`]}  (0x${value_hex})
				</span>`;

				var cell3 = row.insertCell(3);
				cell3.innerHTML = `<span>${OffLine_TOOL_Firmware_Size[`${j}`]}</span>`;
			}
		}
	}
	return 0;
}

list_offline_tool_firmware(false);
offline_list_ram_tool_bin.onclick = async () => {
	console.log("offline_list_ram_tool_bin");
	list_offline_tool_firmware(true);
}

offline_download_ram_tool_bin.onclick = async () => {
	console.log("offline_download_ram_tool_bin");
	layer.msg("Downloading please wait"); 
	let ret = await unlock_flash_of_evk();
	console.log("unlock_flash_of_evk ret:",ret);
	if (ret != 0) {
		layer.msg("Download Failed"); 
		return ERROR_Typdef.ELOCKFLASH;
	}

	await crc32_check_look_table(0xffffffff,g_offline_ram_tool_data,g_offline_ram_tool_data.length);
	console.warn("crc_ret:  " + crc_ret);
	console.warn("crc_ret:  " + crc_ret.toString(16));
	RAM_TOOL_INFO_DATA[4] = (crc_ret)&0xff;
	RAM_TOOL_INFO_DATA[5] = (crc_ret>>8)&0xff;
	RAM_TOOL_INFO_DATA[6] = (crc_ret>>16)&0xff;
	RAM_TOOL_INFO_DATA[7] = (crc_ret>>24)&0xff;

	RAM_TOOL_INFO_DATA[8] = (g_offline_ram_tool_data.length)&0xff;
	RAM_TOOL_INFO_DATA[9] = (g_offline_ram_tool_data.length>>8)&0xff;
	RAM_TOOL_INFO_DATA[10] = (g_offline_ram_tool_data.length>>16)&0xff;
	RAM_TOOL_INFO_DATA[11] = (g_offline_ram_tool_data.length>>24)&0xff;

	await crc32_check_look_table(0xffffffff,RAM_TOOL_INFO_DATA.slice(4),4092);
	RAM_TOOL_INFO_DATA[0] = (crc_ret)&0xff;
	RAM_TOOL_INFO_DATA[1] = (crc_ret>>8)&0xff;
	RAM_TOOL_INFO_DATA[2] = (crc_ret>>16)&0xff;
	RAM_TOOL_INFO_DATA[3] = (crc_ret>>24)&0xff;
	console.warn("crc_ret:  " + crc_ret);
	console.warn("crc_ret:  " + crc_ret.toString(16));

	ret = await download_bin_to_evk_flash(RAM_TOOL_INFO_DATA,4096,RAM_TOOL_INFO_ADDR);
	if (ret != 0) {
		await lock_flash_of_evk();
		layer.msg("Download Failed"); 
		return ERROR_Typdef.ELOCKFLASH;
	}

	ret = await download_bin_to_evk_flash(g_offline_ram_tool_data,g_offline_ram_tool_data.length,RAM_TOOL_START_ADDR,OffLineDownloadNumMax);
	if (ret != 0) {
		await lock_flash_of_evk();
		layer.msg("Download Failed"); 
		return ERROR_Typdef.ELOCKFLASH;
	}

	ret = await lock_flash_of_evk();
	console.log("lock_flash_of_evk ret:",ret);
	if (ret != 0) {
		layer.msg("Download Failed"); 
		return ERROR_Typdef.ELOCKFLASH;
	}
	layer.msg("Download Successed"); 
	return 0;
}
*/
/********************************************************************/

SWSButton.onclick = async () => {
 	console.log("SWSButton");
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
		var sws_config = new Array();
		sws_config[0] = parseInt("0x"+input1.value); sws_config[1] = parseInt("0x"+input2.value);
		sws_config[2] = parseInt("0x"+input3.value); sws_config[3] = parseInt("0x"+input4.value);
		await usb_evk_sws_mcu(ChipType,sws_config);
		BurningEVKBusy = 0;
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY;
	}
	return 0;
}

StallButton.onclick = async () => {
	console.log("StallButton");
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
		SingleStepFlag = 1;
 		await usb_evk_stall_mcu(TL_ModeTypdef.EVK,ChipType,parseInt("0x"+input5.value),parseInt("0x"+input6.value));
		 BurningEVKBusy = 0;
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY;
	}
	return 0;
}

StartButton.onclick = async () => {
	console.log("StartButton");
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
		await usb_evk_start_mcu(TL_ModeTypdef.EVK,ChipType,parseInt("0x"+input7.value),parseInt("0x"+input8.value));
		BurningEVKBusy = 0;
	}
	else {
		LogConsole("BurningEVK is Busy");
	}
	return 0;
}	

EraseButton.onclick = async () => {
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
		var msecond1;
		var msecond2;
		msecond1 = new Date();
		console.log("EraseButton");
		//LogConsole("EraseSizeInput.value/4,:"+EraseSizeInput.value/4);
		await SectorEraseFlash(TL_ModeTypdef.EVK,ChipType,hex2int(EraseAddrInput.value),EraseSizeInput.value/4,flash_erase_callback);
		msecond2 = new Date();
		LogConsole("Total Time:  "+ parseInt(msecond2-msecond1) +"  ms");
		BurningEVKBusy = 0; 
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY;
	}
	return 0;
}

function flash_erase_callback(page_adr) {
	LogConsole("Flash Sector (4K) Erase at address: 0x"+dec2hex(page_adr,6));
}

ActivateButton.onclick = async () => {
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
	 	console.log("ActivateButton");
	 	await usb_evk_activate_mcu(ChipType);
		BurningEVKBusy = 0; 
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY;
	}
	return 0;
}

RunButton.onclick = async () => {
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
	    console.log("RunButton");
		await usb_evk_run_mcu(TL_ModeTypdef.EVK,ChipType);
		BurningEVKBusy = 0; 
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY;
	}
	return 0;
}

PauseButton.onclick = async () => {
	console.log("PauseButton");
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
		SingleStepFlag = 1;
	 	await usb_evk_pause_mcu(TL_ModeTypdef.EVK,ChipType);
		BurningEVKBusy = 0; 
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY;
	}
	return 0;
}

StepButton.onclick = async () => {
	console.log("StepButton");
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
		 if(0 == SingleStepFlag) {
			while(0 == SingleStepFlag) {
				await usb_evk_step_mcu(TL_ModeTypdef.EVK,ChipType);
				await sleep(200);
			}
		}
		else {
			await usb_evk_step_mcu(TL_ModeTypdef.EVK,ChipType);
		}
		BurningEVKBusy = 0; 
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY;
	}
	return 0;
}

PCButton.onclick = async () => {
	 console.log("PCButton");
	 if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
		if(0 == SingleStepFlag) {
			while(0 == SingleStepFlag) {
				await usb_evk_getpc_mcu(TL_ModeTypdef.EVK,ChipType);
				await sleep(200);
			}
		}
		else {
			await usb_evk_getpc_mcu(TL_ModeTypdef.EVK,ChipType);
		}
		BurningEVKBusy = 0; 
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY;
	}
	return 0;
}
 
ResetButton.onclick = async () => {
	console.log("ResetButton");
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
	    await usb_evk_reset_mcu(TL_ModeTypdef.EVK,ChipType,LoadOBJChoose);
		BurningEVKBusy = 0; 
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY;
	}
	return 0;
}
 
ClearButton.onclick = async () => {
	 console.log("ClearButton");
 	 var ele1 = document.getElementById("Console1");
	 var ele2 = document.getElementById("Console2");
	 var ele3 = document.getElementById("Console3");
	 var ele4 = document.getElementById("Console4");
	 var ele5 = document.getElementById("Console5");
	 var ele6 = document.getElementById("Console6");
	 ele1.value = ""; 
	 ele2.value = "";
	 ele3.value = "";
	 ele4.value = "";  
	 ele5.value = "";  
	 ele6.value = "";  
};

usbConnectButton.onclick = async () => {
	//if(USBConnectedFlag == -1) {
		if(0 == BurningEVKBusy) {
			BurningEVKBusy = 1;
			await usb_connect();
			BurningEVKBusy = 0;
		}
		else {
			LogConsole("BurningEVK is Busy");
			return ERROR_Typdef.EBUSY;
		}
	/*}
	else {
		LogConsole("USB is Linked");
		layer.msg ("USB is Linked");
	}*/
	return 0;
}
 
DownloadButton.onclick = async () => {
	await LogConsole("DownloadButton","backstage");
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
		if(TL_ObjTypdef.FLASH == LoadOBJChoose) {
			await BinDownload(g_file_data,g_file_length,hex2int(DownloadAddrInput.value));
		}
		else if(TL_ObjTypdef.CORE == LoadOBJChoose) {
			await BinDownload(g_file_data,g_file_length,hex2int(SramDownloadAddrInput.value));
		}
 		BurningEVKBusy = 0;
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY;
	}
	return 0;
}

FwDownloadButton1.onclick = async () => {
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
 		await DownloadBin(CHIP_8266,"./bin/fw/V3.6.bin","BurningEVKFirmware");
 	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY;
	}
	return 0;
}

FwDownloadButton2.onclick = async () => {
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
 		await DownloadBin(CHIP_8266,"./bin/fw/V3.7.bin","BurningEVKFirmware");
 	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY;
	}
	return 0;
}

MultiDownloadButton.onclick = async () => {
	await LogConsole("MultiDownloadButton","backstage");
	if(USBConnectedFlag != 1) {
		await LogConsole("USB device not connected!");
		return ERROR_Typdef.EPERM;
	}
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
		var ret = -1;
		var show = "";
 		if(true == MultiCheck1.checked) {
			if(MultiBinFileLen1 > 0) {
 				ret = await BinDownload(MultiBinFileData1,MultiBinFileLen1,hex2int(DownloadAddrInput1.value),"1");
				if(0 == ret) {
					await LogConsole("Address1 Download Success");
					await sleep(20);
					show += "\r\n FileName:  "+  MultiBinFileName1 +"  adr1:  0x"+DownloadAddrInput1.value+"  size:  " + MultiBinFileLen1 +"B\r\n";
				}
				else {
					await LogConsole("Address1 Download Filed!");
					await LogConsole(" Download  terminated!");
					return ERROR_Typdef.EAGAIN;
				}
			} 
			else {
				await LogConsole("Skip, File not selected");
			}
		}
    	if(false == MultiCheck1.checked && true == MultiCheck6.checked && 0 == NetWorkBusy) {
			NetWorkBusy = 1;
		    await DownloadBin(ChipType,ServerFilePath[0],ServerFileAttribute[0],DownloadAddrInput1.value);
			await sleep(20);
 			show += "\r\n FileName:  "+  ServerFileName[0] +"  adr1:  0x"+DownloadAddrInput1.value+"  size:  " + ServerFileLength[0] +"B\r\n";
 		}
		while(NetWorkBusy) {
			await sleep(2);
 		}
		if(true == MultiCheck2.checked) {
			if(MultiBinFileLen2 > 0) {
				ret = 	await BinDownload(MultiBinFileData2,MultiBinFileLen2,hex2int(DownloadAddrInput2.value),"1");
				if(0 == ret) {
					await LogConsole("Address2 Download Success");
					await sleep(20);
					show += "\r\n FileName:  "+  MultiBinFileName2 +"  adr2:  0x"+DownloadAddrInput2.value+"  size:  " + MultiBinFileLen2 +"B\r\n";
				}
				else {
					await LogConsole("Address2 Download Filed!");
					await LogConsole(" Download  terminated!");
					return ERROR_Typdef.EAGAIN;
				}
			} 
			else {
				await LogConsole("Skip, File not selected");
			}
		}

		if(false == MultiCheck2.checked && true == MultiCheck7.checked && 0 == NetWorkBusy) {
		   NetWorkBusy = 1;
		   await DownloadBin(ChipType,ServerFilePath[1],ServerFileAttribute[1],DownloadAddrInput2.value);
		   await sleep(20);
 		   show += "\r\n FileName:  "+  ServerFileName[1] +"  adr2:  0x"+DownloadAddrInput2.value+"  size:  " + ServerFileLength[1] +"B\r\n";
 		}
		while(NetWorkBusy) {
			await sleep(2);
 		}
		if(true == MultiCheck3.checked) {
			if(MultiBinFileLen3 > 0) {
				ret = await BinDownload(MultiBinFileData3,MultiBinFileLen3,hex2int(DownloadAddrInput3.value),"1");
				if(0 == ret) {
					await LogConsole("Address3 Download Success");
					await sleep(20);
					show += "\r\n FileName:  "+  MultiBinFileName3 +"  adr3:  0x"+DownloadAddrInput3.value+"  size:  " + MultiBinFileLen3 +"B\r\n";
				}
				else {
					await LogConsole("Address3 Download Filed!");
					await LogConsole(" Download  terminated!");
					return ERROR_Typdef.EAGAIN;
				}
			} 
			else {
				await LogConsole("Skip, File not selected");
			}
		}

		if(false == MultiCheck3.checked && true == MultiCheck8.checked && 0 == NetWorkBusy) {
		   NetWorkBusy = 1;
		   await DownloadBin(ChipType,ServerFilePath[2],ServerFileAttribute[2],DownloadAddrInput3.value);
		   await sleep(20);
 		   show += "\r\n FileName:  "+  ServerFileName[2] +"  adr3:  0x"+DownloadAddrInput3.value+"  size:  " + ServerFileLength[2] +"B\r\n";
	   }
	    while(NetWorkBusy) {
			await sleep(2);
 		}
		if(true == MultiCheck4.checked) {
			if(MultiBinFileLen4 > 0) {
				ret = await BinDownload(MultiBinFileData4,MultiBinFileLen4,hex2int(DownloadAddrInput4.value),"1");
				if(0 == ret) {
					await LogConsole("Address4 Download Success");
					await sleep(20);
					show += "\r\n FileName:  "+  MultiBinFileName4 +"  adr4:  0x"+DownloadAddrInput4.value+"  size:  " + MultiBinFileLen4 +"B\r\n";
				}
				else {
					await LogConsole("Address4 Download Filed!");
					await LogConsole(" Download  terminated!");
					return ERROR_Typdef.EAGAIN;
				}
			} 
			else {
				await LogConsole("Skip, File not selected");
			}
		}
 
		if(false == MultiCheck4.checked && true == MultiCheck9.checked && 0 == NetWorkBusy) {
		   NetWorkBusy = 1;
		   await DownloadBin(ChipType,ServerFilePath[3],ServerFileAttribute[3],DownloadAddrInput4.value);
		   await sleep(20);
 		   show += "\r\n FileName:  "+  ServerFileName[3] +"  adr4:  0x"+DownloadAddrInput4.value+"  size:  " + ServerFileLength[3] +"B\r\n";
	   }
	   while(NetWorkBusy) {
			await sleep(2);
 		}
		if(true == MultiCheck5.checked) {
			if(MultiBinFileLen5 > 0) {
				ret = await BinDownload(MultiBinFileData5,MultiBinFileLen5,hex2int(DownloadAddrInput5.value),"1");
				if(0 == ret) {
					await LogConsole("Address5 Download Success");
					await sleep(20);
					show += "\r\n FileName:  "+  MultiBinFileName5 +"  adr5:  0x"+DownloadAddrInput5.value+"  size:  " + MultiBinFileLen5 +"B\r\n";
				}
				else {
					await LogConsole("Address5 Download Filed!");
					await LogConsole(" Download  terminated!");
					return ERROR_Typdef.EAGAIN;
				}
			} 
			else {
				await LogConsole("Skip, File not selected");
			}
		}

		if(false == MultiCheck5.checked && true == MultiCheck10.checked && 0 == NetWorkBusy) {
		   NetWorkBusy = 1;
		   await DownloadBin(ChipType,ServerFilePath[4],ServerFileAttribute[4],DownloadAddrInput5.value);
		   await sleep(20);
 		   show += "\r\n FileName:  "+  ServerFileName[4] +"  adr5:  0x"+DownloadAddrInput5.value+"  size:  " + ServerFileLength[4] +"B\r\n";
	   }
	    while(NetWorkBusy) {
			await sleep(2);
 		}
		if(show != "") {
			await LogConsole(show + "Download Success");
			if(1 == AutoResetFlag) {
				await LogConsole("Automatic reset after download");
				await usb_evk_reset_mcu(TL_ModeTypdef.EVK,ChipType,LoadOBJChoose);
			}
 		}
		else {
			await LogConsole("no file selected!");
		}
 		BurningEVKBusy = 0;
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY;
	}
	return 0;
}

