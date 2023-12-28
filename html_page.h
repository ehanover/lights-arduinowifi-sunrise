#pragma once

// https://www.htmlminifiers.com/html-minifier
// const char html_page[] = "<!DOCTYPE html> <html> <head> <title>Sunrise lights</title> <style> * { margin: 0; padding: 0; /* font-family: 'Brush Script MT', serif; */ /* font-family: ui-monospace; */ font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif; font-size: large; } html, body { overflow-x: hidden; } body { position: relative; padding: 10px; } #main { background-color: white; margin: auto; max-width: 650px; } #submit { margin: 5px; } #tabs { display: flex; justify-content: space-between; } .tab-link { background-color: white; padding: 10px 0; flex-grow: 1; text-decoration: underline; text-align: center; } .tab-link-selected { background-color: gray; border-top-left-radius: 10px; border-top-right-radius: 10px; } .tab-content { display: none; background-color: gray; padding: 10px; min-height: 150px; flex-direction: column; align-items: center; } .content-spacing { margin: 20px 0; } </style> </head> <body> <div id='main'> <b>Lights</b> <br/> <br/> <div id='tabs'> <a class='tab-link' onclick='showTab(0)'>Sunrise</a> <a class='tab-link' onclick='showTab(1)'>Fade</a> <a class='tab-link' onclick='showTab(2)'>Rave</a> <a class='tab-link' onclick='showTab(3)'>Solid</a> </div> <div class='tab-content'> <input style='margin-top: 10px' id='solid-color' type='color' value='#ff00ff'> <span class='content-spacing'> <label for='solid-brightness'>Brightness: </label> <br/> <input id='solid-brightness' type='range' min='0' max='255' step='1' value='128'/> </span> </div> <div style='text-align: center;'> <button id='submit' onclick='submit()'>Submit</button> <br/> <button id='off' onclick='off()'>Off</button> </div> </div> <script type='text/javascript'> let contents = []; let links = []; let selectedIndex; function showTab(s) { // console.log('switching to', s); contents[selectedIndex].style.display = 'none'; contents[s].style.display = 'flex'; links[selectedIndex].classList.remove('tab-link-selected'); links[s].classList.add('tab-link-selected'); selectedIndex = s; } function off() { postData([0]); } function submit() { if(selectedIndex == 0) { const secondsPerDay = 24 * 60 * 60; const dateNow = new Date(); const dateTarget = new Date(dateNow.toISOString().split('T')[0] + 'T' + document.getElementById('sunrise-time').value); let secondsTillAlarm = Math.floor((dateTarget.getTime() - dateNow.getTime()) / 1000); if(secondsTillAlarm < 0) { secondsTillAlarm += secondsPerDay; } else { // Since dateTarget uses UTC time in calculation, it could be a day into the future. Correct for that: secondsTillAlarm %= secondsPerDay; } postData([1, 65, 66, 1, 2]); } else if(selectedIndex == 1) { } else if(selectedIndex == 2) { } } function postData(d) { const data = new Uint8Array(d); // const url = window.location.href; const url = 'http://192.168.1.186'; fetch(url, {method: 'POST', body: data}); } window.onload = () => { contents = document.getElementsByClassName('tab-content'); links = document.getElementsByClassName('tab-link'); selectedIndex = 0; showTab(1); } </script> </body> </html>";

const char html_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Sunrise lights</title>

    <style>
      * {
        margin: 0;
        padding: 0;

        /* font-family: 'Brush Script MT', serif; */
        /* font-family: ui-monospace; */
        font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
        font-size: large;
      }
      html, body {
        overflow-x: hidden;
      }
      body {
        position: relative;
        padding: 10px;
      }
      #main {
        background-color: white;
        margin: auto;
        max-width: 650px;
      }
      #submit {
        margin: 5px;
      }
      #tabs {
        display: flex;
        justify-content: space-between;
      }
      .tab-link {
        background-color: white;
        padding: 10px 0;
        flex-grow: 1;
        text-decoration: underline;
        text-align: center;
      }
      .tab-link-selected {
        background-color: gray;
        border-top-left-radius: 10px;
        border-top-right-radius: 10px;
      }
      .tab-content {
        display: none;
        background-color: gray;
        padding: 10px;
        min-height: 150px;

        flex-direction: column;
        align-items: center;
      }
      .content-spacing {
        margin: 20px 0;
      }
    </style>
  </head>

  <body>
    <div id='main'>
      <b>Lights</b>
      <br/>
      <br/>
      <div id='tabs'>
        <a class='tab-link' onclick='showTab(0)'>Sunrise</a>
        <a class='tab-link' onclick='showTab(1)'>Fade</a>
        <a class='tab-link' onclick='showTab(2)'>Rave</a>
        <a class='tab-link' onclick='showTab(3)'>Solid</a>
      </div>

      <!-- Sunrise -->
      <div class='tab-content' style='align-items: start;'> 
        <span class='content-spacing'>
          <label for='sunrise-time'>By time</label>
          <input id='sunrise-time' type='time' value='09:00'/>
        </span>

        <span class='content-spacing'>
          <label for='sunrise-color'>get to color</label>
          <input id='sunrise-color' type='color' value='#ff00ff'/>
        </span>

        <span class='content-spacing'>
          <label for='sunrise-brightness'>get to brightness</label>
          <input id='sunrise-brightness' type='number' size='5' value='100'/>
        </span>

        <span class='content-spacing'>
          <label for='sunrise-duration'>over</label>
          <input id='sunrise-duration' type='number' size='5' value='30'/>
          minutes
        </span>
      </div>

      <!-- Fade -->
      <div class='tab-content'>
        <input class='content-spacing' id='fade-color-1' type='color' value='#ff0000'>
        <input class='content-spacing' id='fade-color-2' type='color' value='#00ff00'>
        <input class='content-spacing' id='fade-color-3' type='color' value='#0000ff'>

        <span class='content-spacing'>
          <label for='fade-speed'>Speed: </label>
          <br/>
          <input id='fade-speed' type='range' min='0' max='255' step='1' value='20'/>
        </span>
        <span class='content-spacing'>
          <label for='fade-brightness'>Brightness: </label>
          <br/>
          <input id='fade-brightness' type='range' min='0' max='255' step='1' value='128'/>
        </span>
      </div>

      <!-- Rave -->
      <div class='tab-content'>
        <span class='content-spacing'>
          <label style='margin-top: 10px' for='rave-speed'>Speed: </label>
          <br/>
          <input id='rave-speed' type='range' min='0' max='255' step='1' value='50'/>
        </span>
        <span class='content-spacing'>
          <label for='rave-brightness'>Brightness: </label>
          <br/>
          <input id='rave-brightness' type='range' min='0' max='255' step='1' value='128'/>
        </span>
      </div>

      <!-- Solid -->
      <div class='tab-content'>
        <input style='margin-top: 10px' id='solid-color' type='color' value='#ff00ff'>

        <span class='content-spacing'>
          <label for='solid-brightness'>Brightness: </label>
          <br/>
          <input id='solid-brightness' type='range' min='0' max='255' step='1' value='128'/>
        </span>
      </div>

      <div style='text-align: center;'>
        <button id='submit' onclick='submit()'>Submit</button>
        <br/>
        <button id='off' onclick='off()'>Off</button>
      </div>

    </div>


    <script type='text/javascript'>
      let contents = [];
      let links = [];
      let selectedIndex;

      function showTab(s) {
        // console.log('switching to', s);

        contents[selectedIndex].style.display = 'none';
        contents[s].style.display = 'flex';

        links[selectedIndex].classList.remove('tab-link-selected');
        links[s].classList.add('tab-link-selected');

        selectedIndex = s;
      }

      function off() {
        postData([0]);
      }

      function submit() {
        if(selectedIndex == 0) {
          const secondsPerDay = 24 * 60 * 60;
          const dateNow = new Date();
          const dateTarget = new Date(dateNow.toISOString().split('T')[0] + 'T' + document.getElementById('sunrise-time').value);
          let secondsTillAlarm = Math.floor((dateTarget.getTime() - dateNow.getTime()) / 1000);
          if(secondsTillAlarm < 0) {
            secondsTillAlarm += secondsPerDay;
          } else {
            // Since dateTarget uses UTC time in calculation, it could be a day into the future. Correct for that: 
            secondsTillAlarm %= secondsPerDay;
          }

          postData([1, 65, 66, 1, 2]);
        } else if(selectedIndex == 1) {

        } else if(selectedIndex == 2) {

        }
      }

      function postData(d) {
        const data = new Uint8Array(d);
        // const url = window.location.href;
        const url = 'http://192.168.1.186';
        fetch(url, {method: 'POST', body: data});
      }

      window.onload = () => {
        contents = document.getElementsByClassName('tab-content');
        links = document.getElementsByClassName('tab-link');

        selectedIndex = 0;
        showTab(1);
      }
    </script>
  </body>
</html>
)=====";