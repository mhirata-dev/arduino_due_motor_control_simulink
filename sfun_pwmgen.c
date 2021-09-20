/*
S-function
sfun_pwmgen.c
Copyright (C) by M.Hirata 2021
*/
#define S_FUNCTION_NAME  sfun_pwmgen
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
/* for Arduino */
#ifndef MATLAB_MEX_FILE
#include <Arduino.h>
extern void pwmOUT(int16_t a, int16_t b, int16_t c);
extern int16_t getIU();
extern int16_t getIV();
extern int16_t getIW();
#define NUM_CURRENT_OFFSET  128
volatile int32_t i_count=0;
volatile int32_t ia0_=0, ib0_=0, ic0_=0;
volatile int32_t ia0=0, ib0=0, ic0=0;
volatile int32_t current_offset_id = 0;
#endif

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 0);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return;
    }
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    // Input port
    if (!ssSetNumInputPorts(S, 4)) return;
    // Signal width
    ssSetInputPortWidth(S, 0, 1);
    ssSetInputPortWidth(S, 1, 1);
    ssSetInputPortWidth(S, 2, 1);
    ssSetInputPortWidth(S, 3, 1);
    // Data types
    ssSetInputPortDataType(S, 0, SS_INT32);
    ssSetInputPortDataType(S, 1, SS_INT32);
    ssSetInputPortDataType(S, 2, SS_INT32);
    ssSetInputPortDataType(S, 3, SS_INT32);
    // Direct feed through
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    ssSetInputPortDirectFeedThrough(S, 2, 1);
    ssSetInputPortDirectFeedThrough(S, 3, 1);
    // Contiguous access
    ssSetInputPortRequiredContiguous(S, 0, 1);
    ssSetInputPortRequiredContiguous(S, 1, 1);
    ssSetInputPortRequiredContiguous(S, 2, 1);
    ssSetInputPortRequiredContiguous(S, 3, 1);

    // Output port
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

#undef MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
  static void mdlStart(SimStruct *S)
  {
  }
#endif /*  MDL_START */

static void mdlOutputs(SimStruct *S, int_T tid)
{
}

#define MDL_UPDATE  /* Change to #undef to remove function */
#if defined(MDL_UPDATE)
  static void mdlUpdate(SimStruct *S, int_T tid)
  {
    const int32_T *du     = (int32_T *) ssGetInputPortRealSignal(S, 0);
    const int32_T *dv     = (int32_T *) ssGetInputPortRealSignal(S, 1);
    const int32_T *dw     = (int32_T *) ssGetInputPortRealSignal(S, 2);
    const int32_T *enable = (int32_T *) ssGetInputPortRealSignal(S, 3);
#ifndef MATLAB_MEX_FILE
    if (enable[0]!=0){
        // ON
        digitalWrite(9,HIGH); // Enable
        digitalWrite(8,HIGH); // LED ON
        // PWM Out
        pwmOUT(du[0],dv[0],dw[0]);
        ia0_ = 0; ib0_ = 0; ic0_ = 0; i_count = 0;
    }else{
        // OFF
        digitalWrite(9,LOW); // disable
        digitalWrite(8,LOW); // LED OFF
        // PWM OFF
        pwmOUT(0,0,0);
        // Current offset
        if ( i_count < NUM_CURRENT_OFFSET ) {
            ia0_ += getIU();
            ib0_ += getIV();
            ic0_ += getIW();
            i_count++;
        } else if ( i_count == NUM_CURRENT_OFFSET ) {
            ia0 = ia0_ / NUM_CURRENT_OFFSET;
            ib0 = ib0_ / NUM_CURRENT_OFFSET;
            ic0 = ic0_ / NUM_CURRENT_OFFSET;
            i_count++;
            current_offset_id = 1;
        } else {
            ia0_ = 0; ib0_ = 0; ic0_ = 0; i_count = 0;
        }
    }
#endif
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
#ifndef MATLAB_MEX_FILE
    digitalWrite(9,LOW); // disable
    digitalWrite(8,LOW); // LED OFF
#endif
}

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
