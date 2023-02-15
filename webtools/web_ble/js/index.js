let newservices = [];

let ArrNotifyListen = [
	"00010203-0405-0607-0809-0a0b0c0d2b10",
];

let ArrWriteListen = [
	["00010203-0405-0607-0809-0a0b0c0d2b11","sendBtn","sendDataInput"],
];

let ArrReadListen = [
	["00002a00-0000-1000-8000-00805f9b34fb","readNameBtn",ReadName],
];

function scanBtn_Onclick(){
	console.log('Requesting any Bluetooth Device...');
	navigator.bluetooth.requestDevice({
			acceptAllDevices: true,
			optionalServices: [
				// 00010203-0405-0607-0809-0a0b0c0d2b10	[READ, NOTIFY]
				// 00010203-0405-0607-0809-0a0b0c0d2b11	[READ, WRITEWITHOUTRESPONSE]
				'00010203-0405-0607-0809-0a0b0c0d1910',
				//00010203-0405-0607-0809-0a0b0c0d2b12	[READ, WRITEWITHOUTRESPONSE, NOTIFY]
				'00010203-0405-0607-0809-0a0b0c0d1912',
				// 0000180a-0000-1000-8000-00805f9b34fb	00002a50-0000-1000-8000-00805f9b34fb	[READ]
				"device_information",
				// 00001801-0000-1000-8000-00805f9b34fb	00002a05-0000-1000-8000-00805f9b34fb	[INDICATE]
				"generic_attribute",
				// 00001800-0000-1000-8000-00805f9b34fb	00002a00-0000-1000-8000-00805f9b34fb	[READ, NOTIFY]
				// 00001800-0000-1000-8000-00805f9b34fb	00002a01-0000-1000-8000-00805f9b34fb	[READ]
				// 00001800-0000-1000-8000-00805f9b34fb	00002a04-0000-1000-8000-00805f9b34fb	[READ]
				"generic_access",
			]
		})
		.then(device => {
			console.log('Connecting to GATT Server...');
			return device.gatt.connect();
		})
		.then(server => {
			console.log('Getting Services...');
			return server.getPrimaryServices();
		})
		.then(services => {
			return overrideServices(services);
		})
		.catch(error => {
			console.error('Argh! ' + error);
		});
};

async function overrideServices(services) {
	console.log('Getting Characteristics...');
	let queue = Promise.resolve();
	services.forEach(service => {
		queue = queue.then(_ => service.getCharacteristics().then(characteristics => {
			let properties;
			characteristics.forEach(characteristic => {
				properties = getSupportedProperties(characteristic)
				newservices.push({
					"serive": service,
					"characteristic": characteristic,
					"properties": properties
				})
			});

		}));
	});
	await sleep(200);
	showBtn_Onclick();
	addEventListen();
	return queue;
}

function addNotifyListen(service){
	if (service.properties.indexOf("NOTIFY") != -1) {
		service.characteristic.startNotifications()
		service.characteristic.addEventListener('characteristicvaluechanged', function(event) {
			const value = event.target.value;
			let rec = new Uint8Array(value.buffer);
			let rechex = '';
			for(var i = 0; i < value.byteLength; i++){
				rechex +=rec[i].toString(16) + "  ";
			}
			ConsolePrint('\r\n   ' + service.characteristic.uuid + "  Received:  " + rechex);
		});
	}
}

function addWriteListen(service,button,input){
	if (service.properties.indexOf("WRITE") != -1) {
		document.getElementById(button).addEventListener("click", async function() {
			// Writing 1 is the signal to reset energy expended. 1234 write: 49/0x31/1,50,51,52						
			const value = InputToHex(document.getElementById(input).value);
			if(-1 !== value){
				await service.characteristic.writeValue(value).then(
					function(Resolve){
						ConsolePrint(" Send: " + document.getElementById(input).value);
					},
					function(Reject){
						console.error("Read error!  " + Reject);
					}
				);
			}
		});
	}
}

// Uncaught (in promise) DOMException: GATT operation already in progress.
function addReadListen(service,button,callback){
	if (service.properties.indexOf("READ") != -1) {
		document.getElementById(button).addEventListener("click",async function() {
			await service.characteristic.readValue().then(
				function(Resolve){
					try {
						let readHex = '';
						new Uint8Array(Resolve.buffer).forEach((item,index) =>{
							// if(item == 0){
							// 	throw new Error("000");
							// }
							readHex +=  dec2hex(item,2) + '  ';
						});
						callback(readHex);
					 } catch(e) {
					// 	if(e.message!="000") throw e;
					};
				},
				function(Reject){
				// 	alert("Read error!  " + Reject);
					console.error("Read error!  " + Reject);
				}
			);
		});
	}
}

function ReadName (readHex){
	// '4B  4D  6F  64  75  6C  65'
	//  K	M	o	d	u	l	e
	let DeviceNameHex;
	DeviceNameHex = readHex.split("  00")[0];
	ConsolePrint("Device Name Hex: " + DeviceNameHex);
	let DeviceNameStr = '';
	DeviceNameHex.split("  ").forEach(Char =>{
		DeviceNameStr += String.fromCharCode(hex2int(Char));
	});
	ConsolePrint("Device Name Str: " + DeviceNameStr);
}

function showBtn_Onclick(){
    ConsolePrint("           service                                  characteristic                          properties")
    newservices.forEach(newservice => {
        ConsolePrint(newservice.characteristic.service.uuid +"    "+ newservice.characteristic.uuid  +"    "+  newservice.properties)
    });
}

function addEventListen(){
	newservices.forEach(newservice => {
		if (ArrNotifyListen.indexOf(newservice.characteristic.uuid) != -1) {
			addNotifyListen(newservice);
		}
		ArrWriteListen.forEach(Arr =>{
			if (Arr[0] == newservice.characteristic.uuid) {
				addWriteListen(newservice,Arr[1],Arr[2]);
			}
		});
		ArrReadListen.forEach(Arr =>{
			if (Arr[0] == newservice.characteristic.uuid) {
				addReadListen(newservice,Arr[1],Arr[2]);
			}
		});
	});
}

function clearBtn_Onclick() {
    document.getElementById("Console1").value = ""; 
}

function ClearConsole_Onclick(){
	console.clear();
}

function ConsolePrint(str){
    Console1Obj = document.getElementById("Console1");
	Console1Obj.value +=  str + "\r\n";
    var mstr = Console1Obj.value; 
    mstr = mstr.replace(/\r/gi,"");
    mstr = mstr.split("\n");
    var n = mstr.length;
    if(n > 50000) {
        Console1Obj.value = ""; 
    }
    Console1Obj.scrollTop = Console1Obj.scrollHeight;
}

function getSupportedProperties(characteristic) {
	let supportedProperties = [];
	for (const p in characteristic.properties) {
		if (characteristic.properties[p] === true) {
			supportedProperties.push(p.toUpperCase());
		}
	}
	return '[' + supportedProperties.join(', ') + ']';
}

/*******UTOOLS********/
function sleep(time) {
    return new Promise((resolve) => setTimeout(resolve, time));
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

function dec2hex(dec, len) {
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

function InputToHex(str){
	var reg = RegExp("  ");
	if(str.match(reg)) {
		alert("Too many spaces");
		return -1;
	}
	var data = str.trim().split(" ");
	var len  = data.length;
	var ret_data = new Uint8Array(len);
	for(var i = 0; i < len; i++) {
		ret_data[i] = hex2int(data[i])&0xff;
	}
	return ret_data;
}



