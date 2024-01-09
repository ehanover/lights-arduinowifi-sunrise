let contents = [];
let links = [];
let selectedIndex;

async function fetchMainPage() {
  // Load the real HTML from elsewhere so it's faster
  try {
    let response = await fetch('https://raw.githubusercontent.com/ehanover/lights-arduinowifi-sunrise/main/main.html');
    document.body.innerHTML = await response.text();
  } catch (err) {
    console.log('Fetch error:' + err);
  }
}

function inputToRGB(e) {
  const v = document.getElementById(e).value;
  return [parseInt(v.substr(1,2), 16), parseInt(v.substr(3,2), 16), parseInt(v.substr(5,2), 16)];
}

function hueInputToByte(e) {
  const h = document.getElementById(e).value;
  return Math.floor(255 * (h / 330.0));
}

function showTab(s) {
  contents[selectedIndex].style.display = 'none';
  contents[s].style.display = 'flex';

  links[selectedIndex].classList.remove('tab-link-selected');
  links[s].classList.add('tab-link-selected');

  selectedIndex = s;
}

function submit() {
  if(selectedIndex == 3) {
    const secondsPerDay = 24 * 60 * 60;
    const dateNow = new Date();
    const dateTarget = new Date(dateNow.toISOString().split('T')[0] + 'T' + document.getElementById('sunrise-time').value);
    let secondsTillAlarm = Math.floor((dateTarget.getTime() - dateNow.getTime()) / 1000);
    if(secondsTillAlarm < 0) {
      secondsTillAlarm += secondsPerDay;
    } else {
      // Since dateTarget uses UTC time in calculation, it could be a into the next day. Correct for that: 
      secondsTillAlarm %= secondsPerDay;
    }

    console.log("seconds till:", secondsTillAlarm);
    postData([2, 
      (secondsTillAlarm >>> 16) & 0xff, (secondsTillAlarm >>> 8) & 0xff, (secondsTillAlarm >>> 0) & 0xff]
      .concat(inputToRGB('sunrise-color'))
      .concat(document.getElementById('sunrise-duration').value));
  } else if(selectedIndex == 1) {
    postData([3, 
      hueInputToByte('swirl-hue-1'), hueInputToByte('swirl-hue-2'), hueInputToByte('swirl-hue-3'),
      document.getElementById('swirl-speed').value, document.getElementById('swirl-brightness').value]);
  } else if(selectedIndex == 0) {
    postData([4, 
      1 ? document.getElementById('rave-value-rainbow').checked : 0, 
      document.getElementById('rave-speed').value, document.getElementById('rave-brightness').value]);
  } else if(selectedIndex == 2) {
    postData([1]
      .concat(inputToRGB('solid-color')));
  }
}

function postData(d) {
  const data = new Uint8Array(d);
  console.log("posting data:", data);
  const url = window.location.href;
  // const url = 'http://192.168.4.44';
  fetch(url, {method: 'POST', body: data});
}

window.onload = async () => {
  await fetchMainPage();

  contents = document.getElementsByClassName('tab-content');
  links = document.getElementsByClassName('tab-link');

  const swirlHue1 = document.getElementById('swirl-hue-1');
  swirlHue1.addEventListener('input', () => document.getElementById('swirl-hue-1-parent').style.backgroundColor = `hsl(${swirlHue1.value}, 100%, 50%)`);
  const swirlHue2 = document.getElementById('swirl-hue-2');
  swirlHue2.addEventListener('input', () => document.getElementById('swirl-hue-2-parent').style.backgroundColor = `hsl(${swirlHue2.value}, 100%, 50%)`);
  const swirlHue3 = document.getElementById('swirl-hue-3');
  swirlHue3.addEventListener('input', () => document.getElementById('swirl-hue-3-parent').style.backgroundColor = `hsl(${swirlHue3.value}, 100%, 50%)`);

  selectedIndex = 0;
  showTab(1);
}