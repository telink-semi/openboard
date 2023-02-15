// 主要就是缓存内容
const CACHE_VERSION = '1.0.0'
const CACHE_SCOP = 'web_bdt_cache_v'
const CACHE_NAME = CACHE_SCOP + CACHE_VERSION
const URLS = [
    './',
    './bdt.css',
    './bindata.js',
    './click.js',
    './manifest2.json',
    './sw.js',
    './sw-register.js',
    './usb.js',
    './images/bdt-32.png',
    './images/bdt-192.png',
    './images/bdt-512.png',
    './layui-v2.6.8//layui/css/layui.css',
    './layui-v2.6.8//layui/css/modules/code.css',
    './layui-v2.6.8//layui/css/modules/laydate/default/laydate.css',
    './layui-v2.6.8//layui/css/modules/layer/default/icon-ext.png',
    './layui-v2.6.8//layui/css/modules/layer/default/icon.png',
    './layui-v2.6.8//layui/css/modules/layer/default/layer.css',
    './layui-v2.6.8//layui/css/modules/layer/default/loading-0.gif',
    './layui-v2.6.8//layui/css/modules/layer/default/loading-1.gif',
    './layui-v2.6.8//layui/css/modules/layer/default/loading-2.gif',
    './layui-v2.6.8//layui/font/iconfont.eot',
    './layui-v2.6.8//layui/font/iconfont.svg',
    './layui-v2.6.8//layui/font/iconfont.ttf',
    './layui-v2.6.8//layui/font/iconfont.woff',
    './layui-v2.6.8//layui/font/iconfont.woff2',
    './layui-v2.6.8//layui/layui.js',
    './layui-v2.6.8//layui/logo/bitbug_favicon.ico',
    './layui-v2.6.8//layui/logo/logo.png',
    './layui-v2.6.8//layui/myfont/iconfont.css',
    './layui-v2.6.8//layui/myfont/iconfont.ttf',
    './layui-v2.6.8//layui/myfont/iconfont.woff',
    './layui-v2.6.8//layui/myfont/iconfont.woff2',
    './layui-v2.6.8//layui/彩色/demo.css',
    './layui-v2.6.8//layui/彩色/demo_index.html',
    './layui-v2.6.8//layui/彩色/iconfont.css',
    './layui-v2.6.8//layui/彩色/iconfont.js',
    './layui-v2.6.8//layui/彩色/iconfont.json',
    './layui-v2.6.8//layui/彩色/iconfont.ttf',
    './layui-v2.6.8//layui/彩色/iconfont.woff',
    './layui-v2.6.8//layui/彩色/iconfont.woff2',
    './layui-v2.6.8//layui/黑色/demo.css',
    './layui-v2.6.8//layui/黑色/demo_index.html',
    './layui-v2.6.8//layui/黑色/iconfont.css',
    './layui-v2.6.8//layui/黑色/iconfont.js',
    './layui-v2.6.8//layui/黑色/iconfont.json',
    './layui-v2.6.8//layui/黑色/iconfont.ttf',
    './layui-v2.6.8//layui/黑色/iconfont.woff',
    './layui-v2.6.8//layui/黑色/iconfont.woff2',
]

self.addEventListener('install', async event => {
    // 开启一个cache,得到了一个cache对象
    const cache = await caches.open(CACHE_NAME)
    // cache对象就可以存储我们的资源 等待cache把所有资源存储起来
    await cache.addAll(URLS)
    console.log('install', event)
    // 进入到activate
    await self.skipWaiting()
})

//主要清除缓存
self.addEventListener('activate', async event => {
    console.log('activate', event)
    // 清除旧的资源,获取所有资源的key
    const keys = await caches.keys()
    keys.forEach(key => {
        if(key.includes(CACHE_SCOP) && key !== CACHE_NAME){
            caches.delete(key)
        }
    })
   // return 
   // 这里可以判断如果cache里本来就没内容 表示第一次安装 就不用通知用户了
    self.clients.matchAll().then((clients) => {
        if(clients && clients.length){
            clients.forEach(function(client){
                //给每个已经打开的标签都 postMessage
                client.postMessage('sw.update')
            })
        }
    })
    // service worker激活后，立即获取控制权
    await self.clients.claim()
})

// fetch会在请求发送的时候触发
// 判断资源是否能够请求成功，如果能够请求成功，我们就响应请求结果，如果请求失败，读取cache缓存即可
self.addEventListener('fetch', event => {
    // 请求缓存
    const req = event.request
    // 只缓存同源的内容
    const url = new URL(req.url)
   // console.log(req,url)
    if(url.origin !== self.origin){
        return
    }
    // 给浏览器响应
    // event.respondWith(networkFirst(req))
    // 对于静态资源缓存优先，对于需要动态更新的数据，网络优先
    if(req.url.includes('/api')){
        event.respondWith(networkFirst(req))
    }
    else{
        event.respondWith(cacheFirst(req))
    }
})

//网络优先
async function networkFirst(req){
    const cache = await caches.open(CACHE_NAME)
    try{
        // 先从网络读取资源 如果获取到了数据，应该往缓存中存一份
        const fresh = await fetch(req)
        // 网络优先 获取到的数据 应该再次更新到缓存 
        // 把响应的备份存储到缓存中
        cache.put(req, fresh.clone())
        return fresh
    }
    catch(e){
        //去缓存中读取
        const cached = await cache.match(req)
        return cached
     }
}
//缓存优先
async function cacheFirst(req){
    const cache  = await caches.open(CACHE_NAME)
    const cached = await cache.match(req)
    //如果从缓存中得到了
    if(cached){
        return cached
    }
    else{
        const fresh = await fetch(req)
        return fresh 
    }
}

