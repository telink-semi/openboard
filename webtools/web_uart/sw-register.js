
if('serviceWorker' in navigator){    
/*
    navigator.serviceWorker.getRegistration().then(function(regs){
        for(var reg of regs){
            reg.unregister()
        }
    })
*/
    navigator.serviceWorker.register(
        '/web_uart/sw.js?v=202209071708',{scope:'/web_uart/'}
    ).then((registration)=>{
      // console.log('注册成功',registration)
        console.log('page uart\'s scope is: ',registration.scope)
    }).catch((error)=>{
        console.log('注册失败',error)
    });

    navigator.serviceWorker.addEventListener('message', function (e){
        if(e.data == 'sw.update'){
            // 可换成提醒用户刷新页面
            window.location.reload()
        }
    })
}

