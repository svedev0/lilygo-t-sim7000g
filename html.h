char html[] = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {
      display: inline-block;
      margin: 0px auto;
      font-family: Helvetica;
      text-align: center;
    }
    h1 {
      padding: 30px 0;
    }
    button {
      margin: 2px;
      padding: 15px 30px;
      color: white;
      font-size: 18px;
      text-decoration: none;
      background-color: #4CAF50;
      border: none;
    }
  </style>
  <script>
    window.history.pushState({}, document.title, '/');
  </script>
</head>
<body>
  <h1>ESP32 LTE</h1>
  <p>
    <a href="/send-sms">
      <button>SEND SMS</button>
    </a>
  </p>
  <p>
    <a href="/send-get">
      <button>SEND GET</button>
    </a>
  </p>
</body>
</html>
)=====";
