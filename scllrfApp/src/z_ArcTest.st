/* Rf12Fcc: Interface to the Old 6-GeV PVs
 * ---------------------------------------
 * File:      ArcTest.st
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
 *   seq(&ArcTest, "CM=04")
 *   seq(&ArcTest, "CM=1M")
 *   seq(&ArcTest, "CM=1N")
 *   seq(&ArcTest, "CM=1O")
 *   seq(&ArcTest, "CM=1P")
 *   seq(&ArcTest, "CM=1Q")
 *   seq(&ArcTest, "CM=2M")
 *   seq(&ArcTest, "CM=2N")
 *   seq(&ArcTest, "CM=2O")
 *   seq(&ArcTest, "CM=2P")
 *   seq(&ArcTest, "CM=2Q")
 */

program ArcTest

/* option +r;   Re-entrant code. */
option +c;   /* Wait for all database connections. */

/* System header files */
/* %% #include <math.h> */


/* INPUTS */
/* ------ */

long    burtInit;
assign  burtInit   to "R{CM}XIARCinit";
monitor burtInit;

double  period;
assign  period    to "R{CM}XIARCper";
monitor period;

int     man1;
assign  man1      to "R{CM}XATMAN.B0";
monitor man1;
int     man2;
assign  man2      to "R{CM}XATMAN.B1";
monitor man2;
int     man3;
assign  man3      to "R{CM}XATMAN.B2";
monitor man3;
int     man4;
assign  man4      to "R{CM}XATMAN.B3";
monitor man4;
int     man5;
assign  man5      to "R{CM}XATMAN.B4";
monitor man5;
int     man6;
assign  man6      to "R{CM}XATMAN.B5";
monitor man6;
int     man7;
assign  man7      to "R{CM}XATMAN.B6";
monitor man7;
int     man8;
assign  man8      to "R{CM}XATMAN.B7";
monitor man8;

int     faults;
assign  faults    to "R{CM}XCWAD";
/*                 ......................... no monitor ????? */

int     fault1;
assign  fault1    to "R{CM}XCWADb.B8";
/* monitor fault1; .................................... ????? */
int     fault2;
assign  fault2    to "R{CM}XCWADb.B9";
/* monitor fault2; .................................... ????? */
int     fault3;
assign  fault3    to "R{CM}XCWADb.BA";
/* monitor fault3; .................................... ????? */
int     fault4;
assign  fault4    to "R{CM}XCWADb.BB";
/* monitor fault4; .................................... ????? */
int     fault5;
assign  fault5    to "R{CM}XCWADb.BC";
/* monitor fault5; .................................... ????? */
int     fault6;
assign  fault6    to "R{CM}XCWADb.BD";
/* monitor fault6; .................................... ????? */
int     fault7;
assign  fault7    to "R{CM}XCWADb.BE";
/* monitor fault7; .................................... ????? */
int     fault8;
assign  fault8    to "R{CM}XCWADb.BF";
/* monitor fault8; .................................... ????? */

/* OUTPUTS */
/* ------- */

int     faultsProc;
assign  faultsProc  to "R{CM}XCWAD.PROC";

int     mask1;
assign  mask1       to "R{CM}XCWAMbits.B0";
int     mask2;
assign  mask2       to "R{CM}XCWAMbits.B1";
int     mask3;
assign  mask3       to "R{CM}XCWAMbits.B2";
int     mask4;
assign  mask4       to "R{CM}XCWAMbits.B3";
int     mask5;
assign  mask5       to "R{CM}XCWAMbits.B4";
int     mask6;
assign  mask6       to "R{CM}XCWAMbits.B5";
int     mask7;
assign  mask7       to "R{CM}XCWAMbits.B6";
int     mask8;
assign  mask8       to "R{CM}XCWAMbits.B7";

int     test1;
assign  test1       to "R{CM}XCWATbits.B0";
int     test2;
assign  test2       to "R{CM}XCWATbits.B1";
int     test3;
assign  test3       to "R{CM}XCWATbits.B2";
int     test4;
assign  test4       to "R{CM}XCWATbits.B3";
int     test5;
assign  test5       to "R{CM}XCWATbits.B4";
int     test6;
assign  test6       to "R{CM}XCWATbits.B5";
int     test7;
assign  test7       to "R{CM}XCWATbits.B6";
int     test8;
assign  test8       to "R{CM}XCWATbits.B7";

int     result1;
assign  result1     to "R{CM}XATFLTb.B0";
int     result2;
assign  result2     to "R{CM}XATFLTb.B1";
int     result3;
assign  result3     to "R{CM}XATFLTb.B2";
int     result4;
assign  result4     to "R{CM}XATFLTb.B3";
int     result5;
assign  result5     to "R{CM}XATFLTb.B4";
int     result6;
assign  result6     to "R{CM}XATFLTb.B5";
int     result7;
assign  result7     to "R{CM}XATFLTb.B6";
int     result8;
assign  result8     to "R{CM}XATFLTb.B7";

int     clear1;
assign  clear1      to "R{CM}XCWAFCbits.B0";
int     clear2;
assign  clear2      to "R{CM}XCWAFCbits.B1";
int     clear3;
assign  clear3      to "R{CM}XCWAFCbits.B2";
int     clear4;
assign  clear4      to "R{CM}XCWAFCbits.B3";
int     clear5;
assign  clear5      to "R{CM}XCWAFCbits.B4";
int     clear6;
assign  clear6      to "R{CM}XCWAFCbits.B5";
int     clear7;
assign  clear7      to "R{CM}XCWAFCbits.B6";
int     clear8;
assign  clear8      to "R{CM}XCWAFCbits.B7";

long    nowCav;     /* 0x01 0x02 0x04 0x08 0x10 0x20 0x40 0x80 */
assign  nowCav      to "R{CM}XIARCrun";

/* MISCELLANEOUS */
/* ------------- */

int     presentAutoCavity;
int     xxx;       /* Used with burtInit */
int     firstTime;
int     autoMode;
int     tries;

/* ====================================================== */

ss ARC_TEST
    {
    state Init
        {
        when ()
            {
            printf("....................................... ArcTest.st\n");
            man1 = 0;  pvPut(man1);
            man2 = 0;  pvPut(man2);
            man3 = 0;  pvPut(man3);
            man4 = 0;  pvPut(man4);
            man5 = 0;  pvPut(man5);
            man6 = 0;  pvPut(man6);
            man7 = 0;  pvPut(man7);
            man8 = 0;  pvPut(man8);
            presentAutoCavity = 1;
            period = 450.0; pvPut(period);
            result1 = 1; pvPut(result1);    /* Set all 8 results to fail. */
            result2 = 1; pvPut(result2);
            result3 = 1; pvPut(result3);
            result4 = 1; pvPut(result4);
            result5 = 1; pvPut(result5);
            result6 = 1; pvPut(result6);
            result7 = 1; pvPut(result7);
            result8 = 1; pvPut(result8);
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
        when (xxx == 5)
            {
            } state Cavity5
        when (xxx == 6)
            {
            } state Cavity6
        when (xxx == 7)
            {
            } state Cavity7
        when (xxx == 8)
            {
            } state Cavity8

        when (xxx >= 9)
            {
            firstTime = 0;
            xxx = 0;
            burtInit = 0;  pvPut(burtInit);
            man1 = 0;  pvPut(man1);
            man2 = 0;  pvPut(man2);
            man3 = 0;  pvPut(man3);
            man4 = 0;  pvPut(man4);
            man5 = 0;  pvPut(man5);
            man6 = 0;  pvPut(man6);
            man7 = 0;  pvPut(man7);
            man8 = 0;  pvPut(man8);
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
        when (man5)
            {
            printf("Manual test - cavity 5\n");
            } state Cavity5
        when (man6)
            {
            printf("Manual test - cavity 6\n");
            } state Cavity6
        when (man7)
            {
            printf("Manual test - cavity 7\n");
            } state Cavity7
        when (man8)
            {
            printf("Manual test - cavity 8\n");
            } state Cavity8

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
        when (presentAutoCavity == 5)
            {
            autoMode = 1;
            } state Cavity5
        when (presentAutoCavity == 6)
            {
            autoMode = 1;
            } state Cavity6
        when (presentAutoCavity == 7)
            {
            autoMode = 1;
            } state Cavity7
        when (presentAutoCavity == 8)
            {
            autoMode = 1;
            } state Cavity8
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
    state Cavity5
        {
        when ()
            {
            tries = 1;
            } state Cavity5Loop
        }
    state Cavity5Loop
        {
        when ()
            {
            /* Read the fault register. */
            faultsProc = 1;   pvPut(faultsProc);
            pvGet(fault5);
            if (firstTime)
                fault5 = 0;  /* Always test for Burt init. */
            } state Cavity5a
        }
    state Cavity5a
        {
        when (fault5)
            {
            /* This is an actual fault, so do not test. */
            result5 = 0;
            pvPut(result5);
            } state Done
        when (!fault5)
            {
            /* There is no fault at this time, so do the test. */
            nowCav = 0x10;    pvPut(nowCav);
            mask5   = 1;      pvPut(mask5);
            test5   = 1;      pvPut(test5);
            } state Cavity5b
        }
    state Cavity5b
        {
        when (delay(0.001))
            {
            test5   = 0;      pvPut(test5);
            faultsProc = 1;   pvPut(faultsProc);
            pvGet(fault5);
            %% result5 = fault5 ? 0 : 1;
            clear5  = 1;      pvPut(clear5);
            clear5  = 0;      pvPut(clear5);
            mask5   = 0;      pvPut(mask5);
            } state Cavity5c
        }
    state Cavity5c
        {
        when (result5 == 0)
            {
            /* Passed test result. Done. */
            pvPut(result5);
            } state Done
        when (result5 != 0 && tries >= 3)
            {
            /* Three failed test result. Done. */
            pvPut(result5);
            } state Done
        when (result5 != 0 && tries < 3)
            {
            /* Failed test result, try again. */
            ++tries;
            } state Cavity5Loop
        }
/*--------------------------------------------------*/
    state Cavity6
        {
        when ()
            {
            tries = 1;
            } state Cavity6Loop
        }
    state Cavity6Loop
        {
        when ()
            {
            /* Read the fault register. */
            faultsProc = 1;   pvPut(faultsProc);
            pvGet(fault6);
            if (firstTime)
                fault6 = 0;  /* Always test for Burt init. */
            } state Cavity6a
        }
    state Cavity6a
        {
        when (fault6)
            {
            /* This is an actual fault, so do not test. */
            result6 = 0;
            pvPut(result6);
            } state Done
        when (!fault6)
            {
            /* There is no fault at this time, so do the test. */
            nowCav = 0x20;    pvPut(nowCav);
            mask6   = 1;      pvPut(mask6);
            test6   = 1;      pvPut(test6);
            } state Cavity6b
        }
    state Cavity6b
        {
        when (delay(0.001))
            {
            test6   = 0;      pvPut(test6);
            faultsProc = 1;   pvPut(faultsProc);
            pvGet(fault6);
            %% result6 = fault6 ? 0 : 1;
            clear6  = 1;      pvPut(clear6);
            clear6  = 0;      pvPut(clear6);
            mask6   = 0;      pvPut(mask6);
            } state Cavity6c
        }
    state Cavity6c
        {
        when (result6 == 0)
            {
            /* Passed test result. Done. */
            pvPut(result6);
            } state Done
        when (result6 != 0 && tries >= 3)
            {
            /* Three failed test result. Done. */
            pvPut(result6);
            } state Done
        when (result6 != 0 && tries < 3)
            {
            /* Failed test result, try again. */
            ++tries;
            } state Cavity6Loop
        }
/*--------------------------------------------------*/
    state Cavity7
        {
        when ()
            {
            tries = 1;
            } state Cavity7Loop
        }
    state Cavity7Loop
        {
        when ()
            {
            /* Read the fault register. */
            faultsProc = 1;   pvPut(faultsProc);
            pvGet(fault7);
            if (firstTime)
                fault7 = 0;  /* Always test for Burt init. */
            } state Cavity7a
        }
    state Cavity7a
        {
        when (fault7)
            {
            /* This is an actual fault, so do not test. */
            result7 = 0;
            pvPut(result7);
            } state Done
        when (!fault7)
            {
            /* There is no fault at this time, so do the test. */
            nowCav = 0x40;    pvPut(nowCav);
            mask7   = 1;      pvPut(mask7);
            test7   = 1;      pvPut(test7);
            } state Cavity7b
        }
    state Cavity7b
        {
        when (delay(0.001))
            {
            test7   = 0;      pvPut(test7);
            faultsProc = 1;   pvPut(faultsProc);
            pvGet(fault7);
            %% result7 = fault7 ? 0 : 1;
            clear7  = 1;      pvPut(clear7);
            clear7  = 0;      pvPut(clear7);
            mask7   = 0;      pvPut(mask7);
            } state Cavity7c
        }
    state Cavity7c
        {
        when (result7 == 0)
            {
            /* Passed test result. Done. */
            pvPut(result7);
            } state Done
        when (result7 != 0 && tries >= 3)
            {
            /* Three failed test result. Done. */
            pvPut(result7);
            } state Done
        when (result7 != 0 && tries < 3)
            {
            /* Failed test result, try again. */
            ++tries;
            } state Cavity7Loop
        }
/*--------------------------------------------------*/
    state Cavity8
        {
        when ()
            {
            tries = 1;
            } state Cavity8Loop
        }
    state Cavity8Loop
        {
        when ()
            {
            /* Read the fault register. */
            faultsProc = 1;   pvPut(faultsProc);
            pvGet(fault8);
            if (firstTime)
                fault8 = 0;  /* Always test for Burt init. */
            } state Cavity8a
        }
    state Cavity8a
        {
        when (fault8)
            {
            /* This is an actual fault, so do not test. */
            result8 = 0;
            pvPut(result8);
            } state Done
        when (!fault8)
            {
            /* There is no fault at this time, so do the test. */
            nowCav = 0x80;    pvPut(nowCav);
            mask8   = 1;      pvPut(mask8);
            test8   = 1;      pvPut(test8);
            } state Cavity8b
        }
    state Cavity8b
        {
        when (delay(0.001))
            {
            test8   = 0;      pvPut(test8);
            faultsProc = 1;   pvPut(faultsProc);
            pvGet(fault8);
            %% result8 = fault8 ? 0 : 1;
            clear8  = 1;      pvPut(clear8);
            clear8  = 0;      pvPut(clear8);
            mask8   = 0;      pvPut(mask8);
            } state Cavity8c
        }
    state Cavity8c
        {
        when (result8 == 0)
            {
            /* Passed test result. Done. */
            pvPut(result8);
            } state Done
        when (result8 != 0 && tries >= 3)
            {
            /* Three failed test result. Done. */
            pvPut(result8);
            } state Done
        when (result8 != 0 && tries < 3)
            {
            /* Failed test result, try again. */
            ++tries;
            } state Cavity8Loop
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
            man5 = 0;  pvPut(man5);
            man6 = 0;  pvPut(man6);
            man7 = 0;  pvPut(man7);
            man8 = 0;  pvPut(man8);
            } state WaitForButtons
        }

    }

