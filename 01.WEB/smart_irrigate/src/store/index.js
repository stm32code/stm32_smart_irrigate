import Vue from 'vue'
import Vuex from 'vuex'
import mqtt from 'mqtt'
import { constants } from 'buffer'

Vue.use(Vuex)

var options = {
  connectTimeout: 4000, // 超时时间
  //clientId: 'h9sj66W0Eui.SmartWater-Web|securemode=2,signmethod=hmacsha256,timestamp=1682960078058|', // 产品ID
  //username: 'SmartWater-Web&h9sj66W0Eui', // 设备ID 943284199
  //password: '8d01978c2de07956559a0a1625c8945ad233dea5bc4726c724dfbbca5394a9a7', // 鉴权
  clientId: 'k11q5MRdCXb.traffice_light_app|securemode=2,signmethod=hmacsha256,timestamp=1714293301898|', // 产品ID
  username: 'traffice_light_app&k11q5MRdCXb', // 设备ID 943284199
  password: '30b7de98a2bf3eabfdad4b83cbe752c2de138f6deb00abdabdfddb0234b7e33a', // 鉴权
  cleanSession: false,
  keepAlive: 60 // 心跳值，心跳值太大可能会连接不成功，这个参考文档
}
var subscription = {
  topic: '/broadcast/k11q5MRdCXb/waterpumpapp_topic',
  qos: 0
}

export default new Vuex.Store({
  state: {
    temp:0,
    humi:0,
    date:[],
    light:0,
    Soil1_v:0,
    SoilHumiOne:[],
    SoilHumiTwo:[],
    WaterPumpOne:0,
    WaterPumpTwo:0,
    Soil2_v:0,
    Flage:0,
    client: null,
  },
  mutations: {
    // 类似于set方法
    MqttClient: (state) => {//建立连接
      try {
        state.client = mqtt.connect("ws://iot-06z00bhlj16tvj8.mqtt.iothub.aliyuncs.com:443", options)
        //state.client = mqtt.connect("ws://mqtt.heclouds.com:6002", options)
        //state.client = mqtt.connect("ws://127.0.0.1:6002", options)
      } catch (error) {
        console.log("mqtt连接失败", error)
      }
      /**
       * 连接成功则订阅主题
       */
      state.client.on("connect", (error) => {
        console.log("连接成功")
        const { topic, qos } = subscription //订阅主题
        try {
          state.client.subscribe(topic, qos, (error) => {
            if (!error) {
              console.log('订阅成功')
            } else {
              console.log('订阅失败')
            }
          })
        } catch (error) {
          console.log('订阅错误:', error)
        }
      })
      /**
       * 接收消息
       */
      state.client.on('message', (topic, message) => {
        console.log('收到来自', topic, '的消息', message.toString());
        var date=new Date();
        var timeStr = date.getFullYear() + '-';
        if(date.getMonth() < 9) {
          //月份从0开始的
          timeStr += '0';
        }
        timeStr += date.getMonth() + 1 + '-';
        timeStr += date.getDate() < 10 ? ('0' + date.getHours()) : date.getHours();
        timeStr += ' ';
        timeStr += date.getHours() < 10 ? ('0' + date.getHours()) : date.getHours();
        timeStr += ':';
        timeStr += date.getMinutes() < 10 ? ('0' + date.getMinutes()) : date.getMinutes();
        timeStr += ':';
        timeStr += date.getSeconds() < 10 ? ('0' + date.getSeconds()) : date.getSeconds();

        console.log(timeStr);
        disposeData(state,message.toString(),timeStr);
      })
      // 连接错误处理
      state.client.on('error', (error) => {
        console.log('连接出错: ', error)
      })
    },
    pushClinet(state){
        
    }
  },
  getters: {

  },
  actions: {
    connectMqtt: (context) => {
      console.log("dsfa")
      context.commit("MqttClient")
    }
  },
  modules: {

  }
})

function disposeData(state, message,timeStr) {
  var total=state.SoilHumiOne.length;
  var json=JSON.parse(message);
  console.log(json);
  if(json.light!=null){
    state.light=json.light;
  }
  if(json.Flage!=null){
    state.Flage=json.Flage;
  }
  if(json.humi!=null){
    state.humi=json.humi;
  }
  if(json.temp!=null){
    state.temp=json.temp;
  }
  if(json.Soil1_v!=null){
    state.Soil1_v=json.Soil1_v;
  }
  if(json.Soil2_v!=null){
    state.Soil2_v=json.Soil2_v;
  }
  if(json.Water_Pump1!=null){
    state.WaterPumpOne=json.Water_Pump1;
  }
  if(json.Water_Pump2!=null){
    state.WaterPumpTwo=json.Water_Pump2;
  }
  if(json.Soil_Humi1!=null&&json.Soil_Humi2!=null){
    if(total==10){
      state.date.shift();
      state.SoilHumiOne.shift();
      state.SoilHumiTwo.shift();
      total=total-1;
      state.SoilHumiOne[total]=Math.round(json.Soil_Humi1);
      state.SoilHumiTwo[total]=Math.round(json.Soil_Humi2);
      state.date[total]=timeStr.substring(11,19);
    }else{
      state.SoilHumiOne[total]=Math.round(json.Soil_Humi1);
      state.SoilHumiTwo[total]=Math.round(json.Soil_Humi2);
      state.date[total]=timeStr.substring(11,19);
    }
  }
}

