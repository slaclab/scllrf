/* Rf12Fcc: Epics Quench Detector
 * ------------------------------
 * File:      INTmbboCopy.st
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
 *   seq(&INTmbboCopy, "PREFX=INT1")
 *   seq(&INTmbboCopy, "PREFX=INT2")
 */

program INTmbboCopy

/* option +r;   Re-entrant code. */
option +c;   /* Wait for all database connections. */

/* System header files */
%% #include <math.h>


/* INPUTS */
/* ------ */

double  mbbo_RXCWAM;
assign  mbbo_RXCWAM          to "{PREFX}:RXCWAMb";
monitor mbbo_RXCWAM;

double  mbbo_RXCWAT;
assign  mbbo_RXCWAT          to "{PREFX}:RXCWATb";
monitor mbbo_RXCWAT;

double  mbbo_RXATFLT;
assign  mbbo_RXATFLT          to "{PREFX}:RXATFLTb";
monitor mbbo_RXATFLT;

double  mbbo_RXCWAFC;
assign  mbbo_RXCWAFC          to "{PREFX}:RXCWAFCb";
monitor mbbo_RXCWAFC;

double  mbbo_RXCWWTS;
assign  mbbo_RXCWWTS          to "{PREFX}:RXCWWTSb";
monitor mbbo_RXCWWTS;

double  mbbo_RXCWWM;
assign  mbbo_RXCWWM          to "{PREFX}:RXCWWMb";
monitor mbbo_RXCWWM;

double  mbbo_RXCWWFC;
assign  mbbo_RXCWWFC          to "{PREFX}:RXCWWFCb";
monitor mbbo_RXCWWFC;

double  mbbo_RXSTMPM;
assign  mbbo_RXSTMPM          to "{PREFX}:RXSTMPMb";
monitor mbbo_RXSTMPM;

double  mbbo_RXSTMPFC;
assign  mbbo_RXSTMPFC          to "{PREFX}:RXSTMPFCb";
monitor mbbo_RXSTMPFC;

double  mbbo_RXCPLTMPM;
assign  mbbo_RXCPLTMPM          to "{PREFX}:RXCPLTMPMb";
monitor mbbo_RXCPLTMPM;

double  mbbo_RXCPLTMPFC;
assign  mbbo_RXCPLTMPFC          to "{PREFX}:RXCPLTMPFCb";
monitor mbbo_RXCPLTMPFC;

double  mbbo_RXCPLFEPM;
assign  mbbo_RXCPLFEPM          to "{PREFX}:RXCPLFEPMb";
monitor mbbo_RXCPLFEPM;

double  mbbo_RXCPLFEPFC;
assign  mbbo_RXCPLFEPFC          to "{PREFX}:RXCPLFEPFCb";
monitor mbbo_RXCPLFEPFC;

double  mbbo_RXCVFM;
assign  mbbo_RXCVFM          to "{PREFX}:RXCVFMb";
monitor mbbo_RXCVFM;

double  mbbo_RXCVFC;
assign  mbbo_RXCVFC          to "{PREFX}:RXCVFCb";
monitor mbbo_RXCVFC;

double  mbbo_RXCIENM;
assign  mbbo_RXCIENM          to "{PREFX}:RXCIENMb";
monitor mbbo_RXCIENM;

double  mbbo_RXCIENC;
assign  mbbo_RXCIENC          to "{PREFX}:RXCIENCb";
monitor mbbo_RXCIENC;

double  mbbo_RXFFSDM;
assign  mbbo_RXFFSDM          to "{PREFX}:RXFFSDMb";
monitor mbbo_RXFFSDM;

double  mbbo_RXFFSDC;
assign  mbbo_RXFFSDC          to "{PREFX}:RXFFSDCb";
monitor mbbo_RXFFSDC;

double  mbbo_RXICTL;
assign  mbbo_RXICTL          to "{PREFX}:RXICTLb";
monitor mbbo_RXICTL;

/* OUTPUTS */
/* ------- */

double  ao_RXCWAM;
assign  ao_RXCWAM          to "{PREFX}:RXCWAM_W";
monitor ao_RXCWAM;

double  ao_RXCWAT;
assign  ao_RXCWAT          to "{PREFX}:RXCWAT_W";
monitor ao_RXCWAT;

double  ao_RXATFLT;
assign  ao_RXATFLT          to "{PREFX}:RXATFLT_W";
monitor ao_RXATFLT;

double  ao_RXCWAFC;
assign  ao_RXCWAFC          to "{PREFX}:RXCWAFC_W";
monitor ao_RXCWAFC;

double  ao_RXCWWTS;
assign  ao_RXCWWTS          to "{PREFX}:RXCWWTS_W";
monitor ao_RXCWWTS;

double  ao_RXCWWM;
assign  ao_RXCWWM          to "{PREFX}:RXCWWM_W";
monitor ao_RXCWWM;

double  ao_RXCWWFC;
assign  ao_RXCWWFC          to "{PREFX}:RXCWWFC_W";
monitor ao_RXCWWFC;

double  ao_RXSTMPM;
assign  ao_RXSTMPM          to "{PREFX}:RXSTMPM_W";
monitor ao_RXSTMPM;

double  ao_RXSTMPFC;
assign  ao_RXSTMPFC          to "{PREFX}:RXSTMPFC_W";
monitor ao_RXSTMPFC;

double  ao_RXCPLTMPM;
assign  ao_RXCPLTMPM          to "{PREFX}:RXCPLTMPM_W";
monitor ao_RXCPLTMPM;

double  ao_RXCPLTMPFC;
assign  ao_RXCPLTMPFC          to "{PREFX}:RXCPLTMPFC_W";
monitor ao_RXCPLTMPFC;

double  ao_RXCPLFEPM;
assign  ao_RXCPLFEPM          to "{PREFX}:RXCPLFEPM_W";
monitor ao_RXCPLFEPM;

double  ao_RXCPLFEPFC;
assign  ao_RXCPLFEPFC          to "{PREFX}:RXCPLFEPFC_W";
monitor ao_RXCPLFEPFC;

double  ao_RXCVFM;
assign  ao_RXCVFM          to "{PREFX}:RXCVFM_W";
monitor ao_RXCVFM;

double  ao_RXCVFC;
assign  ao_RXCVFC          to "{PREFX}:RXCVFC_W";
monitor ao_RXCVFC;

double  ao_RXCIENM;
assign  ao_RXCIENM          to "{PREFX}:RXCIENM_W";
monitor ao_RXCIENM;

double  ao_RXCIENC;
assign  ao_RXCIENC          to "{PREFX}:RXCIENC_W";
monitor ao_RXCIENC;

double  ao_RXFFSDM;
assign  ao_RXFFSDM          to "{PREFX}:RXFFSDM_W";
monitor ao_RXFFSDM;

double  ao_RXFFSDC;
assign  ao_RXFFSDC          to "{PREFX}:RXFFSDC_W";
monitor ao_RXFFSDC;

double  ao_RXICTL;
assign  ao_RXICTL          to "{PREFX}:RXICTL_W";
monitor ao_RXICTL;

/* MISCELLANEOUS */
/* ------------- */

/* ====================================================== */

ss INT_MBBO_COPY
    {
    state Init
        {
        when ()
            {
            printf("....................................... INTmbboCopy.st\n");
            } state Step1
        }

    state Step1
        {
        when (delay(1.0))
            {
            ao_RXCWAM = mbbo_RXCWAM;          pvPut(ao_RXCWAM);
            ao_RXCWAT = mbbo_RXCWAT;          pvPut(ao_RXCWAT);
            ao_RXATFLT = mbbo_RXATFLT;        pvPut(ao_RXATFLT);
            ao_RXCWAFC = mbbo_RXCWAFC;        pvPut(ao_RXCWAFC);
            ao_RXCWWTS = mbbo_RXCWWTS;        pvPut(ao_RXCWWTS);
            ao_RXCWWM = mbbo_RXCWWM;          pvPut(ao_RXCWWM);
            ao_RXCWWFC = mbbo_RXCWWFC;        pvPut(ao_RXCWWFC);
            ao_RXSTMPM = mbbo_RXSTMPM;        pvPut(ao_RXSTMPM);
            ao_RXSTMPFC = mbbo_RXSTMPFC;      pvPut(ao_RXSTMPFC);
            ao_RXCPLTMPM = mbbo_RXCPLTMPM;    pvPut(ao_RXCPLTMPM);
            ao_RXCPLTMPFC = mbbo_RXCPLTMPFC;  pvPut(ao_RXCPLTMPFC);
            ao_RXCPLFEPM = mbbo_RXCPLFEPM;    pvPut(ao_RXCPLFEPM);
            ao_RXCPLFEPFC = mbbo_RXCPLFEPFC;  pvPut(ao_RXCPLFEPFC);
            ao_RXCVFM = mbbo_RXCVFM;          pvPut(ao_RXCVFM);
            ao_RXCVFC = mbbo_RXCVFC;          pvPut(ao_RXCVFC);
            ao_RXCIENM = mbbo_RXCIENM;        pvPut(ao_RXCIENM);
            ao_RXCIENC = mbbo_RXCIENC;        pvPut(ao_RXCIENC);
            ao_RXFFSDM = mbbo_RXFFSDM;        pvPut(ao_RXFFSDM);
            ao_RXFFSDC = mbbo_RXFFSDC;        pvPut(ao_RXFFSDC);
            ao_RXICTL = mbbo_RXICTL;          pvPut(ao_RXICTL);
            } state Step1
        }
    }

