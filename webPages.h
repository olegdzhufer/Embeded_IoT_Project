#ifndef webPages_h
#define webPages_h

#include <Arduino.h>
#include "settings.h"


const String legendColour = "#5d2212";  // Only use HTML colour names
const String titleColour = "#5a1500";
const String backgrndColour = "#ac8a7f";
// const String data1Colour    = "blue";
// const String data2Colour    = "orange";

String WiFiSignal() {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
  float Signal = WiFi.RSSI();
  Signal = 90 / 40.0 * Signal + 212.5;  // From Signal = 100% @ -50dBm and Signal = 10% @ -90dBm and y = mx + c
  if (Signal > 100)
    Signal = 100;
  return " " + String(Signal, 0) + "%";
}

// .addressDelivery .btn-grey {
//     background: #ac897c;
//     border: none;
//     border-radius: 4px;
//     color: #fff;
//     cursor: pointer;
//     display: inline-block;
//     margin: 0 auto;
//     width: auto;
//     padding: 8px 20px;
//     vertical-align: top;
//     font-size: 20px;
//     line-height: 1.2;
// }
// .addressDelivery .btn-grey:hover,
// button:hover {
//     background: #5a1500;
// }

void append_HTML_header(bool refreshMode) {
  webpage = "<!DOCTYPE html><html lang='en'>";
  webpage += "<head>";
  webpage += "<title>" + sitetitle + "</title>";
  webpage += "<meta charset='UTF-8'>";
  if (refreshMode) webpage += "<meta http-equiv='refresh' content='5'>";  // 5-secs refresh time, test needed to prevent auto updates repeating some commands
  webpage += "<script src=\"https://code.jquery.com/jquery-3.2.1.min.js\"></script>";
  webpage += "<style>";
  webpage += "body             {width:68em;margin-left:auto;margin-right:auto;font-family:Open Sans,sans-serif;font-size:16px;color: #5a1500;background-color:#e1e1ff;text-align:center;}";
  webpage += ".centre          {margin-left:auto;margin-right:auto;}";
  webpage += "h2               {margin-top:0.3em;margin-bottom:0.3em;font-size:1.4em;}";
  webpage += "h3               {margin-top:0.3em;margin-bottom:0.3em;font-size:1.2em;}";
  webpage += "h4               {margin-top:0.3em;margin-bottom:0.3em;font-size:0.8em;}";
  webpage += ".on              {color: red;}";
  webpage += ".off             {color: limegreen;}";
  webpage += ".topnav          {overflow: hidden;background-color:#eee4df;}";
  webpage += ".topnav a        {float:left;color:#5d2212;text-align:center;padding:1em 1.14em;text-decoration:none;font-size:1.3em;}";
  // webpage += ".topnav a:hover  {background-color:deepskyblue;color:white;}";
  webpage += ".topnav a:hover  {background: #ac8a7f;color: #fff;}";
  webpage += ".topnav a.active {background: #5a1500;color: #fff;}";
  webpage += "table tr, td     {padding:0.2em 0.5em 0.2em 0.5em;font-size:1.0em;font-family:Arial,Helvetica,sans-serif;}";
  webpage += "col:first-child  {background:#eee4df}col:nth-child(2){background:#CCC}col:nth-child(8){background:#CCC}";
  webpage += "tr:first-child   {background:#eee4df}";
  webpage += ".large           {font-size:1.8em;padding:0;margin:0}";
  webpage += ".medium          {font-size:1.4em;padding:0;margin:0}";
  webpage += ".ps              {font-size:0.7em;padding:0;margin:0}";
  webpage += "#outer           {width:100%;display:flex;justify-content:center;}";
  webpage += "footer           {padding:0.08em;background-color:#ac897c;font-size:1.1em;}";
  webpage += ".numberCircle    {border-radius:50%;width:2.7em;height:2.7em;border:0.11em solid #5d2212;padding:0.2em;color:#5d2212;text-align:center;font-size:3em;";
  webpage += "                  display:inline-flex;justify-content:center;align-items:center;}";
  webpage += ".wifi            {padding:3px;position:relative;top:1em;left:0.36em;}";
  webpage += ".wifi, .wifi:before {display:inline-block;border:9px double transparent;border-top-color:currentColor;border-radius:50%;}";
  webpage += ".wifi:before     {content:'';width:0;height:0;}";
  webpage += "</style></head>";
  webpage += "<body>";
  webpage += "<div class='topnav'>";
  webpage += "<a href='/'>Status</a>";
  webpage += "<a href='graphs'>Graph</a>";
  webpage += "<a href='timer'>Schedule</a>";
  webpage += "<a href='setup'>Setup</a>";
  webpage += "<a href='help'>Help</a>";
  webpage += "<a href='changemode'>Change Mode</a>";
  webpage += "<a href='btns?'></a>";
  webpage += "<a href='reload?'></a>";
  webpage += "<a href=''></a>";
  webpage += "<a href='WiFi'></a>";
  webpage += "<div class='wifi'/></div><span>" + WiFiSignal() + "</span>";
  webpage += "</div><br>";
}

void append_HTML_footer() {
  webpage += "<footer>";
  webpage += "<p class='medium'>Dragee IoT</p>";
  webpage += "</footer>";
  webpage += "</body></html>";
}

void homepage() {
  #ifdef TEMP_S
    Temperature = readSensorComplete();
  #else
    Temperature = 0;
  #endif

  append_HTML_header(Refresh);
  webpage += "<h2>Temperature and Relay status</h2><br>";
  webpage += "<div class='numberCircle'><span class=" + String((RelayState == "ON" ? "'on'>" : "'off'>")) + String(Temperature, 1) + "&deg;</span></div><br><br><br>";
  webpage += "<table class='centre'>";
  webpage += "<tr>";
  webpage += "<td>Temperature</td>";
  webpage += "<td>Target Temperature</td>";
  webpage += "<td>Thermostat Status</td>";
  webpage += "<td>Schedule Status</td>";
  if (ManualOverride) {
    webpage += "<td>ManualOverride</td>";
  }
  webpage += "</tr>";
  webpage += "<tr>";
  webpage += "<td class='large'>" + String(Temperature, 1) + "&deg;</td>";
  webpage += "<td class='large'>" + String(TargetTemp, 1) + "&deg;</td>";
  webpage += "<td class='large'><span class=" + String((RelayState == "ON" ? "'on'>" : "'off'>")) + RelayState + "</span></td>";
  webpage += "<td class='large'><span class=" + String((TimerState == "ON" ? "'on'>" : "'off'>")) + TimerState + "</span></td>";
  if (ManualOverride) {
    webpage += "<td class='large'>" + String(ManualOverride ? "ON" : "OFF") + "</td>";
  }
  webpage += "</tr>";
  webpage += "</table>";
  webpage += "<br>";
  append_HTML_footer();
}


String PreLoadChartData(byte Channel, String Type) {
  byte r = 0;
  String Data = "";
  do {
    if (Type == "Temperature") {
      Data += "[" + String(r) + "," + String(sensordata[Channel][r].Temp, 1) + "," + String(TargetTemp, 1) + "],";
    }

    r++;
  } while (r < SensorReadings);
  Data += "]";
  return Data;
}

void AddGraph(byte Channel, String Type, String Title, String GraphType, String Units, String Colour, String Div) {
  String Data = PreLoadChartData(Channel, Title);
  webpage += "function draw" + Type + String(Channel) + "() {";
  if (Type == "GraphT") {
    webpage += " var data = google.visualization.arrayToDataTable(" + String("[['Hour', 'Rm T°', 'Tgt T°'],") + Data + ");";
  } else
    webpage += " var data = google.visualization.arrayToDataTable(" + String("[['Hour', 'RH %'],") + Data + ");";
  webpage += " var options = {";
  webpage += "  title: '" + Title + "',";
  webpage += "  titleFontSize: 14,";
  webpage += "  backgroundColor: '" + backgrndColour + "',";
  webpage += "  legendTextStyle: { color: '" + legendColour + "' },";
  webpage += "  titleTextStyle:  { color: '" + titleColour + "' },";
  webpage += "  hAxis: {color: '#FFF'},";
  webpage += "  vAxis: {color: '#FFF', title: '" + Units + "'},";
  webpage += "  curveType: 'function',";
  webpage += "  pointSize: 1,";
  webpage += "  lineWidth: 1,";
  webpage += "  width:  450,";
  webpage += "  height: 280,";
  webpage += "  colors:['" + Colour + (Type == "GraphT" ? "', 'orange" : "") + "'],";
  webpage += "  legend: { position: 'right' }";
  webpage += " };";
  webpage += " var chart = new google.visualization.LineChart(document.getElementById('" + Div + GraphType + String(Channel) + "'));";
  webpage += "  chart.draw(data, options);";
  webpage += " };";
}


void graphs() {
  append_HTML_header(Refresh);
  webpage += "<h2>Sensor Readings</h2>";
  webpage += "<script type='text/javascript' src='https://www.gstatic.com/charts/loader.js'></script>";
  webpage += "<script type='text/javascript'>";
  webpage += "google.charts.load('current', {'packages':['corechart']});";
  webpage += "google.charts.setOnLoadCallback(drawGraphT1);";  // Pre-load function names for Temperature graphs
  // webpage += "google.charts.setOnLoadCallback(drawGraphH1);"; // Pre-load function names for Humidity graphs
  AddGraph(1, "GraphT", "Temperature", "TS", "°C", "red", "chart_div");
  // AddGraph(1, "GraphH", "Humidity",    "HS", "%",  "blue", "chart_div");
  webpage += "</script>";
  webpage += "<div id='outer'>";
  webpage += "<table>";
  webpage += "<tr>";
  webpage += "  <td><div id='chart_divTS1' style='width:50%'></div></td>";
  webpage += "</tr>";
  webpage += "</table>";
  webpage += "<br>";
  webpage += "</div>";
  webpage += "<p>Heating status : <span class=" + String((RelayState == "ON" ? "'on'>" : "'off'>")) + RelayState + "</span></p>";
  append_HTML_footer();
}


void timerSet() {
  append_HTML_header(noRefresh);
  webpage += "<h2>Thermostat Schedule Setup</h2><br>";
  webpage += "<h3>Enter required temperatures and time, use Clock symbol for ease of time entry</h3><br>";
  webpage += "<FORM action='/handletimer'>";
  webpage += "<table class='centre'>";
  webpage += "<col><col><col><col><col><col><col><col>";
  webpage += "<tr><td>Control</td>";
  webpage += "<td>" + Timer[0].DoW + "</td>";
  for (byte dow = 1; dow < 6; dow++) {  // Heading line showing DoW
    webpage += "<td>" + Timer[dow].DoW + "</td>";
  }
  webpage += "<td>" + Timer[6].DoW + "</td>";
  webpage += "</tr>";
  for (byte p = 0; p < NumOfEvents; p++) {
    webpage += "<tr><td>Temp</td>";
    webpage += "<td><input type='text' name='" + String(0) + "." + String(p) + ".Temp' value='" + Timer[0].Temp[p] + "' maxlength='5' size='6'></td>";
    for (int dow = 1; dow < 6; dow++) {
      webpage += "<td><input type='text' name='" + String(dow) + "." + String(p) + ".Temp' value='" + Timer[dow].Temp[p] + "' maxlength='5' size='5'></td>";
    }
    webpage += "<td><input type='text' name='" + String(6) + "." + String(p) + ".Temp' value='" + Timer[6].Temp[p] + "' maxlength='5' size='5'></td>";
    webpage += "</tr>";
    webpage += "<tr><td>Start</td>";
    webpage += "<td><input type='time' name='" + String(0) + "." + String(p) + ".Start' value='" + Timer[0].Start[p] + "'></td>";
    for (int dow = 1; dow < 6; dow++) {
      webpage += "<td><input type='time' name='" + String(dow) + "." + String(p) + ".Start' value='" + Timer[dow].Start[p] + "'></td>";
    }
    webpage += "<td><input type='time' name='" + String(6) + "." + String(p) + ".Start' value='" + Timer[6].Start[p] + "'></td>";
    webpage += "</tr>";
    webpage += "<tr><td>Stop</td>";
    webpage += "<td><input type='time' name='" + String(0) + "." + String(p) + ".Stop' value='" + Timer[0].Stop[p] + "'></td>";
    for (int dow = 1; dow < 6; dow++) {
      webpage += "<td><input type='time' name='" + String(dow) + "." + String(p) + ".Stop' value='" + Timer[dow].Stop[p] + "'></td>";
    }
    webpage += "<td><input type='time' name='" + String(6) + "." + String(p) + ".Stop' value='" + Timer[6].Stop[p] + "'></td>";
    webpage += "</tr>";
    if (p < (NumOfEvents - 1)) {
      webpage += "<tr><td></td><td></td>";
      for (int dow = 2; dow < 7; dow++) {
        webpage += "<td>-</td>";
      }
      webpage += "<td></td></tr>";
    }
  }
  webpage += "</table>";
  webpage += "<div class='centre'>";
  webpage += "<br><input type='submit' value='Enter'><br><br>";
  webpage += "</div></form>";
  append_HTML_footer();
}
//#########################################################################################
void Setup() {
  append_HTML_header(noRefresh);
  webpage += "<h2>Thermostat System Setup</h2><br>";
  webpage += "<h3>Enter required parameter values</h3><br>";
  webpage += "<FORM action='/handlesetup'>";
  webpage += "<table class='centre'>";
  webpage += "<tr>";
  webpage += "<td>Setting</td><td>Value</td>";
  webpage += "</tr>";
  webpage += "<tr>";
  webpage += "<td><label for='hysteresis'>Hysteresis value (e.g. 0 - 1.0&deg;) [N.N]</label></td>";
  webpage += "<td><input type='text' size='4' pattern='[0-9][.][0-9]' name='hysteresis' value='" + String(Hysteresis, 1) + "'></td>";  // 0.0 valid input style
  webpage += "</tr>";
  webpage += "<tr>";
  webpage += "<td><label for='frosttemp'>Frost Protection Temperature&deg; [NN]</label></td>";
  webpage += "<td><input type='text' size='4' pattern='[0-9]*' name='frosttemp' value='" + String(FrostTemp) + "'></td>";  // 00-99 valid input style
  webpage += "</tr>";
  webpage += "<tr>";
  webpage += "<td><label for='earlystart'>Early start duration (mins) [NN]</label></td>";
  webpage += "<td><input type='text' size='4' pattern='[0-9]*' name='earlystart' value='" + String(EarlyStart) + "'></td>";  // 00-99 valid input style
  webpage += "</tr>";
  webpage += "<tr>";
  webpage += "<td><label for='manualoveride'>Manual heating over-ride </label></td>";
  webpage += "<td><select name='manualoverride'><option value='ON'>ON</option>";
  webpage += "<option selected value='OFF'>OFF</option></select></td>";  // ON/OFF
  webpage += "</tr>";
  webpage += "<td><label for='manualoverridetemp'>Manual Override Temperature&deg; </label></td>";
  webpage += "<td><input type='text' size='4' pattern='[0-9]*' name='manualoverridetemp' value='" + String(ManOverrideTemp, 0) + "'></td>";  // 00-99 valid input style
  webpage += "</tr>";
  webpage += "</table>";
  webpage += "<br><input type='submit' value='Enter'><br><br>";
  webpage += "</form>";
  append_HTML_footer();
}
//#########################################################################################
void Help() {
  append_HTML_header(noRefresh);
  webpage += "<h2>Help</h2><br>";
  webpage += "<div style='text-align: left;font-size:1.1em;'>";
  webpage += "<br><u><b>Setup Menu</b></u>";
  webpage += "<p><i>Hysteresis</i> - this setting is used to prevent unwanted rapid switching on/off of the heating as the room temperature";
  webpage += " nears or falls towards the set/target-point temperature. A normal setting is 0.5&deg;C, the exact value depends on the environmental characteristics, ";
  webpage += "for example, where the thermostat is located and how fast a room heats or cools.</p>";
  webpage += "<p><i>Frost Protection Temperature</i> - this setting is used to protect from low temperatures and pipe freezing in cold conditions. ";
  webpage += "It helps prevent low temperature damage by turning on the heating until the risk of freezing has been prevented.</p>";
  webpage += "<p><i>Early Start Duration</i> - if greater than 0, begins heating earlier than scheduled so that the scheduled temperature is reached by the set time.</p>";
  webpage += "<p><i>Heating Manual Override</i> - switch the heating on and control to the desired temperature, switched-off when the next timed period begins.</p>";
  webpage += "<p><i>Heating Manual Override Temperature</i> - used to set the desired manual override temperature.</p>";
  webpage += "<u><b>Schedule Menu</b></u>";
  webpage += "<p>Determines the heating temperature for each day of the week and up to 4 heating periods in a day. ";
  webpage += "To set the heating to come on at 06:00 and off at 09:00 with a temperature of 20&deg; enter 20 then the required start/end times. ";
  webpage += "Repeat for each day of the week and heating period within the day for the required heat profile.</p>";
  webpage += "<u><b>Graph Menu</b></u>";
  webpage += "<p>Displays the target temperature set and the current measured temperature. ";
  webpage += "Thermostat status is also displayed as temperature varies.</p>";
  webpage += "<u><b>Status Menu</b></u>";
  webpage += "<p>Displays the current temperature. ";
  webpage += "<p>Displays the temperature the current state of the thermostat (ON/OFF) and ";
  webpage += "timer status (ON/OFF).</p>";
  webpage += "</div>";
  append_HTML_footer();
}


void changeMode() {
  append_HTML_header(noRefresh);
  webpage += "<h2>Change Mode</h2><br>";
  webpage += "<div style='text-align: left;font-size:1.1em;'>";
  //<!-- button-->
  // Heat buttons
  webpage += "      <button class=\"button\" style='font-size: 1.1em; padding: 10px 20px; margin: 5px; color: white; background-color: #4CAF50; border: none; border-radius: 5px; cursor: pointer; transition: background-color 0.3s ease;' onclick=\"send_heat(1)\">HEAT ON</button>\n";
  webpage += "      <button class=\"button off\" style='font-size: 1.1em; padding: 10px 20px; margin: 5px; color: white; background-color: #f44336; border: none; border-radius: 5px; cursor: pointer; transition: background-color 0.3s ease;' onclick=\"send_heat(0)\">HEAT OFF</button><br>\n";
  // Cold buttons
  webpage += "      <button class=\"button\" style='font-size: 1.1em; padding: 10px 20px; margin: 5px; color: white; background-color: #4CAF50; border: none; border-radius: 5px; cursor: pointer; transition: background-color 0.3s ease;' onclick=\"send_cold(2)\">COLD ON</button>\n";
  webpage += "      <button class=\"button off\" style='font-size: 1.1em; padding: 10px 20px; margin: 5px; color: white; background-color: #f44336; border: none; border-radius: 5px; cursor: pointer; transition: background-color 0.3s ease;' onclick=\"send_cold(3)\">COLD OFF</button><br>\n";
  // Fan buttons
  webpage += "      <button class=\"button\" style='font-size: 1.1em; padding: 10px 20px; margin: 5px; color: white; background-color: #4CAF50; border: none; border-radius: 5px; cursor: pointer; transition: background-color 0.3s ease;' onclick=\"send_fan(4)\">FAN ON</button>\n";
  webpage += "      <button class=\"button off\" style='font-size: 1.1em; padding: 10px 20px; margin: 5px; color: white; background-color: #f44336; border: none; border-radius: 5px; cursor: pointer; transition: background-color 0.3s ease;' onclick=\"send_fan(5)\">FAN OFF</button><br>\n";
  webpage += "    </div>\n";
  webpage += "    <br>\n";
  webpage += "    <div>\n";
  webpage += "      <h2>\n";
  webpage += "        HEAT State: <span id=\"heat_state\">NA</span>\n";
  webpage += "      </h2>\n";
  webpage += "      <h2>\n";
  webpage += "        COLD State: <span id=\"cold_state\">NA</span>\n";
  webpage += "      </h2>\n";
  webpage += "      <h2>\n";
  webpage += "        FAN State: <span id=\"fan_state\">NA</span>\n";
  webpage += "      </h2>\n";
  webpage += "    </div>\n";
  webpage += "    <script>\n";
  webpage += "      function send_heat(btn_sts) {\n";
  webpage += "        var xhttp = new XMLHttpRequest();\n";
  webpage += "        xhttp.onreadystatechange = function() {\n";
  webpage += "          if (this.readyState == 4 && this.status == 200) {\n";
  webpage += "            document.getElementById(\"heat_state\").innerHTML = this.responseText;\n";
  webpage += "          }\n";
  webpage += "        };\n";
  webpage += "        xhttp.open(\"GET\", \"heat_set?heat_state=\" + btn_sts, true);\n";
  webpage += "        xhttp.send();\n";
  webpage += "      }\n";
  webpage += "      function send_cold(btn_sts) {\n";
  webpage += "        var xhttp = new XMLHttpRequest();\n";
  webpage += "        xhttp.onreadystatechange = function() {\n";
  webpage += "          if (this.readyState == 4 && this.status == 200) {\n";
  webpage += "            document.getElementById(\"cold_state\").innerHTML = this.responseText;\n";
  webpage += "          }\n";
  webpage += "        };\n";
  webpage += "        xhttp.open(\"GET\", \"cold_set?cold_state=\" + btn_sts, true);\n";
  webpage += "        xhttp.send();\n";
  webpage += "      }\n";
  webpage += "      function send_fan(btn_sts) {\n";
  webpage += "        var xhttp = new XMLHttpRequest();\n";
  webpage += "        xhttp.onreadystatechange = function() {\n";
  webpage += "          if (this.readyState == 4 && this.status == 200) {\n";
  webpage += "            document.getElementById(\"fan_state\").innerHTML = this.responseText;\n";
  webpage += "          }\n";
  webpage += "        };\n";
  webpage += "        xhttp.open(\"GET\", \"fan_set?fan_state=\" + btn_sts, true);\n";
  webpage += "        xhttp.send();\n";
  webpage += "      }\n";
  webpage += "    </script>\n";
  webpage += "\n";
  append_HTML_footer();
}





#endif