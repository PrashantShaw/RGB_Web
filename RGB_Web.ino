/* RGB web server with ESP8266
* analogWrite with values received from web page
*
* WiFi router that creates a fixed domain: http://rgb
* web page returns POST request with 3 RGB parameters http://192.168.1.5/
* web page inspired by https://github.com/dimsumlabs/nodemcu-httpd
*/

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const char *ssid = "Prashant RGB";
const char *password = "rgb007";
const byte DNS_PORT = 53;
const int redLED = 0;   //D3
const int greenLED = 4; //D2
const int blueLED = 5;  //D1
IPAddress apIP(192, 168, 1, 5);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String webpage = ""
"<!DOCTYPE html><html><head><title>RGB control</title><meta name='mobile-web-app-capable' content='yes' />"
"<meta name='viewport' content='width=device-width' /></head><body style='margin: 0px; padding: 0px;'>"
"<canvas id='colorspace'></canvas>"
"<button id='calmness' style='background-color: rgb(69, 179, 224); padding: 1rem; margin: 1rem; border-radius: 5px;'>Calmness</button>"
"<button id='warmth' style='background-color: rgb(225, 120, 45); padding: 1rem; margin: 1rem; border-radius: 5px;'>Warmth</button>"
"<button id='energetic' style='background-color: rgb(237, 44, 35); padding: 1rem; margin: 1rem; border-radius: 5px;'>Energetic</button>"
"<button id='freshness' style='background-color: rgb(184, 216, 8); padding: 1rem; margin: 1rem; border-radius: 5px;'>Freshness</button>"
"<button id='happiness' style='background-color: rgb(225, 225, 0); padding: 1rem; margin: 1rem; border-radius: 5px;'>Happiness</button>"
"<button id='love' style='background-color: rgb(235, 80, 94); padding: 1rem; margin: 1rem; border-radius: 5px;'>Love</button>"
"<button id='alert' style='background-color: rgb(0, 255, 254); padding: 1rem; margin: 1rem; border-radius: 5px;'>Stay Alert</button>"
"<button id='exciting' style='background-color: rgb(255, 0, 254); padding: 1rem; margin: 1rem; border-radius: 5px;'>Exciting</button></body>"
"<script type='text/javascript'>"
"(function () {"
" var canvas = document.getElementById('colorspace');"
" var energetic = document.getElementById('energetic');"
" var happiness = document.getElementById('happiness');"
" var calmness = document.getElementById('calmness');"
" var warmth = document.getElementById('warmth');"
" var freshness = document.getElementById('freshness');"
" var love = document.getElementById('love');"
" var alert = document.getElementById('alert');"
" var exciting = document.getElementById('exciting');"
" var ctx = canvas.getContext('2d');"
" function drawCanvas() {"
" var colours = ctx.createLinearGradient(0, 0, window.innerWidth, 0);"
" for(var i=0; i <= 360; i+=10) {"
" colours.addColorStop(i/360, 'hsl(' + i + ', 100%, 50%)');"
" }"
" ctx.fillStyle = colours;"
" ctx.fillRect(0, 0, window.innerWidth, window.innerHeight);"
" var luminance = ctx.createLinearGradient(0, 0, 0, ctx.canvas.height);"
" luminance.addColorStop(0, '#ffffff');"
" luminance.addColorStop(0.05, '#ffffff');"
" luminance.addColorStop(0.5, 'rgba(0,0,0,0)');"
" luminance.addColorStop(0.95, '#000000');"
" luminance.addColorStop(1, '#000000');"
" ctx.fillStyle = luminance;"
" ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);"
" }"
" var eventLocked = false;"
" function handleEvent(clientX, clientY) {"
" if(eventLocked) {"
" return;"
" }"
" function colourCorrect(v) {"
" return Math.round(1023-(v*v)/64);"
" }"
" var data = ctx.getImageData(clientX, clientY, 1, 1).data;"
" var params = ["
" 'r=' + colourCorrect(data[0]),"
" 'g=' + colourCorrect(data[1]),"
" 'b=' + colourCorrect(data[2])"
" ].join('&');"
" var req = new XMLHttpRequest();"
" req.open('POST', '?' + params, true);"
" req.send();"
" eventLocked = true;"
" req.onreadystatechange = function() {"
" if(req.readyState == 4) {"
" eventLocked = false;"
" }"
" }"
" }"
" canvas.addEventListener('click', function(event) {"
" handleEvent(event.clientX, event.clientY, true);"
" }, false);"
" canvas.addEventListener('touchmove', function(event){"
" handleEvent(event.touches[0].clientX, event.touches[0].clientY);"
"}, false);"
" energetic.addEventListener('click', () => {"
" if (eventLocked) {return;}"
" var params = ['r=' + 7,'g=' + 1023,'b=' + 1023].join('&');"
" var req = new XMLHttpRequest();"
" req.open('POST', '?' + params, true);req.send();eventLocked = true;"
" req.onreadystatechange = function () {"
" if (req.readyState == 4) {eventLocked = false;}}}, false);"
" love.addEventListener('click', () => {"
" if (eventLocked) {return;}"
" var params = ['r=' + 15,'g=' + 1023,'b=' + 1005].join('&');"
" var req = new XMLHttpRequest();"
" req.open('POST', '?' + params, true);req.send();eventLocked = true;"
" req.onreadystatechange = function () {"
" if (req.readyState == 4) {eventLocked = false;}}}, false);"
" calmness.addEventListener('click', () => {"
" if (eventLocked) {return;}"
" var params = ['r=' + 1023,'g=' + 7,'b=' + 951].join('&');"
" var req = new XMLHttpRequest();"
" req.open('POST', '?' + params, true);req.send();eventLocked = true;"
" req.onreadystatechange = function () {"
" if (req.readyState == 4) {eventLocked = false;}}}, false);"
" warmth.addEventListener('click', () => {"
" if (eventLocked) {return;}"
" var params = ['r=' + 7,'g=' + 1012,'b=' + 1023].join('&');"
" var req = new XMLHttpRequest();"
" req.open('POST', '?' + params, true);req.send();eventLocked = true;"
" req.onreadystatechange = function () {"
" if (req.readyState == 4) {eventLocked = false;}}}, false);"
" freshness.addEventListener('click', () => {"
" if (eventLocked) {return;}"
" var params = ['r=' + 494,'g=' + 294,'b=' + 1022].join('&');"
" var req = new XMLHttpRequest();"
" req.open('POST', '?' + params, true);req.send();eventLocked = true;"
" req.onreadystatechange = function () {"
" if (req.readyState == 4) {eventLocked = false;}}}, false);"
" happiness.addEventListener('click', () => {"
" if (eventLocked) {return;}"
" var params = ['r=' + 7,'g=' + 984,'b=' + 1023].join('&');"
" var req = new XMLHttpRequest();"
" req.open('POST', '?' + params, true);req.send();eventLocked = true;"
" req.onreadystatechange = function () {"
" if (req.readyState == 4) {eventLocked = false;}}}, false);"
" alert.addEventListener('click', () => {"
" if (eventLocked) {return;}"
" var params = ['r=' + 1023,'g=' + 7,'b=' + 15].join('&');"
" var req = new XMLHttpRequest();"
" req.open('POST', '?' + params, true);req.send();eventLocked = true;"
" req.onreadystatechange = function () {"
" if (req.readyState == 4) {eventLocked = false;}}}, false);"
" exciting.addEventListener('click', () => {"
" if (eventLocked) {return;}"
" var params = ['r=' + 7,'g=' + 1023,'b=' + 15].join('&');"
" var req = new XMLHttpRequest();"
" req.open('POST', '?' + params, true);req.send();eventLocked = true;"
" req.onreadystatechange = function () {"
" if (req.readyState == 4) {eventLocked = false;}}}, false);"
" function resizeCanvas() {"
" canvas.width = window.innerWidth;"
" canvas.height = window.innerHeight;"
" drawCanvas();"
" }"
" window.addEventListener('resize', resizeCanvas, false);"
" resizeCanvas();"
" drawCanvas();"
" document.ontouchmove = function(e) {e.preventDefault()};"
" })();"
"</script></html>";

//////////////////////////////////////////////////////////////////////////////////////////////////

void handleRoot() {
// Serial.println("handle root..");
String red = webServer.arg(0); // read RGB arguments
String green = webServer.arg(1);
String blue = webServer.arg(2);

if((red != "") && (green != "") && (blue != ""))
{
  analogWrite(redLED, 1023 - red.toInt());
  analogWrite(greenLED, 1023 - green.toInt());
  analogWrite(blueLED, 1023 - blue.toInt());
}


Serial.print("Red: ");
Serial.println(red.toInt()); 
Serial.print("Green: ");
Serial.println(green.toInt()); 
Serial.print("Blue: ");
Serial.println(blue.toInt()); 
Serial.println();

webServer.send(200, "text/html", webpage);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {

pinMode(redLED, OUTPUT);
pinMode(greenLED, OUTPUT);
pinMode(blueLED, OUTPUT);

analogWrite(redLED, 0);
analogWrite(greenLED, 0);
analogWrite(blueLED, 0);

delay(1000);
Serial.begin(115200);
Serial.println();

WiFi.mode(WIFI_AP);
WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
WiFi.softAP(ssid);

// if DNSServer is started with "*" for domain name, it will reply with provided IP to all DNS request
dnsServer.start(DNS_PORT, "rgb", apIP);

webServer.on("/", handleRoot);

webServer.begin();

testRGB();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {

dnsServer.processNextRequest();
webServer.handleClient();

}
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

void testRGB() { // fade in and out of Red, Green, Blue

analogWrite(redLED, 0); // R off
analogWrite(greenLED, 0); // G off
analogWrite(blueLED, 0); // B off

fade(redLED); // R
fade(greenLED); // G
fade(blueLED); // B
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void fade(int pin) {

for (int u = 0; u < 1024; u++) {
analogWrite(pin, u);
delay(1);
}
for (int u = 0; u < 1024; u++) {
analogWrite(pin, 1023 - u);
delay(1);
}
}
