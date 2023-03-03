var CHIP_8366   = 0x01;
var CHIP_8368   = 0x02;
var CHIP_83671  = 0x03;
var CHIP_83672  = 0x04;
var CHIP_83691  = 0x05;
var CHIP_83692  = 0x06;
var CHIP_8232   = 0x07;
var CHIP_8233   = 0x08;
var CHIP_8266   = 0x09;
var CHIP_8267   = 0x0a;
var CHIP_8269   = 0x0b;
var CHIP_8255   = 0x0c;
var CHIP_8258   = 0x0d;
var CHIP_8278   = 0x0e;
var CHIP_91     = 0x0f;

var Mars_B91    = 0x0f;
var ChipNumber  = 0x10;

var UCMD_CORE   = 0x0100;
var UCMD_FRD    = 0x8000;
var UCMD_FSWIRE = 0x4000;

var ChipName   = new Array(ChipNumber);
var LoadBinary = new Array(ChipNumber);

ChipName[CHIP_8366]  = "8366";     ChipName[CHIP_8368]  = "8368";     
ChipName[CHIP_83671] = "8367_i";   ChipName[CHIP_83672] = "8367_e";   
ChipName[CHIP_83691] = "8369_i";   ChipName[CHIP_83692] = "8369_e"; 
ChipName[CHIP_8232]  = "8232";     ChipName[CHIP_8233]  = "8233";		
ChipName[CHIP_8266]  = "8266";     ChipName[CHIP_8267]  = "8267";     
ChipName[CHIP_8269]  = "8269";     ChipName[CHIP_8255]  = "8255";
ChipName[CHIP_8258]  = "8258";     ChipName[CHIP_8278]  = "8278";   
ChipName[CHIP_91]	 = "B91";

LoadBinary[CHIP_8366]  = dut_8366_flash_v0244;	  
LoadBinary[CHIP_8368]  = dut_8368_flash_v0246;   
LoadBinary[CHIP_83671] = dut_8231_flash_i_v0002;
LoadBinary[CHIP_83672] = dut_8231_flash_e_v0002;  
LoadBinary[CHIP_83691] = dut_8231_flash_i_v0002;   
LoadBinary[CHIP_83692] = dut_8231_flash_e_v0002; 
LoadBinary[CHIP_8232]  = dut_8232_flash_v0002;     
LoadBinary[CHIP_8233]  = dut_8233_flash_v0002;
LoadBinary[CHIP_8266]  = dut_8266_flash_v0245;    
LoadBinary[CHIP_8267]  = dut_8267_flash_v0002;     
LoadBinary[CHIP_8269]  = dut_8269_flash_v0002;  	 
LoadBinary[CHIP_8255]  = dut_8255_flash_v0002;
LoadBinary[CHIP_8258]  = dut_8258_flash_v0005;     
LoadBinary[CHIP_8278]  = dut_8278_flash_v0001;    	 
LoadBinary[CHIP_91]    = dut_9518_flash_v0001;  
 
var lock_flash_buffer =  new Uint8Array([	
    0x0e,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x4b,0x4e,0x4c,0x54,0x00,0x01,0x88,0x00,
    0x56,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x24,0x08,0x25,0x09,0x25,0x0a,0x91,0x02,0x02,0xca,0x08,0x50,0x04,0xb1,0xfa,0x87,
    0x14,0x08,0xc0,0x6b,0x15,0x08,0x85,0x06,0x13,0x08,0xc0,0x6b,0x14,0x08,0x85,0x06,
    0x00,0xa0,0x14,0x09,0x14,0x0a,0x91,0x02,0x02,0xca,0x08,0x50,0x04,0xb1,0xfa,0x87,
    0x13,0x09,0x14,0x0a,0x91,0x02,0x02,0xca,0x08,0x50,0x04,0xb1,0xfa,0x87,0x0f,0x09,
    0x03,0xa0,0x08,0x40,0x04,0xa0,0x48,0x40,0x0f,0x09,0x10,0x0a,0x10,0x0b,0x9a,0x02,
    0x04,0xca,0x08,0x58,0x10,0x50,0x04,0xb1,0x04,0xb2,0xf8,0x87,0x00,0x90,0x5e,0x98,
    0xfe,0x87,0xc0,0x46,0x12,0x00,0x00,0x00,0x13,0x00,0x00,0x00,0x80,0x9f,0x80,0x00,
    0xc0,0x9f,0x80,0x00,0x00,0x8c,0x80,0x00,0x04,0x8c,0x80,0x00,0x0c,0x06,0x80,0x00,
    0x00,0x83,0x80,0x00,0x00,0x84,0x80,0x00,0x84,0x01,0x00,0x00,0x00,0x8c,0x80,0x00,
    0x00,0x8c,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x8c,0x80,0x00,0x00,0xc0,0x80,0x00,
    0x00,0x65,0xff,0x64,0xd8,0x6b,0x41,0x06,0x4a,0x06,0x53,0x06,0x5c,0x06,0x65,0x06,
    0x3f,0x64,0x00,0x90,0x31,0x98,0x3f,0x6c,0x88,0x06,0x91,0x06,0x9a,0x06,0xa3,0x06,
    0xac,0x06,0xd0,0x6b,0xff,0x6c,0x00,0x69,0xc0,0x46,0xc0,0x46,0xc0,0x46,0xc0,0x46,
    0xf0,0x65,0x00,0xa9,0x1d,0xcd,0x03,0xec,0x80,0xa0,0x04,0xf4,0x00,0xa2,0x40,0xa6,
    0x3f,0xa7,0xa4,0x06,0x03,0x80,0x01,0xb2,0x04,0xb3,0x8a,0x02,0x11,0xc0,0x5d,0x48,
    0x1c,0x48,0x2d,0xf2,0x25,0x03,0xdc,0x48,0x98,0x48,0x26,0x02,0xf3,0xc0,0x3c,0x00,
    0x03,0xac,0xf0,0xc1,0x64,0x06,0x25,0x03,0x01,0xb2,0x28,0x40,0x04,0xb3,0x8a,0x02,
    0xed,0xc1,0x08,0xec,0xf0,0x6d,0xc0,0x46,0x70,0x07,0xc0,0x46,0x00,0x65,0x06,0x0b,
    0x00,0xa2,0x1a,0x40,0x05,0x08,0x08,0xa1,0xff,0x97,0xd2,0x9f,0x04,0x0b,0x1a,0x58,
    0x01,0xb2,0x1a,0x50,0xfb,0x87,0xc0,0x46,0x73,0x00,0x80,0x00,0x64,0x01,0x00,0x00,
    0x00,0x8c,0x80,0x00,0x0d,0x00,0x01,0xc3,0x0d,0x00,0x00,0xc3,0x0c,0x00,0x06,0xc3,
    0x0d,0x00,0x01,0xc3,0x0d,0x00,0x00,0xc3,0x0c,0x00,0x01,0xc3,0x0c,0x00,0x7e,0xc3,
    0x0d,0x00,0x01,0xc3]);

var unlock_flash_buffer =  new Uint8Array([	
    0x0e,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x4b,0x4e,0x4c,0x54,0x00,0x01,0x88,0x00,
    0x56,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x24,0x08,0x25,0x09,0x25,0x0a,0x91,0x02,0x02,0xca,0x08,0x50,0x04,0xb1,0xfa,0x87,
    0x14,0x08,0xc0,0x6b,0x15,0x08,0x85,0x06,0x13,0x08,0xc0,0x6b,0x14,0x08,0x85,0x06,
    0x00,0xa0,0x14,0x09,0x14,0x0a,0x91,0x02,0x02,0xca,0x08,0x50,0x04,0xb1,0xfa,0x87,
    0x13,0x09,0x14,0x0a,0x91,0x02,0x02,0xca,0x08,0x50,0x04,0xb1,0xfa,0x87,0x0f,0x09,
    0x03,0xa0,0x08,0x40,0x04,0xa0,0x48,0x40,0x0f,0x09,0x10,0x0a,0x10,0x0b,0x9a,0x02,
    0x04,0xca,0x08,0x58,0x10,0x50,0x04,0xb1,0x04,0xb2,0xf8,0x87,0x00,0x90,0x5e,0x98,
    0xfe,0x87,0xc0,0x46,0x12,0x00,0x00,0x00,0x13,0x00,0x00,0x00,0x80,0x9f,0x80,0x00,
    0xc0,0x9f,0x80,0x00,0x00,0x8c,0x80,0x00,0x04,0x8c,0x80,0x00,0x0c,0x06,0x80,0x00,
    0x00,0x83,0x80,0x00,0x00,0x84,0x80,0x00,0x84,0x01,0x00,0x00,0x00,0x8c,0x80,0x00,
    0x00,0x8c,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x8c,0x80,0x00,0x00,0xc0,0x80,0x00,
    0x00,0x65,0xff,0x64,0xd8,0x6b,0x41,0x06,0x4a,0x06,0x53,0x06,0x5c,0x06,0x65,0x06,
    0x3f,0x64,0x00,0x90,0x31,0x98,0x3f,0x6c,0x88,0x06,0x91,0x06,0x9a,0x06,0xa3,0x06,
    0xac,0x06,0xd0,0x6b,0xff,0x6c,0x00,0x69,0xc0,0x46,0xc0,0x46,0xc0,0x46,0xc0,0x46,
    0xf0,0x65,0x00,0xa9,0x1d,0xcd,0x03,0xec,0x80,0xa0,0x04,0xf4,0x00,0xa2,0x40,0xa6,
    0x3f,0xa7,0xa4,0x06,0x03,0x80,0x01,0xb2,0x04,0xb3,0x8a,0x02,0x11,0xc0,0x5d,0x48,
    0x1c,0x48,0x2d,0xf2,0x25,0x03,0xdc,0x48,0x98,0x48,0x26,0x02,0xf3,0xc0,0x3c,0x00,
    0x03,0xac,0xf0,0xc1,0x64,0x06,0x25,0x03,0x01,0xb2,0x28,0x40,0x04,0xb3,0x8a,0x02,
    0xed,0xc1,0x08,0xec,0xf0,0x6d,0xc0,0x46,0x70,0x07,0xc0,0x46,0x00,0x65,0x06,0x0b,
    0x00,0xa2,0x1a,0x40,0x05,0x08,0x08,0xa1,0xff,0x97,0xd2,0x9f,0x04,0x0b,0x1a,0x58,
    0x01,0xb2,0x1a,0x50,0xfb,0x87,0xc0,0x46,0x73,0x00,0x80,0x00,0x64,0x01,0x00,0x00,
    0x00,0x8c,0x80,0x00,0x0d,0x00,0x01,0xc3,0x0d,0x00,0x00,0xc3,0x0c,0x00,0x06,0xc3,
    0x0d,0x00,0x01,0xc3,0x0d,0x00,0x00,0xc3,0x0c,0x00,0x01,0xc3,0x0c,0x00,0x00,0xc3,
    0x0d,0x00,0x01,0xc3]);

// cmd list
var TL_CMDTypdef ={
	DOWNLOAD  :0x01,
	ERASE	  :0x02,
	ACTIVATE  :0x03,
	SETTING   :0x04,
	READ      :0x05,
	WRITE     :0x06,
	RESET     :0x07,
	START     :0x08,
	STALL     :0x09,
	RUN       :0x0a,
	STOP      :0x0b,
	STEP      :0x0c,
	PC        :0x0d,
	BKP       :0x0e,
	SWS       :0x0f,
	LOG       :0x10,
	HELP      :0x11,
};

// object list
var TL_ObjTypdef = {
	FLASH	  :0x01,
	CORE	  :0x02,
	ANALOG	  :0x03,
	OTP	      :0x04,
	MCU   	  :0x05,
	INI_FILE  :0x06,
};

//communicate list
var TL_ModeTypdef = {
	USB		: 0x01,
  //RSVD	: 0x02,
	EVK		: 0x04,
};

//flash operate list
var TL_Dut_Flash_cmdTypdef = {
    TL_DUTCMD_FLASH_ASK        : 0x40,
    TL_DUTCMD_FLASH_WRITE      : 0x41,
    TL_DUTCMD_FLASH_CHECK      : 0x42,
    TL_DUTCMD_FLASH_WBYTE	   : 0x43,   //write_bytes
    TL_DUTCMD_FLASH_BYTE_CHK   : 0x44,
    TL_DUTCMD_FLASH_WID		   : 0x45,   //write_id
    TL_DUTCMD_FLASH_ID_CHK	   : 0x46,
    TL_DUTCMD_FLASH_ZERO       : 0x47,
    TL_DUTCMD_FLASH_CPID       : 0x48,
    TL_DUTCMD_FLASH_FAST_WRITE : 0x49,
    TL_DUTCMD_FLASH_ERASE      : 0x4d,
    TL_DUTCMD_FLASH_PROTECT    : 0x4e,
    TL_DUTCMD_FLASH_READ	   : 0x4f,
    TL_DUTCMD_FLASH_CRC        : 0x50,
};
 
var ERROR_Typdef = { 
	EPERM    	 : -1,   //  没有操作权限 资源不存在
	ENOENT   	 : -2,   //  没有这个文件或者文件夹
 	EINTR    	 : -3,   //  中断占用
    ENAMETOOLONG : -4,   //  文件名太长
	ENOMEM       : -5,   //  内存不够/溢出
	EAGAIN		 : -6,   //	 再试一次
	EPIPE    	 : -7,   //  连接太多
	EFBIG  		 : -8,	 //  文件太大
	EBUSY    	 : -9,   //  设备或者资源在忙
 	ESPIPE   	 : -10,  //  非法定位
 	E2BIG 		 : -11,  //  参数太长
    EOPDIS 	     : -12,  //  操作中断 
	ECRC 	     : -13,  //  校验出错
	ERANGE 	     : -14,  //  不在范围内
	ETYPE 	     : -15,  //  类型出错
	ESUPPORT     : -16,  //  功能不支持
	EEVKCMD      : -17,  //  USB发送命令返回出错
	ESWIRE 		 : -18,  //  Swire同步出错
	ETIME 		 : -19,  //  超时出错
	EACK 		 : -20,  //  ACK出错
	ERESET		 : -21,  //  复位出错
	EACTIVE		 : -22,  //  激活出错
	ELOCKFLASH   : -23,  //  FLASH解锁/上锁出错
};

var PRINT_Typdef = { 
	SYS_LOG_ERR  : 4, 
	SYS_LOG_DBG  : 3, 
	SYS_LOG_WRN  : 2, 
	SYS_LOG_INF  : 1, 
	SYS_LOG_OFF  : 0,
};

var	AutoResetFlag     = 0;	    //	自动复位标志位 1:下载后自动复位 0：下载后手动复位
var ChipType          = CHIP_91;//	芯片类型 默认B91
var BinFileChanged    = -1;	    //	下载文件是否与上次一样  -1:还没有选择过 0：选择的跟上次一样  1：首次打开或者与上次不一样
var USBConnectedFlag  = -1;	    //	USB设备是否已经连接 	1：已经连接 
var BinLoadAdr        = 0;		//  USB下载地址  
var PC 				  = 0;		//	PC地址
var	LoadOBJChoose     = 0x01;   //	下载位置选择  0x01:FLASH  0x02：SRAM   
var crc_ret 		  = 0;		//  JS计算出来的CRC结果
var crc_check 		  = 0;		//  发送命令返回过来的CRC结果		
var USBEvkCmdFlag     = 0;      //  usb_evk_cmd发送命令是否成功标志位 0：成功  -1:失败  
var SingleStepFlag    = 1;      //  1：单步  0：连续
var SingleWriteFlag   = 0;	    //  1: 从起始地址写同一数据	0: 从起始地址写多组数据
var ShowVariableFlag  = 1;   	//  1: 解析反汇编文件中的变量  0：解析反汇编文件中的函数
var LstFileSortMode   = 1;		//  1: 按地址来排序	2: 按名称来排序
var LoadBootBinFlag   = 1;   	//  1: 加载bin是对的， 0：加载引导bin是错的
var RefreshFlag       = 0; 	    //  1: refresh按下过  html中缩放界面用
var BurningEVKBusy    = 0;		//  1: busy  0:idle
var NetWorkBusy   	  = 0;		//  1: busy  0:idle
var LoopOnce          = 0;	    //  单次循环变量
var ShowTimeFlag 	  = 1;		//  显示时间戳标志位
var IsWindows         = 0;		//  是不是windows系统
var PrintLevel 		  = PRINT_Typdef.SYS_LOG_DBG; // 打印限制等级 默认3 

var crc32_table = new Array();
var crc_generate_one_flag = 0;
var R_BUF_SIZE = 1024*1024*4;				 // 单次读写FLASH/模拟寄存器最大值
var R_buffer = new Uint8Array(1024*1024*4);  // 读写FLASH/模拟寄存器buffer
var device; 								 // This is the USBDevice selected by the user
var g_rcv_len;								 // usb读取数据长度
var g_rcv_data = new Uint8Array(4096*32);	 // usb读 返回
var g_evk_rcv_data = new Uint8Array(4096*32);// evk读 返回

// 多地址下载 文件名 长度 缓存

var MultiBinFileLen1;
var MultiBinFileName1;
var MultiBinFileData1 = new Uint8Array(1024*1024*4);

var MultiBinFileLen2;
var MultiBinFileName2;
var MultiBinFileData2 = new Uint8Array(1024*1024*4);

var MultiBinFileLen3;
var MultiBinFileName3;
var MultiBinFileData3 = new Uint8Array(1024*1024*4);

var MultiBinFileLen4;
var MultiBinFileName4;
var MultiBinFileData4 = new Uint8Array(1024*1024*4);

var MultiBinFileLen5;
var MultiBinFileName5;
var MultiBinFileData5 = new Uint8Array(1024*1024*4);

function sleep(time) {
    return new Promise((resolve) => setTimeout(resolve, time));
}

async function LogConsole(str,target = "ALL",level = PrintLevel) {
	if(level >= PRINT_Typdef.SYS_LOG_DBG) {
		switch(target) {
			case "ALL":	      LogPrint(str,"Console1");
							  LogPrint(str,"Console2");
							  LogPrint(str,"Console3");
							  LogPrint(str,"Console4"); 
							  LogPrint(str,"Console5"); 
							  LogPrint(str,"Console6"); break;
			case "Console1":  LogPrint(str,"Console1"); break;
			case "Console2":  LogPrint(str,"Console2"); break;
			case "Console3":  LogPrint(str,"Console3"); break;
			case "Console4":  LogPrint(str,"Console4"); break;
			case "Console5":  LogPrint(str,"Console5"); break;
			case "Console6":  LogPrint(str,"Console6"); break;
			case "backstage": console.log(str);	   		break;
			default: 		  LogConsole("Print parameter error:" + str,"ALL");break;
		}
	}
}

async function LogPrint(str,target = "Console1") {
	var date     =  new Date(); 
	var hour     =  date.getHours(); 
	var min      =  date.getMinutes();
	var second   =  date.getSeconds();
	var msecond  =  date.getMilliseconds();
	if(hour < 10)	  hour = "0"+hour;
	if(min  < 10)	  min  = "0"+min;
	if(second < 10)	  second = "0"+second;
	if(msecond < 10)  msecond = "00"+msecond;		
	else if(msecond<100) msecond = "0"+msecond;
	var now = "time:"+hour+":"+min+":"+second+"."+msecond+"    ";
	var ele = document.getElementById(target);
	var mstr = ele.value; 
	mstr = mstr.replace(/\r/gi,"");
	mstr = mstr.split("\n");
	var n = mstr.length;
	if(1 == ShowTimeFlag) {
		ele.value = ele.value  + now + str +"\r\n";
	}
	else { 
		ele.value = ele.value + str + "\r\n";
	}
	ele.scrollTop = ele.scrollHeight;	
	if(n > 5000) {
		ele.value = ""; 
	}
}

function usb_evk_rcv_buffer_print(len,offset) {
	for(var i = 0; i < len; i++) {
		LogConsole("[usb_evk_rcv_buffer_print]:state<"+(i+offset).toString(16)+">-0x"+g_evk_rcv_data[i].toString(16),"backstage");	
	}
}

function dec2hex(dec, len) {
	  var hex = "";
	  dec = dec +0;
	  while(dec) {
		  var last = dec & 15;
		  hex = String.fromCharCode(((last>9)?55:48)+last) + hex;
		  dec >>= 4;
	  }
	  if(len) {
		  while(hex.length < len) hex = '0' + hex;
	  }
	  return hex;
}

function dec2hex2(dec) {
	return (dec).toString(16);
}

function hex2int(hex) {
	var len = hex.length, a = new Array(len), code;
	for(var i = 0; i < len; i++) {
		code = hex.charCodeAt(i);
		if (48 <= code && code < 58) {
			code -= 48;
		} else {
			code = (code & 0xdf) - 65 + 10;
		}
		a[i] = code;
	}
	return a.reduce(function(acc, c) {
	    acc = 16 * acc + c;
		return acc;
	},0);
}

function exchange(str) {
    var arr = str.split('');
    for(var i = 0; i < arr.length; i++) {
        if(arr[i] == 0) {
            arr[i] = 1;
        }
		else {
            arr[i] = 0;
        }
    }
    str = arr.join("");
    return str;
}

function Num2Binary(number) { // complement
    //-123: -1111011  123:1111011
    //0000101 == ~(-(-123)-1) == ~1111010 
    if(number < 0) {
        number = -number;
        number = number - 1;
        var str = (Array(32).join("0") + number.toString(2)).slice(-32);
        str = exchange(str);
    }
	else {
        var str = (Array(32).join("0") + number.toString(2)).slice(-32);
    }
	return str;
}

async function crc32_check_look_table(crc,array,length) {
    var crc_value = 0;
	var temp = "";
	var poly_value = "11101101101110001000001100100000"; // 0xedb88320
	if(length == 0) {
		crc_ret = crc;
		return ERROR_Typdef.ERANGE;
	}
	if(!crc_generate_one_flag) {
		for(var i = 0; i < 256; i++) {
			crc_value = i;
			for(var j = 0; j < 8; j++) { //crc_value = (crc_value&1?(crc_value>>1)^poly_value:crc_value>>1);
				if(typeof crc_value == "number") {
					crc_value = Num2Binary(crc_value);
				}
				else if(typeof crc_value == "string") {}
				if(crc_value.slice(-1) == "1") {
					crc_value = "0" + crc_value.slice(0,31);
					for(var k = 0; k < 32; k++) {
						if(crc_value[k] == poly_value[k]) {
							temp += "0";
						}
						else {
							temp += "1";
						}
					}
					crc_value = temp;
					temp = "";
				}
				else {
					crc_value = "0" + crc_value.slice(0,31);
				}	
			}
			crc32_table[i] = parseInt(crc_value,2);
		}
		crc_generate_one_flag  = 1;		
	}
	temp = "";
	var value0;
	var value1;
	var value2;
	var value3;
	for(var f = 0; f < length; f++) {// crc = crc32_table[(crc^array[i])&0xff]^(crc>>8);
		if(typeof crc == "number") {
			crc  = Num2Binary(crc);
		}
		else if(typeof crc == "string") {

		}
		value0 = Num2Binary(array[f]);
		for(var k = 0;k < 32; k++) {
			if(crc[k] == value0[k]) {
				temp += "0";
			}
			else {
				temp += "1";
			}
		}
		value1 = temp.slice(-8);
		value2 = crc32_table[parseInt(value1,2)];
		value2 = Num2Binary(value2);
		value3 = "00000000" + crc.slice(0,24);
		temp = "";
		for(var d = 0; d < 32; d++) {
			if(value2[d] == value3[d]) {
				temp += "0";
			}
			else {
				temp += "1";
			}
		}
		crc = temp;
		//crc = parseInt(temp,2);
		temp  = "";
 	}
	temp  = "";
	crc  = Num2Binary(crc);
	for(var g = 0; g < 32; g++) {    // crc_ret ^= 0xffffffff;
		if(crc[g] == "1") {
			temp += "0";
		}
		else {
			temp += "1";
		}
	}
	crc = parseInt(temp,2);
	//LogConsole("crc:  " + crc);
	//LogConsole("crc:  " + crc.toString(16));
	crc_ret = crc;
	return 0; 
}

async function usb_connect() {
	const myfilters = [
        { 'vendorId': 0x2341, 'productId': 0x8036 },
        { 'vendorId': 0x248A, 'productId': 0x826A }, ];
	await navigator.usb.requestDevice({ filters:  myfilters})
        .then((selectedDevice) => {
        device = selectedDevice;
        return device.open(); // Begin a session.
    })
    .then(() => {
		USBConnectedFlag = 1;
		layer.msg("usb connected!");
 		LogConsole("usb connected!");
		LogConsole(device,"backstage");
		device.selectConfiguration(1);   // 1 Select configuration #1 for the device.
		return device.claimInterface(0); // 1 Request exclusive control over interface #2.
    })
    .catch((error) => {
		console.error(error);
		USBConnectedFlag = -1;
		LogConsole("usb connect fail!");
		layer.msge("usb connect fail!");
    });
}

async function usb_read(adr,len) {
	var t_int = adr;
	var temp_data = new Uint8Array(len);
	temp_data[0] = 2;
	temp_data[1] = 3;
	temp_data[2] = 4;
	temp_data[3] = 5;
	await device.controlTransferIn({
        requestType: "vendor",
		recipient: "interface",
		request: 0x02, // vendor-specific request: enable channels
		value: t_int,  // 0x7c (channels 1, 2 and 5)
		index: 0x00,   // Interface 1 is the recipient
        },
        6+4+len
    ).then((res) => {
	    for(var i = 0 ; i < len; i++) {
			temp_data[i]  = res.data.getUint8(i);
			g_rcv_data[i] = res.data.getUint8(i);			 
		}
		g_rcv_len = len;  // then the actual read data is obtained according to res
	});
	//await sleep(2); 
	return temp_data;
}

async function usb_write(adr,data,len) {
	//if(Object.prototype.toString.call(data)=='[object Array]')
	var t_int = adr;
	var temp_data = new Uint8Array(len+8);
	temp_data[0] = 0x02;
	temp_data[1] = (t_int>>8) & 0xff; 
	temp_data[2] = t_int & 0xff;
	temp_data[3] = data[0];
	temp_data[4] = data[0];
	temp_data[5] = data[0];
	temp_data[6] = data[0];
	temp_data[7] = data[0];
	for(var i = 0; i < len; i++) {
		temp_data[8+i] = data[i];
	}
	device.controlTransferOut({
		requestType: "vendor",
		recipient: "interface",
		request: 0x02, // vendor-specific request: enable channels
		value: t_int, // 0x7c (channels 1, 2 and 5)
		index: 0x00, // Interface 1 is the recipient
	},temp_data)	 
	.then( (res) => {
	// console.log(res);  
	}) 
	/* 
	if(0 == IsWindows) { 
		await sleep(2);
	}
	*/
	await sleep(2);
}

async function usb_write2(adr,data,len) {
	var t_int = adr;
	var temp_data = new Uint8Array(len+8);
	var adr_h = (t_int >> 16) & 0x3f;
	temp_data[0] = 0x80 | adr_h;
	temp_data[1] = (t_int>>8) & 0xff; 
	temp_data[2] = t_int & 0xff;
	temp_data[3] = data[0];
	temp_data[4] = data[0];
	temp_data[5] = data[0];
	temp_data[6] = data[0];
	temp_data[7] = data[0];
	for(var i = 0; i < len; i++) {
	    temp_data[8+i] = data[i];
	}
	device.controlTransferOut({
		requestType: "vendor",
		recipient: "interface",
		request: 0x02, // vendor-specific request: enable channels
		value: t_int, // 0x7c (channels 1, 2 and 5)
		index: 0x00, // Interface 1 is the recipient
	},temp_data)	 
	.then( (res) => {
	// console.log(res);  
	}) 
}

async function usb_write3(adr,data,len) {
	var step = 1024;
	var buff = new Uint8Array(1024);
	for(var i = 0; i < len; i = i+step) {
		var n = (len - i) > step ? step : (len - i);
		var madr = adr + i;
		for(var j = 0; j < step; j++) {
			buff[j] = data[i+j];
		}
		await usb_write(madr,buff,n); // write to evk buffer
 	}
}
 
async function usb_evk_cmd(adr,len,cmd) { // EVKCommand
	var msecond1;
	var msecond2;
	var rlen = -1;
	var cmdbuf = new Uint8Array(8);
 	cmdbuf[0] = adr & 0xff;
	cmdbuf[1] = (adr>>8) & 0xff;
	cmdbuf[2] = (adr>>16) & 0xff;
	cmdbuf[3] = (adr>>24) & 0xff;
	cmdbuf[4] = len & 0xff;
	cmdbuf[5] = (len>>8) & 0xff;
	cmdbuf[6] = cmd & 0xff;
	cmdbuf[7] = (cmd>>8) & 0xff;
	await usb_write(0x9ff8,cmdbuf,8);
	msecond1 = new Date();
	while(rlen != len) {
		await usb_read(0x9ff0,16); // wait usb read done!
		rlen = (g_rcv_data[1]*256) + g_rcv_data[0];
		if((g_rcv_data[3]&0xf0) != 0) {
		    await LogConsole("[usb_evk_cmd]:error!");
			USBEvkCmdFlag = -1;
			return ERROR_Typdef.EEVKCMD;
		}
		else if(rlen == len) {
			//LogConsole("usb_evk_cmd ok!");
			USBEvkCmdFlag = 0;
			return 0;
		}
	    await sleep(2);
		msecond2 = new Date();
		if(parseInt(msecond2-msecond1) > 2000) {
			await LogConsole("wait for ack timeout:  "+ parseInt(msecond2-msecond1) +"  ms");
			return ERROR_Typdef.ETIME;
		}
	}
	return 0; 
}

async function usb_evk_write(adr,data,len) {
	var step = 1024;
	var buff = new Uint8Array(1024);
	for(var i = 0; i < len; i = i + step) {
		var n = (len - i) > step ? step : (len - i);
		var madr = adr + i;
		for(var j = 0; j < step; j++) {
			buff[j] = data[i+j];
		}
		await usb_write(0xa000,buff,n); // write to evk buffer
		await usb_evk_cmd(madr,n,UCMD_CORE|UCMD_FSWIRE);	
	}
}

async function usb_evk_read(adr,len) {
	var step = 1024;
	var buff = new Uint8Array(len);
 	for(var i = 0; i < len; i = i + step) {
		var n = (len - i) > step ? step : (len - i);
		var madr = adr + i;
		await usb_evk_cmd(madr,n,UCMD_CORE|UCMD_FSWIRE|UCMD_FRD);
		await usb_read(0xa000,n);
		for(var j = 0; j < step; j++) {	
			g_evk_rcv_data[i+j] = g_rcv_data[j];
			buff[i+j] = g_rcv_data[j];
		}	
	}
	return buff;
}

async function usb_evk_set_chip(type) {
	var cmdbuf = new Uint8Array(2);
	if(CHIP_91 == type) {
 	    cmdbuf[0] = 0xd0;
		await usb_write(0x9fff,cmdbuf,1);
	}
	else if((CHIP_8255 == type)||(CHIP_8258 == type)||(CHIP_8278 == type)) { 
 		cmdbuf[0] = 0x50;
		await usb_write(0x9fff,cmdbuf,1);
	}
	else {
 		cmdbuf[0] = 0x10;
		await usb_write(0x9fff,cmdbuf,1);
	}
	await  sleep(10);
	return 0; 
}

async function usb_evk_dut_cmd(type, cmd ,p0,p1, mode = TL_ModeTypdef.EVK) { // TL_Dut_Flash_cmd_Process
	var cnt = 0;
	var msecond1;
	var msecond2;
	var cmdbuf = new Uint8Array(16);
	cmdbuf[0] = cmd;
	cmdbuf[1] = p0 & 0xff;
	cmdbuf[2] = (p0>>8) & 0xff;
	cmdbuf[3] = (p0>>16) & 0xff;
	cmdbuf[4] = (p0>>24) & 0xff;
	cmdbuf[5] = p1 & 0xff;
	cmdbuf[6] = (p1>>8) & 0xff;
	cmdbuf[7] = (p1>>16) & 0xff;
	cmdbuf[8] = (p1>>24) & 0xff;
	var timeout_ms = 8000;
	if(TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_CRC == cmd) {
	    if(p1 > 1024*512) {
		    if((p1%524288) == 0) {
			   cnt = p1/524288;
		    }
		    else {
			    cnt = (p1/524288)+1;
		    }
		    timeout_ms = 8000*cnt;
	   }
	}
	if(TL_ModeTypdef.EVK == mode) {
		if(CHIP_91 == type) {
			await usb_evk_write(0xc0000007,cmdbuf,9);
			cmdbuf[0] = cmd|0x80;
			await usb_evk_write(0xc0000007,cmdbuf,1);
			msecond1 = new Date();
			await usb_evk_read(0xc0000007,1);
			while(g_evk_rcv_data[0]&0x80) {
				await usb_evk_read(0xc0000007,1);
				msecond2 = new Date();
				if(parseInt(msecond2-msecond1) > timeout_ms) {
					LogConsole("wait for ack timeout:  "+ parseInt(msecond2-msecond1) +"  ms");
					return ERROR_Typdef.ETIME;
				}
			}
			await usb_evk_read(0xc0000004,12);
			if((g_evk_rcv_data[2]&0xff) != cmd) {
				LogConsole(" wait for ack err! ");
				return ERROR_Typdef.EACK;
			}
		}
		else if((CHIP_8255 == type)||(CHIP_8258 == type)||(CHIP_8278 == type)) {
			await usb_evk_write(0x40007,cmdbuf,9);
			cmdbuf[0] = cmd|0x80;
			await usb_evk_write(0x40007,cmdbuf,1);
			msecond1 = new Date();	
			await usb_evk_read(0x40007,1);
			while(g_evk_rcv_data[0]&0x80) {
			    await usb_evk_read(0x40007,1);
				msecond2 = new Date();
				if(parseInt(msecond2-msecond1) > timeout_ms) {
					LogConsole("wait for ack timeout:  "+ parseInt(msecond2-msecond1) +"  ms");
					return ERROR_Typdef.ETIME;
				}
			}
			await usb_evk_read(0x40004,12);
			if((g_evk_rcv_data[2]&0xff) != cmd) {
				LogConsole(" wait for ack err! ");
				return ERROR_Typdef.EACK;
			}
		}
		else {
			await usb_evk_write(0x8007,cmdbuf,9);
			cmdbuf[0] = cmd|0x80;
			await usb_evk_write(0x8007,cmdbuf,1);
			msecond1 = new Date();
			g_evk_rcv_data[0] = 0x80;
			while(g_evk_rcv_data[0]&0x80) {
				await usb_evk_read(0x8007,1);
				msecond2 = new Date();
				if(msecond2-msecond1 > timeout_ms) {
					LogConsole("wait for ack timeout:  "+ parseInt(msecond2-msecond1) +"  ms");
					return ERROR_Typdef.ETIME;
				}
			}
			await usb_evk_read(0x8004,12);
			if((g_evk_rcv_data[2]&0xff) != cmd) {
				LogConsole(" wait for ack err! ");
				return ERROR_Typdef.EACK;
			}
		}
	}
	else if(TL_ModeTypdef.USB == mode ) {
		if((CHIP_8255 == type)||(CHIP_8258 == type)||(CHIP_8278 == type)) {
			await usb_write2(0x40007,cmdbuf,9);
			cmdbuf[0] = cmd|0x80;
			await usb_write2(0x40007,cmdbuf,1);
			msecond1 = new Date();	
			await usb_read(0x40007,1);
			while(g_rcv_data[0]&0x80) {
				await usb_read(0x40007,1);
				msecond2 = new Date();
				if(parseInt(msecond2-msecond1) > timeout_ms) {
					LogConsole("wait for ack timeout:  "+ parseInt(msecond2-msecond1) +"  ms");
					return ERROR_Typdef.ETIME;
				}
			}
			await usb_read(0x40004,12);
			if((g_rcv_data[2]&0xff) != cmd) {
				LogConsole(" wait for ack err! ");
				return ERROR_Typdef.EACK;
			}
		}
		else {
			await usb_write3(0x8007,cmdbuf,9);
			await sleep(10);
			cmdbuf[0] = cmd|0x80;
			await usb_write3(0x8007,cmdbuf,1);
 			msecond1 = new Date();
		    g_rcv_data[0] = 0x80;
			while(g_rcv_data[0]&0x80) {
 				await usb_read(0x8007,1);
				msecond2 = new Date();
				if(msecond2-msecond1 > timeout_ms) {
					await LogConsole("wait for ack timeout:  "+ parseInt(msecond2-msecond1) +"  ms");
					return ERROR_Typdef.ETIME;
				}
			}
 			await usb_read(0x8004,12);
			if((g_rcv_data[2]&0xff) != cmd) {
				await LogConsole(" wait for ack err! ");
				return ERROR_Typdef.EACK;
			}
		}
	}
	if(TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_ASK == cmd) {
		BinLoadAdr = g_rcv_data[0] + (g_rcv_data[1]*256);
		if((CHIP_8255 == type)||(CHIP_8258 == type)||(CHIP_8278 == type)) {
			BinLoadAdr = (BinLoadAdr+0x40000-0x8000);
		}
		else if(CHIP_91 == type) {
			BinLoadAdr = (BinLoadAdr+0xC0000000);
		}
		else {
			BinLoadAdr = BinLoadAdr; //B150
		}
	}
	if(TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_CRC == cmd) {
		var sum_size = g_rcv_data[4] + g_rcv_data[5]*256 + g_rcv_data[6]*256*256 + g_rcv_data[7]*256*256*256;
		crc_check = g_rcv_data[8] + g_rcv_data[9]*256 + g_rcv_data[10]*256*256 + g_rcv_data[11]*256*256*256;
		crc_check = crc_check>>>0;
        if(sum_size != p1) {
			LogConsole(" crc check err! ");
			LogConsole(" crc_check: " + crc_check);
			LogConsole(" sum_size: " + sum_size);
			return ERROR_Typdef.ECRC;
		}
	}
	return 0; 
}

async function ANALOG_RW_Func(mode,type,addr,size,value,cmd) { // ANALOG_RW_Func
	var msecond1;
	var msecond2;
 	var Break_W  = 0;
    var buffer1  = new Uint8Array(2);
	var buffer2  = new Uint8Array(2);
	var buffer3  = new Uint8Array(2);
	if(TL_ModeTypdef.USB == mode ) {
		LogConsole("  done not support the function ");
		return ERROR_Typdef.ESUPPORT;
	}
	if(TL_ModeTypdef.EVK == mode) {
		await usb_evk_set_chip(type);
		if(TL_CMDTypdef.READ == cmd) {
			if(size > R_BUF_SIZE) {
				LogConsole(" read memory size exceed! ");
			}
			for(var j = 0; j < R_BUF_SIZE; j++) {
				R_buffer[j] = 0;
			}
            for(var i = 0; i < size; i++) {
				buffer1[0] = addr + i;
				buffer2[0] = 0x40;
				if(CHIP_91 == type) {
					await usb_evk_write(0x140180,buffer1,1);
					await usb_evk_write(0x140182,buffer2,1);
				}
				else {
					await usb_evk_write(0xb8,buffer1,1);	
					await usb_evk_write(0xba,buffer2,1);						
				}
				msecond1 = new Date();
 				while((buffer2[0]&0x01) != 0x00) {
					if(CHIP_91 == type) {
						await usb_evk_read(0x140182,1);
						buffer2[0] = g_evk_rcv_data[0];
					}
					else {
						await usb_evk_read(0xba,1);	
						buffer2[0] = g_evk_rcv_data[0];						
					}
					msecond2 = new Date();
					if(parseInt(msecond2-msecond1)>100) {
						Break_W=1;
						LogConsole("  read analog register timeout: " + 100 + " ms");
						break;
					}
				}
				if(Break_W == 1) {
					return ERROR_Typdef.ETIME;
				}
				if(CHIP_91 == type) {
					await usb_evk_read(0x140184,1);
					buffer1[0] = g_evk_rcv_data[0];
				}
				else {
					await usb_evk_read(0xb9,1);	
					buffer1[0] = g_evk_rcv_data[0];						
				}
				R_buffer[i] = buffer1[0];
				if(0 == (i+1)%64) LogConsole( i+1 + "  bytes have finished!");
			}
		}
		else if(TL_CMDTypdef.WRITE == cmd) {
			Break_W = 0;
			for(var k = 0; k < size; k++) {
				buffer1[0] = addr + k;
                buffer2[0] = value[k];
                buffer3[0] = 0x60;
				if(CHIP_91 == type) {
					await usb_evk_write(0x140180,buffer1,1);
					await usb_evk_write(0x140184,buffer2,1);
					await usb_evk_write(0x140182,buffer3,1);
				}
				else {
					await usb_evk_write(0xb8,buffer1,1);	
					await usb_evk_write(0xb9,buffer2,1);	
					await usb_evk_write(0xba,buffer3,1);					
				}
				buffer3[0] = 0x01;
				msecond1 = new Date();
				while((buffer3[0]&0x01) != 0x00) {
					if(CHIP_91 == type) {
						await usb_evk_read(0x140182,1);
						buffer3[0] = g_evk_rcv_data[0];
					}
					else {
						await usb_evk_read(0xba,1);	
						buffer3[0] = g_evk_rcv_data[0];						
					}
					msecond2 = new Date();
					if(parseInt(msecond2-msecond1) > 100) {
						Break_W = 1;
						LogConsole("  write analog register timeout: " + 100 + " ms");
						break;
					}
				}
				if(1 == Break_W) {
					return ERROR_Typdef.ETIME;
				}
				buffer1[0] = 0;
				if(CHIP_91 == type) {
					await usb_evk_write(0x140182,buffer1,1);
				}
				else {
					await usb_evk_write(0xba,buffer1,1);	
				}
			}	
			//LogConsole(" Write  "+size+"  bytes at address "+ dec2hex2(addr));			
		}
		LogConsole(" Write size "+  size + "bytes at address " + addr,"backstage");
	}
	return 0; 
}

async function FlASH_RW_Func(mode,type,addr,size,value,cmd) { // FlASH_RW_Func
	var max_rdat_len = 8;
	var num   = Math.floor(size/max_rdat_len);
	var num_s = size - (num*max_rdat_len);

	if(TL_ModeTypdef.USB == mode ) {
		LogConsole("  done not support the function ");
		return ERROR_Typdef.ESUPPORT;
	}
	if(TL_ModeTypdef.EVK == mode) {
		await usb_evk_mcu_load_ram_bin(TL_ModeTypdef.EVK,type,LoadBinary[type],LoadBinary[type].length);
		if(TL_CMDTypdef.READ == cmd) {
			if(size > R_BUF_SIZE) {
				LogConsole(" read memory size exceed! ");
			}
			for(var j = 0; j < R_BUF_SIZE; j++) {
				R_buffer[j] = 0;
			}
			for(var i = 0; i < num; i++) {
				var ret = await usb_evk_dut_cmd(type,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_READ,addr+max_rdat_len*i,max_rdat_len);// TL_Dut_Flash_cmd_Process 
				if(ret != 0) {
					LogConsole(" FlASH Read Failed! ");
					return ERROR_Typdef.ECRC;
				}
				var ram_adr = 0x8004;
				if((CHIP_8255 == type)||(CHIP_8258 == type)||(CHIP_8278 == type)) {
					ram_adr = 0x40004;
				}
				else if(CHIP_91 == type) {
					ram_adr = 0xC0000004;
				}
				else {
					ram_adr = 0x8004;
				}
				if(max_rdat_len > 2) {
					if((CHIP_8255 == type)||(CHIP_8258 == type)||(CHIP_8278 == type)) {
						ram_adr = 0x40008; 
					}
					else if(CHIP_91 == type) {
						ram_adr = 0xC0000008; 
					}
					else {
						ram_adr = 0x8008;
					}
				}
				await usb_evk_set_chip(type);
				await usb_evk_read(ram_adr,max_rdat_len);
				for(var j = 0; j < max_rdat_len; j++) {
					R_buffer[max_rdat_len*i + j] = g_evk_rcv_data[j];
				}
				if(0 == ((i+1)*max_rdat_len)%1024) LogPrint((i+1)*max_rdat_len + "  bytes have finished!");
			}
			if(num_s != 0){
				var ret = await usb_evk_dut_cmd(type,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_READ,addr+max_rdat_len*num,num_s);
				if(ret != 0) {
					LogConsole(" FlASH Read Failed! ");
					return -1;
				}
				var ram_adr = 0x8004;
				if((CHIP_8255 == type)||(CHIP_8258 == type)||(CHIP_8278 == type)) {
					ram_adr = 0x40004;
				}
				else if(CHIP_91 == type) {
					ram_adr = 0xC0000004;
				}
				else {
					ram_adr = 0x8004;
				}
				if(num_s > 2) {
					if((CHIP_8255 == type)||(CHIP_8258 == type)||(CHIP_8278 == type)) {
						ram_adr = 0x40008; 
					}
					else if(CHIP_91 == type) {
						ram_adr = 0xC0000008; 
					}
					else {
						ram_adr = 0x8008;
					}
				}
				await usb_evk_set_chip(type);
				await usb_evk_read(ram_adr,num_s);
				for(var j = 0; j < num_s; j++) {
					R_buffer[max_rdat_len*num + j] = g_evk_rcv_data[j];
				}
			}
			LogConsole(num*max_rdat_len+num_s + "  bytes have finished!");
		}
		else if(TL_CMDTypdef.WRITE == cmd) {
			var ret = await usb_evk_dut_cmd(type,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_ASK,0,0);
			if(ret != 0) {
				LogConsole(" FlASH Write Failed! TL_DUTCMD_FLASH_ASK ");
				return ERROR_Typdef.ECRC;
			}
			await usb_evk_write(BinLoadAdr,value,size);
			var ret = await usb_evk_dut_cmd(type,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_WRITE,addr,size);
			if(ret != 0) {
				LogConsole(" FlASH Write Failed! TL_DUTCMD_FLASH_WRITE");
				return ERROR_Typdef.ECRC;
			}
			LogConsole(" Write  "+ size + "  bytes at address " + dec2hex2(addr));			
		}
	}
	return 0; 
}

async function usb_evk_sws_mcu(type,sws_config) { // SWS_MCU
 	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!");
		return ERROR_Typdef.EPERM;
	}
 	var buffer = new Uint8Array(2);
	buffer[0] = sws_config[1];
 	await usb_write(sws_config[0]+2,buffer,1);
	await usb_evk_set_chip(type);
 	if(CHIP_91 == type) {
		buffer[0] = sws_config[3];
		await usb_evk_cmd(0x100c00+3,1,0x0f00);//set slave fifo mode enable register of sws
		await usb_evk_cmd(0x00,1,0x0700);
		await usb_evk_cmd(0x00,1,0x0700);
		await usb_evk_write(0x100c00+2,buffer,1);
		await usb_evk_read(0x100c00+2,1);
 		buffer[1] = g_evk_rcv_data[0];
	}
	else {
		buffer[0] = sws_config[3];
		await usb_evk_cmd(sws_config[2]+3,1,0x0f00);//set slave fifo mode enable register of sws
		await usb_evk_cmd(0x00,1,0x0700);
		await usb_evk_cmd(0x00,1,0x0700);
		await usb_evk_write(sws_config[2]+2,buffer,1);
		await usb_evk_read(sws_config[2]+2,1);
	}
	if((g_evk_rcv_data[0] == sws_config[3]) && (0 == USBEvkCmdFlag)) {
		LogConsole(" TC32 EVK: Swire ok! ");
	}	
	else {
		USBEvkCmdFlag = -1;
		LogConsole(" TC32 EVK: Swire err! ");
		return ERROR_Typdef.ESWIRE; 
	}
	return 0; 
}

async  function usb_evk_stall_mcu(mode,type,adr,data) { // Stall_Func
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!");
		return ERROR_Typdef.EPERM;
	}
	var cmdbuf = new Array();
  	if(TL_ModeTypdef.EVK == mode) {
		if(CHIP_91 == type) {
			LogConsole(" This chip done not support the function ");
			return ERROR_Typdef.ESUPPORT;
		}
		await usb_evk_set_chip(type); 
		cmdbuf[0] = data;
 		await usb_write(0xa000,cmdbuf,1);   // 1.write adr and data to sram 
		await usb_evk_cmd(adr,1,0x0900); 	// send cmd to evk 
	}
	else if(TL_ModeTypdef.USB == mode ) {
		LogConsole("  done not support the function ");
		return ERROR_Typdef.ESUPPORT;
	}
	LogConsole(" stall mcu ");
	return 0;
}

async function usb_evk_start_mcu(mode,type,adr,data) { // Start_Func
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!");
		return ERROR_Typdef.EPERM;
	}
	var cmdbuf = new Array();
	if(TL_ModeTypdef.EVK == mode) {
		if(CHIP_91 == type) {
			LogConsole("This chip does not support this function");
			return ERROR_Typdef.ESUPPORT;
		}
		await usb_evk_set_chip(type); 
		cmdbuf[0] = data;
 		await usb_evk_write(adr,cmdbuf,1);   
 	}
	else if(TL_ModeTypdef.USB == mode ) {
		LogConsole("does not support this function");
		return ERROR_Typdef.ESUPPORT;
	}
	await LogConsole(" start mcu ");
	return 0;
}

async function usb_evk_activate_mcu(type) { // Activate_MCU
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!");
		return ERROR_Typdef.EPERM;
	}
  	var buffer = new Uint8Array(2);
 	var mstatus1;
	var mstatus2;
	var msecond1;
	var msecond2;
	buffer[0] = 0;
	for(var m = 0; m < 2; m++) {
		await usb_evk_set_chip(type);
	    await usb_read(0x9ff4,4);
		mstatus1 = g_rcv_data[3]*256*256*256 + g_rcv_data[2]*256*256+g_rcv_data[1]*256 + g_rcv_data[0];
		buffer[0] = 0x4d;
		await usb_write(0x9fff,buffer,1);
		await usb_read(0x9ff4,4);
		mstatus2 = g_rcv_data[3]*256*256*256 + g_rcv_data[2]*256*256+g_rcv_data[1]*256 + g_rcv_data[0];
	    msecond1 = new Date();
 		while(mstatus1 == mstatus2) {
			await usb_read(0x9ff4,4);
			mstatus2 = g_rcv_data[3]*256*256*256 + g_rcv_data[2]*256*256+g_rcv_data[1]*256 + g_rcv_data[0];
			sleep(100);
			msecond2 = new Date();
			if(parseInt(msecond2-msecond1) > 5000) {
				 LogConsole(" activate timeout!   "+ parseInt(msecond2-msecond1) + "  ms");
				 return ERROR_Typdef.ETIME;
			}
		}
 	}
	buffer[0] = 0;
	g_evk_rcv_data[0] = 0;
	if(CHIP_91 == type) {
		await usb_evk_read(0x1401c0+0x3f,1);
		buffer[0] = g_evk_rcv_data[0];
	}
	else {
		await usb_evk_read(0x7f,1);
		buffer[0] = g_evk_rcv_data[0];
	}
 	if((buffer[0] == 0x53||buffer[0] == 0x55) && (0 == USBEvkCmdFlag)) {
 		LogConsole(" Activate OK! ");
		return 0;
    }
    else {
 		LogConsole(" Activate failed! ");
		 return ERROR_Typdef.EACTIVE;
    }
}

async function usb_evk_run_mcu(mode,type) { // Run_MCU
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!");
		return ERROR_Typdef.EPERM;
	}
	var buffer = new Uint8Array(2);
	if(TL_ModeTypdef.EVK == mode) {
		if(CHIP_91 == type) {
			LogConsole(" This chip does not support this function");
			return ERROR_Typdef.ESUPPORT;
		}
		await usb_evk_set_chip(type); 
		await usb_evk_read(0x613,1);
		buffer[0] = (g_evk_rcv_data[0]|0x80);
 		await usb_evk_write(0x613,buffer,1);   
	    await usb_evk_read(0x602,1);
		buffer[0] = (g_evk_rcv_data[0]&0xfd);
		await usb_evk_write(0x602,buffer,1);  
 	}
	else if(TL_ModeTypdef.USB == mode ) {
		LogConsole("does not support this function");
		return ERROR_Typdef.ESUPPORT;
	}
	LogConsole(" mcu run ");
	return 0;
}

async function usb_evk_pause_mcu(mode,type) { // Stop_MCU
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!");
		return ERROR_Typdef.EPERM;
	}
	var buffer = new Uint8Array(2);
	if(TL_ModeTypdef.EVK == mode) {
		if(CHIP_91 == type) {
			LogConsole(" This chip does not support this function");
			return ERROR_Typdef.ESUPPORT;
		}
		await usb_evk_set_chip(type); 
		await usb_evk_read(0x602,1);
		buffer[0] = (g_evk_rcv_data[0]|0x02);
 		await usb_evk_write(0x602,buffer,1);   
	    await usb_evk_read(0x6bc,4);
 	}
	else if(TL_ModeTypdef.USB == mode) {
		LogConsole("does not support this function");
		return ERROR_Typdef.ESUPPORT;
	}
	PC = g_evk_rcv_data[0] + g_evk_rcv_data[1]*256 + g_evk_rcv_data[2]*256*256 + g_evk_rcv_data[3]*256*256*256;
	LogConsole("pause mcu, pc : 0x"+dec2hex(PC,6));
	return 0;
}

async function usb_evk_step_mcu(mode,type) { // Step_Func
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!","Console2");
		return ERROR_Typdef.EPERM;
	}
	var cmdbuf = new Uint8Array(2);	
 	if(TL_ModeTypdef.EVK == mode) {
		if(CHIP_91 == type) {
			LogConsole(" This chip done not support the function ","Console2");
			return ERROR_Typdef.ESUPPORT;
		}
		await usb_evk_set_chip(type); 
		await usb_evk_read(0x602,1);
		cmdbuf[0] = g_evk_rcv_data[0];
	    cmdbuf[0] = (cmdbuf[0]|0x02);
		await usb_evk_write(0x602,cmdbuf,1);
		await usb_evk_read(0x613,1);
		cmdbuf[0] = g_evk_rcv_data[0];
		cmdbuf[0] = (cmdbuf[0]|0x80);
		await usb_evk_write(0x613,cmdbuf,1);
		await usb_evk_read(0x6bc,4);
 	}
	else if(TL_ModeTypdef.USB == mode ) {
		LogConsole("does not support this function");
		return ERROR_Typdef.ESUPPORT;
	}
	PC = g_evk_rcv_data[0] + g_evk_rcv_data[1]*256 + g_evk_rcv_data[2]*256*256 + g_evk_rcv_data[3]*256*256*256;
	LogConsole("step mcu, pc : 0x"+dec2hex(PC,6),"Console2");
	return 0;
}

async function usb_evk_reset_mcu(mode,type,dst) { // Reset_MCU
	var value  = new Uint8Array(1);
	var cmdbuf = new Uint8Array(2);
	cmdbuf[0] = 0x20;
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!");
		return ERROR_Typdef.EPERM;
	}
	if(TL_ModeTypdef.USB == mode ) {
		LogConsole("does not support this function");
		return ERROR_Typdef.ESUPPORT;
	}
	else if(TL_ModeTypdef.EVK == mode) {
		await usb_evk_set_chip(type);
		if(TL_ObjTypdef.CORE == dst) {
			if(CHIP_91 == type) {
				cmdbuf[0] = 0xff;
				await usb_evk_write(0x1401e2,cmdbuf,1);
			}
			else {
				cmdbuf[0] = 0x88;
				// if(type == CHIP_8255) cmd_buffer[0] = 0x80;// if(Type == CHIP_8255_A2) 
				await usb_evk_write(0x602,cmdbuf,1);	
			}
		}
		else {
			if(CHIP_91 == type) {
				value[0] = 0x01;
				await ANALOG_RW_Func(TL_ModeTypdef.EVK,CHIP_91,0x7f,1,value,TL_CMDTypdef.WRITE); 
 				await usb_evk_write(0x1401ef,cmdbuf,1);
			}
			else {
 				await usb_evk_write(0x6f,cmdbuf,1);
			}
		}
	}
	if(TL_ObjTypdef.CORE == dst) {
        LogConsole(" program run ");
    }
    else {
		LogConsole(" reset mcu ");
    }
	return 0;
}

async function usbReadtButtonFun() {
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!","Console3");
		return ERROR_Typdef.EPERM;
	}
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
		var t_addr = hex2int(usbRWAddr.value);
		var t_len  = hex2int(usbRWLength.value);
		await usb_read(t_addr,t_len);
		var Rdata = "";
		for(var i = 0; i < t_len; i++) {
			Rdata += dec2hex(g_rcv_data[i],2)+" ";
		}
		LogConsole(Rdata,"Console3");
		BurningEVKBusy = 0; 
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY; 
	}
	return 0;
}

async function usbWirtetButtonFun() {
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!","Console3");
		return ERROR_Typdef.EPERM;
	}
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
		if(1 == SingleWriteFlag) {
			var readtwobytes = usbRWData.value[0]+usbRWData.value[1];
			var reg1 = RegExp(" ");
			if(readtwobytes.match(reg1)) {
				LogConsole("Too many parameters or spaces","Console3");
				return ERROR_Typdef.E2BIG;
			}
			var t_data = hex2int(readtwobytes);
			var t_len  = hex2int(usbRWLength.value);
			var t_addr = hex2int(usbRWAddr.value);
			var s_data = new Uint8Array(t_len);
			for(var i = 0 ; i < t_len; i++) {
				s_data[i] = t_data&0xff;
			}
			await usb_write(t_addr,s_data,t_len);
			LogConsole("usb write done!","Console3");
		}
		else {
			var reg2 = RegExp("  ");
			if(usbRWData.value.match(reg2)) {
				LogConsole("Too many spaces","Console3");
				return ERROR_Typdef.E2BIG;
			}
			var t_data = usbRWData.value.trim().split(" ");
			var t_len  = t_data.length;
			var t_addr = hex2int(usbRWAddr.value);
			var s_data = new Uint8Array(t_len);
			for(var i = 0; i < t_len; i++) {
				s_data[i] = hex2int(t_data[i])&0xff;
			}
			await usb_write(t_addr,s_data,t_len);
			LogConsole("usb write done!","Console3");
		}
		BurningEVKBusy = 0; 
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY; 
	}
	return 0;
}

async function CoreReadtButtonFun() {
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!","Console3");
		return ERROR_Typdef.EPERM;
	}
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
		var t_len  = hex2int(usbRWLength.value);
		var t_addr = hex2int(usbRWAddr.value);
		if(CHIP_91 == ChipType) {
			if((t_addr >= 0) && (t_addr <= 0x20000)) {
				t_addr = t_addr + hex2int("C0000000");//ILM
			}
			else if( (t_addr>=0x80000)&&(t_addr<=0xA0000)) {
				t_addr = t_addr + hex2int("C0200000");//DLM
			}
		}
		await usb_evk_set_chip(ChipType);
		await usb_evk_read(t_addr,t_len);
		var Rdata = "0x" + usbRWAddr.value + ":   ";
		for(var i = 0; i < t_len; i++) {
				Rdata+=dec2hex(g_rcv_data[i],2)+" ";
		}
		LogConsole(Rdata,"Console3");
		BurningEVKBusy = 0; 
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY; 
	}
	return 0;
}

async function CoreWirtetButtonFun() {
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!","Console3");
		return ERROR_Typdef.EPERM;
	}
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;	
		if(1 == SingleWriteFlag) {
			var readtwobytes = usbRWData.value[0]+usbRWData.value[1];
			var reg1 = RegExp(" ");
			if(readtwobytes.match(reg1)) {
				LogConsole("Too many parameters or spaces","Console3");
				return ERROR_Typdef.E2BIG;
			}
			var t_data = hex2int(readtwobytes);
			var t_len  = hex2int(usbRWLength.value);
			var t_addr = hex2int(usbRWAddr.value);
			if(CHIP_91 == ChipType) {
				if((t_addr >= 0) && (t_addr <= 0x20000)) {
					t_addr = t_addr +hex2int("C0000000");//ILM
				}
				else if((t_addr >= 0x80000) && (t_addr <= 0xA0000)) {
					t_addr = t_addr +hex2int("C0200000");//DLM
				}
			}
			var s_data = new Uint8Array(t_len);
			for(var i = 0; i < t_len; i++) {
				s_data[i] = t_data&0xff;
			}
			await usb_evk_write(t_addr,s_data,t_len);
			LogConsole("Core write done!","Console3");
		}
		else {
			var reg2 = RegExp("  ");
			if(usbRWData.value.match(reg2)) {
				LogConsole("Too many spaces","Console3");
				return ERROR_Typdef.E2BIG;
			}
			var  t_data = usbRWData.value.trim().split(" ");
			var t_len  = t_data.length;
			var t_addr = hex2int(usbRWAddr.value);
			if(CHIP_91 == ChipType) {
				if((t_addr >= 0) && (t_addr <= 0x20000)) {
					t_addr = t_addr +hex2int("C0000000");//ILM
				}
				else if((t_addr >= 0x80000) && (t_addr <= 0xA0000)) {
					t_addr = t_addr +hex2int("C0200000");//DLM
				}
			}
			var s_data = new Uint8Array(t_len);
			for(var i = 0; i < t_len; i++) {
				s_data[i] = hex2int(t_data[i])&0xff;
			}
			await usb_evk_write(t_addr,s_data,t_len);
			LogConsole("Core write done!","Console3");
		}
		BurningEVKBusy = 0; 
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY; 
	}
	return 0;
}

async function AnalogReadtButtonFun() {
	var value  = new Uint8Array(1);
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!","Console3");
		return ERROR_Typdef.EPERM;
	}
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
		var t_len  = hex2int(usbRWLength.value);
		var t_addr = hex2int(usbRWAddr.value);
		await ANALOG_RW_Func(TL_ModeTypdef.EVK,ChipType,t_addr,t_len,value,TL_CMDTypdef.READ); 
		var Rdata = "0x" + usbRWAddr.value + ":   ";
		for(var i = 0; i < t_len; i++) {
			Rdata += dec2hex(R_buffer[i],2)+" ";
		}
		LogConsole(Rdata,"Console3");
		BurningEVKBusy = 0; 
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY; 
	}
	return 0;
}

async function AnalogWirtetButtonFun() {
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!","Console3");
		return ERROR_Typdef.EPERM;
	}
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;	
		if(1 == SingleWriteFlag) {
			var readtwobytes = usbRWData.value[0]+usbRWData.value[1];
			var reg1 = RegExp(" ");
			if(readtwobytes.match(reg1)) {
				LogConsole("Too many parameters or spaces","Console3");
				return ERROR_Typdef.E2BIG;
			}
			var t_data = hex2int(readtwobytes);
			var t_len  = hex2int(usbRWLength.value);
			var t_addr = hex2int(usbRWAddr.value);
			var s_data = new Uint8Array(t_len);
			for(var i = 0; i < t_len; i++) {
				s_data[i] = t_data&0xff;
			}
			await ANALOG_RW_Func(TL_ModeTypdef.EVK,ChipType,t_addr,t_len,s_data,TL_CMDTypdef.WRITE); 
			LogConsole("Analog write done!","Console3");
		}
		else {
			var reg2 = RegExp("  ");
			if(usbRWData.value.match(reg2)) {
				LogConsole("Too many spaces","Console3");
				return ERROR_Typdef.E2BIG;
			}
			var  t_data = usbRWData.value.trim().split(" ");
			var t_len  = t_data.length;
			var t_addr = hex2int(usbRWAddr.value);
			var s_data = new Uint8Array(t_len);
			for(var i = 0; i < t_len; i++) {
				s_data[i] = hex2int(t_data[i])&0xff;
			}
			await ANALOG_RW_Func(TL_ModeTypdef.EVK,ChipType,t_addr,t_len,s_data,TL_CMDTypdef.WRITE); 
			LogConsole("Analog write done!","Console3");
		}
		BurningEVKBusy = 0; 
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY; 
	}
	return 0;
}

async function FlashReadtButtonFun() {
	var value  = new Uint8Array(1);
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!","Console3");
		return ERROR_Typdef.EPERM;
	}
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
		var t_len  = hex2int(usbRWLength.value);
		var t_addr = hex2int(usbRWAddr.value);
		await FlASH_RW_Func(TL_ModeTypdef.EVK,ChipType,t_addr,t_len,value,TL_CMDTypdef.READ);
		var Rdata = "0x" + usbRWAddr.value + ":   ";
		for(var i = 0; i < t_len; i++) {
			Rdata += dec2hex(R_buffer[i],2) + " ";
		}
		LogConsole(Rdata,"Console3");
		BurningEVKBusy = 0; 
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY; 
	}
}

async function FlashWirtetButtonFun() {
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!","Console3");
		return ERROR_Typdef.EPERM;
	}
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;	
		if(1 == SingleWriteFlag) {
			var readtwobytes = usbRWData.value[0]+usbRWData.value[1];
			var reg1 = RegExp(" ");
			if(readtwobytes.match(reg1)) {
				LogConsole("Too many parameters or spaces","Console3");
				return ERROR_Typdef.E2BIG;
			}
			var t_data = hex2int(readtwobytes);
			var t_len  = hex2int(usbRWLength.value);
			var t_addr = hex2int(usbRWAddr.value);
			var s_data = new Uint8Array(t_len);
			for(var i = 0; i < t_len; i++) {
				s_data[i] = t_data&0xff;
			}
			await FlASH_RW_Func(TL_ModeTypdef.EVK,ChipType,t_addr,t_len,s_data,TL_CMDTypdef.WRITE);
			LogConsole("Flash write done!","Console3");
		}
		else {
			var reg2 = RegExp("  ");
			if(usbRWData.value.match(reg2)) {
				LogConsole("Too many spaces","Console3");
				return ERROR_Typdef.E2BIG;
			}
			var  t_data = usbRWData.value.trim().split(" ");
			var t_len  = t_data.length;
			var t_addr = hex2int(usbRWAddr.value);
			var s_data = new Uint8Array(t_len);
			for(var i = 0; i < t_len; i++) {
				s_data[i] = hex2int(t_data[i])&0xff;
			}
			await FlASH_RW_Func(TL_ModeTypdef.EVK,ChipType,t_addr,t_len,s_data,TL_CMDTypdef.WRITE);
			LogConsole("Flash write done!","Console3");
		}
		BurningEVKBusy = 0; 
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY; 
	}
	return 0;
}

async function SectorEraseFlash(mode,type,addr,Sector_num,callback) { // SectorEraseFlash
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!");
		return ERROR_Typdef.EPERM;
	}
	var callback_en = 0;
	if(mode == TL_ModeTypdef.USB) {
		LogConsole("does not support this function");
		return ERROR_Typdef.ESUPPORT;
	}
	if(mode == TL_ModeTypdef.EVK) {
 		await usb_evk_mcu_load_ram_bin2(type,TL_ModeTypdef.EVK);
		 if(USBEvkCmdFlag == -1) {
			await LogConsole("Error loading boot bin");
			return ERROR_Typdef.EEVKCMD;
		 }
		for(var i = 0; i < Sector_num; i++) {//erase flash
			await usb_evk_dut_cmd(type,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_ERASE,addr + (i*0x1000),4);
			callback_en = 1;
			if((typeof callback == "function") && (callback_en==1)) {
				callback(hex2int(EraseAddrInput.value)+i*0x1000);// Flash Sector (4K) Erase at address: 6d000 	
			}
		}
	}	
	return 0;
}

async function usb_evk_mcu_load_ram_bin(mode,type,data,len) { // MCU_Init
	var cmdbuf_w = new Uint8Array(2);
	var cmdbuf_r = new Uint8Array(2);
	if(TL_ModeTypdef.USB == mode ) {
		LogConsole("USB mode is temporarily not supported");
		return ERROR_Typdef.ESUPPORT;
	}
	else if(TL_ModeTypdef.EVK == mode) {
		await usb_evk_set_chip(type); // set swire interface
		if(CHIP_91 == type) {
			for(var m = 0;m < 4; m++) {
				cmdbuf_w[0] = 0x10;
				await usb_evk_cmd(0x100c03,1,0x0f00);
				await usb_evk_cmd(0x00,1,0x0700);
				await usb_evk_cmd(0x00,1,0x0700);
				await usb_evk_write(0x100c02,cmdbuf_w,1);
				await usb_evk_read(0x100c02,1);				
				if(0x10 == g_evk_rcv_data[0] && 0 == USBEvkCmdFlag) {
					LogConsole("TC32 EVK : Swire OK");	
					break;
				}
				else if(m >= 3) {
					LogConsole("  TC32 EVK: Swire err! ");	
					return ERROR_Typdef.ESWIRE;
				}
			}
 			await  usb_evk_set_chip(CHIP_91); // set swire interface
			cmdbuf_w[0] = 0xe7;
			await usb_evk_write(0x1401e2,cmdbuf_w,1);// reset mcu ??
			//await sleep(20);
			cmdbuf_w[0] = 0x00;
			await usb_evk_write(0x1401d5,cmdbuf_w,1);// disable irq ??
			await usb_evk_write(0x1401d6,cmdbuf_w,1);
			await usb_evk_write(0x1401d7,cmdbuf_w,1);
			await usb_evk_write(0xc0000000,data,len);
			cmdbuf_w[0] = 0xff;
			await usb_evk_write(0x1401e2,cmdbuf_w,1);
 		}
		else {
			cmdbuf_w[0] = 0xab;
			cmdbuf_w[1] = 0x00;
			for(var g = 0; g < 4; g++) {
				await usb_evk_cmd(0x00,1,0x0700); 
				await usb_evk_cmd(0x00,1,0x0700); 
				await usb_evk_cmd(0xb2,1,0x8100);
				if((CHIP_8255 == type)||(CHIP_8258 == type)||(CHIP_8278 == type)) {
					await usb_evk_write(0x40004,cmdbuf_w,1);
					await usb_evk_read(0x40004,1);
					cmdbuf_r[0] = g_evk_rcv_data[0];
				}
				else {
					await usb_evk_write(0x8004,cmdbuf_w,1);
					await usb_evk_read(0x8004,1);
					cmdbuf_r[0] = g_evk_rcv_data[0];
				}
				if(0xab == g_evk_rcv_data[0] && 0 == USBEvkCmdFlag) {
					LogConsole("TC32 EVK : Swire OK");	
					break;
				}
				else if(g >= 3) {
					LogConsole(" [131]: TC32 EVK: Swire err!");	
					return ERROR_Typdef.ESWIRE;
				}
			}
			if(USBEvkCmdFlag == -1) {
				return ERROR_Typdef.EEVKCMD;
			}
			//*********    reset mcu    *********************
			cmdbuf_w[0] = 0x05;
			for(var k = 0; k < 4; k++) {
				await usb_evk_write(0x602,cmdbuf_w,1);
				await usb_evk_read(0x602,1);
				cmdbuf_r[0] = g_evk_rcv_data[0];
				if((cmdbuf_r[0]&0x05) == 0x05) {
					break;
				}
				else if(k >= 3) {
					await LogConsole("reset mcu err! ");	
				}
			}
			await LogConsole("TC32 EVK: Swire OK!");	
			//********************   Disable watch dog, Disable interrupt,download file to ram and start MCU   *********************
			cmdbuf_w[0] = 0x00;
			await usb_evk_write(0x622,cmdbuf_w,1);
			await usb_evk_write(0x643,cmdbuf_w,1);
			if((CHIP_8255 == type)||(CHIP_8258 == type)||(CHIP_8278 == type)) {
				await usb_evk_write(0x40000,data,len);
			}
			else {
				await usb_evk_write(0x8000,data,len);
				if(type == CHIP_8366||type == CHIP_8368) {
					cmdbuf_w[0] = 0x01;	
					await usb_evk_write(0x8ff0,cmdbuf_w,1);//select 1:12M xtal(or 2:16M xtal) for 8366/8368
					await usb_evk_write(0x8ff1,cmdbuf_w,1);//select 1:internal vpp(or 2:external vpp) for 8366
				}
			}
			cmdbuf_w[0] = 0x88;	
			/*
				if(type == CHIP_8258) {//CHIP_8255_A2
					cmdbuf_w[0] = 0x80;
				}
			*/
			await usb_evk_write(0x602,cmdbuf_w,1); // start MCU
		}
	}
	return 0; 
}

async function usb_evk_mcu_load_ram_bin2(type,mode) { // MCU_Init
	if(type <= ChipNumber) {
		layer.msg ("The selected chip type is: " + ChipName[type]); 
		LogConsole("The selected chip type is: " + ChipName[type]);
	}
	else {
		layer.msg ("Please select the correct chip type!"); 
		LogConsole("Please select the correct chip type!");
	    return ERROR_Typdef.ERANGE;
	}	
	await usb_evk_mcu_load_ram_bin(mode,type,LoadBinary[type],LoadBinary[type].length);
	await sleep(10);
	if(USBEvkCmdFlag == -1) {
		await LogConsole("Error loading boot bin");
		return ERROR_Typdef.EEVKCMD;
	}
	if(CHIP_91 == type) {
 	    await usb_evk_read(0xc0000000,LoadBinary[type].length);
	}
	else if((CHIP_8255 == type)||(CHIP_8258 == type)||(CHIP_8278 == type)) {
	    await usb_evk_read(0x40000,LoadBinary[type].length);
	}
	else {
	    await usb_evk_read(0x8000,LoadBinary[type].length);
	}
	await sleep(10);
	for(var i = 0; i < LoadBinary[type].length; i++) {
		if((g_evk_rcv_data)[i] != LoadBinary[type][i]) {
			await LogConsole("Error checking boot bin");
			await LogConsole("i:" + i + "   " + g_evk_rcv_data[i] + ":" + LoadBinary[type][i]);
			break;
		}
	}
	if(i < LoadBinary[type].length) {
	    return ERROR_Typdef.ECRC;
	}
	return 0;
}

async function MCU_Init_None_Log(type,mode) { // MCU_Init_None_Log
	if(TL_ModeTypdef.USB == mode) {
//*********    reset mcu    *********************
		var i = 0;
		var buffer = new Uint8Array(2); 
		buffer[0] = 0x05;
		buffer[1] = 0;
		g_rcv_data[0] = 0;
		while((g_rcv_data[0]&0x05) != 0x05) {
			if(CHIP_8255 == type||CHIP_8258 == type||CHIP_8278 == type) {
				await usb_write2(0x602,buffer,1);
				await usb_read(0x602,1);
			}
			else {
				await usb_write(0x602,buffer,1);
				await usb_read(0x602,1);
			}
			i++;
			if(i > 3) {
				LogConsole(" TC32 USB : USB Err! ");
				return ERROR_Typdef.EPERM;
			}
		}
//********************   Disable watch dog, Disable interrupt,download file to ram and start MCU   *********************			
		if(CHIP_8255 == type||CHIP_8258 == type||CHIP_8278 == type) {
			buffer[0] = 0x00;
			await usb_write2(0x622,buffer,1);
			await usb_write2(0x643,buffer,1);
			buffer[0] = 0xff;
			await usb_write2(0x60c,buffer,1);
			await usb_write2(0x60d,buffer,1);
			buffer[0] = 0xff;
			await usb_write2(0x104,buffer,1);
			//await usb_write3(0x40000,g_file_data2,size);///////////
			await usb_write3(0x40000,LoadBinary[type],LoadBinary[type].length);
			buffer[0] = 0x88;
			/*
			if(CHIP_8255_A2 == type) {
				buffer[0] = 0x80;
			}
			*/
			await usb_write2(0x602,buffer,1);
		}
		else {
			buffer[0] = 0x00;
			await usb_write(0x622,buffer,1);
			await usb_write(0x643,buffer,1);
			buffer[0] = 0x40;
			await usb_write(0x60c,buffer,1);
			await usb_write(0x60d,buffer,1);
			buffer[0] = 0xff;
			await usb_write(0x104,buffer,1);
		    if(CHIP_91 == type) {
				await usb_write3(0xc0000000,LoadBinary[type],LoadBinary[type].length);
			}
			else {
				await usb_write3(0x8000,LoadBinary[type],LoadBinary[type].length);
 			}
			if(CHIP_8366 == type||CHIP_8368 == type) {
				buffer[0] = 0x01;	
				await usb_write(0x8ff0,buffer,1);//select 1:12M xtal(or 2:16M xtal) for 8366/8368
				await usb_write(0x8ff1,buffer,1);//select 1:internal vpp(or 2:external vpp) for 8366
			}
			buffer[0] = 0x88;
			await usb_write(0x602,buffer,1);
		}
	}
	return 0;
}

async function TL_Dut_Flash_cmd_Process_None_Log(type, cmd ,p0,p1, mode = TL_ModeTypdef.EVK) { // TL_Dut_Flash_cmd_Process_None_Log
	var cmdbuf = new Uint8Array(16);
	var msecond1;
	var msecond2;
	var timeout_ms = 2000;
	cmdbuf[0] = cmd;
	cmdbuf[1] = p0 & 0xff;
	cmdbuf[2] = (p0>>8) & 0xff;
	cmdbuf[3] = (p0>>16) & 0xff;
	cmdbuf[4] = (p0>>24) & 0xff;
	cmdbuf[5] = p1 & 0xff;
	cmdbuf[6] = (p1>>8) & 0xff;
	cmdbuf[7] = (p1>>16) & 0xff;
	cmdbuf[8] = (p1>>24) & 0xff;
	if(TL_ModeTypdef.USB == mode ) {
		if((CHIP_8255 == type)||(CHIP_8258 == type)||(CHIP_8278 == type)) {
			await usb_write2(0x40007,cmdbuf,9);
			cmdbuf[0] = cmd|0x80;
			await usb_write2(0x40007,cmdbuf,1);
			msecond1 = new Date();	
			await usb_read(0x40007,1);
			while(g_rcv_data[0]&0x80) {
				await usb_read(0x40007,1);
				msecond2 = new Date();
				if(parseInt(msecond2-msecond1) > timeout_ms) {
					LogConsole("wait for ack timeout:  "+ parseInt(msecond2-msecond1) +"  ms");
					return ERROR_Typdef.ETIME;
				}
			}
			await usb_read(0x40004,12);
			if((g_rcv_data[2]&0xff) != cmd) {
				LogConsole(" lock flash succeed! ");
				return 0;
			}
		}
		else {
			await usb_write3(0x8007,cmdbuf,9);
			cmdbuf[0] = cmd|0x80;
			await usb_write3(0x8007,cmdbuf,1);
			msecond1 = new Date();
		    g_rcv_data[0] = 0x80;
			while(g_rcv_data[0]&0x80) {
 				await usb_read(0x8007,1);
				msecond2 = new Date();
				if(msecond2-msecond1 > timeout_ms) {
					await LogConsole("wait for ack timeout:  "+ parseInt(msecond2-msecond1) +"  ms");
					return ERROR_Typdef.ETIME;
				}
			}
			await usb_read(0x8004,12);
			if((g_rcv_data[2]&0xff) != cmd) {
				await LogConsole(" lock flash succeed! ");
				return 0; 
			}
		}
	}
	if(TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_ASK == cmd) {
		BinLoadAdr = g_rcv_data[0] + (g_rcv_data[1]*256);
		if((CHIP_8255 == type)||(CHIP_8258 == type)||(CHIP_8278 == type)) {
			BinLoadAdr = (BinLoadAdr+0x40000-0x8000);
		}
		else if(CHIP_91 == type) {
			BinLoadAdr = (BinLoadAdr+0xC0000000);
		}
		else {
			BinLoadAdr = BinLoadAdr;
		}
	}
	if(TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_CRC == cmd) {
		var sum_size = g_rcv_data[4] + g_rcv_data[5]*256 + g_rcv_data[6]*256*256 + g_rcv_data[7]*256*256*256;
		crc_check = g_rcv_data[8] + g_rcv_data[9]*256 + g_rcv_data[10]*256*256 + g_rcv_data[11]*256*256*256;
		crc_check = crc_check>>>0;
        if(sum_size != p1) {
			LogConsole(" crc check err! ");
			LogConsole(" crc_check: " + crc_check);
			LogConsole(" sum_size: "  + sum_size);
			return ERROR_Typdef.ECRC;
		}
	}
	return 0; 
}

async function usb_evk_flash_write(type,data,len,addr,callback) { // EVK_Download
	var msecond1;
	var msecond2;
 	await usb_evk_mcu_load_ram_bin2(type,TL_ModeTypdef.EVK);
	if(USBEvkCmdFlag == -1) {
	    await LogConsole("Error loading boot bin");
		return ERROR_Typdef.EEVKCMD;
	}
	await LogConsole("Boot bin loaded correctly");
	msecond1 = new Date(); // start timing before erasing flash
	await usb_evk_dut_cmd(type,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_ASK,0,0);
	var Sector_num = ((len%0x1000)==0) ? Math.floor(len/0x1000) : Math.floor((len/0x1000)+1);
	var Page_num = ((len%0x100)==0) ? Math.floor(len/0x100) : Math.floor((len/0x100)+1);
	LogConsole("len:"+len +"		Sector_num:"+Sector_num+ "		Page_num:"+Page_num);
	for(var i = 0; i < Sector_num; i++) {
		await usb_evk_dut_cmd(type,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_ERASE,addr + (i*0x1000),4);
		LogConsole("erase flash : "+dec2hex(addr + (i*0x1000)));
	}
	for(var i = 0; i < Page_num; i++) {
		var temp_len;
		var temp_data;
		var callback_en = 0;
		if(i < Page_num - 1) {
			temp_data = data.slice(i*256, i*256+256);
			temp_len = 256;
			if(i%16 == 0) {
				callback_en = 1;
			}
		}
		else {
			temp_data = data.slice(i*256,len);
			temp_len = len - (i*256);
			if(temp_len > 256) {
				temp_len = 256;
			}
			callback_en = 1;
		}
		await usb_evk_write(BinLoadAdr,temp_data,temp_len);
		await usb_evk_dut_cmd(type,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_WRITE,(i*256)+addr,temp_len);
		if((typeof callback == "function") && (callback_en == 1)) {
		    callback(addr + i*256);
		}
	}
	await crc32_check_look_table(0xffffffff,data,len);
    await usb_evk_dut_cmd(type,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_CRC ,addr,len);
	if(crc_ret != crc_check) {
		LogConsole(" file check err!");
		LogConsole("crc_ret:  " + crc_ret.toString(16));
		LogConsole("crc_check:  " + crc_check.toString(16));
		return ERROR_Typdef.ECRC;
	}
	else {
		LogConsole(" file check success!");
	}
	LogConsole("File Download to Flash at address 0x" + dec2hex(addr)+":  " +len +"  bytes");
	msecond2 = new Date();
	LogConsole("Total Time:  "+ parseInt(msecond2-msecond1) +"  ms");
	return 0;
}

async function usb_evk_ram_write(type,data,len,addr,callback) { // EVK_Download
 	var buffer = new Uint8Array(2);
 	var callback_en = 1;
    var msecond1;
	var msecond2;
	var temp;
	await usb_evk_set_chip(type); // Swrie  sync
	if(CHIP_91 == type) {
		buffer[0] = 0xab;
		buffer[1] = 0x00;
		for(var i = 0; i < 4; i++) {	 
			await usb_evk_cmd(0x00,1,0x0700);
			await usb_evk_cmd(0x00,1,0x0700);//??
			await usb_evk_cmd(0x100C02,1,0x8100);
			await usb_evk_write(0xC0000004,buffer,1);
			await usb_evk_read(0xC0000004,1);
 			if(0xab == g_evk_rcv_data[0]) {
				LogConsole(" TC32 EVK: Swire ok! ");
				break;
			}
			else if(i >= 3) {
			   LogConsole(" TC32 EVK: Swire err! ");	
			   return ERROR_Typdef.ESWIRE;
			}
		}
	}
	else { 
		buffer[0] = 0xab;
		buffer[1] = 0x00;
		for(var j = 0; j < 4; j++) { 
			await usb_evk_cmd(0x00,1,0x0700);
			await usb_evk_cmd(0x00,1,0x0700);
			await usb_evk_cmd(0xb2,1,0x8100);
			if((CHIP_8255 == type)||(CHIP_8258 == type)||(CHIP_8278 == type)) {
				await usb_evk_write(0x40004,buffer,1);
				await usb_evk_read(0x40004,1);
			}
			else {
				await usb_evk_write(0x8004,buffer,1);
				await usb_evk_read(0x8004,1);				
			}
			if(0xab == g_evk_rcv_data[0]) {
				LogConsole(" TC32 EVK: Swire ok! ");
				break;
			}
			else if(j >= 3) {
			   LogConsole(" TC32 EVK: Swire err! ");	
			   return ERROR_Typdef.ESWIRE;
			}
		}
		//*********    reset mcu    *********************
		buffer[0] = 0x05;
		for(var k = 0; k < 4; k++) {	 
			await usb_evk_write(0x602,buffer,1);
			await usb_evk_read(0x602,1);
 			if((g_evk_rcv_data[0]&0x05) == 0x05) {
				LogConsole(" TC32 EVK : Swire OK ");
				break;
			}
			else if(k >= 3) {
			   LogConsole(" [132]: reset mcu err! ");	
			   return ERROR_Typdef.ERESET;
			}
		}
	}
	buffer[0] = 0xab;
	buffer[1] = 0x00;
	buffer[0] = 0x00;
	//********************   Disable watch dog, Disable interrupt,download file to ram and start MCU   *********************
	if(CHIP_91 == type) {
		buffer[0] = 0x00;
		await usb_evk_write(0x140142,buffer,1);
		buffer[0] = 0xe7;
		await usb_evk_write(0x1401e2,buffer,1); 
		buffer[0] = 0x00;
		await usb_evk_write(0x1401d5,buffer,1);	
		buffer[0] = 0x00;
		await usb_evk_write(0x1401d6,buffer,1);		
		buffer[0] = 0x00;
		await usb_evk_write(0x1401d7,buffer,1);		
	}
	else {
		buffer[0] = 0x00;
		await usb_evk_write(0x622,buffer,1);
		await usb_evk_write(0x643,buffer,1);
	}
	var aign_256byte_num = len/256;
    var sbyte_num = len%256;
	var temp_data;
	msecond1 = new Date();
	if(CHIP_91 == type) {
		for(var i = 0; i < aign_256byte_num; i++) {
			temp_data = data.slice(i*256, i*256+256);
			await usb_evk_write(addr+i*256,temp_data,256);	
			if((typeof callback == "function") && (callback_en == 1)) {
				callback(addr+i*256);
			}
		}
		if(sbyte_num != 0) {
			temp_data = data.slice(aign_256byte_num*256,len);
			await usb_evk_write(addr+aign_256byte_num*256,temp_data,sbyte_num);	
		}
	}
	else if((CHIP_8255 == type)||(CHIP_8258 == type)||(CHIP_8278 == type)) {
		for(var i = 0; i < aign_256byte_num; i++) {
			temp_data = data.slice(i*256,i*256+256);
			await usb_evk_write(addr+i*256,temp_data,256);	
			if((typeof callback == "function") && (callback_en==1)) {
				callback(addr+i*256);
			}
		}
		if(sbyte_num != 0) {
			temp_data = data.slice(aign_256byte_num*256,len);
			await usb_evk_write(addr+aign_256byte_num*256,temp_data,sbyte_num);	
		}
	}
	else {
		for(var i = 0; i < aign_256byte_num; i++) {
			temp_data = data.slice(i*256,i*256+256);
			await usb_evk_write(addr+i*256,temp_data,256);	
			if((typeof callback == "function") && (callback_en==1)) {
				callback(addr+i*256);
			}
		}
		if(sbyte_num != 0) {
			temp_data = data.slice(aign_256byte_num*256,len);
			await usb_evk_write(addr+aign_256byte_num*256,temp_data,sbyte_num);	
		}
		if(type == CHIP_8366||type == CHIP_8368) {
			buffer[0] = 0x01;
			await usb_evk_write(0x8ff0,temp_data,1);//??
			await usb_evk_write(0x8ff1,temp_data,1);//??
		}
	}
	LogConsole("File Download to SRAM at address 0x" + dec2hex2(addr)+":  " +len +"  bytes");
	await crc32_check_look_table(0xffffffff,data,len);
	temp = crc_ret;
	await usb_evk_read(addr,len);
	await crc32_check_look_table(0xffffffff,g_evk_rcv_data,len);
	if(temp != crc_ret) {
		LogConsole(" crc check err!");
		LogConsole("temp:  "+temp+" crc_ret:  "+crc_ret);
		return ERROR_Typdef.ECRC;
	}
	else {
		LogConsole(" crc check success!");
	}
	msecond2 = new Date();
	LogConsole("Total Time:  "+ parseInt(msecond2-msecond1) +"  ms");
	return 0;
}

$("#UPFileImport").click(function() {
	$("#UPFileInput").click();
})

async function UPFileRead(type) { // Upgrade
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!");
		return ERROR_Typdef.EPERM;
	}
	var localFile2;
	var g_file_length2;
	var g_file_data2 = new Uint8Array(1024*1024*4);
	await LogConsole("ChipType: " + type);
	localFile2 = document.getElementById("UPFileInput").files[0];
	LogConsole(localFile2,"backstage");
	var reader = new FileReader();
	const _this = this;
	reader.readAsArrayBuffer(localFile2);
	reader.onload = function() {
		var bytes = new Uint8Array(reader.result);    
		var date = new Date(localFile2.lastModified);
		dateTime = date.toLocaleString();
		LogConsole(localFile2.name + "	:  " + dateTime);
		for(var i=0;i<bytes.length;i++) {
			g_file_data2[i] = bytes[i];
		}
		g_file_length2 = bytes.length;
		LogConsole("g_file_length2: " + g_file_length2);
	}
	document.getElementById('UPFileInput').value = null;
	await sleep(20);
	await upgrade(type,g_file_data2,g_file_length2);
}

async function upgrade(type,UPBinData,UPBinLength) {
	var i = 0;
	var size;
	var temp;
	var sbyte_num;
	var temp_data;
	var aign_256byte_num;
	var buffer = new Uint8Array(2); 
	//1.unlock flash of evk
	size =  unlock_flash_buffer.length;
	//*********    reset mcu    *********************
	buffer[0] = 0x05;
	buffer[1] = 0;
	g_rcv_data[0] = 0;
	while((g_rcv_data[0]&0x05) != 0x05) {
		await usb_write(0x602,buffer,1);
		await usb_read(0x602,1);
		i++;
		if(i > 3) {
			await LogConsole(" TC32 USB : USB Err! ");
 			return ERROR_Typdef.EPERM;
		}
	}
	//******  Disable watch dog, Disable interrupt,download file to ram and start MCU    *********
	buffer[0] = 0x00;
	aign_256byte_num = Math.floor(size/256);
	sbyte_num = Math.floor(size%256);
	await LogConsole("size: " + size + " aign_256byte_num: " + aign_256byte_num + " sbyte_num: " + sbyte_num);
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
		await LogConsole(" crc check err! ");
		await LogConsole("temp: " + temp + " crc_ret: " + crc_ret);
		return ERROR_Typdef.ECRC;
	}
	else {
		await LogConsole(" crc check success! ");
	}
	// program run
	buffer[0] = 0x88;
	await usb_write(0x602,buffer,1);
	await sleep(10);
	//2.erase flash of evk to confirm whether unlock flash is  succeeded
	await MCU_Init_None_Log(type,TL_ModeTypdef.USB);
	await LogConsole(" succeed! ");
	await usb_evk_dut_cmd(type,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_ERASE,0,4,TL_ModeTypdef.USB);
	await LogConsole(" unlock flash succeed!");
	//3.load upgrade bin to flash of evk
	await MCU_Init_None_Log(type,TL_ModeTypdef.USB);
	var msecond1;
	var msecond2;
	var EraseSector_Num = (0 == (UPBinLength%0x1000)) ? Math.floor(UPBinLength/0x1000) : Math.floor((UPBinLength/0x1000)+1);
	var PageWrite_Num = (0 == (UPBinLength%0x100)) ? Math.floor(UPBinLength/0x100) : Math.floor((UPBinLength/0x100)+1);
	msecond1 = new Date();
	await LogConsole("UPBinLength: "+UPBinLength +"		EraseSector_Num: "+EraseSector_Num+ "		PageWrite_Num: "+PageWrite_Num);
	await usb_evk_dut_cmd(type,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_ASK,0,0,TL_ModeTypdef.USB);
	var Adr = 0;
	await LogConsole("  BinLoadAdr:  " + BinLoadAdr);
	for(i = 0; i < EraseSector_Num; i++) {
		await usb_evk_dut_cmd(type,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_ERASE,Adr + (i*0x1000),4,TL_ModeTypdef.USB);
	}
	for(var k = 0; k < PageWrite_Num; k++) {
		var temp_data;
		var temp_len;
		var callback_en = 0;
		if(k < PageWrite_Num - 1) {
			temp_data = UPBinData.slice(k*256, k*256+256);
			temp_len = 256;
			if(0 == k%16) {
				callback_en = 1;
			}
		}
		else {
			temp_data = UPBinData.slice(k*256,UPBinLength);
			temp_len = UPBinLength - (k*256);
			if(temp_len > 256) {
				temp_len = 256;
			}
			callback_en = 1;
		}
		await usb_write3(BinLoadAdr,temp_data,temp_len);
		await usb_evk_dut_cmd(type,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_WRITE,(k*256)+Adr,temp_len,TL_ModeTypdef.USB);
		if(1 == callback_en) {
			await LogConsole("Flash Page Program at address: 0x"+dec2hex2(Adr+k*256));
		}
	}
	msecond2 = new Date();
	await LogConsole("Total Time:  "+ parseInt(msecond2-msecond1) +"  ms");
	await crc32_check_look_table(0xffffffff,UPBinData,UPBinLength);
	await usb_evk_dut_cmd(type,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_CRC ,Adr,UPBinLength,TL_ModeTypdef.USB);
	if(crc_ret != crc_check) {
		await LogConsole(" file check err!");
		await LogConsole("crc_ret:  " + crc_ret.toString(16));
		await LogConsole("crc_check:  " + crc_check.toString(16));
		return ERROR_Typdef.ECRC;
	}
	else {
		await LogConsole(" Update FW succeed! ");
	}
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
			await LogConsole(" TC32 USB : USB Err! ");
			return 0;
		}
	}
//********************   Disable watch dog, Disable interrupt,download file to ram and start MCU   *********************
	aign_256byte_num = Math.floor(size/256);
	sbyte_num = Math.floor(size%256);
	await LogConsole("size: " + size + " aign_256byte_num: " + aign_256byte_num + " sbyte_num: " + sbyte_num);
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
		await LogConsole(" crc check err! ");
		await LogConsole("temp: " + temp + " crc_ret: " + crc_ret);
		return ERROR_Typdef.ECRC;
	}
	else {
		await LogConsole(" crc check success! ");
	}
	// program run
	buffer[0] = 0x88;
	await usb_write(0x602,buffer,1);
	await sleep(10);
	//2.erase flash of evk to confirm whether lock flash is succeeded
	await MCU_Init_None_Log(type,TL_ModeTypdef.USB);
	await TL_Dut_Flash_cmd_Process_None_Log(type,TL_Dut_Flash_cmdTypdef.TL_DUTCMD_FLASH_ERASE,0,4,TL_ModeTypdef.USB);
	await LogConsole(" --------------- upgrade firmware succeed, please power on again ------------------ 	 ");
	return 0;
}

async function DownloadBin(type,BinPath,FileType,addr = "Null") {
	if(USBConnectedFlag != 1) {
		LogConsole("USB device not connected!");
		return ERROR_Typdef.EPERM;
	}
	var rawData = new Uint8Array(1024*1024*4);
	await LogConsole("ChipType: " + type);
	var xmlhttp;
	if (window.XMLHttpRequest) {
		//  IE7+, Firefox, Chrome, Opera, Safari 浏览器执行代码 
		xmlhttp = new XMLHttpRequest();
	}
	else {
		// IE6, IE5 浏览器执行代码
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	}
	xmlhttp.onreadystatechange = async function() {
		if(4 == xmlhttp.readyState && 200 == xmlhttp.status) {
			await LogConsole("Get success");
		}
		if(xmlhttp.status != 200 && xmlhttp.status != 0) {
			await LogConsole("Get failed  erro code:" + xmlhttp.status);
		}
	}
	xmlhttp.open("GET",BinPath,true); // The path is not too long
	xmlhttp.responseType = 'arraybuffer';
	xmlhttp.onload = async function(e) {
	    rawData = new Uint8Array(this.response);
		if(4 == xmlhttp.readyState && 200 == xmlhttp.status)  {
			await LogConsole("File length:  " + rawData.length);
			await LogConsole("Load success");
			if("BurningEVKFirmware" == FileType) {
				await upgrade(type,rawData,rawData.length);
			}
			else if("TargetBoardBin" == FileType) {
				await NetWorkDownload(type,rawData,rawData.length,addr);
			}
			else {
				await LogConsole("Wrong file type!");
			}
		}
		else {
			await LogConsole(".....................");
		}
	}
	await xmlhttp.send();
	await sleep(10);
}

async function NetWorkDownload(type,UPBinData,UPBinLength,addr) {
	if(TL_ObjTypdef.FLASH == LoadOBJChoose) {
		if("Null" == addr) {
			await usb_evk_flash_write(type,UPBinData,UPBinLength,hex2int(DownloadAddrInput.value),download_callback);
		}
		else { //hex2int("0000011")
			await usb_evk_flash_write(type,UPBinData,UPBinLength,hex2int(addr),download_callback);
		}
	}
	else if(TL_ObjTypdef.CORE == LoadOBJChoose) {
		if("Null" == addr) {
			await usb_evk_ram_write(type,UPBinData,UPBinLength,hex2int(SramDownloadAddrInput.value),download_callback);
		}
		else {
			await usb_evk_flash_write(type,UPBinData,UPBinLength,hex2int(addr),download_callback);	
		}
	}
	if("Null" == addr) {
		if(1 == AutoResetFlag) {
			await LogConsole("Automatic reset after download");
			await usb_evk_reset_mcu(TL_ModeTypdef.EVK,type,LoadOBJChoose);
		}
	}
	if(1 == NetWorkBusy) {
		NetWorkBusy = 0;
	}

 	if(1 == BurningEVKBusy) {
		BurningEVKBusy = 0;
	}
}

async function BinDownload(data,len,addr,MultiDown = "0") {
	if(USBConnectedFlag != 1) {
		await LogConsole("USB device not connected!");
		return ERROR_Typdef.EPERM;
	}
	else {
		if(MultiDown == "0") {
			if(-1 == BinFileChanged) {
 				await LogConsole("You haven't chosen to download files yet!");
				return ERROR_Typdef.ENOENT;
 			}
			else if(0 == BinFileChanged) {
				await LogConsole("Download the same file as last time!");
 			}
			else if(BinFileChanged != 1) {
				await LogConsole("download error!");
				return ERROR_Typdef.ENOENT;
			}
			var date = new Date(localFile.lastModified);
			dateTime = date.toLocaleString();
			await LogConsole(localFile.name + "	:  " + dateTime);
			await LogConsole("download start!");
			LogConsole(localFile.name,"backstage");
			if(TL_ObjTypdef.FLASH == LoadOBJChoose) {
				await usb_evk_flash_write(ChipType,data,len,addr,download_callback);
			}
			else if(TL_ObjTypdef.CORE == LoadOBJChoose) {
				await usb_evk_ram_write(ChipType,data,len,addr,download_callback);
			}
			if(1 == AutoResetFlag) {
				await LogConsole("Automatic reset after download");
				await usb_evk_reset_mcu(TL_ModeTypdef.EVK,ChipType,LoadOBJChoose);
			}
		}
		else if("1" == MultiDown) {
			await usb_evk_flash_write(ChipType,data,len,addr,download_callback);
		}
	}
	return 0;
}

async function download_callback(page_adr) {
	if(TL_ObjTypdef.FLASH == LoadOBJChoose) {
		await LogConsole("Flash Page Program at address: 0x"+dec2hex2(page_adr));
	}
	else if(TL_ObjTypdef.CORE == LoadOBJChoose) {
		await LogConsole("Sram Page Program at address: 0x"+dec2hex2(page_adr));
	}
}


$("#BinFileImport").click(function() {
		$("#BinFileInput").click();
})

var localFile;
var lastlocalFile;
var g_file_length = 0;
var g_file_data = new Uint8Array(1024*1024*4); // bin文件最大
async function BinFileRead() {
	LogConsole("file read test begin...","backstage");
	localFile = document.getElementById("BinFileInput").files[0];
	LogConsole(localFile,"backstage");
	LogConsole(lastlocalFile,"backstage");
	if(undefined == lastlocalFile) {
		BinFileChanged = 1;
	}
	else {
		if((lastlocalFile.name == localFile.name) && (lastlocalFile.lastModified == localFile.lastModified)) {
			BinFileChanged = 0;
		}
		else {
			BinFileChanged = 1; 
		}
	}
	lastlocalFile = localFile;
	var reader = new FileReader();
	reader.readAsArrayBuffer(localFile);
	reader.onload = function() {
		var bytes = new Uint8Array(reader.result);  
 		var date = new Date(localFile.lastModified);
		dateTime = date.toLocaleString();
		LogConsole(localFile.name + "	:  " + dateTime);
		for(var i = 0; i < bytes.length; i++) {
			g_file_data[i] = bytes[i];
		}
		g_file_length = bytes.length;
		LogConsole("file read end...","backstage");
 		document.getElementById('BinFileImport').innerHTML = '<i class = "layui-icon layui-icon-export" ></i>' + localFile.name;
	}
	document.getElementById('BinFileInput').value = null;
	return 0;
}

$("#FileUploadImport").click(function() {
	$("#FileUploadInput").click();
})

async function UPileRead() {
	var file = document.getElementById("FileUploadInput").files[0];
	var fileName = file.name;
	LogConsole(document.getElementById('FileUploadInput'),"backstage");
	LogConsole(document.getElementById('FileUploadInput').files,"backstage");
	LogConsole(file,"backstage");
 	var reader = new FileReader();
	reader.readAsArrayBuffer(file);
	reader.onload = function() {
		LogConsole(reader.result,"backstage");
		var Mdata = reader.result;
 		if (window.XMLHttpRequest) {//  IE7+, Firefox, Chrome, Opera, Safari
			xhr=new XMLHttpRequest();
		}
		else { // IE6, IE5
			xhr=new ActiveXObject("Microsoft.XMLHTTP");
		}
		xhr.withCredentials = true;
		xhr.addEventListener("readystatechange", function() {
			if (this.readyState == 4 && this.status == 200) {
				LogConsole(this.responseText);
			}
			if(this.status != 200 && this.status != 0) {
				LogConsole( "Network error :" + this.status);
			}
		});
		xhr.open("POST", "./bin/upload_file.php");
		xhr.setRequestHeader("Path", Directory.value);
		xhr.setRequestHeader("Link",LinkCode.value);
		xhr.setRequestHeader("Name", fileName);
		xhr.setRequestHeader("Content-Type", "application/octet-stream");
		xhr.send(Mdata);
 		LogConsole("file read end...","backstage");
	    document.getElementById('FileUploadImport').innerHTML = '<i class = "layui-icon layui-icon-upload-drag" ></i> ' +fileName;
 	}
	document.getElementById('FileUploadInput').value = null;
	return 0;
}

$("#MultiBinFileImport1").click(function() {
    $("#MultiBinFileInput1").click();
})
$("#MultiBinFileImport2").click(function() {
	$("#MultiBinFileInput2").click();
})
$("#MultiBinFileImport3").click(function() {
    $("#MultiBinFileInput3").click();
})
$("#MultiBinFileImport4").click(function() {
	$("#MultiBinFileInput4").click();
})
$("#MultiBinFileImport5").click(function() {
	$("#MultiBinFileInput5").click();
})

async function MultiBinFileRead(id) {
	LogConsole("file read test begin...","backstage");
	var localFile = document.getElementById(id).files[0];
	LogConsole(localFile,"backstage");
	var date = new Date(localFile.lastModified);
	dateTime = date.toLocaleString();
	LogConsole(localFile.name + "	:  " + dateTime);
 
	var reader = new FileReader();
	reader.readAsArrayBuffer(localFile);
	reader.onload = function() {
		var bytes = new Uint8Array(reader.result);  
		if(id == "MultiBinFileInput1") {
			for(var i = 0; i < bytes.length; i++) {
				MultiBinFileData1[i] = bytes[i];
			}
			MultiBinFileLen1 = bytes.length;
			MultiBinFileName1 = localFile.name;
			document.getElementById("MultiBinFileImport1").value=localFile.name;
		}
		else if(id == "MultiBinFileInput2") {
			for(var i = 0; i < bytes.length; i++) {
				MultiBinFileData2[i] = bytes[i];
			}
			MultiBinFileLen2 = bytes.length;
			MultiBinFileName2 = localFile.name;
			document.getElementById("MultiBinFileImport2").value=localFile.name;
		}
		else if(id == "MultiBinFileInput3") {
			for(var i = 0; i < bytes.length; i++) {
				MultiBinFileData3[i] = bytes[i];
			}
			MultiBinFileLen3 = bytes.length;
			MultiBinFileName3 = localFile.name;
			document.getElementById("MultiBinFileImport3").value=localFile.name;
		}
		else if(id == "MultiBinFileInput4") {
			for(var i = 0; i < bytes.length; i++) {
				MultiBinFileData4[i] = bytes[i];
			}
			MultiBinFileLen4 = bytes.length;
			MultiBinFileName4 = localFile.name;
			document.getElementById("MultiBinFileImport4").value=localFile.name;
		}
		else if(id == "MultiBinFileInput5") {
			for(var i = 0; i < bytes.length; i++) {
				MultiBinFileData5[i] = bytes[i];
			}
			MultiBinFileLen5 = bytes.length;
			MultiBinFileName5 = localFile.name;
			document.getElementById("MultiBinFileImport5").value=localFile.name;
		}
		else {
			LogConsole(" ERROR : MultiBinFileRead ");
			return ERROR_Typdef.ERANGE;
		}
		document.getElementById(id).value = null;
		LogConsole("file read end...","backstage");
	}
	return 0;
}


async function usb_evk_getpc_mcu(mode,type) { // get_pc
	if(USBConnectedFlag != 1) {
 		LogConsole("USB device not connected!","Console2");
		return ERROR_Typdef.EPERM;
	}
	if(TL_ModeTypdef.EVK == mode) {
		if(CHIP_91 == type) {
			LogConsole(" This chip done not support the function ","Console2");
			return ERROR_Typdef.ESUPPORT;
		}
		await usb_evk_set_chip(type); 
		await usb_evk_read(0x6bc,4);
 	}
	else if(TL_ModeTypdef.USB == mode ) {
		LogConsole("does not support this function");
		return ERROR_Typdef.ESUPPORT;
	}
	PC = g_evk_rcv_data[0] + g_evk_rcv_data[1]*256 + g_evk_rcv_data[2]*256*256 + g_evk_rcv_data[3]*256*256*256;
	LogConsole(" pc : 0x"+dec2hex(PC,6),"Console2");
 
	for(var m = 0; m < LstFileLineCount; m++) { 
		if(PC >= hex2int(LineAddrSort[m]) && PC < hex2int(LineAddrSort[m+1])) {
			LogConsole(LineFunctionSort[m],"Console2");
			var value = dec2hex2(PC);
			for(var n= LineCount[m];n < LineCount[m+1]; n++) {
				if(LstFileRow[n].indexOf(value) > 0 && LstFileRow[n].indexOf(value+" <") <= 0) {
					LogConsole(LstFileRow[n],"Console2"); 
				}
			}
		}
	}
	return 0;
}

var LstFileRow	     = 0; 	//  反汇编文件一行一行的读取
var ValidAnalyNumVar = 0; 	// 符合解析规则的数目 变量
var ValidAnalyNumFun = 0;	// 符合解析规则的数目 函数

var AnalyNameVar = new Array();
var AnalyAddrVar = new Array();
var AnalyLenVar  = new Array();

var AnalyNameFun = new Array();
var AnalyAddrFun = new Array();
var AnalyLenFun  = new Array();

var Addr = new Array();


var VariableName2 = new Array();
var OriginalAddr2 = new Array();
var Addr2 = new Array();
var Len2 = new Array();
var FunctionVariableName2 = new Array();
var FunctionOriginalAddr2 = new Array();
var FunctionLen2 = new Array();
var Data = new Array();

var LineCount = new Array();
var LineAddrSort = new Array();
var LineFunctionSort = new Array();
var LstFileLineCount = 0;

$("#LstFileImport").click(function() {
    $("#LstFileInput").click();
})

function LstFileRead() {
	ValidAnalyNumVar = 0;
	ValidAnalyNumFun = 0;
	var LstFile = document.getElementById("LstFileInput").files[0];
	LogConsole(LstFile,"backstage");
	var reader = new FileReader();
	reader.readAsText(LstFile);
	reader.onload = function() {
		LogConsole(LstFile.name,"backstage");
		var bytes = reader.result;
		LstFileRow = bytes.split("\n");
		LogConsole(bytes.length,"backstage");
		LogConsole("file read end...","backstage");
		LogConsole(LstFileRow.length,"backstage");

		var reg1 = RegExp(/SYMBOL TABLE:/);
		var reg21 = RegExp(/Disassembly of section .vectors:/);
		var reg22 = RegExp(/Disassembly of section .ram_boot:/);
		var reg23 = RegExp(/Disassembly of section .rodata:/);
		var reg24 = RegExp(/>:/);
		var reg3  = RegExp(/00000000/);
		var reg31  = RegExp(/data_no_init/);
		
		var reg4  = RegExp(/\.data/);
		var reg5  = RegExp(/\.retention_data/);
		var reg6  = RegExp(/\.sbss/);
		var reg7  = RegExp(/\.bss/);
		var reg8  = RegExp(/ l/);
		var reg9  = RegExp(/\.ram_code/);
		var reg10 = RegExp(/\.text/);
		var reg11 = RegExp(/\.hidden/);

		for(var j = 0; j < LstFileRow.length; j++) {
			if(LstFileRow[j].match(reg1)) {
				var start = j;
			}
		}
		for(var j = start; j < LstFileRow.length; j++) {
			if((LstFileRow[j].match(reg21))||(LstFileRow[j].match(reg22))) {
				var end = j;
			}
		}
		for(var j = end; j < LstFileRow.length; j++) {
			if((LstFileRow[j].match(reg23))) {
				var end2 = j;
			}
		}
		if(isNaN(end2)) { 
			end2 = LstFileRow.length + 1;
		}
		start = start + 1;
		end  = end - 3;
		end2 = end2 - 1;
		LogConsole("start:"+start,"backstage");
		LogConsole("end:"+end,"backstage");
		LogConsole("end2:"+end2,"backstage");		
		LstFileLineCount = 0;
		for(var j = end + 5; j < end2; j++) {
			if((LstFileRow[j].match(reg24))) {
				LineCount[LstFileLineCount] = j; // 行号
				LineAddrSort[LstFileLineCount] = LstFileRow[j].substr(0,8);// PC值
				LineFunctionSort[LstFileLineCount] = LstFileRow[j];
				LstFileLineCount++;
			}
		}

		for(var k = start; k < end; k++) { // 00080000 l    d  .data	00000000 .data
			if((!LstFileRow[k].match(reg3))&&(!LstFileRow[k].match(reg31))) {
				if((LstFileRow[k].match(reg4)||LstFileRow[k].match(reg5)||LstFileRow[k].match(reg6)||LstFileRow[k].match(reg7))) {
						AnalyAddrVar[ValidAnalyNumVar] = LstFileRow[k].substr(0,8);
						var temp = LstFileRow[k].substr(0,8);
						if(CHIP_91 == ChipType) {
							if((hex2int(temp) >= 0 )&& (hex2int(temp) <= 0x20000)) {
								Addr[ValidAnalyNumVar] = hex2int(temp)+hex2int("C0000000");//ILM
							}
							else if( (hex2int(temp) >= 0x80000) && (hex2int(temp) <= 0xA0000)) {
								Addr[ValidAnalyNumVar] = hex2int(temp)+hex2int("C0200000");//DLM
							}
						}else { 
								Addr[ValidAnalyNumVar] = hex2int(temp);
						}
						if(LstFileRow[k].match(reg4)) { //	data
							AnalyNameVar[ValidAnalyNumVar] = LstFileRow[k].substr(32);
							if(AnalyNameVar[ValidAnalyNumVar].match(reg11)) { //.hidden
								AnalyNameVar[ValidAnalyNumVar] = AnalyNameVar[ValidAnalyNumVar].substr(7);
							}
							AnalyLenVar[ValidAnalyNumVar] = LstFileRow[k].substr(23,8);
						}
						else if(LstFileRow[k].match(reg5)) {//	retention_data
							AnalyNameVar[ValidAnalyNumVar] = LstFileRow[k].substr(42);
							if(AnalyNameVar[ValidAnalyNumVar].match(reg11)) {//.hidden
								AnalyNameVar[ValidAnalyNumVar] = AnalyNameVar[ValidAnalyNumVar].substr(7);
							}
							AnalyLenVar[ValidAnalyNumVar] = LstFileRow[k].substr(33,8);
						}
						else if(LstFileRow[k].match(reg6)) {
							AnalyNameVar[ValidAnalyNumVar] = LstFileRow[k].substr(32);
							if(AnalyNameVar[ValidAnalyNumVar].match(reg11)) {//.hidden
								AnalyNameVar[ValidAnalyNumVar] = AnalyNameVar[ValidAnalyNumVar].substr(7);
							}
							AnalyLenVar[ValidAnalyNumVar] = LstFileRow[k].substr(23,8);
						}
						else if(LstFileRow[k].match(reg7)) {
							AnalyNameVar[ValidAnalyNumVar] = LstFileRow[k].substr(31);
							if(AnalyNameVar[ValidAnalyNumVar].match(reg11)) {//.hidden
								AnalyNameVar[ValidAnalyNumVar] = AnalyNameVar[ValidAnalyNumVar].substr(7);
							}
							AnalyLenVar[ValidAnalyNumVar] = LstFileRow[k].substr(22,8);
						}
						ValidAnalyNumVar = ValidAnalyNumVar + 1;
				}
				else if((LstFileRow[k].match(reg9)||LstFileRow[k].match(reg10))) {
					AnalyAddrFun[ValidAnalyNumFun] = LstFileRow[k].substr(0,8);
					if(LstFileRow[k].match(reg9)) {
						AnalyNameFun[ValidAnalyNumFun] = LstFileRow[k].substr(36);
						if(AnalyNameFun[ValidAnalyNumFun].match(reg11)) {//.hidden
							AnalyNameFun[ValidAnalyNumFun] = AnalyNameFun[ValidAnalyNumFun].substr(7);
						}
						AnalyLenFun[ValidAnalyNumFun] = LstFileRow[k].substr(28,8);
					}
					else if(LstFileRow[k].match(reg10)) {
						AnalyNameFun[ValidAnalyNumFun] = LstFileRow[k].substr(32);
						if(AnalyNameFun[ValidAnalyNumFun].match(reg11)) {//.hidden
							AnalyNameFun[ValidAnalyNumFun] = AnalyNameFun[ValidAnalyNumFun].substr(7);
						}
						AnalyLenFun[ValidAnalyNumFun] = LstFileRow[k].substr(24,8);
					}
					ValidAnalyNumFun = ValidAnalyNumFun + 1;
				}
			}
		}

		var temp;
		for(var q = 0; q < ValidAnalyNumVar; q++) {
			VariableName2[q] = AnalyNameVar[q];
			OriginalAddr2[q] = AnalyAddrVar[q];
			Addr2[q] = Addr[q];
			Len2[q] = AnalyLenVar[q];
		}
		for(var w = 0; w < ValidAnalyNumFun; w++) {
			FunctionVariableName2[w] = AnalyNameFun[w];
			FunctionOriginalAddr2[w] = AnalyAddrFun[w];
			FunctionLen2[w] = AnalyLenFun[w];
		}

		/***** 按名称来排序 *****/
		for(var a = 0; a < ValidAnalyNumVar; a++) {
			for(var b = 0; b < ValidAnalyNumVar - 1 - a; b++) {
				if (VariableName2[b] > VariableName2[b+1]) {    
					temp = VariableName2[b+1];  
					VariableName2[b+1] = VariableName2[b];
					VariableName2[b] = temp;

					temp = OriginalAddr2[b+1];  
					OriginalAddr2[b+1] = OriginalAddr2[b];
					OriginalAddr2[b] = temp;

					temp = Addr2[b+1];  
					Addr2[b+1] = Addr2[b];
					Addr2[b] = temp;

					temp = Len2[b+1];  
					Len2[b+1] = Len2[b];
					Len2[b] = temp;
				}
			}
		}
		for (var i = 0; i < ValidAnalyNumFun; i++) {
			for (var j = 0; j < ValidAnalyNumFun - 1 - i; j++) {
				if (FunctionVariableName2[j] > FunctionVariableName2[j+1]) { 
					temp = FunctionVariableName2[j+1];
					FunctionVariableName2[j+1] = FunctionVariableName2[j];
					FunctionVariableName2[j] = temp;
					
					temp = FunctionOriginalAddr2[j+1];  
					FunctionOriginalAddr2[j+1] = FunctionOriginalAddr2[j];
					FunctionOriginalAddr2[j] = temp;

					temp = FunctionLen2[j+1];  
					FunctionLen2[j+1] = FunctionLen2[j];
					FunctionLen2[j] = temp;
				}
			}
		} 

		var OriginalAddrSort = new Array();
		for(var n = 0; n < ValidAnalyNumVar; n++) {
			OriginalAddrSort[n] = hex2int(AnalyAddrVar[n]); //十进制
		}

		for(var a = 0; a < ValidAnalyNumVar; a++) {
			for (var b = 0; b < ValidAnalyNumVar - 1 - a; b++) {
				if (OriginalAddrSort[b] > OriginalAddrSort[b+1]) { 
					temp = OriginalAddrSort[b+1];  
					OriginalAddrSort[b+1] = OriginalAddrSort[b];
					OriginalAddrSort[b] = temp;

					temp = Addr[b+1];  
					Addr[b+1] = Addr[b];
					Addr[b] = temp;

					temp = AnalyNameVar[b+1];  
					AnalyNameVar[b+1] = AnalyNameVar[b];
					AnalyNameVar[b] = temp;

					temp = AnalyLenVar[b+1];  
					AnalyLenVar[b+1] = AnalyLenVar[b];
					AnalyLenVar[b] = temp;
				}
			}
		}
		for(var c = 0; c < ValidAnalyNumVar; c++) {
			AnalyAddrVar[c] = dec2hex(OriginalAddrSort[c],8);
		}	

		var FunctionAddrSort = new Array();
		for(var m = 0; m < ValidAnalyNumFun; m++) {
			FunctionAddrSort[m] = hex2int(AnalyAddrFun[m]); //十进制
		}
		for (var i = 0; i < ValidAnalyNumFun; i++) {
			for (var j = 0; j < ValidAnalyNumFun - 1 - i; j++) {
				if (FunctionAddrSort[j] > FunctionAddrSort[j+1]) {  
					temp = FunctionAddrSort[j+1];
					FunctionAddrSort[j+1] = FunctionAddrSort[j];
					FunctionAddrSort[j] = temp;

					temp = AnalyNameFun[j+1];  
					AnalyNameFun[j+1] = AnalyNameFun[j];
					AnalyNameFun[j] = temp;

					temp = AnalyLenFun[j+1];  
					AnalyLenFun[j+1] = AnalyLenFun[j];
					AnalyLenFun[j] = temp;
				}
			}
		}
		for(var m = 0; m < ValidAnalyNumFun; m++) {
			AnalyAddrFun[m] = dec2hex(FunctionAddrSort[m],8);
		}	 
	}
	document.getElementById('LstFileInput').value = null;
}
 
async function RefreshDatas() { 
	if(0 == BurningEVKBusy) {
		BurningEVKBusy = 1;
		if(1 == ShowVariableFlag) { // analytical variables
			if(USBConnectedFlag != 1) {
				await LogConsole("USB device not connected!");
				BurningEVKBusy = 0;
				return ERROR_Typdef.EPERM;
			}
			await LogConsole("Data refreshing. Please wait");
			if(ValidAnalyNumVar != 0) {
				if(undefined == Addr[0]) {
					await LogConsole("plese select file or chip type correctly!","Console2");
					BurningEVKBusy = 0;
					return ERROR_Typdef.ERANGE;
				}
				else {
					for(var j = 0; j< ValidAnalyNumVar; j++) {
						Data[j] = "";
					}			
					await usb_evk_set_chip(ChipType);	
					if(1 == LstFileSortMode) { // sort by address
						for(var j = 0; j < ValidAnalyNumVar; j++) {
							await usb_evk_read(Addr[j],hex2int(AnalyLenVar[j]));
							for(var i = 0; i < hex2int(AnalyLenVar[j]); i++) {
								Data[j] += dec2hex(g_rcv_data[i],2)+" ";
							}
							//await LogConsole(" Data[ "+j+" ]: " + Data[j]);
						} 
					}
					else { // sort by name
						for(var j = 0; j < ValidAnalyNumVar; j++) {
							await usb_evk_read(Addr2[j],hex2int(Len2[j]));
							for(var i = 0; i < hex2int(Len2[j]); i++) {
								Data[j] += dec2hex(g_rcv_data[i],2)+" ";
							}
							//await LogConsole(" Data[ "+j+" ]: " + Data[j]);
						} 
					}
				}
			}  
			else {
				await LogConsole("plese select file or chip type correctly!","Console2");
				BurningEVKBusy = 0;
				return ERROR_Typdef.ERANGE;
			}
			var htmlStr = '<table border = "1">';
			htmlStr += '<thead>\n' +
			'    <th>AnalyNameVar</th>\n' +
			'    <th>Addr</th>\n' +
			'    <th>AnalyLenVar</th>\n' +
			'    <th>Data(LSB first)</th>\n' +
			'  </thead>';
			var dt = document.getElementById('TelinkDebug');
			if(1 == LstFileSortMode) {  // sort by address
				for(var i = 0; i < ValidAnalyNumVar; i++) {
					htmlStr += '<tr>\n' +
					'    <td>'+AnalyNameVar[i]+'</td>\n' +
					'    <td>'+AnalyAddrVar[i]+'</td>\n' +
					'    <td>'+AnalyLenVar[i]+'</td>\n' +
					'    <td>'+Data[i]+'</td>\n' +
					'  </tr>';
				}
			}
			else {
				for(var i = 0 ; i < ValidAnalyNumVar; i ++) {
					htmlStr += '<tr>\n' +
					'    <td>'+VariableName2[i]+'</td>\n' +
					'    <td>'+OriginalAddr2[i]+'</td>\n' +
					'    <td>'+Len2[i]+'</td>\n' +
					'    <td>'+Data[i]+'</td>\n' +
					'  </tr>';
				}
			}
			htmlStr += '</table>';
			dt.innerHTML = htmlStr;
			RefreshFlag = 1;
			await AutoZoom();
			await LogConsole("Data list refresh complete","Console2");
		}
		else { // analytical functions
			if(ValidAnalyNumFun != 0) {
				if(undefined == AnalyAddrFun[0]) {
					await LogConsole("plese select file or chip type correctly!","Console2");
					BurningEVKBusy = 0;
					return ERROR_Typdef.ERANGE;
				}
			}  
			else {
				await LogConsole("plese select file or chip type correctly!","Console2");
				BurningEVKBusy = 0;
				return ERROR_Typdef.ERANGE;
			}
			var htmlStr = '<table border = "1">';
			htmlStr += '<thead>\n' +
			'    <th>AnalyNameVar</th>\n' +
			'    <th>Addr</th>\n' +
			'    <th>AnalyLenVar</th>\n' +
			'  </thead>';
			var dt = document.getElementById('TelinkDebug');
			if(1 == LstFileSortMode) {
				for(var i = 0; i < ValidAnalyNumFun; i++) {
					htmlStr += '<tr>\n' +
					'    <td>' + AnalyNameFun[i] + '</td>\n' +
					'    <td>' + AnalyAddrFun[i] + '</td>\n' +
					'    <td>' + AnalyLenFun[i] + '</td>\n' +
					'  </tr>';
				}
			}
			else {
				for(var i = 0; i < ValidAnalyNumFun; i++) {
					htmlStr +='<tr>\n' +
					'    <td>' + FunctionVariableName2[i] + '</td>\n' +
					'    <td>' + FunctionOriginalAddr2[i] + '</td>\n' +
					'    <td>' + FunctionLen2[i] + '</td>\n' +
					'  </tr>';
				}
			}
			htmlStr += '</table>';
			dt.innerHTML = htmlStr;
			RefreshFlag  = 1;
			await AutoZoom();
			await LogConsole("Function list refresh complete","Console2");
		}
		BurningEVKBusy = 0; 
	}
	else {
		LogConsole("BurningEVK is Busy");
		return ERROR_Typdef.EBUSY;
	}
	return 0;
}

