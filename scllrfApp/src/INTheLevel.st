/* Rf12Fcc: Epics Quench Detector
 * ------------------------------
 * File:      INTheLevel.st
 * Author:    George Lahti (gel) x7272
 * Date:      06 Sep 2011
 * Revise: 
 * Type:      Epics Sequencer
 *
 * REQUIREMENTS
 *
 * DESCRIPTION
 *
 * USAGE
 *   seq(&INTheLevel, "PREFX=INT1")
 *   seq(&INTheLevel, "PREFX=INT2")
 */

program INTheLevel

/* option +r;   Re-entrant code. */
option -c;   /* Do NOT wait for all database connections. */

/* System header files */
%% #include <math.h>


/* INPUTS */
/* ------ */

double  heLev;
assign  heLev             to "SRFCMTFLLRETURN";
/* assign  heLev             to "{PREFX}:RXHLLBUG"; */
monitor heLev;

double  heLevMax;
assign  heLevMax          to "{PREFX}:RXHLLMAX";
monitor heLevMax;

double  heLevMin;
assign  heLevMin          to "{PREFX}:RXHLLMIN";
monitor heLevMin;

#if 0
double  ictlb;
assign  ictlb             to "{PREFX}:RXICTLb";
monitor ictlb;
#endif

double  heLevClr;
assign  heLevClr          to "{PREFX}:RXHLLCLR";
monitor heLevClr;

/* OUTPUTS */
/* ------- */

double  heLevLatch;
assign  heLevLatch        to "{PREFX}:RXHLLFLTb.B0";

double  heLevInsta;
assign  heLevInsta        to "{PREFX}:RXHLLFLTb.B1";

double  heLevNocon;
assign  heLevNocon        to "{PREFX}:RXHLLFLTb.B2";

double  ictlb0;
assign  ictlb0              to "{PREFX}:RXICTLb.B0";

/* MISCELLANEOUS */
/* ------------- */

evflag  tryZ;

double oldLev;
double oldLevMax;
double oldLevMin;
/* double oldIctlb; */

/* ====================================================== */

ss INT_HE_LEVEL
    {
    state Init
        {
        when ()
            {
            printf("....................................... INTheLevel.st - LEV\n");
            oldLev    =  12345.0;
            oldLevMax =  56789.0;
            oldLevMin = -56789.0;
            efClear(tryZ);
            } state Step1
        }

    state Step1
        {
        when (heLevClr)
            {
            heLevLatch = 0.0;
            pvPut(heLevLatch);
            ictlb0 = 0.0;
            pvPut(ictlb0);
            efSet(tryZ);
            } state Step1
        when (pvConnected(heLev) && (efTest(tryZ) || (heLev != oldLev) || (heLevMax != oldLevMax) || (heLevMin != oldLevMin)))
            {
            efClear(tryZ);
            heLevNocon = 0.0;
            pvPut(heLevNocon);
            oldLev    = heLev;
            oldLevMax = heLevMax;
            oldLevMin = heLevMin;
            if ((heLev < heLevMin) || (heLev > heLevMax))
                {
                heLevLatch = 1.0;
                pvPut(heLevLatch);
                ictlb0 = 1.0;
                pvPut(ictlb0);
                heLevInsta = 1.0;
                pvPut(heLevInsta);
                }
            else
                {
                heLevInsta = 0.0;
                pvPut(heLevInsta);
                }
            } state Step1
        when (delay(5.0))
            {
            if (!pvConnected(heLev))
                {
                heLevNocon = 1.0;
                pvPut(heLevNocon);
                }
            } state Step1
        }
    }

#if 0
???????????????????????????????????????????????????????????
ss ICTL_COPY
    {
    state Init
        {
        when ()
            {
            printf("....................................... INTheLevel.st - COPY\n");
            oldIctlb = 54321.0;
            } state DoCopy
        }

    state DoCopy
        {
        when (ictlb != oldIctlb)
            {
            oldIctlb = ictlb;
            ictl = ictlb;
            pvPut(ictl);
            } state DoCopy
        }
    }
???????????????????????????????????????????????????????????
#endif

