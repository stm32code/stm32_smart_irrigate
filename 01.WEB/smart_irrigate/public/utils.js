export function getCookie(name)
{
    let arr=document.cookie.split('; ');
    let i=0;
    for(i=0;i<arr.length;i++)
    {
        //arr2->['username', 'abc']
        let arr2=arr[i].split('=');
        if(arr2[0] === name)
        {
            return unescape(decodeURIComponent(arr2[1]));
        }
    }
    return '';
}

// 设置cookie
export  function setCookie(name, value, iDay)
{
    let oDate=new Date();

    oDate.setDate(oDate.getDate()+iDay);

    document.cookie=name+'='+escape(value)+';expires='+oDate;
}

export function delCookie(name) {setCookie(name, "", -1)};

export function formatDate(date, fmt) {
    date = new Date(date);
    if (/(y+)/.test(fmt)) {
        fmt = fmt.replace(RegExp.$1, (date.getFullYear() + '')
            .substr(4 - RegExp.$1.length))
    }
    let otime = {
        'M+': date.getMonth() + 1,
        'd+': date.getDate(),
        'h+': date.getHours(),
        'm+': date.getMinutes(),
        's+': date.getSeconds()
    }
    for (let k in otime) {
        if (new RegExp(`(${k})`).test(fmt)) {
            let str = otime[k] + ''
            fmt = fmt.replace(RegExp.$1, (RegExp.$1.length === 1) ? str : padLeftZero(str))
        }
    }
    return fmt
}

function padLeftZero(str) {
    return ('00' + str).substr(str.length)
}