/*
S-function
sfun_sincos.c
Copyright (C) by M.Hirata 2021
*/
#define S_FUNCTION_NAME  sfun_sincos
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"

// sine table
int16_T _sin_table[] = {
    0,   201,   402,   603,   804,  1005,  1206,  1407, 
 1608,  1809,  2009,  2210,  2411,  2611,  2811,  3012, 
 3212,  3412,  3612,  3812,  4011,  4211,  4410,  4609, 
 4808,  5007,  5205,  5404,  5602,  5800,  5998,  6195, 
 6393,  6590,  6787,  6983,  7180,  7376,  7571,  7767, 
 7962,  8157,  8351,  8546,  8740,  8933,  9127,  9319, 
 9512,  9704,  9896, 10088, 10279, 10469, 10660, 10850, 
11039, 11228, 11417, 11605, 11793, 11980, 12167, 12354, 
12540, 12725, 12910, 13095, 13279, 13463, 13646, 13828, 
14010, 14192, 14373, 14553, 14733, 14912, 15091, 15269, 
15447, 15624, 15800, 15976, 16151, 16326, 16500, 16673, 
16846, 17018, 17190, 17361, 17531, 17700, 17869, 18037, 
18205, 18372, 18538, 18703, 18868, 19032, 19195, 19358, 
19520, 19681, 19841, 20001, 20160, 20318, 20475, 20632, 
20788, 20943, 21097, 21251, 21403, 21555, 21706, 21856, 
22006, 22154, 22302, 22449, 22595, 22740, 22884, 23028, 
23170, 23312, 23453, 23593, 23732, 23870, 24008, 24144, 
24279, 24414, 24548, 24680, 24812, 24943, 25073, 25202, 
25330, 25457, 25583, 25708, 25833, 25956, 26078, 26199, 
26320, 26439, 26557, 26674, 26791, 26906, 27020, 27133, 
27246, 27357, 27467, 27576, 27684, 27791, 27897, 28002, 
28106, 28209, 28311, 28411, 28511, 28610, 28707, 28803, 
28899, 28993, 29086, 29178, 29269, 29359, 29448, 29535, 
29622, 29707, 29792, 29875, 29957, 30038, 30118, 30196, 
30274, 30350, 30425, 30499, 30572, 30644, 30715, 30784, 
30853, 30920, 30986, 31050, 31114, 31177, 31238, 31298, 
31357, 31415, 31471, 31527, 31581, 31634, 31686, 31737, 
31786, 31834, 31881, 31927, 31972, 32015, 32058, 32099, 
32138, 32177, 32214, 32251, 32286, 32319, 32352, 32383, 
32413, 32442, 32470, 32496, 32522, 32546, 32568, 32590, 
32610, 32629, 32647, 32664, 32679, 32693, 32706, 32718, 
32729, 32738, 32746, 32753, 32758, 32762, 32766, 32767, 
};

// theta is 15bit range
typedef struct
{
  int16_T sinv;
  int16_T cosv;
} sincos16_t;

sincos16_t sincost(int16_T theta)
{
  sincos16_t out;
  int16_T theta10; // theta in 10bit
  
  theta10 = (((int32_T)theta + (int32_T)32768) & 32767)>>5;
  
  uint8_T region = (uint8_T)((theta10 & 0x300) >> 8);
  uint8_T index  = (uint8_T)(theta10 & 0xff);
  switch (region){
    case 0:
      out.sinv = _sin_table[index];
      out.cosv = _sin_table[0xffu - index];
      break;
      
    case 1:
      out.sinv =  _sin_table[0xffu - index];
      out.cosv = -_sin_table[index];
      break;
      
    case 2:
      out.sinv = -_sin_table[index];
      out.cosv = -_sin_table[0xffu - index];
      break;

    case 3:
      out.sinv = -_sin_table[0xff - index];
      out.cosv =  _sin_table[index];
      break;
  }
  return out;
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
    if (!ssSetNumInputPorts(S, 1)) return;
    // Signal width
    ssSetInputPortWidth(S, 0, 1);
    // Data types
    ssSetInputPortDataType(S, 0, SS_INT32);
    // Direct feed through
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    // Contiguous access
    ssSetInputPortRequiredContiguous(S, 0, 1);

    // Output port
    if (!ssSetNumOutputPorts(S, 2)) return;
    // width
    ssSetOutputPortWidth(S, 0, 1);
    ssSetOutputPortWidth(S, 1, 1);
    // type
    ssSetOutputPortDataType(S, 0, SS_INT32);
    ssSetOutputPortDataType(S, 1, SS_INT32);

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
    const int32_T *theta = (int32_T *) ssGetInputPortRealSignal(S, 0);
    int32_T *sin = (int32_T *) ssGetOutputPortRealSignal(S, 0);
    int32_T *cos = (int32_T *) ssGetOutputPortRealSignal(S, 1);
    sincos16_t sc = sincost(theta[0]);
    sin[0] = sc.sinv;
    cos[0] = sc.cosv;
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
