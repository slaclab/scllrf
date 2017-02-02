@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@ not needed any more [gel:08oct2013] @@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/* Rf12Fcc: Epics Quench Detector
 * ------------------------------
 * File:      Quench2.st
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
 *   seq(&Quench2, "CM=2O,CAV=1")
 */

program Quench2

/* option +r;   Re-entrant code. */
option +c;   /* Wait for all database connections. */

/* System header files */
%% #include <math.h>

/* INPUTS */
/* ------ */

double  gmes;
assign  gmes    to "R{CM}{CAV}GMESF";
monitor gmes;

double  crfp;
assign  crfp    to "R{CM}{CAV}CRFP";
monitor crfp;

double  run;
assign  run     to "R{CM}{CAV}Q2run";
monitor run;

double  dly;
assign  dly     to "R{CM}{CAV}Q2dly";
monitor dly;

double  elim;
assign  elim    to "R{CM}{CAV}Q2elim";  /*  */
monitor elim;

double  rlim;
assign  rlim    to "R{CM}{CAV}Q2rlim";  /*  */
monitor rlim;

/* OUTPUTS */
/* ------- */

int     rfOff;
assign  rfOff   to "R{CM}{CAV}CNTL2b.I";

double  q2trip;
assign  q2trip  to "R{CM}{CAV}Q2trip";  /*  */

/* MISCELLANEOUS */
/* ------------- */

int count;
double gsum;
double csum;
double gavg;
double cavg;

/* ====================================================== */

ss QUENCH_TWO
    {
    state Init
        {
        when ()
            {
            printf("....................................... Quench2.st\n");
            q2trip = 0.0;
            pvPut(q2trip);
            } state Inactive
        }

    state Inactive
        {
        when (run != 0.0)
            {
            gsum = 0.0;
            csum = 0.0;
            count = 0;
            q2trip = 0.0;
            pvPut(q2trip);
            } state Active
        }

    state Active
        {
        when (run == 0.0)
            {
            } state Inactive
        when ()
            {
            gsum += gmes;
            csum += crfp;
            if (++count >= 5)
                {
                count = 0;
                gavg = gsum / 5.0;
                if (gavg >= elim)
                    {
                    cavg = csum / 5.0;
                    if ((gavg / cavg) < rlim)
                        {
                        rfOff = 0;
                        pvPut(rfOff);
                        q2trip = 1.0;
                        pvPut(q2trip);
                        }
                    }
                gsum = 0.0;
                csum = 0.0;
                }
            } state DelaySome
        }

    state DelaySome
        {
        when (q2trip != 0)
            {
            run = 0.0;
            pvPut(run);
            } state Inactive
        when (delay(dly))
            {
            } state Active
        }

    }

