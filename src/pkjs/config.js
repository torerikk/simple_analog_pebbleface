module.exports = [
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Connection"
      },
      {
        "type": "toggle",
        "messageKey": "disconnectWarning",
        "label": "Warn on disconnect"
      },
      {
        "type": "text",
        "defaultValue": "When on the clock will vibrate and a text is shown on the screen when Bluetooth connection to the phone is lost."
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Colors"
      },
      {
        "type": "color",
        "messageKey": "backgroundColor",
        "defaultValue": "aaaaaa",
        "label": "Background",
        "sunlight": true,
        "allowGray": true
      },
      {
        "type": "color",
        "messageKey": "clockStrokeColor",
        "defaultValue": "000000",
        "label": "Clock Stroke",
        "sunlight": true,
        "allowGray": true
      },
      {
        "type": "color",
        "messageKey": "clockfaceColor",
        "defaultValue": "ffffff",
        "label": "Clock face",
        "sunlight": true,
        "allowGray": true
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save"
  }
];