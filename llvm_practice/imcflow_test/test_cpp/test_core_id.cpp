void test_core_id() {
  short hid = __builtin_IMCE_GET_CORE_HID();
  short cid = __builtin_IMCE_GET_CORE_WID();

  if(hid == 0 && cid == 0) {
    __builtin_IMCE_SETFLAG(1);
  } else if (hid == 0 && cid == 1) {
    __builtin_IMCE_SETFLAG(2);
  } else if (hid == 1 && cid == 0) {
    __builtin_IMCE_SETFLAG(3);
  } else if (hid == 1 && cid == 1) {
    __builtin_IMCE_SETFLAG(4);
  }
}