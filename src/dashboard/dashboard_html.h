#pragma once

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Inkubator Bayi</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
:root{--bg:#0f172a;--card:#1e293b;--border:#334155;--text:#e2e8f0;--text2:#94a3b8;--accent:#38bdf8;--green:#22c55e;--red:#ef4444;--orange:#f97316;--purple:#a78bfa}
body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;background:var(--bg);color:var(--text);min-height:100vh;padding:16px}
.header{text-align:center;padding:20px 0}
.header h1{font-size:1.8rem;background:linear-gradient(135deg,var(--accent),var(--purple));-webkit-background-clip:text;-webkit-text-fill-color:transparent;margin-bottom:4px}
.header p{color:var(--text2);font-size:.85rem}
.alarm-banner{display:none;background:var(--red);color:#fff;text-align:center;padding:14px;border-radius:12px;margin-bottom:16px;font-size:1.1rem;font-weight:700;letter-spacing:2px}
.alarm-banner.show{display:block;animation:blink 1s infinite}
.status-bar{display:flex;justify-content:center;gap:24px;padding:12px;background:var(--card);border-radius:12px;margin-bottom:20px;flex-wrap:wrap}
.status-item{display:flex;align-items:center;gap:6px;font-size:.8rem;color:var(--text2)}
.status-dot{width:8px;height:8px;border-radius:50%}
.dot-green{background:var(--green)}.dot-red{background:var(--red)}.dot-orange{background:var(--orange)}
@keyframes blink{0%,100%{opacity:1}50%{opacity:.3}}
.alarm-on{animation:blink 0.5s infinite;color:var(--red)!important;font-weight:600}
.grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(300px,1fr));gap:16px;max-width:1400px;margin:0 auto}
.card{background:var(--card);border:1px solid var(--border);border-radius:16px;padding:20px;backdrop-filter:blur(10px)}
.card h3{font-size:.9rem;color:var(--text2);margin-bottom:16px;text-transform:uppercase;letter-spacing:1px;font-weight:500}
.temp-display{text-align:center;padding:20px 0}
.temp-big{font-size:4rem;font-weight:700;line-height:1}
.temp-unit{font-size:1.5rem;color:var(--text2);margin-left:4px}
.temp-setpoint{font-size:1rem;color:var(--accent);margin-top:8px}
.pid-output{font-size:3rem;font-weight:700;text-align:center;padding:16px 0}
.pid-label{font-size:.8rem;color:var(--text2);margin-top:4px}
.gauge-container{display:flex;gap:20px;justify-content:center}
.gauge{text-align:center;flex:1}
.gauge-label{font-size:.8rem;color:var(--text2);margin-bottom:4px}
.gauge-value{font-size:1.8rem;font-weight:600}
.gauge-bar{height:6px;background:var(--border);border-radius:3px;margin-top:8px;overflow:hidden}
.gauge-fill{height:100%;border-radius:3px;transition:width .3s}
.pid-params{display:flex;flex-direction:column;gap:12px}
.param-row{display:flex;align-items:center;gap:12px}
.param-label{width:30px;font-weight:600;color:var(--accent)}
.param-input{flex:1;background:var(--bg);border:1px solid var(--border);border-radius:8px;padding:8px 12px;color:var(--text);font-size:.9rem;outline:none}
.param-input:focus{border-color:var(--accent)}
.param-btn{background:var(--accent);color:#000;border:none;border-radius:8px;padding:8px 16px;cursor:pointer;font-weight:600;font-size:.85rem}
.param-btn:hover{opacity:.85}
.info-grid{display:grid;grid-template-columns:1fr 1fr;gap:8px}
.info-item{display:flex;justify-content:space-between;padding:6px 0;border-bottom:1px solid var(--border)}
.info-item:last-child{border:none}
.info-key{color:var(--text2);font-size:.8rem}
.info-val{font-weight:500;font-size:.85rem}
.chart-container{height:250px;position:relative}
#tempChart{width:100%!important;height:100%!important}
.sound-toggle{position:fixed;bottom:16px;right:16px;background:var(--card);border:1px solid var(--border);border-radius:50%;width:48px;height:48px;display:flex;align-items:center;justify-content:center;cursor:pointer;font-size:1.3rem;z-index:100}
.sound-toggle:hover{border-color:var(--accent)}
@media(max-width:640px){.grid{grid-template-columns:1fr}.temp-big{font-size:3rem}.pid-output{font-size:2rem}.status-bar{gap:12px}}
</style>
</head>
<body>
<div class="header">
<h1>Inkubator Bayi</h1>
<p>PID Temperature Control System</p>
</div>

<div class="alarm-banner" id="alarmBanner">PERINGATAN: SUHU MELEBIHI BATAS!</div>

<div class="status-bar">
<div class="status-item"><div class="status-dot" id="wsDot"></div><span id="wsStatus">Connecting...</span></div>
<div class="status-item"><div class="status-dot dot-green" id="sensorDot"></div><span id="sensorStatus">Sensor OK</span></div>
<div class="status-item" id="alarmItem" style="display:none"><span class="alarm-on">ALARM</span></div>
<div class="status-item"><span id="uptime">00:00:00</span></div>
<div class="status-item"><span id="ip">--</span></div>
</div>

<div class="grid">
<div class="card">
<h3>Temperature</h3>
<div class="temp-display">
<span class="temp-big" id="temp">--</span><span class="temp-unit">&deg;C</span>
<div class="temp-setpoint">Setpoint: <span id="spDisp">37.0</span>&deg;C</div>
</div>
</div>

<div class="card">
<h3>PID Output</h3>
<div class="pid-output" id="pidOut">--</div>
<div class="pid-label">Error: <span id="errDisp">0.00</span>&deg;C</div>
</div>

<div class="card">
<h3>Actuators</h3>
<div class="gauge-container">
<div class="gauge">
<div class="gauge-label">Heater</div>
<div class="gauge-value" id="heaterVal">0</div>
<div class="gauge-bar"><div class="gauge-fill" id="heaterBar" style="width:0;background:var(--red)"></div></div>
</div>
<div class="gauge">
<div class="gauge-label">Fan</div>
<div class="gauge-value" id="fanVal">0</div>
<div class="gauge-bar"><div class="gauge-fill" id="fanBar" style="width:0;background:var(--accent)"></div></div>
</div>
</div>
</div>

<div class="card">
<h3>PID Tunings</h3>
<div class="pid-params">
<div class="param-row">
<span class="param-label">Kp</span>
<input class="param-input" type="number" step="0.1" id="kpIn" value="2.0">
<button class="param-btn" onclick="sendPID()">Apply</button>
</div>
<div class="param-row">
<span class="param-label">Ki</span>
<input class="param-input" type="number" step="0.1" id="kiIn" value="0.5">
</div>
<div class="param-row">
<span class="param-label">Kd</span>
<input class="param-input" type="number" step="0.1" id="kdIn" value="1.0">
</div>
<div class="param-row">
<span class="param-label">SP</span>
<input class="param-input" type="number" step="0.1" id="spIn" value="37.0">
</div>
</div>
</div>

<div class="card">
<h3>System Info</h3>
<div class="info-grid">
<div class="info-item"><span class="info-key">Free Heap</span><span class="info-val" id="heap">--</span></div>
<div class="info-item"><span class="info-key">WiFi RSSI</span><span class="info-val" id="rssi">--</span></div>
<div class="info-item"><span class="info-key">Kp</span><span class="info-val" id="kpVal">--</span></div>
<div class="info-item"><span class="info-key">Ki</span><span class="info-val" id="kiVal">--</span></div>
<div class="info-item"><span class="info-key">Kd</span><span class="info-val" id="kdVal">--</span></div>
<div class="info-item"><span class="info-key">State</span><span class="info-val" id="stateVal">--</span></div>
</div>
</div>

<div class="card" style="grid-column:1/-1">
<h3>Temperature History</h3>
<div class="chart-container"><canvas id="tempChart"></canvas></div>
</div>
</div>

<div class="sound-toggle" id="soundBtn" title="Toggle Alarm Sound">&#128266;</div>

<script src="https://cdn.jsdelivr.net/npm/chart.js@4.4.7/dist/chart.umd.min.js"></script>
<script>
let ws,reconTimer,chart,tempData=[],spData=[],chartLabels=[];
const MAX_PTS=60;
const stateMap={0:'INIT',1:'NORMAL',2:'SENSOR_FAULT',3:'OVERTEMP',4:'OFFLINE'};
let chartReady=false;
let audioCtx=null;
let alarmOsc=null;
let alarmGain=null;
let soundEnabled=true;
let alarmActive=false;

function initAudio(){
  if(!audioCtx){
    try{
      audioCtx=new(window.AudioContext||window.webkitAudioContext)();
      alarmGain=audioCtx.createGain();
      alarmGain.connect(audioCtx.destination);
      alarmGain.gain.value=0;
      alarmOsc=audioCtx.createOscillator();
      alarmOsc.type='square';
      alarmOsc.frequency.value=2000;
      alarmOsc.connect(alarmGain);
      alarmOsc.start();
    }catch(e){}
  }
  if(audioCtx&&audioCtx.state==='suspended'){audioCtx.resume();}
}

function alarmSoundOn(){
  if(!soundEnabled||!audioCtx)return;
  alarmGain.gain.setTargetAtTime(0.15,audioCtx.currentTime,0.05);
}

function alarmSoundOff(){
  if(!audioCtx)return;
  alarmGain.gain.setTargetAtTime(0,audioCtx.currentTime,0.05);
}

document.getElementById('soundBtn').addEventListener('click',function(){
  initAudio();
  soundEnabled=!soundEnabled;
  this.textContent=soundEnabled?'\u{1F50A}':'\u{1F507}';
  if(!soundEnabled)alarmSoundOff();
  if(alarmActive&&soundEnabled)alarmSoundOn();
});

document.addEventListener('click',function(){initAudio();},{once:true});

function initChart(){
  try{
    const ctx=document.getElementById('tempChart').getContext('2d');
    chart=new Chart(ctx,{type:'line',data:{labels:chartLabels,datasets:[
      {label:'Suhu Aktual',data:tempData,borderColor:'#38bdf8',backgroundColor:'rgba(56,189,248,0.08)',fill:true,tension:.3,pointRadius:0,borderWidth:2},
      {label:'Setpoint',data:spData,borderColor:'#f97316',backgroundColor:'transparent',borderDash:[6,3],tension:0,pointRadius:0,borderWidth:2}
    ]},options:{responsive:true,maintainAspectRatio:false,animation:{duration:0},scales:{x:{display:false},y:{ticks:{color:'#94a3b8'},grid:{color:'#334155'}}},plugins:{legend:{labels:{color:'#94a3b8',usePointStyle:true,pointStyle:'line',padding:16}}}}});
    chartReady=true;
  }catch(e){
    document.getElementById('tempChart').outerHTML='<div style="color:#ef4444;text-align:center;padding:40px">Chart.js tidak bisa dimuat</div>';
  }
}

function pad(n){return String(n).padStart(2,'0')}
function fmtTime(s){return pad(Math.floor(s/3600))+':'+pad(Math.floor(s%3600/60))+':'+pad(s%60)}

function connect(){
  ws=new WebSocket('ws://'+location.host+'/ws');
  ws.onopen=function(){
    document.getElementById('wsDot').className='status-dot dot-green';
    document.getElementById('wsStatus').textContent='Connected';
  };
  ws.onclose=function(){
    document.getElementById('wsDot').className='status-dot dot-red';
    document.getElementById('wsStatus').textContent='Disconnected';
    clearTimeout(reconTimer);
    reconTimer=setTimeout(connect,2000);
  };
  ws.onmessage=function(e){
    try{update(JSON.parse(e.data))}catch(ex){}
  };
}

function update(d){
  document.getElementById('temp').textContent=d.temperature.toFixed(1);
  document.getElementById('spDisp').textContent=d.setpoint.toFixed(1);
  document.getElementById('pidOut').textContent=d.pid_output.toFixed(1);
  document.getElementById('errDisp').textContent=d.error.toFixed(2);
  document.getElementById('heaterVal').textContent=d.heater_pwm;
  document.getElementById('fanVal').textContent=d.fan_pwm;
  document.getElementById('heaterBar').style.width=(d.heater_pwm/1023*100)+'%';
  document.getElementById('fanBar').style.width=(d.fan_pwm/1023*100)+'%';
  document.getElementById('heap').textContent=d.heap;
  document.getElementById('rssi').textContent=d.wifi+' dBm';
  document.getElementById('kpVal').textContent=d.kp.toFixed(2);
  document.getElementById('kiVal').textContent=d.ki.toFixed(2);
  document.getElementById('kdVal').textContent=d.kd.toFixed(2);
  document.getElementById('stateVal').textContent=stateMap[d.state]||'UNKNOWN';
  document.getElementById('uptime').textContent=fmtTime(d.uptime);
  document.getElementById('sensorDot').className='status-dot '+(d.sensor_status===0?'dot-green':'dot-red');
  document.getElementById('sensorStatus').textContent=d.sensor_status===0?'Sensor OK':'Sensor Error';

  alarmActive=!!d.alarm;
  document.getElementById('alarmBanner').className='alarm-banner'+(alarmActive?' show':'');
  document.getElementById('alarmItem').style.display=alarmActive?'flex':'none';
  document.getElementById('temp').style.color=alarmActive?'#ef4444':'';

  if(alarmActive){alarmSoundOn()}else{alarmSoundOff()}

  const now=new Date();
  chartLabels.push(pad(now.getMinutes())+':'+pad(now.getSeconds()));
  tempData.push(d.temperature);
  spData.push(d.setpoint);
  if(chartLabels.length>MAX_PTS){chartLabels.shift();tempData.shift();spData.shift()}
  if(chartReady)chart.update();
}

function sendPID(){
  const kp=parseFloat(document.getElementById('kpIn').value)||0;
  const ki=parseFloat(document.getElementById('kiIn').value)||0;
  const kd=parseFloat(document.getElementById('kdIn').value)||0;
  const sp=parseFloat(document.getElementById('spIn').value)||37;
  if(ws&&ws.readyState===1){
    ws.send(JSON.stringify({kp:kp,ki:ki,kd:kd,setpoint:sp}));
  }
}

window.addEventListener('load',function(){initChart();connect()});
</script>
</body>
</html>
)rawliteral";
