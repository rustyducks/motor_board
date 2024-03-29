#include "power_check.h"

#ifdef __cplusplus
extern "C" {
#endif
    #include "ch.h"
    #include "hal.h"
    #include "globalVar.h"
#ifdef __cplusplus
}
#endif
//#include "printf.h"

#include "BytesWriteBuffer.h"
#include "utils.h"
#include "messages.h"

using namespace protoduck;


#define PERIOD_BATTERY_REPORT 1000

static msg_t sendBatteryReport(float voltage);

ADCConversionGroup adc1cfg = {
  .circular = false,
  .num_channels = 1,
  .end_cb = NULL,
  .error_cb = NULL,
  .cr1 =0,
  .cr2 = ADC_CR2_SWSTART,
  .smpr1 = 0, //channels 10 to 18 unused
  .smpr2 = ADC_SMPR2_SMP_AN9(ADC_SAMPLE_480),
  .sqr1 = ADC_SQR1_NUM_CH(1),
  .sqr2 = 0,  // sequence 7 to 12 unused
  .sqr3 = ADC_SQR3_SQ1_N(ADC_CHANNEL_IN9)

};


static THD_WORKING_AREA(waPowerCheck, 1000);
static void power_check (void *arg) {
  (void)arg;
  chRegSetThreadName("power_check");

  adcInit();
  adcStart(&ADCD1, NULL);

  adcsample_t sample;

  static systime_t lastBatteryReportTime = 0;

  while (true) {
    msg_t status = adcConvert(&ADCD1, &adc1cfg, &sample, 1);
    if(status == MSG_OK) {
      float power_voltage = sample * ADC_TO_VOLTS;

      if(power_voltage > BAT_LOW) {   // power voltage good
        palClearLine(LINE_LED_ORANGE);
        palClearLine(LINE_LED_RED);
      }
      else if(power_voltage > BAT_CRITICAL) { // power voltage low, but not critical
        palToggleLine(LINE_LED_ORANGE);
        palClearLine(LINE_LED_RED);
      } else if(power_voltage < BAT_CRITICAL) { //power voltage critical
        palToggleLine(LINE_LED_RED);
        palClearLine(LINE_LED_ORANGE);
      }

      //chprintf ((BaseSequentialStream*)&SDU1, "%f\r\n", power_voltage);
      if(chVTTimeElapsedSinceX(lastBatteryReportTime) > chTimeMS2I(PERIOD_BATTERY_REPORT)) {
        if(sendBatteryReport(power_voltage) == MSG_OK) {
            lastBatteryReportTime = chVTGetSystemTime();
        }
      }
    }
    chThdSleepMilliseconds(1000);  //increase to 2000 or more
  }
}

msg_t sendBatteryReport(float voltage) {
  Message msg;
  auto& battery_report = msg.mutable_bat();
  battery_report.set_voltage(voltage);
  msg_t ret = post_message(msg, Message::MsgType::STATUS, TIME_IMMEDIATE);
  return ret;
}

void start_power_check() {
    chThdCreateStatic(waPowerCheck, sizeof(waPowerCheck), NORMALPRIO, &power_check, NULL); // lancement du thread 
}
