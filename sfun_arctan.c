/*
S-function
sfun_arctan.c
Copyright (C) by M.Hirata 2021
*/
#define S_FUNCTION_NAME  sfun_arctan
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

int16_T _arctan_table[] = {
  4096,2418,1278,649,326,163,81,41,20,10,5,3,1
};

int16_T arctan(int16_T yin, int16_T xin)
{
  int32_T i;
  int32_T x, y, z;
  int16_T zout;
  if (xin < 0) {
    if ( yin > 0 ){
      x =  yin;
      y = -xin;
      z = 8192; // pi/2
    } else {
      x =  -yin;
      y =   xin;
      z = -8192; // -pi/2;
    }
  } else {
    x = xin;
    y = yin;
    z = 0;
  }
  int xtmp = x;
  int ytmp = y;
  for (i=0; i < 13; i++){
    if ( y < 0 ){
      x -= ytmp;
      y += xtmp;
      z -= _arctan_table[i];
    } else {
      x += ytmp;
      y -= xtmp;
      z += _arctan_table[i];
    }
    xtmp = x>>(i+1);
    ytmp = y>>(i+1);
  }
  zout = (0x7fff & (z + 16384)) - 16384;
  return zout;
}

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 0);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return;
    }
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    // Input port
    if (!ssSetNumInputPorts(S, 2)) return;
    // Signal width
    ssSetInputPortWidth(S, 0, 1);
    ssSetInputPortWidth(S, 1, 1);
    // Data types
    ssSetInputPortDataType(S, 0, SS_INT32);
    ssSetInputPortDataType(S, 1, SS_INT32);
    // Direct feed through
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    // Contiguous access
    ssSetInputPortRequiredContiguous(S, 0, 1);
    ssSetInputPortRequiredContiguous(S, 1, 1);

    // Output port
    if (!ssSetNumOutputPorts(S, 1)) return;
    // width
    ssSetOutputPortWidth(S, 0, 1);
    // type
    ssSetOutputPortDataType(S, 0, SS_INT32);

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
    const int32_T *Ea = (int32_T *) ssGetInputPortRealSignal(S, 0);
    const int32_T *Eb = (int32_T *) ssGetInputPortRealSignal(S, 1);
    int32_T *theta    = (int32_T *) ssGetOutputPortRealSignal(S, 0);
    theta[0] = arctan(Ea[0],Eb[0]);
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
