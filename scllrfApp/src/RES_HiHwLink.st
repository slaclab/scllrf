/* RF 12GeV Steppers - Link Between Highland12.c and Hardware Register PVs
 * -----------------------------------------------------------------------
 * File:   RES_HiHwLink.st
 * Path:   /usr/devsite/csite7/it/llapp_R3.14/SLAC_scllrf/1-3/scllrf/scllrfApp/src
 * Copied: cp /usr/devsite/csite7/it/llapp_R3.14/Rf12Step/1-2F/iocCode/src/HiHwLink.st_8 RES_HiHwLink.st
 * From:   The Rf12Step file name is: HiHwLink.st
 * Author: George Lahti (gel)
 * Date:   17 May 2011 (based on the work in 2008) (Rf12Step app)
 * Date:   24 May 2017 - convert for use in CMTF SLAC LCLS2 Project
 * Type:   Epics Sequencer
 * What:   This sequencer copies values between the Highland12.c PV fields
 *         and the hardware registers (PVs).
 *
 * DESCRIPTION
 *
 * .......................................................... more
 * .......................................................... more
 *
 * Sequencer variable names notation:
 * - uXxxx = To/From the Highland12.c subroutine (u = user)
 * - hXxxx = Hardware Registers/PVs (h = hrdw)
 * - rXxxx = Results/Status from the Hardware (r = results)
 *
 * USAGE
 *   seq(&RES_HiHwLink, "CM=1, CAV=1")  ...... RES1:
 *   seq(&RES_HiHwLink, "CM=1, CAV=2")
 *   seq(&RES_HiHwLink, "CM=1, CAV=3")
 *   seq(&RES_HiHwLink, "CM=1, CAV=4")
 *   seq(&RES_HiHwLink, "CM=2, CAV=1")  ...... RES2:
 *   seq(&RES_HiHwLink, "CM=2, CAV=2")
 *   seq(&RES_HiHwLink, "CM=2, CAV=3")
 *   seq(&RES_HiHwLink, "CM=2, CAV=4")
 *
 * MUST put "#include" AFTER the "program" name line.
 * %%#include <time.h>
 */

program RES_HiHwLink

option +r;   /* Re-entrant code. */
option +c;   /* Wait for all database connections. */

/*----------------------*/

/* User Go command (from Highland12.c) */
double  uGo;
assign  uGo      to "RES{CM}:R{CAV}TGO";
monitor uGo;

/* User Go command +/- steps */
double  uGoSteps;
assign  uGoSteps to "RES{CM}:R{CAV}TGOSTEPS";
monitor uGoSteps;

/* User Abort command (from Highland12.c) */
double  uAbort;
assign  uAbort   to "RES{CM}:R{CAV}TABORT";
monitor uAbort;

/* User Clear command (from Highland12.c) */
double  uClear;
assign  uClear   to "RES{CM}:R{CAV}TCLEAR";
monitor uClear;



double  hSteps;
/* assign  hSteps   to "R{CM}{CAV}TSTPS"; */
assign  hSteps   to "RES{CM}:MOTOR{CAV}_STEPS_W";



/* Requested Go command (to hrt) */
double  hGo;
assign  hGo      to "RES{CM}:MOTOR{CAV}_CNTL_W.B0";

/* Requested Abort command (to hrt) */
double  hAbort;
assign  hAbort   to "RES{CM}:MOTOR{CAV}_CNTL_W.B3";

int     stop;
assign  stop     to "RES{CM}:MOTOR{CAV}_CNTL_W.B3";
monitor stop;

/* Requested Clear-Done command (to hrt) */
double  hClrDone;
assign  hClrDone to "RES{CM}:MOTOR{CAV}_CNTL_W.B4";

int     dirAA;
assign  dirAA    to "RES{CM}:MOTOR{CAV}_CNTL_W.B5";
monitor dirAA;

int     otto;
assign  otto     to "RES{CM}:MOTOR{CAV}_CNTL_W.B7";
monitor otto;

/*----------------------*/

int     dirA;
assign  dirA     to "RES{CM}:MOTOR{CAV}_STAT_R.B1";
monitor dirA;

int     limB;
assign  limB     to "RES{CM}:MOTOR{CAV}_STAT_R.B3";
monitor limB;

int     limA;
assign  limA     to "RES{CM}:MOTOR{CAV}_STAT_R.B4";
monitor limA;

int     inhib;
assign  inhib    to "RES{CM}:MOTOR{CAV}_STAT_R.B6";
monitor inhib;

int     sgnLim;
assign  sgnLim   to "RES{CM}:MOTOR{CAV}_STAT_R.B8";
monitor sgnLim;

int     absLim;
assign  absLim   to "RES{CM}:MOTOR{CAV}_STAT_R.B9";
monitor absLim;


#if 0
@@@@@@@@@@@ @@@@@@@@@@@@@@ @@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@
double  pvAslo;
assign  pvAslo   to "R{CM}{CAV}TPVASLO";
monitor pvAslo;

double  tstphzAslo;
assign  tstphzAslo   to "R{CM}{CAV}TSTPHZ.ASLO";
double  tstphzrAslo;
assign  tstphzrAslo  to "R{CM}{CAV}TSTPHZr.ASLO";

double  sgnlmtAslo;
assign  sgnlmtAslo   to "R{CM}{CAV}SGNLMT.ASLO";
double  sgnlmtrAslo;
assign  sgnlmtrAslo  to "R{CM}{CAV}SGNLMTr.ASLO";

double  abslmtAslo;
assign  abslmtAslo   to "R{CM}{CAV}ABSLMT.ASLO";
double  abslmtrAslo;
assign  abslmtrAslo  to "R{CM}{CAV}ABSLMTr.ASLO";

double  abssubAslo;
assign  abssubAslo   to "R{CM}{CAV}ABSSUB.ASLO";
double  abssubrAslo;
assign  abssubrAslo  to "R{CM}{CAV}ABSSUBr.ASLO";

double  sgnstpAslo;
assign  sgnstpAslo   to "R{CM}{CAV}SGNSTP.ASLO";

double  absstpAslo;
assign  absstpAslo   to "R{CM}{CAV}ABSSTP.ASLO";

double  tstpsAslo;
assign  tstpsAslo    to "R{CM}{CAV}TSTPS.ASLO";
double  tstpsrAslo;
assign  tstpsrAslo   to "R{CM}{CAV}TSTPSr.ASLO";
double  tstplAslo;
assign  tstplAslo    to "R{CM}{CAV}TSTPL.ASLO";
double  tstpmAslo;
assign  tstpmAslo    to "R{CM}{CAV}TSTPM.ASLO";

double  tvelsAslo;
assign  tvelsAslo    to "R{CM}{CAV}TVELS.ASLO";
double  tvelsrAslo;
assign  tvelsrAslo   to "R{CM}{CAV}TVELSr.ASLO";
double  tvellAslo;
assign  tvellAslo    to "R{CM}{CAV}TVELL.ASLO";

double  taccsAslo;
assign  taccsAslo    to "R{CM}{CAV}TACCS.ASLO";
double  taccsrAslo;
assign  taccsrAslo   to "R{CM}{CAV}TACCSr.ASLO";
double  tacclAslo;
assign  tacclAslo    to "R{CM}{CAV}TACCL.ASLO";

double  oldPvAslo;
@@@@@@@@@@@ @@@@@@@@@@@@@@ @@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@
#endif


/* MISCELLANEOUS */

#define PULSE_DELAY  (2.0)

char *pCM;
char *pCAV;

/*----------------------*/

ss RES_HI_HW_LINK
    {
    state Init
	{
	when()
	    {
            pCM = macValueGet("CM");
            pCAV = macValueGet("CAV");
            printf("----\n");
	    printf("---- RES_HiHwLink.st [CM=%s] [CAV=%s]@@@@@@@@@@@@@@@\n",pCM,pCAV);
            printf("----\n");
	    hGo      = 0;  pvPut(hGo);
	    hAbort   = 0;  pvPut(hAbort);
	    hClrDone = 0;  pvPut(hClrDone);
	    } state WaitForButton
	}

    state WaitForButton
	{
	when (uGo != 0)        /* GO (MOVE) COMMAND */
	    {
            printf("---------------------------- RES[%s]_Hi[%s] 0\n",pCM,pCAV);
            printf("RES[%s]_Hi[%s] %d %d %d %d %d   %d %d    %d %d\n",pCM,pCAV,
                inhib,otto,stop,sgnLim,absLim,  limA,dirA,  limB,!dirA);
            } state GoButtonPushed

	when (uAbort != 0)     /* ABORT COMMAND */
	    {
            /* 1 --> hAbort
             * 0 --> hAbort
             */
            printf("---------------------------- RES[%s]_Hi[%s] 1\n",pCM,pCAV);
	    hAbort = 1;  pvPut(hAbort);
	    hAbort = 0;  pvPut(hAbort);
	    } state AbortDelay

	when (uClear != 0)     /* CLEAR COMMAND */
	    {
            /* 1 --> hClrDone
             * 0 --> hClrDone
             */
            printf("---------------------------- RES[%s]_Hi[%s] 2\n",pCM,pCAV);
	    hClrDone = 1;  pvPut(hClrDone);
	    hClrDone = 0;  pvPut(hClrDone);
	    } state ClearDelay
	}

    state GoButtonPushed
        {
        when(inhib || otto || stop || sgnLim || absLim || (limA && dirA) || (limB && !dirA))
            {
            printf("---------------------------- RES[%s]_Hi[%s] 3\n",pCM,pCAV);
            uGo = 2;  pvPut(uGo);   /* Error condition -- no go */
            /*} state GoDelay*/
            } state Step0

        when (delay(0.5))
            {
            } state Step0
        }

    state Step0
        {
        when ()
            {
            printf("---------------------------- RES[%s]_Hi[%s] 4\n",pCM,pCAV);
            if (uGoSteps >= 0.0)
                {
                printf("---------------------------- RES[%s]_Hi[%s] 5\n",pCM,pCAV);
                hSteps = uGoSteps;  pvPut(hSteps);
                dirAA = 0;           pvPut(dirAA);
                }
            else
                {
                printf("---------------------------- RES[%s]_Hi[%s] 6\n",pCM,pCAV);
                hSteps = -uGoSteps; pvPut(hSteps);
                dirAA = 1;           pvPut(dirAA);
                }
            printf("---------------------------- RES[%s]_Hi[%s] 7\n",pCM,pCAV);
            } state Step1
        }

    state Step1
        {
        when (delay(PULSE_DELAY))
            {
            printf("---------------------------- RES[%s]_Hi[%s] Z 1\n",pCM,pCAV);
            /* 1 --> hClrDone
             * 0 --> hClrDone
             * 1 --> hGo
             * 0 --> hGo
             */
            hClrDone = 1;  pvPut(hClrDone);
	    } state Step2
        }

    state Step2
        {
        when (delay(PULSE_DELAY))
            {
            printf("---------------------------- RES[%s]_Hi[%s] Z 2\n",pCM,pCAV);
            hClrDone = 0;  pvPut(hClrDone);
	    } state Step3
        }

    state Step3
        {
        when (delay(PULSE_DELAY))
            {
            printf("---------------------------- RES[%s]_Hi[%s] Z 3\n",pCM,pCAV);
            hGo = 1;       pvPut(hGo);
	    } state Step4
        }

    state Step4
        {
        when (delay(PULSE_DELAY))
            {
            printf("---------------------------- RES[%s]_Hi[%s] Z 3\n",pCM,pCAV);
            hGo = 0;       pvPut(hGo);
	    } state GoDelay
        }

    state GoDelay
        {
        when (delay(4.0))
            {
            printf("---------------------------- RES[%s]_Hi[%s] 8\n",pCM,pCAV);
            uGo = 0;  pvPut(uGo);
	    } state WaitForButton
        }

    state AbortDelay
        {
        when (delay(1.5))
            {
            printf("---------------------------- RES[%s]_Hi[%s] 9\n",pCM,pCAV);
            uAbort = 0;  pvPut(uAbort);
	    } state WaitForButton
        }

    state ClearDelay
        {
        when (delay(1.5))
            {
            printf("---------------------------- RES[%s]_Hi[%s] A\n",pCM,pCAV);
            uClear = 0;  pvPut(uClear);
	    } state WaitForButton
        }

    }



#if 0
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
ss RES_HI_HW_ASLO
    {
    state Init
	{
	when()
	    {
	    printf("------------------------------------- HiHwLink.st HI_HW_ASLO\n");
            oldPvAslo = 0.0;
            } state WaitForChange
        }

    state WaitForChange
        {
        when (pvAslo != oldPvAslo)
            {
            printf("Change ASLO from %f to %f\n",oldPvAslo,pvAslo);

            tstphzAslo  = pvAslo;   pvPut(tstphzAslo);
            tstphzrAslo = pvAslo;   pvPut(tstphzrAslo);

            sgnlmtAslo  = pvAslo;   pvPut(sgnlmtAslo);
            sgnlmtrAslo = pvAslo;   pvPut(sgnlmtrAslo);

            abslmtAslo  = pvAslo;   pvPut(abslmtAslo);
            abslmtrAslo = pvAslo;   pvPut(abslmtrAslo);

            abssubAslo  = pvAslo;   pvPut(abssubAslo);
            abssubrAslo = pvAslo;   pvPut(abssubrAslo);

            sgnstpAslo  = pvAslo;   pvPut(sgnstpAslo);
            absstpAslo  = pvAslo;   pvPut(absstpAslo);

            tstpsAslo   = pvAslo;   pvPut(tstpsAslo);
            tstpsrAslo  = pvAslo;   pvPut(tstpsrAslo);
            tstplAslo   = pvAslo;   pvPut(tstplAslo);
            tstpmAslo   = pvAslo;   pvPut(tstpmAslo);

            tvelsAslo   = pvAslo;   pvPut(tvelsAslo);
            tvelsrAslo  = pvAslo;   pvPut(tvelsrAslo);
            tvellAslo   = pvAslo;   pvPut(tvellAslo);

            taccsAslo   = pvAslo;   pvPut(taccsAslo);
            taccsrAslo  = pvAslo;   pvPut(taccsrAslo);
            tacclAslo   = pvAslo;   pvPut(tacclAslo);

            oldPvAslo   = pvAslo;
            } state DebugDelay
        }

    state DebugDelay
        {
        when (delay(2.0))
            {
            printf("DebugDelay ........................................\n");
            } state WaitForChange
        }

    }
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#endif

