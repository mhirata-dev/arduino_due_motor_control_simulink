/*
S-function
sfun_read_current.c
Copyright (C) by M.Hirata 2021
*/
#define S_FUNCTION_NAME  sfun_read_current
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
extern volatile int32_t ia0;
extern volatile int32_t ib0;
extern volatile int32_t ic0;
extern volatile int32_t current_offset_id;
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
    if (!ssSetNumInputPorts(S, 0)) return;

    // Output port
    if (!ssSetNumOutputPorts(S, 3)) return;
    // width
    ssSetOutputPortWidth(S, 0, 1);
    ssSetOutputPortWidth(S, 1, 1);
    ssSetOutputPortWidth(S, 2, 1);
    // type
    ssSetOutputPortDataType(S, 0, SS_INT32);
    ssSetOutputPortDataType(S, 1, SS_INT32);
    ssSetOutputPortDataType(S, 2, SS_INT32);

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
    int32_T *iu = (int32_T *) ssGetOutputPortRealSignal(S, 0);
    int32_T *iv = (int32_T *) ssGetOutputPortRealSignal(S, 1);
    int32_T *iw = (int32_T *) ssGetOutputPortRealSignal(S, 2);
#ifndef MATLAB_MEX_FILE
    int32_t ia, ib, ic;
    if (current_offset_id == 1){
        ia = getIU() - ia0;
        ib = getIV() - ib0;
        ic = getIW() - ic0;
    } else {
        ia = 0;
        ib = 0;
        ic = 0;
    }
    iu[0] = -ia;
    iv[0] = -ib;
    iw[0] = -ic;
#endif
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
