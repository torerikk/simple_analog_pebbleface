(function() {
  loadOptions();
  submitHandler();
})();

function submitHandler() {
  var $submitButton = $('#submitButton');

  $submitButton.on('click', function() {
    console.log('Submit');

    var return_to = getQueryParam('return_to', 'pebblejs://close#');
    document.location = return_to + encodeURIComponent(JSON.stringify(getAndStoreConfigData()));
  });
}
//clockfaceColorPicker //clockStrokeColorPicker
function loadOptions() {
  var $backgroundColorPicker = $('#backgroundColorPicker');
  var $clockfaceColorPicker = $('#clockfaceColorPicker');
  var $clockStrokeColorPicker = $('#clockStrokeColorPicker');
  var $btWarningToggle = $('#btWarningToggle');
	console.log($btWarningToggle);
  if (localStorage.backgroundColor) {
    $backgroundColorPicker[0].value = localStorage.backgroundColor;
    $clockfaceColorPicker[0].value = localStorage.clockfaceColor;
    $clockStrokeColorPicker[0].value = localStorage.clockStrokeColor;
    $btWarningToggle[0].checked = localStorage.disconnectWarning === 'true';
  }
}

function getAndStoreConfigData() {
  var $backgroundColorPicker = $('#backgroundColorPicker');
  var $clockfaceColorPicker = $('#clockfaceColorPicker');
  var $clockStrokeColorPicker = $('#clockStrokeColorPicker');
  var $btWarningToggle = $('#btWarningToggle');

  var options = {
    backgroundColor: $backgroundColorPicker.val(),
    clockfaceColor: $clockfaceColorPicker.val(),
    clockStrokeColor: $clockStrokeColorPicker.val(),
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