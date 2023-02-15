
if('serviceWorker' in navigator){    
/*
    navigator.serviceWorker.getRegistration().then(function(regs){
        for(var reg of regs){
            reg.unregister()
        }
    })
*/
    navigator.serviceWorker.register(
        '/web_bdt/sw.js?v=202209071707',{scope:'/web_bdt/'}
    ).then((registration)=>{
      //  console.log('注册成功',registration)
        console.log('page bdt\'s scope is: ',registration.scope)

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

