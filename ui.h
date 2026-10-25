#pragma once
#include <pgmspace.h>
#include "config.h"

#define UI_STR_(x) #x
#define UI_STR(x) UI_STR_(x)

// Static UI assets stay in flash. HTTP routes and streaming live in http_server.cpp.

static const char HTML_BEGIN[] PROGMEM = R"html(
<!DOCTYPE HTML>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="theme-color" content="#152923">
    <link rel="icon" type="image/svg+xml" href="/icons/favicon-8c5115a27260.svg">

)html";

static const char HTML_LAYOUT[] PROGMEM = R"html(
    <style>
      :root{color-scheme:light;--ink:#152923;--muted:#60716a;--line:#dce4dc;--green:#236349;--paper:#fffefa}
      *{box-sizing:border-box}body{margin:0;background:#eff2eb;color:var(--ink);font:16px/1.5 system-ui,-apple-system,sans-serif}
      .center_div{max-width:520px;margin:auto;padding:32px 24px 48px}.brand{display:flex;align-items:center;gap:12px;font-size:19px;font-weight:750;letter-spacing:-.5px}.version{font-size:12px;font-weight:600;letter-spacing:0;color:var(--muted)}.brand-mark{display:block;width:42px;height:42px}
      .nav{display:flex;gap:6px;background:#e2e8df;padding:5px;border-radius:14px;margin:26px 0 30px}.nav img{width:24px;height:24px;display:block}.nav a{display:flex;align-items:center;justify-content:center;min-height:48px;flex:1;text-align:center;padding:10px;border-radius:10px;text-decoration:none;color:var(--muted);font-weight:650}.nav a[aria-current]{background:var(--paper);color:var(--ink);box-shadow:0 2px 5px #1529230a}
      h1{font-size:30px;line-height:1.2;letter-spacing:-1px;margin:0 0 8px}h2,h3{font-size:20px;letter-spacing:-.5px;margin:0 0 18px}p{color:var(--muted);font-size:14px}
      .card{background:var(--paper);border:1px solid var(--line);border-radius:24px;padding:24px;margin-bottom:20px}
      button{font:inherit;font-weight:650;cursor:pointer;min-height:48px;width:100%;border:1px solid var(--line);border-radius:12px;color:var(--ink);background:#edf1e9;padding:10px 12px;touch-action:manipulation;transition:background .15s,transform .15s}button:hover{background:#e1e8dc}button:active{transform:scale(.97);background:#d4dfce}button:disabled{opacity:.45;cursor:default}button.primary{background:var(--green);color:white;border-color:var(--green)}button.primary:hover{background:#194e38}button:focus-visible,a:focus-visible,input:focus-visible{outline:3px solid #b17a21;outline-offset:3px}
      .row{display:flex;gap:8px}.row>*{flex:1;min-width:0}
      label{display:flex;justify-content:space-between;font-size:12px;font-weight:650;color:var(--muted);margin:14px 0 6px}input{font:inherit;width:100%;max-width:100%;padding:12px;border:1px solid var(--line);border-radius:10px;background:var(--paper);color:var(--ink);min-height:46px;margin-bottom:8px}
      .lamp{display:grid;place-items:center;width:180px;height:180px;margin:8px auto 24px;border-radius:50%;border:6px solid #e2e8df;background:#000;transition:background .15s,box-shadow .15s}.lamp output{font-size:30px;font-weight:650;letter-spacing:-1px;color:#fff;text-shadow:0 1px 4px #0008}
      input[type=range]{-webkit-appearance:none;appearance:none;height:28px;padding:0;margin:0 0 6px;border:1px solid var(--line);border-radius:14px;touch-action:none}
      input[type=range]::-webkit-slider-thumb{-webkit-appearance:none;width:36px;height:36px;border-radius:50%;background:#fff;border:3px solid var(--ink);box-shadow:0 1px 4px #0004}
      input[type=range]::-moz-range-thumb{width:30px;height:30px;border-radius:50%;background:#fff;border:3px solid var(--ink);box-shadow:0 1px 4px #0004}
      #hue{background:linear-gradient(90deg,#f00,#ff0,#0f0,#0ff,#00f,#f0f,#f00)}
      .status{min-height:48px;padding:14px 18px;border:1px solid var(--line);border-radius:14px;margin:20px 0 10px;font-size:13px;background:#fae7df;color:#933c28}hr{border:0;border-top:1px solid var(--line);margin:24px 0}a{color:var(--green)}[id$="res"]{overflow-wrap:anywhere}#vm a{display:inline-block;padding:10px 0}
      @media(max-width:600px){.center_div{padding:20px 16px 32px}.card{padding:20px;border-radius:20px}.nav{margin:22px 0}h1{font-size:27px}}
      @media(prefers-reduced-motion:reduce){button,.lamp{transition:none}}
    </style>
  </head>
  <body><div class="contain"><div class="center_div">
  <header class="brand"><img class="brand-mark" src="/icons/favicon-8c5115a27260.svg" width="42" height="42" alt="" draggable="false"><div>RGB Lamp <small class="version">V)html" UI_STR(FIRMWARE_VERSION) R"html(</small></div></header>
  <nav class="nav" aria-label="Main navigation"><a href="/" aria-label="Home" title="Home"><img src="/icons/home-e4ff8833c2f2.svg" width="24" height="24" alt="" draggable="false"></a><a href="/settings" aria-label="Settings" title="Settings"><img src="/icons/settings-08c94bb532ed.svg" width="24" height="24" alt="" draggable="false"></a></nav>
  <script>document.querySelectorAll('.nav a').forEach(function(a){if(a.getAttribute('href')===(location.pathname==='/'?'/':'/settings'))a.setAttribute('aria-current','page');});</script>
)html";

static const char HTML_END[] PROGMEM = "</div></div></body></html>";

static const char INDEX_HTML[] PROGMEM = R"html(
  <main class="card">
    <div class="lamp" id="lamp"><output id="pct" aria-live="polite">–</output></div>
    <label for="hue">Colour</label><input type="range" id="hue" min="0" max="359">
    <label for="sat">Saturation</label><input type="range" id="sat" min="0" max="100">
    <label for="bri">Brightness</label><input type="range" id="bri" min="0" max="100">
  </main>
  <p id="res" class="status" role="status" aria-live="polite" hidden></p>
<script>
  function $(id){return document.getElementById(id);}
  /* Colour at full value from hue (0-359) and saturation (0-100). */
  function rgb(h,s){
    s/=100;return [5,3,1].map(function(n){var k=(n+h/60)%6;return Math.round(255*(1-s*Math.max(0,Math.min(k,4-k,1))));});
  }
  function hex(c){return c.map(function(v){return (v<16?'0':'')+v.toString(16);}).join('');}
  function css(c){return 'rgb('+c.join(',')+')';}
  function render(){
    var h=+$('hue').value,s=+$('sat').value,b=+$('bri').value,c=rgb(h,s),k=b/100;
    var lit=c.map(function(v){return Math.round(v*(.25+.75*k));});
    $('lamp').style.background=b?css(lit):'#2a2f2c';
    $('lamp').style.boxShadow=b?'0 0 '+(10+50*k)+'px '+css(c):'none';
    $('pct').value=b?b+'%':'Off';
    $('sat').style.background='linear-gradient(90deg,#fff,'+css(rgb(h,100))+')';
    $('bri').style.background='linear-gradient(90deg,#000,'+css(c)+')';
  }
  /* Only one request in flight; while busy, the newest value is sent next. */
  var busy=false,pending=false;
  async function push(){
    if(busy){pending=true;return;}busy=true;
    do{
      pending=false;
      var controller=new AbortController(),timeout=setTimeout(function(){controller.abort();},8000);
      try{
        var r=await fetch('/set?color='+hex(rgb(+$('hue').value,+$('sat').value))+'&brightness='+$('bri').value,{signal:controller.signal});
        if(!r.ok)throw new Error(await r.text());$('res').hidden=true;
      }catch(e){$('res').textContent='Could not reach the lamp. '+(e.name==='AbortError'?'Device timed out.':e.message);$('res').hidden=false;}
      finally{clearTimeout(timeout);}
    }while(pending);
    busy=false;
  }
  ['hue','sat','bri'].forEach(function(id){$(id).addEventListener('input',function(){render();push();});});
  /* State from the device: "RRGGBB,brightness". Hue and saturation come from the colour. */
  function lamp(state){
    var p=state.split(','),n=parseInt(p[0],16),c=[n>>16&255,n>>8&255,n&255];
    var max=Math.max.apply(0,c),min=Math.min.apply(0,c),d=max-min,h=0;
    if(d){h=max===c[0]?((c[1]-c[2])/d+6)%6:max===c[1]?(c[2]-c[0])/d+2:(c[0]-c[1])/d+4;}
    $('hue').value=Math.round(h*60)%360;$('sat').value=max?Math.round(100*d/max):0;$('bri').value=+p[1];
    render();
  }
</script>
)html";

static const char SETTINGS_HTML[] PROGMEM = R"html(
  <section class="card">
  <h3>System</h3>
  <form action="/hostname" method="post">
    <label for="hostname">Hostname</label>
    <input id="hostname" name="hostname" maxlength="31" pattern="[A-Za-z0-9]([A-Za-z0-9-]{0,29}[A-Za-z0-9])?" autocapitalize="none" spellcheck="false" aria-describedby="hostname-help">
    <p id="hostname-help">Use letters, numbers and hyphens, without .local. Leave blank to restore the default. Saving restarts the device.</p>
    <button class="primary" type="submit">Save hostname and restart</button>
  </form><hr>
  <div class="row">
    <button type="button" onclick="location.href='/selectap';">Configure WiFi</button>
    <button type="button" onclick="get('/update','sres')">Check for update</button>
  </div>
  <p id='sres'></p>
<script>
  function $(id){ return document.getElementById(id); }
  $('hostname').value=hostCur;
  function get(url,out){
    return fetch(url).then(r=>r.text().then(t=>{ $(out).innerText=t; return r.ok; })).catch(()=>{$(out).innerText='Connection failed. Please try again.';return false;});
  }
</script>
)html";

static const char APLIST_HTML[] PROGMEM = R"html(
  <style>#vm{height:50vh;overflow-y:auto;margin-bottom:1em;}</style>
  <h1>WiFi setup</h1><p>Choose your network to connect the lamp.</p><h3 id='ttl'>Available networks</h3>
  <div id='vm'>Please wait...</div>
  <form method='get' action='wifisave'>
    <button type='button' onclick='refresh();'>Rescan</button><br/><br/>
    <label for='s'>Network name (blank for access point mode)</label><input id='s' name='s' maxlength='32' placeholder='SSID (Leave blank for AP mode)'><br>
    <label for='p'>WiFi password</label><input id='p' name='p' type='password' maxlength='63' autocomplete='new-password' placeholder='Password'><br>
    <br><button class='primary' type='submit'>Save and connect</button>
  </form>
<script>
  function c(l){
    document.getElementById('s').value=l.innerText||l.textContent;
    document.getElementById('p').focus();
  }
  function refresh(){
    var vm=document.getElementById('vm');
    vm.innerHTML='Please wait...';
    fetch('/aplist').then(r=>r.text()).then(t=>{
      vm.innerHTML='';
      if(!t.length){ document.getElementById('ttl').innerHTML='No networks found.'; return; }
      t.split('|').forEach(function(s,i){
        vm.innerHTML+='<span>'+(i+1)+": </span><a href='#p' onclick='c(this)'>"+s+'</a><br>';
      });
    });
  }
  refresh();
</script>
)html";

// Small fragments used around dynamic settings and status values.
static const char UI_TITLE_OPEN[] PROGMEM = "<title>";
static const char UI_TITLE_CLOSE[] PROGMEM = "</title>";
static const char UI_LAMP_SET[] PROGMEM = "<script>lamp('";
static const char UI_STRING_CALL_END[] PROGMEM = "');</script>";
static const char UI_HOST_STATE[] PROGMEM = "<script>var hostCur='";
static const char UI_STRING_SCRIPT_END[] PROGMEM = "';</script>";
static const char UI_PARAGRAPH_OPEN[] PROGMEM = "<p>";
static const char UI_BREAK[] PROGMEM = "<br>";
static const char UI_SYSTEM_END[] PROGMEM = "</p></section>";
static const char UI_PARAGRAPH_CLOSE[] PROGMEM = "</p>";
static const char UI_WIFI_INVALID[] PROGMEM = "<p>Sorry, this module can only remember SSID up to 32 and a PASSWORD up to 63 bytes long.</p>";
static const char UI_WIFI_CONNECTING[] PROGMEM = "Saving settings and connecting to SSID: ";
static const char UI_WIFI_LINK[] PROGMEM = "<br>Then open <a href='http://";
static const char UI_LOCAL_LINK_TEXT[] PROGMEM = ".local'>http://";
static const char UI_WIFI_LINK_END[] PROGMEM = ".local</a> or the IP your router assigned.";
static const char UI_WIFI_AP[] PROGMEM = "Saving settings and switching to AP mode only.";
static const char UI_CONNECTION_TITLE[] PROGMEM = "<h1>Connection Status</h1><p>";
static const char UI_HOST_INVALID[] PROGMEM = "<h1>Invalid hostname</h1><p>Use 1–31 letters, numbers or hyphens. Start and end with a letter or number.</p><a href='/settings'>Back to Settings</a>";
static const char UI_HOST_SAVED[] PROGMEM = "<h1>Hostname saved</h1><p>Restarting. Reconnect at <a href='http://";
static const char UI_HOST_LINK_END[] PROGMEM = ".local</a>, or use the device IP address.</p>";

// SVG URLs include a content hash; update the hash and page references after edits.

static const char ICON_HOME[] PROGMEM = R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 512 512"><!--! Font Awesome Free 7.3.0 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license/free (Icons: CC BY 4.0, Fonts: SIL OFL 1.1, Code: MIT License) Copyright 2026 Fonticons, Inc. --><path fill="#152923" d="M277.8 8.6c-12.3-11.4-31.3-11.4-43.5 0l-224 208c-9.6 9-12.8 22.9-8 35.1S18.8 272 32 272l16 0 0 176c0 35.3 28.7 64 64 64l288 0c35.3 0 64-28.7 64-64l0-176 16 0c13.2 0 25-8.1 29.8-20.3s1.6-26.2-8-35.1l-224-208zM240 320l32 0c26.5 0 48 21.5 48 48l0 96-128 0 0-96c0-26.5 21.5-48 48-48z"/></svg>)svg";

static const char ICON_SETTINGS[] PROGMEM = R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="-16 -16 544 544"><!--! Font Awesome Free 7.3.0 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license/free (Icons: CC BY 4.0, Fonts: SIL OFL 1.1, Code: MIT License) Copyright 2026 Fonticons, Inc. --><path fill="#152923" d="M195.1 9.5C198.1-5.3 211.2-16 226.4-16l59.8 0c15.2 0 28.3 10.7 31.3 25.5L332 79.5c14.1 6 27.3 13.7 39.3 22.8l67.8-22.5c14.4-4.8 30.2 1.2 37.8 14.4l29.9 51.8c7.6 13.2 4.9 29.8-6.5 39.9L447 233.3c.9 7.4 1.3 15 1.3 22.7s-.5 15.3-1.3 22.7l53.4 47.5c11.4 10.1 14 26.8 6.5 39.9l-29.9 51.8c-7.6 13.1-23.4 19.2-37.8 14.4l-67.8-22.5c-12.1 9.1-25.3 16.7-39.3 22.8l-14.4 69.9c-3.1 14.9-16.2 25.5-31.3 25.5l-59.8 0c-15.2 0-28.3-10.7-31.3-25.5l-14.4-69.9c-14.1-6-27.2-13.7-39.3-22.8L73.5 432.3c-14.4 4.8-30.2-1.2-37.8-14.4L5.8 366.1c-7.6-13.2-4.9-29.8 6.5-39.9l53.4-47.5c-.9-7.4-1.3-15-1.3-22.7s.5-15.3 1.3-22.7L12.3 185.8c-11.4-10.1-14-26.8-6.5-39.9L35.7 94.1c7.6-13.2 23.4-19.2 37.8-14.4l67.8 22.5c12.1-9.1 25.3-16.7 39.3-22.8L195.1 9.5zM256.3 336a80 80 0 1 0 -.6-160 80 80 0 1 0 .6 160z"/></svg>)svg";

static const char ICON_FAVICON[] PROGMEM = R"svg(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 32 32"><rect width="32" height="32" rx="8" fill="#152923"/><circle cx="11" cy="13" r="5" fill="#ff5a5a"/><circle cx="21" cy="13" r="5" fill="#5ad36b"/><circle cx="16" cy="21" r="5" fill="#5a8cff"/></svg>)svg";

struct IconAsset { const char* url; PGM_P content; };
static const IconAsset ICON_ASSETS[] = {
  {"/icons/home-e4ff8833c2f2.svg", ICON_HOME},
  {"/icons/settings-08c94bb532ed.svg", ICON_SETTINGS},
  {"/icons/favicon-8c5115a27260.svg", ICON_FAVICON},
};
