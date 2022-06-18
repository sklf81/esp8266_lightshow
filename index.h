const char HTML_CONTENT [] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="de" dir="ltr">
  <head>
    <meta charset="utf-8">
    <style>
      body{
        margin: 0px;
        background-color: #151515;
        padding-bottom: 50px;
      }
      h1{
        text-align: center;
        font-family: Arial;
        font-weight: 600;
        font-size: 300%;
        color: #FFFFFF;
      }
      form{
        position: relative;
        left: 50%;
        margin-left: -100px;
        width: 200px;
        height: 100px;
      }
      .cw{
        background-color:  transparent;
		    border: 0px solid;
        border-color: transparent;
        margin-right: -6px;
        margin-top: 0px;
        filter: drop-shadow(2px 2px 2px #000000);
        width: 100%;
        height: 50px;
        display: block;
      }
      .cw:hover{
        background-color: #FF00FB;
        color: #FFFFFF;
        filter: drop-shadow(0px 0px 0px #000000);
      }
      .cw:focus{
        outline: none;
      }
      .button{
        background-color: #FFFFFF;
        border-style: outset;
        border-color: #151515;
        border-width: 2px;
        margin-right: 4px;
        filter: drop-shadow(2px 2px 2px #000000);
        font-size: 150%;
        margin: 0 auto;
        left: 50%;
        width: 200px;
        margin-left: 0px;
        margin-top: 10px;
        margin-bottom: 14px;
        font-family: Arial;
        font-style: normal;
        font-weight: bold;
      }
      .button:hover{
        background-color: #FF00FB;
        color: #FFFFFF;
        border-style: solid;
        border-color: white;
        filter: drop-shadow(0px 0px 0px #000000);
      }
      .button[type="submit"]{
        width: 100%;
        height: 50px;
      }
      .show{
        background-color: #FFFFFF;
        border-style: outset;
        border-color: #151515;
        border-width: 2px;
        margin-right: 4px;
        filter: drop-shadow(2px 2px 2px #000000);
        margin: 0 auto;
        left: 50%;
        width: 98px;
        margin-left: 0px;
        margin-top: 10px;
        margin-bottom: 14px;
        font-family: Arial;
        font-style: normal;
        font-weight: bold;
      }
      .show:hover{
        background-color: #FF00FB;
        color: #FFFFFF;
        border-style: solid;
        border-color: white;
        filter: drop-shadow(0px 0px 0px #000000);
      }
      .inputdesc{
        margin-top: 20px;
        margin-bottom: 20px;
        text-align: center;
        font-family: Arial;
        font-weight: 600;
        font-size: 160%;
        color: #FFFFFF;
      }
      .inputtext{
        background-color: #FFFFFF;
        border-style: outset;
        border-color: #151515;
        border-width: 2px;
        margin-right: 4px;
        filter: drop-shadow(2px 2px 2px #000000);
        width: 200px;
        margin-bottom: 10px;
        height: 25px;
      }
      .container {
        font-family: Arial;
        font-weight: 600;
        color: #FFFFFF;
        display: block;
        position: relative;
        left: 50%;
        margin-left: -50px;
        width: 100px;
        padding-left: 35px;
        margin-bottom: 6px;
        margin-top: 10px;
        cursor: pointer;
        font-size: 22px;
        -webkit-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
      }
      .container input {
        display: block;
        position: relative;
        opacity: 0;
        cursor: pointer;
      }
      .checkmark {
        position: absolute;
        top: 0;
        left: 0;
        height: 25px;
        width: 25px;
        background-color: #eee;
        border-radius: 50%;
        filter: drop-shadow(2px 2px 2px #000000);
      }
      .container:hover input ~ .checkmark {
        background-color: #130613;
        filter: drop-shadow(0px 0px 0px #000000);
      }
      .container input:checked ~ .checkmark {
        background-color: #FF00FB;
      }
      .checkmark:after {
        content: "";
        position: absolute;
        display: none;
      }
      .container input:checked ~ .checkmark:after {
        display: block;
      }
      .container .checkmark:after {
       	top: 9px;
      	left: 9px;
      	width: 8px;
      	height: 8px;
      	border-radius: 50%;
      	background: white;
      }
      #gradientlength{
        opacity: 0;
      }
      .invisible{
        opacity: 100;
      }
    </style>
    <script>
      var gradient_length = 0;

      function addElement(type, id, text, parentid){
        var child = document.createElement(type);
        child.id = id;
        var text = document.createTextNode(text);
        var parent = document.getElementById(parentid);
        child.appendChild(text);
        parent.appendChild(child);
      }
      function addAttribute(elementId, attribute, value){
        var element = document.getElementById(elementId);
        element.setAttribute(attribute, value);
      }
      function addColor(position){
        var elementId = "cw" + position;
        var name = "Farbe " + (position + 1)
        addElement("input", elementId, name, "gradient");
        var element = document.getElementById(elementId);
        element.className = "cw";
        element.name = "cw" + position;
        addAttribute(elementId, "type", "color");
        addAttribute("gradientlength", "value", position + 1);
        addAttribute(elementId, "onchange", "refresh()");
      }
      function copyToClipboard(string){
        var element = document.createElement("input");
        element.setAttribute("type", "text");
        element.setAttribute("value", string);
        document.getElementById("invisible").appendChild(element);
        element.select();
        document.execCommand("copy");
        element.parentNode.removeChild(element);
      }
      function getShowDataString(){
        var output = "sdf...";
        output += "dly{";
        output += document.getElementById("delayinput").value;
        output += "}";

        for (var i = 0; i < gradient_length; i++) {
          output += "cw" + i + "{"+ document.getElementById("cw" + i).value + "}";
        }
        output += "e";
        return output;
      }

      function getShowFromString(text){
        var string_input = text;
        var value_delay = "";
        var position_colors = [];
        var value_colors = [];
        if(string_input[0] == 's' && string_input[1] == 'd' && string_input[2] == 'f'){
          var position_delay = string_input.search("dly{") + 3;
          for (var i = position_delay + 1; position_delay > 0 && string_input[i] != "}"; i++) {
            value_delay += string_input[i];
          }
          for (var i = 0; string_input.search("cw") > 0 ; i++) {
            position_colors[i] = string_input.search("cw") + 4;
            if(position_colors[i] > 0){
              value_colors[i] = "";
              var j
              for (j = position_colors[i]; string_input[j] != "}"; j++) {
                value_colors[i] += string_input[j];
              }
              string_input = string_input.slice(position_colors[i], string_input.length);
            }

            else break;
          }
          document.getElementById("delayinput").value = value_delay;
          for (var i = 0; i < gradient_length; i++) {
            document.getElementById("cw"+i).parentNode.removeChild(document.getElementById("cw"+i));
          }
          gradient_length = 0;
          for (var i = 0; i < value_colors.length; i++) {
            addColor(i);
            document.getElementById("cw"+i).value = value_colors[i];
            gradient_length++;
          }
        }
        else {
          alert("wrong data format");
        }
      }
      function refresh(){
        document.getElementById("showinput").value = getShowDataString();
      }
      window.onload = refresh;
    </script>
    <title>RGB Control</title>
  </head>
  <body>
    <label class="invisible" id="invisible"></label>

    <h1>Lightshow</h1>
    <form action="/actionpage" class="form">
      <div class="inputdesc">Delay[ms]</div>
      <input type="number" class="inputtext" id="delayinput" onchange="refresh()"name="delayinput" min="1" max="65536‬" value="10" autocomplete="off"></input>
      <div class="inputdesc">Data</div>
      <input type="text" class="inputtext" id="showinput" name="showinput" autocomplete="off" value=""></input>
      <input type="button" class="show" id="copyinput" onclick="copyToClipboard(getShowDataString()); refresh()" name="saveshow" value="Copy Show"></input>
      <input type="button" class="show" id="pasteinput" onclick="getShowFromString(document.getElementById('showinput').value); refresh()" name="loadshow" value="Load Show"></input>

      <label class="gradient" id="gradient">
        <input type="button" class="button" onclick="addColor(gradient_length++); refresh()" name="colormode" value="Add Color"></input>
        <input id="gradientlength" name="gradientlength" value="0"></input>
      </label>

      <label class="container">Stroke<input type="radio" checked="checked" name="colormode" value="stroke"></input><span class="checkmark"></span></label>
      <label class="container">Smooth<input type="radio" name="colormode" value="smooth"></input><span class="checkmark"></span></label>
      <label class="container">Smoothloop<input type="radio" name="colormode" value="smoothloop"></input><span class="checkmark"></span></label>
      <label class="container">Fade<input type="radio" name="colormode" value="fade"></input><span class="checkmark"></span></label>
      <label class="container">Off<input type="radio" name="colormode" value="off"></input><span class="checkmark"></span></label>


      <input type="submit" class="button" name="submitnormal" value="Submit"></input>
    </form>
  </body>
</html>

)=====";