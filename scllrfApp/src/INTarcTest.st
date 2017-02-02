/* Rf12Fcc: Interface to the Old 6-GeV PVs
 * ---------------------------------------
 * File:      INTarcTest.st
 * Author:    George Lahti (gel) x7272
 * Date:      21 Aug 2011
 * Revise: 
 * Type:      Epics Sequencer
 * See:       rf6zone.sch rf6cavity.sch
 *
 * REQUIREMENTS
 *
 * DESCRIPTION
 *
 *
 * USAGE
 *   seq(&INTarcTest, "PREFX=INT1")
 *   seq(&INTarcTest, "PREFX=INT2")
 */

program INTarcTest

/* option +r;   Re-entrant code. */
option +c;   /* Wait for all database connections. */

/* System header files */
/* %% #include <math.h> */


/* INPUTS */
/* ------ */

int    burtInit;
assign  burtInit   to "{PREFX}:RXIARCinit";
monitor burtInit;

double  period;
assign  period    to "{PREFX}:RXIARCper";
monitor period;

int     man1;
assign  man1      to "{PREFX}:RXATMAN.B0";
monitor man1;
int     man2;
assign  man2      to "{PREFX}:RXATMAN.B1";
monitor man2;
int     man3;
assign  man3      to "{PREFX}:RXATMAN.B2";
monitor man3;
int     man4;
assign  man4      to "{PREFX}:RXATMAN.B3";
monitor man4;

int     faults;
assign  faults    to "{PREFX}:RXCWAD_R";

int     fault1;
assign  fault1    to "{PREFX}:RXCWADb.B4";
int     fault2;
assign  fault2    to "{PREFX}:RXCWADb.B5";
int     fault3;
assign  fault3    to "{PREFX}:RXCWADb.B6";
int     fault4;
assign  fault4    to "{PREFX}:RXCWADb.B7";

/* OUTPUTS */
/* ------- */

int     faultsProc;
assign  faultsProc  to "{PREFX}:RXCWAD_R.PROC";

int     mask1;
assign  mask1       to "{PREFX}:RXCWAMb.B0";
int     mask2;
assign  mask2       to "{PREFX}:RXCWAMb.B1";
int     mask3;
assign  mask3       to "{PREFX}:RXCWAMb.B2";
int     mask4;
assign  mask4       to "{PREFX}:RXCWAMb.B3";

int     test1;
assign  test1       to "{PREFX}:RXCWATb.B0";
int     test2;
assign  test2       to "{PREFX}:RXCWATb.B1";
int     test3;
assign  test3       to "{PREFX}:RXCWATb.B2";
int     test4;
assign  test4       to "{PREFX}:RXCWATb.B3";

int     result1;
assign  result1     to "{PREFX}:RXATFLTb.B0";
int     result2;
assign  result2     to "{PREFX}:RXATFLTb.B1";
int     result3;
assign  result3     to "{PREFX}:RXATFLTb.B2";
int     result4;
assign  result4     to "{PREFX}:RXATFLTb.B3";

int     clear1;
assign  clear1      to "{PREFX}:RXCWAFCb.B0";
int     clear2;
assign  clear2      to "{PREFX}:RXCWAFCb.B1";
int     clear3;
assign  clear3      to "{PREFX}:RXCWAFCb.B2";
int     clear4;
assign  clear4      to "{PREFX}:RXCWAFCb.B3";

int    nowCav;     /* 0x01 0x02 0x04 0x08 0x10 0x20 0x40 0x80 */
assign  nowCav      to "{PREFX}:RXIARCrun";

/* MISCELLANEOUS */
/* ------------- */

int     presentAutoCavity;
int     xxx;       /* Used with burtInit */
int     firstTime;
int     autoMode;
int     tries;

/* ====================================================== */

ss INT_ARC_TEST
    {
    state Init
        {
        when ()
            {
            printf("....................................... INTarcTest.st\n");
            man1 = 0;  pvPut(man1);
            man2 = 0;  pvPut(man2);
            man3 = 0;  pvPut(man3);
            man4 = 0;  pvPut(man4);
            presentAutoCavity = 1;
            period = 450.0; pvPut(period);
            result1 = 1; pvPut(result1);    /* Set all 8 results to fail. */
            result2 = 1; pvPut(result2);
            result3 = 1; pvPut(result3);
            result4 = 1; pvPut(result4);
            xxx = 1;
            firstTime = 1;
            autoMode = 0;
            } state WaitForBurt
        }

    state WaitForBurt
        {
        when (xxx == 1 && burtInit == 1)
            {
            } state Cavity1
        when (xxx == 2)
            {
            } state Cavity2
        when (xxx == 3)
            {
            } state Cavity3
        when (xxx == 4)
            {
            } state Cavity4

        when (xxx >= 5)   /* Yes 5; one more than last cavity. */
            {
            firstTime = 0;
            xxx = 0;
            burtInit = 0;  pvPut(burtInit);
            man1 = 0;  pvPut(man1);
            man2 = 0;  pvPut(man2);
            man3 = 0;  pvPut(man3);
            man4 = 0;  pvPut(man4);
            } state WaitForButtons
        }

    state WaitForButtons
        {
        when (man1)
            {
            printf("Manual test - cavity 1\n");
            } state Cavity1
        when (man2)
            {
            printf("Manual test - cavity 2\n");
            } state Cavity2
        when (man3)
            {
            printf("Manual test - cavity 3\n");
            } state Cavity3
        when (man4)
            {
            printf("Manual test - cavity 4\n");
            } state Cavity4

        when (delay(period))
            {
            } state NextPeriodicCavity
        }

    state NextPeriodicCavity
        {
        when ()
            {
            presentAutoCavity += 1;
            if (presentAutoCavity > 8)
                presentAutoCavity = 1;
            } state SelectCavity
        }

    state SelectCavity
        {
        when (presentAutoCavity == 1)
            {
            autoMode = 1;
            } state Cavity1
        when (presentAutoCavity == 2)
            {
            autoMode = 1;
            } state Cavity2
        when (presentAutoCavity == 3)
            {
            autoMode = 1;
            } state Cavity3
        when (presentAutoCavity == 4)
            {
            autoMode = 1;
            } state Cavity4
        when ()
            {
            printf("presentAutoCavity OOPS (%d) .............\n",presentAutoCavity);
            presentAutoCavity = 1;
            } state Done
        }


/*--------------------------------------------------*/
    state Cavity1
        {
        when ()
            {
            tries = 1;
            } state Cavity1Loop
        }
    state Cavity1Loop
        {
        when ()
            {
            /* Read the fault register. */
            faultsProc = 1;   pvPut(faultsProc);
            pvGet(fault1);
            if (firstTime)
                fault1 = 0;  /* Always test for Burt init. */
            } state Cavity1a
        }
    state Cavity1a
        {
        when (fault1)
            {
            /* This is an actual fault, so do not test. */
            result1 = 0;
            pvPut(result1);
            } state Done
        when (!fault1)
            {
            /* There is no fault at this time, so do the test. */
            nowCav = 0x01;    pvPut(nowCav);
            mask1   = 1;      pvPut(mask1);
            test1   = 1;      pvPut(test1);
            } state Cavity1b
        }
    state Cavity1b
        {
        when (delay(0.001))
            {
            test1   = 0;      pvPut(test1);
            faultsProc = 1;   pvPut(faultsProc);
            pvGet(fault1);
            %% result1 = fault1 ? 0 : 1;
            clear1  = 1;      pvPut(clear1);
            clear1  = 0;      pvPut(clear1);
            mask1   = 0;      pvPut(mask1);
            } state Cavity1c
        }
    state Cavity1c
        {
        when (result1 == 0)
            {
            /* Passed test result. Done. */
            pvPut(result1);
            } state Done
        when (result1 != 0 && tries >= 3)
            {
            /* Three failed test result. Done. */
            pvPut(result1);
            } state Done
        when (result1 != 0 && tries < 3)
            {
            /* Failed test result, try again. */
            ++tries;
            } state Cavity1Loop
        }
/*--------------------------------------------------*/
    state Cavity2
        {
        when ()
            {
            tries = 1;
            } state Cavity2Loop
        }
    state Cavity2Loop
        {
        when ()
            {
            /* Read the fault register. */
            faultsProc = 1;   pvPut(faultsProc);
            pvGet(fault2);
            if (firstTime)
                fault2 = 0;  /* Always test for Burt init. */
            } state Cavity2a
        }
    state Cavity2a
        {
        when (fault2)
            {
            /* This is an actual fault, so do not test. */
            result2 = 0;
            pvPut(result2);
            } state Done
        when (!fault2)
            {
            /* There is no fault at this time, so do the test. */
            nowCav = 0x02;    pvPut(nowCav);
            mask2   = 1;      pvPut(mask2);
            test2   = 1;      pvPut(test2);
            } state Cavity2b
        }
    state Cavity2b
        {
        when (delay(0.001))
            {
            test2   = 0;      pvPut(test2);
            faultsProc = 1;   pvPut(faultsProc);
            pvGet(fault2);
            %% result2 = fault2 ? 0 : 1;
            clear2  = 1;      pvPut(clear2);
            clear2  = 0;      pvPut(clear2);
            mask2   = 0;      pvPut(mask2);
            } state Cavity2c
        }
    state Cavity2c
        {
        when (result2 == 0)
            {
            /* Passed test result. Done. */
            pvPut(result2);
            } state Done
        when (result2 != 0 && tries >= 3)
            {
            /* Three failed test result. Done. */
            pvPut(result2);
            } state Done
        when (result2 != 0 && tries < 3)
            {
            /* Failed test result, try again. */
            ++tries;
            } state Cavity2Loop
        }
/*--------------------------------------------------*/
    state Cavity3
        {
        when ()
            {
            tries = 1;
            } state Cavity3Loop
        }
    state Cavity3Loop
        {
        when ()
            {
            /* Read the fault register. */
            faultsProc = 1;   pvPut(faultsProc);
            pvGet(fault3);
            if (firstTime)
                fault3 = 0;  /* Always test for Burt init. */
            } state Cavity3a
        }
    state Cavity3a
        {
        when (fault3)
            {
            /* This is an actual fault, so do not test. */
            result3 = 0;
            pvPut(result3);
            } state Done
        when (!fault3)
            {
            /* There is no fault at this time, so do the test. */
            nowCav = 0x04;    pvPut(nowCav);
            mask3   = 1;      pvPut(mask3);
            test3   = 1;      pvPut(test3);
            } state Cavity3b
        }
    state Cavity3b
        {
        when (delay(0.001))
            {
            test3   = 0;      pvPut(test3);
            faultsProc = 1;   pvPut(faultsProc);
            pvGet(fault3);
            %% result3 = fault3 ? 0 : 1;
            clear3  = 1;      pvPut(clear3);
            clear3  = 0;      pvPut(clear3);
            mask3   = 0;      pvPut(mask3);
            } state Cavity3c
        }
    state Cavity3c
        {
        when (result3 == 0)
            {
            /* Passed test result. Done. */
            pvPut(result3);
            } state Done
        when (result3 != 0 && tries >= 3)
            {
            /* Three failed test result. Done. */
            pvPut(result3);
            } state Done
        when (result3 != 0 && tries < 3)
            {
            /* Failed test result, try again. */
            ++tries;
            } state Cavity3Loop
        }
/*--------------------------------------------------*/
    state Cavity4
        {
        when ()
            {
            tries = 1;
            } state Cavity4Loop
        }
    state Cavity4Loop
        {
        when ()
            {
            /* Read the fault register. */
            faultsProc = 1;   pvPut(faultsProc);
            pvGet(fault4);
            if (firstTime)
                fault4 = 0;  /* Always test for Burt init. */
            } state Cavity4a
        }
    state Cavity4a
        {
        when (fault4)
            {
            /* This is an actual fault, so do not test. */
            result4 = 0;
            pvPut(result4);
            } state Done
        when (!fault4)
            {
            /* There is no fault at this time, so do the test. */
            nowCav = 0x08;    pvPut(nowCav);
            mask4   = 1;      pvPut(mask4);
            test4   = 1;      pvPut(test4);
            } state Cavity4b
        }
    state Cavity4b
        {
        when (delay(0.001))
            {
            test4   = 0;      pvPut(test4);
            faultsProc = 1;   pvPut(faultsProc);
            pvGet(fault4);
            %% result4 = fault4 ? 0 : 1;
            clear4  = 1;      pvPut(clear4);
            clear4  = 0;      pvPut(clear4);
            mask4   = 0;      pvPut(mask4);
            } state Cavity4c
        }
    state Cavity4c
        {
        when (result4 == 0)
            {
            /* Passed test result. Done. */
            pvPut(result4);
            } state Done
        when (result4 != 0 && tries >= 3)
            {
            /* Three failed test result. Done. */
            pvPut(result4);
            } state Done
        when (result4 != 0 && tries < 3)
            {
            /* Failed test result, try again. */
            ++tries;
            } state Cavity4Loop
        }
/*--------------------------------------------------*/

    state Done
        {
        when (autoMode)
            {
            autoMode = 0;
            } state Done2
        when (delay(3.0))
            {
            } state Done2
        }

    state Done2
        {
        when (firstTime)
            {
            xxx += 1;
            } state WaitForBurt

        when (!firstTime)
            {
            /* printf("Done period=%f presentAutoCavity=%d\n",period,presentAutoCavity); */
            burtInit = 0;  pvPut(burtInit);
            man1 = 0;  pvPut(man1);
            man2 = 0;  pvPut(man2);
            man3 = 0;  pvPut(man3);
            man4 = 0;  pvPut(man4);
            } state WaitForButtons
        }

    }

