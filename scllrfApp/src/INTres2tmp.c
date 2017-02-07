/* LN2 Magnet Teststand - Resistance to Temperature Conversion
 * -----------------------------------------------------------
 * File:      INTres2tmp.c
 * Author:    George E. Lahti (gel)
 * Date:      18 Feb 2014
 * Revised:   
 * Type:      Epics subroutine record
 * What:      Convert an input into a calibrated output,
 *            via a linear interpolation table.
 *            The output endpoint values extend outside the region,
 *            with no extrapolation.
 *
 * Reference: 
 *    Calibration tables from Probir:    [ballard:19sep2014]
 */

#include <stdlib.h>
#include <callback.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <subRecord.h>
#include <math.h>

#include <stdio.h>  /* for printf() */

#include "registryFunction.h"
#include "epicsExport.h"

/* Inputs */
#define VOLTS       (psub->a)    /* Voltage in volts */
#define MILLI_AMPS  (psub->b)    /* Current in mA */

/* Outputs */
#define TEMPERATURE (psub->val)  /* Temperature in degC */


/* MISC */
#define POINTS (96)
struct CalTable
    {
    double x[POINTS];  /* input  (resistance in ohms) */
    double y[POINTS];  /* output (temperature in degC) */
    };
struct CalTable pv =     /* 96 points */
    {
    {   /* X axis, input, resistance in ohms */
    188.86, 186.84, 185.01, 183.19, 181.36,
    179.53, 177.69, 175.86, 174.02, 172.17,
    170.33, 168.48, 166.63, 164.77, 162.91,
    161.05, 159.19, 157.33, 155.46, 153.58,
    151.71, 149.83, 147.95, 146.07, 144.18,

    142.29, 140.4,  138.51, 136.61, 134.71,
    132.8,  130.9,  128.99, 127.08, 125.16,
    123.24, 121.32, 119.4,  117.47, 115.54,
    113.61, 111.67, 109.73, 107.79, 105.85,
    103.9,  101.95, 100,    98.04,  96.09,

    94.12, 92.16, 90.19, 88.22, 86.25,
    84.27, 82.29, 80.31, 78.32, 76.33,
    74.33, 72.33, 70.33, 68.33, 66.31,
    64.3, 62.28, 60.26, 58.23, 56.19,
    54.15, 52.11, 50.06, 48, 45.94,

    43.88, 41.8, 39.72, 37.64, 35.54,
    33.34, 31.33, 29.22, 27.1, 24.97,
    22.83, 20.68, 18.52, 16.36, 14.2,
    12.04, 9.88, 7.72, 5.56, 3.4,
    1.24,
    },  /* end of x axis */

    {   /* Y axis, output, temperature in degC */
    235, 230, 225, 220, 215,
    210, 205, 200, 195, 190,
    185, 180, 175, 170, 165,
    160, 155, 150, 145, 140,
    135, 130, 125, 120, 115,

    110, 105, 100, 95, 90,
    85,  80,  75,  70, 65,
    60,  55,  50,  45, 40,
    35,  30,  25,  20, 15,
    10,  5,   0,   -5, -10,

    -15, -20, -25, -30, -35,
    -40, -45, -50, -55, -60,
    -65, -70, -75, -80, -85,
    -90, -95, -100, -105, -110,
    -115, -120, -125, -130, -135,

    -140, -145, -150, -155, -160,
    -165, -170, -175, -180, -185,
    -190, -195, -200, -205, -210,
    -215, -220, -225, -230, -235,
    -240,
    }   /* end of y axis */
    };


#if 0
--------------------------------------------------
struct CalTable pv =    /* 88 points */
    {
    {   /* X axis, input, resistance in ohms */
    188.86, 186.84, 185.01, 183.19, 181.36,
    179.53, 177.69, 175.86, 174.02, 172.17,
    170.33, 168.48, 166.63, 164.77, 162.91,
    161.05, 159.19, 157.33, 155.46, 153.58,
    151.71, 149.83, 147.95, 146.07, 144.18,

    142.29, 140.4,  138.51, 136.61, 134.71,
    132.8,  130.9,  128.99, 127.08, 125.16,
    123.24, 121.32, 119.4,  117.47, 115.54,
    113.61, 111.67, 109.73, 107.79, 105.85,
    103.9,  101.95, 100,    98.04,   96.09,

    94.12, 92.16, 90.19, 88.22, 86.25,
    84.27, 82.29, 80.31, 78.32, 76.33,
    74.33, 72.33, 70.33, 68.33, 66.31,
    64.3,  62.28, 60.26, 58.23, 56.19,
    54.15, 52.11, 50.06, 48,    45.94,

    43.88, 41.8,  39.72, 37.64, 35.54,
    33.34, 31.33, 29.22, 27.1,  24.97,
    22.83, 20.68, 18.52,
    },  /* end of x axis */

    {   /* Y axis, output, temperature in degC */
    235, 230, 225, 220, 215,
    210, 205, 200, 195, 190,
    185, 180, 175, 170, 165,
    160, 155, 150, 145, 140,
    135, 130, 125, 120, 115,

    110, 105, 100, 95, 90,
    85,  80,  75,  70, 65,
    60,  55,  50,  45, 40,
    35,  30,  25,  20, 15,
    10,  5,   0,   -5, -10,

    -15,  -20,  -25,  -30,  -35,
    -40,  -45,  -50,  -55,  -60,
    -65,  -70,  -75,  -80,  -85,
    -90,  -95,  -100, -105, -110,
    -115, -120, -125, -130, -135,

    -140, -145, -150, -155, -160,
    -165, -170, -175, -180, -185,
    -190, -195, -200,
    }   /* end of y axis */
    };
--------------------------------------------------
#endif


static long INTres2tmpInit(struct subRecord *psub)
    {
    TEMPERATURE = 666.0;
    return 0;
    }

static long INTres2tmpProc(struct subRecord *psub)
    {
    /* TEMPERATURE = VOLTS * 1000.0 + MILLI_AMPS; */

    double x;  /* Input value */
    double y;  /* Output value */
    double slope;
    int    i;  /* X&Y index (0 to 87) */

    double v = VOLTS;
    double ma = MILLI_AMPS;

    if (ma == 0.0)
        {
        /* Divide by zero error */
        TEMPERATURE = 123.456;
        return 0;
        }

    x = VOLTS * 1000.0 / MILLI_AMPS;
    
    for (i = 0; i < POINTS; ++i)
        {
        /* if(x < pv[j].x[i]) */
        if(x > pv.x[i])
            break;
        }

    if(i == 0)
        y = pv.y[0];
    else if(i <= (POINTS-1))
        {
        slope = (pv.y[i] - pv.y[i-1]) / (pv.x[i] - pv.x[i-1]);
        y = pv.y[i-1] + (x - pv.x[i-1]) * slope;
        }
    else
        y = pv.y[POINTS-1];

    TEMPERATURE = y;

    /* if (VOLTS == 1.07) */
        /* printf("v=%f  a=%f  t=%f\n", VOLTS, MILLI_AMPS, TEMPERATURE); */

    return 0;
    }


/* Subroutine Registration
 */
static registryFunctionRef INTres2tmpRef[] = {
    {"INTres2tmpInit",(REGISTRYFUNCTION)INTres2tmpInit},
    {"INTres2tmpProc",(REGISTRYFUNCTION)INTres2tmpProc}
    };
void INTres2tmp_reg(void)
    {
    registryFunctionRefAdd(INTres2tmpRef,NELEMENTS(INTres2tmpRef));
    }
epicsExportRegistrar(INTres2tmp_reg);

