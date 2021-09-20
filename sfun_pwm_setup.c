/*
S-function
sfun_pwm_setup.c
Copyright (C) by M.Hirata 2021
*/
#define S_FUNCTION_NAME  sfun_pwm_setup
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
/* for Arduino */
#ifndef MATLAB_MEX_FILE
#include <Arduino.h>
#include "mh_pwm_adc.h"
#include "mh_pwm_adc.cpp"
#endif

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 0);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return;
    }
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, 0)) return;
    if (!ssSetNumOutputPorts(S, 0)) return;

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    ssSetOperatingPointCompliance(S, USE_DEFAULT_OPERATING_POINT);
    ssSetOptions(S, 0);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
//    ssSetSampleTime(S, 0, CONTINUOUS_SAMPLE_TIME);
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
}

#undef MDL_INITIALIZE_CONDITIONS   /* Change to #undef to remove function */
#if defined(MDL_INITIALIZE_CONDITIONS)
  static void mdlInitializeConditions(SimStruct *S)
  {
  }
#endif /* MDL_INITIALIZE_CONDITIONS */

#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
  static void mdlStart(SimStruct *S)
  {
#ifndef MATLAB_MEX_FILE
  // PWM and ADC
  setup_pwm();
  setup_adc();
  // Enabler
  pinMode(11, OUTPUT); // EN1
  pinMode(12, OUTPUT); // EN2
  pinMode(13, OUTPUT); // EN3
  pinMode(9, OUTPUT);  // Enabler (ON/OFF)
  pinMode(8, OUTPUT);  // LED
  pinMode(7, OUTPUT);  // Monitor
  digitalWrite(11, HIGH); // EN1
  digitalWrite(12, HIGH); // EN2
  digitalWrite(13, HIGH); // EN3
  digitalWrite(9, LOW); // Enarbler (ON/OFF)
  digitalWrite(8, LOW); // LED
  digitalWrite(7, LOW); // Monitor
#endif
  }
#endif /*  MDL_START */

static void mdlOutputs(SimStruct *S, int_T tid)
{
}

#undef MDL_UPDATE  /* Change to #undef to remove function */
#if defined(MDL_UPDATE)
  static void mdlUpdate(SimStruct *S, int_T tid)
  {
  }
#endif /* MDL_UPDATE */

#undef MDL_DERIVATIVES  /* Change to #undef to remove function */
#if defined(MDL_DERIVATIVES)
  static void mdlDerivatives(SimStruct *S)
  {
  }
#endif /* MDL_DERIVATIVES */

static void mdlTerminate(SimStruct *S)
{
}

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
