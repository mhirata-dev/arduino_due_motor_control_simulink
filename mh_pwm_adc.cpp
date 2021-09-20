// mh_pwm_adc.cpp
// Copyright (C) by M.Hirata 2021

#include <Arduino.h>
#include "mh_pwm_adc.h"

// Function for control
// void updateControl();
// PWM
void setup_pwm()
{
  // PWMユニットへのクロックを供給
  pmc_enable_periph_clk(PWM_INTERFACE_ID);
  // 使用チャンネルをdisableにする
  PWMC_DisableChannel(PWM_INTERFACE, 0);
  PWMC_DisableChannel(PWM_INTERFACE, 1);
  PWMC_DisableChannel(PWM_INTERFACE, 2);
  PWMC_DisableChannel(PWM_INTERFACE, 3);
  // PWM channnels 0,1,2,3に対応したピン設定
  // PDR: PIO Disable Register
  // IDR: PIO Interrupt Disable Register
  // ABSR: PIO Peripheral AB Select Register
  //   0: Peripheral A functionを割り当てる
  //   1: Peripheral B functionを割り当てる
  PIOC->PIO_PDR   = 0x2A8;  // PIO controlの無効化
  PIOC->PIO_IDR   = 0x2A8;  // PIO interruptsの無効化
  PIOC->PIO_ABSR |= 0x2A8;  // B peripheralへ切り換え
  // クロックの設定
  // 引数: clock Aの周波数(0の場合は使用しない), clock Bの周波数(0の場合は使用しない), Master clockの周波数(VARIANT_MCK=84MHz)
  PWMC_ConfigureClocks((unsigned int)PWM_FREQ * MAX_PWM_DUTY * 2, 0, VARIANT_MCK );

  // 同期チャンネル設定
  // pPwm, channels(1+2+4+8=15=0x0F), updateMode, requestMode, requestComparisonSelect
  PWMC_ConfigureSyncChannel(PWM_INTERFACE, 0x0F, 0, 0, 0);
  // 更新周期をPWMの周期と合わせる
  PWMC_SetSyncChannelUpdatePeriod(PWM_INTERFACE, 1);
  // PWM channel 0 を設定すれば，それ以外は同じ設定になる
  // pPwm, channel, prescaler, alignment, polarity 
  // PWM_CMR_CPRE_CLKA: Clock A
  // PWM_CMR_CALG: 中心固定PWM
  PWMC_ConfigureChannel(PWM_INTERFACE, 0, PWM_CMR_CPRE_CLKA, PWM_CMR_CALG, 0);
//  PWMC_ConfigureChannel(PWM_INTERFACE, 1, PWM_CMR_CPRE_CLKA, PWM_CMR_CALG, 0); // same as that of channel 0's config
//  PWMC_ConfigureChannel(PWM_INTERFACE, 2, PWM_CMR_CPRE_CLKA, PWM_CMR_CALG, 0); // same as that of channel 0's config

  // MAX Dutyの設定
  PWMC_SetPeriod(PWM_INTERFACE, 0, MAX_PWM_DUTY);
  PWMC_SetPeriod(PWM_INTERFACE, 1, MAX_PWM_DUTY);
  PWMC_SetPeriod(PWM_INTERFACE, 2, MAX_PWM_DUTY);
  PWMC_SetPeriod(PWM_INTERFACE, 3, MAX_PWM_DUTY);

  // DutyをMAX_PWM_DUTY/2に設定
  PWMC_SetDutyCycle(PWM_INTERFACE, 0, MAX_PWM_DUTY/2);
  PWMC_SetDutyCycle(PWM_INTERFACE, 1, MAX_PWM_DUTY/2);
  PWMC_SetDutyCycle(PWM_INTERFACE, 2, MAX_PWM_DUTY/2);
  PWMC_SetDutyCycle(PWM_INTERFACE, 3, MAX_PWM_DUTY/2);

  // PWM比較器の設定 (38.7.35節)
  // PWM_CMPM_CEN: comparison enable
  // CEN: comparison x enable
  // CTR: comparison x trigger comparison x period counter (CPRCNT)が CTR で定義される値に達したとき，トリガがかかる
  // CPR: comparison x period: CPRCNTの最大値を定義する
  // CPRCNT: comparison x counter
  // CUPR: comparixon x update period
  // CUPRCNT: comparison x update period counter
  int compMode = PWM_CMPM_CEN; //+ PWM_CMPM_CTR(0) + PWM_CMPM_CPR(0) + PWM_CMPM_CUPR(0);
  // pPwm, Comparison x value, comparison x mode
  PWMC_ConfigureComparisonUnit(PWM_INTERFACE, 0, 1, compMode);

  //Use comparison unit 0 for Event line 0 which the ADC hardware is looking for triggers on
  // pPwm, Line x, mode
  // PWM_ELMR_CSELy: comparison y selection
  // y番目のコンペアマッチが起こった場合，event line xにパルスを発生させる
  PWMC_ConfigureEventLineMode(PWM_INTERFACE, 0, PWM_ELMR_CSEL0);

  // PWM割り込みテスト
//  REG_PWM_CMR0 |= (1<<10); // 谷と頂上で割り込み
//  REG_PWM_CMR0 &= ~(1UL<<10); // 谷でのみ割り込み
//  PWM_INTERFACE->PWM_IDR1 = 0xFF;
//  PWM_INTERFACE->PWM_IER1 = 1;
//  PWM_INTERFACE->PWM_IDR2 = 0xFF;
//  PWM_INTERFACE->PWM_IER2 = 0;

  PWMC_EnableChannel(PWM_INTERFACE, 0) ;   // enable
  PWMC_EnableChannel(PWM_INTERFACE, 1) ;   // enable
  PWMC_EnableChannel(PWM_INTERFACE, 2) ;   // enable
  PWMC_EnableChannel(PWM_INTERFACE, 3) ;   // enable
}

void pwmOUT(int16_t a, int16_t b, int16_t c)
{
  // add center value
  a += MAX_PWM_DUTY / 2;
  b += MAX_PWM_DUTY / 2;
  c += MAX_PWM_DUTY / 2;
  // upper limit
  if (a > MAX_PWM_DUTY) a = MAX_PWM_DUTY;
  if (b > MAX_PWM_DUTY) b = MAX_PWM_DUTY;
  if (c > MAX_PWM_DUTY) c = MAX_PWM_DUTY;
  // lower limit
  if (a < 0) a = 0;
  if (b < 0) b = 0;
  if (c < 0) c = 0;
  //
  PWMC_SetDutyCycle (PWM_INTERFACE, 0, a);
  PWMC_SetDutyCycle (PWM_INTERFACE, 1, b);
  PWMC_SetDutyCycle (PWM_INTERFACE, 2, c);
  PWMC_SetSyncChannelUpdateUnlock(PWM_INTERFACE); // 同期更新
}

////////////////////////////////////////////////////////////////////////////////////
// ADC functions
////////////////////////////////////////////////////////////////////////////////////
volatile uint16_t adc_buf[7];
volatile uint16_t pot1Raw;
volatile uint16_t pot2Raw;
volatile uint16_t iuRaw;
volatile uint16_t ivRaw;
volatile uint16_t iwRaw;

void setup_adc()
{
  pmc_enable_periph_clk(ID_ADC);
  /*
   *  SystemCoreClock: 4*12000000=48MHz
   *  ADC_FREQ_MAX: 20MHz
   */
  adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX, ADC_STARTUP_FAST);
  // モードレジスタ(43.7.2節 pp.1333)
  ADC->ADC_MR =   (1 << 0)  // ハードウエアトリガを許可
                + (4 << 1)  // トリガーライン選択 ADC_TRIG4 (PWM Event Line0)
                + (0 << 8)  // プリスケーラ: ADClock = MCK/((Value + 1)*2) (Value=5)
                + (0 << 16) // スタートアップ時間 (8 clocks) (0=0clks, 1=8clks, 2=16clks, 3=24, 4=64, 5=80, 6=96, 7=112,...)
                + (0 << 20) // セトリング時間 (0=3clks, 1=5clks, 2=9clks, 3=17clks)
                + (0 << 24) // トラッキング時間 (Value + 1) clocks (最良の変換結果をもたらすための追従時間)
                + (0 << 28);// 転送時間 ((Value * 2) + 3) clocks (転送時間 = 1*2 + 3 = 5clocks) (Value = 1)
  // ADCチェンネルの選択
  ADC->ADC_CHER = 0b11100011; // = [A0,1,2,6,7]

  // ADC Interrupt Disable Register
  // 27bit = ENDRX (End of Receive Buffer Interrupt Disable)
  ADC->ADC_IDR = ~(1 << 27); // 受信バッファの割り込み禁止を無効化
  // Interrupt enable register ENDRX (end of receive buffer interrupt)を有効化
  ADC->ADC_IER = 1 << 27;    // 受信バッファの割り込みを有効化
  // Peripheral DMA Controller (PDC)のUser Interface
  // receive pointer
  ADC->ADC_RPR = (uint32_t)adc_buf; // DMA buffer
  // Receive counter register
  ADC->ADC_RCR = 5; // ADCの変換個数
  // Receive next pointer register
  ADC->ADC_RNPR = 0;
  // receive next counter register
  ADC->ADC_RNCR = 0;
  // transfer control register
  ADC->ADC_PTCR = 1; // receive transfer enable (enable dma mode)
  //ADC->ADC_CR=2;   // ソフトウエアトリガの場合（今回はPWMでトリガをかけるので不要）
  NVIC_EnableIRQ(ADC_IRQn);
//  NVIC_EnableIRQ(PWM_IRQn);
}

//int tim_count = 0;
//void PWM_Handler()
//{
////    tim_count = REG_PWM_CCNT0;
//    ADC->ADC_CR = 2; // ADCソフトウエアトリガ
//    int f = PWM_INTERFACE->PWM_ISR1; // clear interrupt flag
////    digitalWrite(7, HIGH);
////    digitalWrite(7, LOW);
//}

void ADC_Handler()
{
  REG_PIOC_SODR |= (1<<23); // Set Output Data Register
  int f = ADC->ADC_ISR;
  if (f & (1 << 27)) { //receive counter end of buffer
    // read data
    iuRaw   = adc_buf[4] & 0xfff;
    ivRaw   = adc_buf[3] & 0xfff;
    iwRaw   = adc_buf[2] & 0xfff;
    pot1Raw = adc_buf[1] & 0xfff;
    pot2Raw = adc_buf[0] & 0xfff;
    //
    ADC->ADC_RPR = (uint32_t)adc_buf; // DMA buffer
    ADC->ADC_RCR = 5; //# of samples
    //
    //updateControl();
  }
  REG_PIOC_CODR |= (1<<23); // Clear Output Data Register
}

int16_t getPot1Voltage() {
  int16_t val = pot1Raw;
  return val;
}

int16_t getPot2Voltage() {
  int16_t val = pot2Raw;
  return val;
}

int16_t getIU() {
  int16_t val = iuRaw;
  return val;
}

int16_t getIV() {
  int16_t val = ivRaw;
  return val;
}

int16_t getIW() {
  int16_t val = iwRaw;
  return val;
}

// read angle sensor (AS5048)
/*
void setupAngleSensor(uint16_t slaveSelectPin)
{
  pinMode(slaveSelectPin, OUTPUT);
  digitalWrite(slaveSelectPin, HIGH);
  SPI.begin();
  SPI.setDataMode(SPI_MODE1);
  SPI.setBitOrder(MSBFIRST);
}
*/

/*
uint16_t readAngle(uint16_t slaveSelectPin)
{
  uint16_t r1, r2, theta;
  digitalWrite(slaveSelectPin, LOW);
  r1 = SPI.transfer(0xff) & 0x3f;
  r2 = SPI.transfer(0xff) & 0xff;
  digitalWrite(slaveSelectPin, HIGH);
  theta = 0x3fff - ((r1<<8)+r2);
  return theta;
}
*/
// EOF of mh_motorlib.cpp
