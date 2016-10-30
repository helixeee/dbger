#include "mbed.h"
#include "USBHID.h"

#include "dbger.h"

using namespace dbger;
using namespace dbger::arm_mbed;

MbedCmsisDap cmsis_dap;

MbedUsbhidHostAdapter<1> usbhid;

DigitalInOut swclk(p21, PIN_INPUT, PullNone, 1);
DigitalInOut swdio(p22, PIN_INPUT, PullNone, 1);
MbedGpioPinControllerSwd pin_controller_swd(&swclk, &swdio);
MbedDapPortSwd swd(&pin_controller_swd);

DigitalOut led1(LED1);
DigitalOut led2(LED2);
MbedGpioIndicator indicator(&led1, &led2);

int main(void) {
  cmsis_dap.attachSwdPort(&swd);
  cmsis_dap.attachIndicator(&indicator);
  cmsis_dap.attachHostAdapter(&usbhid);

  usbhid.attach(&cmsis_dap);

  while (1) {
    usbhid.command();
    usbhid.sendResponse();
  }
}
