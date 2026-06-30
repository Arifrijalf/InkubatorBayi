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
.emergency-banner{display:none;background:#dc2626;color:#fff;text-align:center;padding:16px;border-radius:12px;margin-bottom:16px;font-size:1.3rem;font-weight:800;letter-spacing:3px;text-transform:uppercase}
.emergency-banner.show{display:block;animation:emergencyPulse 0.5s infinite}
@keyframes emergencyPulse{0%,100%{opacity:1;background:#dc2626}50%{opacity:.85;background:#991b1b}}
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
.tune-btn{width:100%;padding:12px;border:none;border-radius:8px;font-weight:600;font-size:.9rem;cursor:pointer;transition:all .3s}
.tune-btn-start{background:var(--green);color:#000}
.tune-btn-start:hover{opacity:.85}
.tune-btn-stop{background:var(--red);color:#fff}
.tune-btn-stop:hover{opacity:.85}
.tune-btn:disabled{opacity:.5;cursor:not-allowed}
.tune-progress{height:6px;background:var(--border);border-radius:3px;margin:12px 0;overflow:hidden}
.tune-progress-fill{height:100%;background:var(--green);border-radius:3px;transition:width .5s}
.tune-status{text-align:center;margin-top:12px;font-size:.85rem;color:var(--text2)}
.tune-result{background:var(--bg);border:1px solid var(--border);border-radius:8px;padding:12px;margin-top:12px}
.tune-result h4{font-size:.8rem;color:var(--green);margin-bottom:8px;text-transform:uppercase}
.tune-result .param-row{margin-bottom:4px}
@media(max-width:640px){.grid{grid-template-columns:1fr}.temp-big{font-size:3rem}.pid-output{font-size:2rem}.status-bar{gap:12px}}
</style>
</head>
<body>
<div class="header">
<h1>Inkubator Bayi</h1>
<p>PID Temperature Control System</p>
</div>

<div class="alarm-banner" id="alarmBanner">PERINGATAN: SUHU MELEBIHI BATAS!</div>
<div class="emergency-banner" id="emergencyBanner">EMERGENCY: SISTEM BERHENTI - HEATER OFF / FAN MAX</div>

<div class="status-bar">
<div class="status-item"><div class="status-dot" id="wsDot"></div><span id="wsStatus">Connecting...</span></div>
<div class="status-item"><div class="status-dot dot-green" id="sensorDot"></div><span id="sensorStatus">Sensor OK</span></div>
<div class="status-item" id="alarmItem" style="display:none"><span class="alarm-on">ALARM</span></div>
<div class="status-item" id="emergencyItem" style="display:none"><span class="alarm-on" style="color:#dc2626">EMERGENCY</span></div>
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
<h3>Auto PID Tuning</h3>
<button class="tune-btn tune-btn-start" id="tuneBtn" onclick="startAutoTune()">Start Auto Tune</button>
<div class="tune-progress" id="tuneProgress" style="display:none"><div class="tune-progress-fill" id="tuneProgressFill" style="width:0%"></div></div>
<div class="tune-status" id="tuneStatus"></div>
<div class="tune-result" id="tuneResult" style="display:none"></div>
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
const stateMap={0:'INIT',1:'NORMAL',2:'SENSOR_FAULT',3:'OVERTEMP',4:'OFFLINE',5:'EMERGENCY'};
let chartReady=false;
let audioCtx=null;
let alarmOsc=null;
let alarmGain=null;
let soundEnabled=true;
let alarmActive=false;
let maxTempEver=30;

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
    chart=new Chart(ctx,{
      type:'line',
      data:{
        labels:chartLabels,
        datasets:[
          {label:'Suhu Aktual',data:tempData,borderColor:'#38bdf8',backgroundColor:'rgba(56,189,248,0.08)',fill:true,tension:.3,pointRadius:0,borderWidth:2},
          {label:'Setpoint',data:spData,borderColor:'#f97316',backgroundColor:'transparent',borderDash:[6,3],tension:0,pointRadius:0,borderWidth:2}
        ]
      },
      options:{
        responsive:true,maintainAspectRatio:false,animation:{duration:0},
        scales:{x:{display:false},y:{ticks:{color:'#94a3b8'},grid:{color:'#334155'}}},
        plugins:{
          legend:{labels:{color:'#94a3b8',usePointStyle:true,pointStyle:'line',padding:16}},
          tooltip:{
            mode:'index',intersect:false,
            backgroundColor:'rgba(15,23,42,0.9)',
            titleColor:'#94a3b8',
            bodyColor:'#e2e8f0',
            padding:10,displayColors:false,
            callbacks:{
              label:function(ctx){
                var v=ctx.raw;
                if(ctx.dataset.label==='Suhu Aktual'){return'Suhu: '+v.toFixed(1)+' °C';}
                return ctx.dataset.label+': '+v.toFixed(1);
              }
            }
          }
        }
      }
    });
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

const tunerStateMap={0:'IDLE',1:'RUNNING',2:'COMPLETE',3:'ERROR'};
function update(d){
  if (d.temperature === undefined || d.setpoint === undefined) return;
  
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
  const emergencyActive=!!d.emergency;

  document.getElementById('alarmBanner').className='alarm-banner'+(alarmActive?' show':'');
  document.getElementById('alarmItem').style.display=alarmActive?'flex':'none';
  document.getElementById('emergencyBanner').className='emergency-banner'+(emergencyActive?' show':'');
  document.getElementById('emergencyItem').style.display=emergencyActive?'flex':'none';
  document.getElementById('temp').style.color=alarmActive||emergencyActive?'#ef4444':'';

  if(alarmActive){alarmSoundOn()}else{alarmSoundOff()}

  if(emergencyActive){
    document.getElementById('heaterBar').style.width='0%';
    document.getElementById('heaterVal').textContent='0';
    document.getElementById('fanBar').style.width='100%';
    document.getElementById('fanVal').textContent='1023';
  }

  const now=new Date();
  chartLabels.push(pad(now.getMinutes())+':'+pad(now.getSeconds()));
  tempData.push(d.temperature);
  spData.push(d.setpoint);
if(chartLabels.length>MAX_PTS){chartLabels.shift();tempData.shift();spData.shift()}
  
  const currentMax = Math.max(...tempData, ...spData);
  const currentMin = Math.min(...tempData, ...spData);
  
  if(chartReady && chart.options.scales.y) {
    chart.options.scales.y.max = Math.max(currentMax + 5, 45); 
    chart.options.scales.y.min = Math.min(currentMin - 5, 30);
    chart.update('none');
  }

  // Auto-tuner UI update
  const tunerState = tunerStateMap[d.tuner_state] || 'UNKNOWN';
  document.getElementById('tuneStatus').textContent = 'Status: ' + tunerState;
  document.getElementById('tuneProgressFill').style.width = (d.tuner_progress * 100) + '%';

  if (d.tuner_state === 1) { // RUNNING
    document.getElementById('tuneBtn').textContent = 'Cancel Auto Tune';
    document.getElementById('tuneBtn').className = 'tune-btn tune-btn-stop';
    document.getElementById('tuneBtn').onclick = cancelAutoTune;
    document.getElementById('tuneProgress').style.display = 'block';
    document.getElementById('tuneResult').style.display = 'none';
  } else {
    document.getElementById('tuneBtn').textContent = 'Start Auto Tune';
    document.getElementById('tuneBtn').className = 'tune-btn tune-btn-start';
    document.getElementById('tuneBtn').onclick = startAutoTune;
    document.getElementById('tuneProgress').style.display = 'none';
    if (d.tuner_state === 2 && d.kp && d.ki && d.kd) { // COMPLETE
      document.getElementById('tuneResult').style.display = 'block';
      document.getElementById('tuneResult').innerHTML = `
        <h4>Tuning Complete!</h4>
        <div class="param-row">Kp: <span class="info-val">${d.kp.toFixed(2)}</span></div>
        <div class="param-row">Ki: <span class="info-val">${d.ki.toFixed(2)}</span></div>
        <div class="param-row">Kd: <span class="info-val">${d.kd.toFixed(2)}</span></div>
        <button class="param-btn" onclick="applyTunedPID(${d.kp}, ${d.ki}, ${d.kd})">Apply Tuned PID</button>
      `;
      document.getElementById('kpIn').value = d.kp.toFixed(2);
      document.getElementById('kiIn').value = d.ki.toFixed(2);
      document.getElementById('kdIn').value = d.kd.toFixed(2);
    } else {
      document.getElementById('tuneResult').style.display = 'none';
    }
  }
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

function startAutoTune() {
  if (ws && ws.readyState === 1) {
    ws.send(JSON.stringify({ action: "autotune_start" }));
  }
}

function cancelAutoTune() {
  if (ws && ws.readyState === 1) {
    ws.send(JSON.stringify({ action: "autotune_cancel" }));
  }
}

function applyTunedPID(kp, ki, kd) {
  document.getElementById('kpIn').value = kp.toFixed(2);
  document.getElementById('kiIn').value = ki.toFixed(2);
  document.getElementById('kdIn').value = kd.toFixed(2);
  sendPID();
}

window.addEventListener('load',function(){initChart();connect()});
</script>
</body>
</html>
)rawliteral";
