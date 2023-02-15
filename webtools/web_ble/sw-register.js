
if('serviceWorker' in navigator){    
/*
    navigator.serviceWorker.getRegistration().then(function(regs){
        for(var reg of regs){
            reg.unregister()
        }
    })
*/
    navigator.serviceWorker.register(
        '/web_ble/sw.js?v=202209211827',{scope:'/web_ble/'}
    ).then((registration)=>{
      // console.log('注册成功',registration)
        console.log('page ble\'s scope is: ',registration.scope)
    }).catch((error)=>{
        console.log('注册失败',error)
    });

    navigator.serviceWorker.addEventListener('message', function (e){
        if(e.data == 'sw.update'){
            // 可换成提醒用户刷新页面
            window.location.reload();
        }
    })
}

