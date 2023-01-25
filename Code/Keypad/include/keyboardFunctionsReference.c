/*
  // Enumerators for different report IDs
  enum {
      RID_KEYBOARD = 1,
      RID_CONSUMER_CONTROL, // Media, volume etc ..
  };

// Consumer & Keyboard Control
uint8_t const desc_hid_report[] =
    {
        TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(RID_KEYBOARD)),
        TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(RID_CONSUMER_CONTROL))};

// Output report callback
void hid_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
}

*/
/*
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setReportCallback(NULL, hid_report_callback);
  usb_hid.setBootProtocol(HID_PROTOCOL_NONE);
  usb_hid.begin();
  delay(2000);
  while (!USBDevice.mounted())
    delay(1);
  if (USBDevice.suspended())
  {
    USBDevice.remoteWakeup();
  }
  while (!usb_hid.ready())
    delay(1);
  uint8_t keycode[6] = {0};
  keycode[0] = HID_KEY_A;
  usb_hid.keyboardReport(RID_KEYBOARD, 0, keycode);
  delay(4);
  usb_hid.keyboardRelease(RID_KEYBOARD);
  delay(1000);
  usb_hid.sendReport16(RID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_AL_CALCULATOR);
  delay(1000);
  usb_hid.sendReport16(RID_CONSUMER_CONTROL, 0);
  */