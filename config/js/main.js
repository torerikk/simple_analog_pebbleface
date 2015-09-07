(function() {
  loadOptions();
  submitHandler();
})();

function submitHandler() {
  var $submitButton = $('#submitButton');

  $submitButton.on('click', function() {
    var return_to = getQueryParam('return_to', 'pebblejs://close#');
    document.location = return_to + encodeURIComponent(JSON.stringify(getAndStoreConfigData()));
  });
}

function loadOptions() {
  var $backgroundColorPicker = $('#backgroundColorPicker');
  var $clockfaceColorPicker = $('#clockfaceColorPicker');
  var $clockStrokeColorPicker = $('#clockStrokeColorPicker');
  var $btWarningToggle = $('#btWarningToggle');

  if (localStorage.backgroundColor) {
    $backgroundColorPicker[0].value = '0x' + GColor.hexToSunny(localStorage.backgroundColor);
    $clockfaceColorPicker[0].value = '0x' + GColor.hexToSunny(localStorage.clockfaceColor);
    $clockStrokeColorPicker[0].value = '0x' + GColor.hexToSunny(localStorage.clockStrokeColor);
    $btWarningToggle[0].checked = localStorage.disconnectWarning === 'true';
  }
}

function getAndStoreConfigData() {
  var $backgroundColorPicker = $('#backgroundColorPicker');
  var $clockfaceColorPicker = $('#clockfaceColorPicker');
  var $clockStrokeColorPicker = $('#clockStrokeColorPicker');
  var $btWarningToggle = $('#btWarningToggle');

  var options = {
    backgroundColor: '0x' + GColor.hexToNormal($backgroundColorPicker.val()),
    clockfaceColor: '0x' + GColor.hexToNormal($clockfaceColorPicker.val()),
    clockStrokeColor: '0x' + GColor.hexToNormal($clockStrokeColorPicker.val()),
    disconnectWarning: $btWarningToggle[0].checked
  };
  
  localStorage.backgroundColor = options.backgroundColor;
  localStorage.clockfaceColor = options.clockfaceColor;
  localStorage.clockStrokeColor = options.clockStrokeColor;
  localStorage.disconnectWarning = options.disconnectWarning;

  console.log('Got options: ' + JSON.stringify(options));
  return options;
}

function getQueryParam(variable, defaultValue) {
  var query = location.search.substring(1);
  var vars = query.split('&');
  for (var i = 0; i < vars.length; i++) {
    var pair = vars[i].split('=');
    if (pair[0] === variable) {
      return decodeURIComponent(pair[1]);
    }
  }
  return defaultValue || false;
}