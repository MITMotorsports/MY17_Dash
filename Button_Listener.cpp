

// void sendEnableRequest() {
//   Frame enableMessage = { .id=DASH_ID, .body={1}, .len=1};
//   CAN().write(enableMessage);
// }
//
// void sendDisableRequest() {
//   Frame disableMessage = { .id=DASH_ID, .body={0}, .len=1};
//   CAN().write(disableMessage);
// }
//
// boolean sendEnableRequestWrapper(Task*) {
//   sendEnableRequest();
//   enableFired = true;
//   return false;
// }

// DelayRun sendEnableRequestTask(500, sendEnableRequestWrapper);
//
// void pressRtdButton() {
//   // The enable task will fire automatically if held for >500ms
//   enableFired = false;
//   sendEnableRequestTask.startDelayed();
// }
//
// void releaseRtdButton(unsigned long) {
//   if(enableFired) {
//     // Do nothing since car already enabled before release
//     return;
//   }
//   else {
//     // Button released before 500ms, so driver must want to disable
//     SoftTimer.remove(&sendEnableRequestTask);
//     sendDisableRequest();
//   }
// }
