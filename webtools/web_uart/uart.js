const serialResultsDiv = document.getElementById("Console1");

var port, textEncoder, writableStreamClosed, writer,HexReader,HexWriter;

var man_send_flag = 0;
var g_rcv_callback;

async function connectSerial(baudrate,callback) {
    try {
        // Prompt user to select any serial port.
        port = await navigator.serial.requestPort();
        await port.open({ baudRate: baudrate });
        listenToPort();
        HexWriter = port.writable.getWriter();
        g_rcv_callback = callback;
        return true;
    } catch {
        alert("Serial Connection Failed");
        return false;
    }
}

async function sendCharacterNumber(){
    document.getElementById("lineToSend").value = String.fromCharCode(document.getElementById("lineToSend").value);
}
   
async function sendSerialLine(dataToSend) {
    await writer.write(dataToSend);
}

async function sendSerialHexArray(HexArrayToSend) {
    await HexWriter.write(HexArrayToSend);
}

function String2hex(dec){
    return (dec).toString(16);
}

function dec2hex(dec, len) { // 10进制转16进制补0
    var hex = "";
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

function stringToUint8Array(str){
    var arr = [];
    for (var i = 0, j = str.length; i < j; ++i) {
        arr.push(str.charCodeAt(i));
    }
    var tmpUint8Array = new Uint8Array(arr);
    return tmpUint8Array;
}

function Uint8ArrayToString(fileData){
    var dataString = "";
    for (var i = 0; i < fileData.length; i++) {
        dataString += String.fromCharCode(fileData[i]);
    }
    return dataString;
}

async function readWithTimeout(port, timeout) {
    const reader = port.readable.getReader();
    const timer = setTimeout(() => {
        reader.releaseLock();
    }, timeout);
    const result = await reader.read();
    clearTimeout(timer);
    reader.releaseLock();
    return result;
}
/*
var g_rcv_data =  new Uint8Array(1024*1024);
var g_rcv_index = 0;
var pingpong = 0;
var g_rcv_index_1 = 0;
var g_rcv_index_2 = 0;

function man_send_recive_deal(){
    myprint("\r\n recive length1:  " + g_rcv_index);
    for(var i = 0; i < g_rcv_index; i++){
        appendToTerminal("  0x" + dec2hex(g_rcv_data[i],2),"233");
    }
}

function rf_send_recive_deal(){
    myprint("\r\n recive length2:  " + g_rcv_index);
}

function recive_deal(){
    //    var d = new Date();
    //    var t = d.toLocaleTimeString();
    //    console.log(t);
    if(pingpong++%2){
        g_rcv_index_1 = g_rcv_index;
    }
    else{
        g_rcv_index_2 = g_rcv_index;
    }
    if(g_rcv_index_1 != g_rcv_index_2){ // 接收到一笔数据
        if(man_send_flag){
            man_send_flag = 0;
            man_send_recive_deal();
        }
        else{
            rf_send_recive_deal();
        }
        g_rcv_index_1 = 0;
        g_rcv_index_2 = 0;
        g_rcv_index   = 0;
    }
}

var sleep = function(time) {
    var startTime = new Date().getTime() + parseInt(time, 10);
    while(new Date().getTime() < startTime) {}
}
*/
var  g_time_dataprocess = 0;
var  g_len;
var  meter1;
var  meter2;
var  g_rcv_buffer = new Uint8Array(1024*1024);
var  g_idx;
var  uart_wait_time = 50;

function data_rcv() {
    g_rcv_callback(g_len,g_rcv_buffer);    
}

async function data_process(data) {
    var t_now = Date.now();
    var t_timeout = false;
    if(t_now - g_time_dataprocess > uart_wait_time) {
        t_timeout = true;
    }
    g_time_dataprocess = t_now;
           
    if(t_timeout) { // 100ms
        g_len = data.byteLength;
        meter1 = setTimeout(data_rcv,uart_wait_time);  
        g_idx = 0;
        for(var i = 0; i < data.byteLength; i++){
             g_rcv_buffer[g_idx++] = data[i];
        }
        //console.log("g_user_callback set M1!!");
    }
    else{
        clearTimeout(meter1);
        clearTimeout(meter2);
        for(var i = 0; i < data.byteLength; i++){
            g_rcv_buffer[g_idx++] = data[i];
        }
        //console.log("g_user_callback Cancel M1,M2!!");
        g_len = g_len + data.byteLength;
        meter2 = setTimeout(data_rcv,uart_wait_time);  
        //console.log("g_user_callback set M2");   
    }
}  

async function listenToPort() {
    HexReader = port.readable.getReader();
    // Listen to data coming from the serial device.
    try {
        while (true) {
            const { value, done } = await HexReader.read();
            if (done) {
                // Allow the serial port to be closed later.
                 HexReader.releaseLock();
                break;
            }
            // value is a string.
            /*
            for(var i = 0; i < value.byteLength; i++){
                appendToTerminal("  0x" + dec2hex(value[i],2));
                g_rcv_data[g_rcv_index++] = value[i];
            }
            */
            data_process(value);
        }
    } catch(error) {
        alert("Serial Read Error");
        closePort();
    }finally {
        HexReader.releaseLock();
    }
}

async function appendToTerminal(newStuff) {
    serialResultsDiv.value += newStuff;
    serialResultsDiv.scrollTop = serialResultsDiv.scrollHeight;
}

async function closePort() {
    try {
        await HexReader.cancel();
        await HexWriter.close();
        await port.close();
        alert("Serial Close OK!");
    } catch(error) {
        alert("Serial Close Error");
    }
}
