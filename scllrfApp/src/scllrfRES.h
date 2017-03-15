#ifndef SCLLRFRES_DRIVER_H
#define SCLLRFRES_DRIVER_H

/**
 *-----------------------------------------------------------------------------
 * Title      : superconducting low level RF EPICS interface
 * ----------------------------------------------------------------------------
 * File       : scllrfRESDriver.h
 * Author     : Garth Brown, gwbrown@slac.stanford.edu
 * Created    : June 17, 2016
 * Last update: September 6, 2016
 * ----------------------------------------------------------------------------
 * Description:
 * Header defining register interface to scllrfRES, autogenerated from a python
 * dictionary.
 *
 * ----------------------------------------------------------------------------
 * This file is part of LCLS II. It is subject to
 * the license terms in the LICENSE.txt file found in the top-level directory
 * of this distribution and at:
    * https://confluence.slac.stanford.edu/display/ppareg/LICENSE.html.
 * No part of LCLS II, including this file, may be
 * copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE.txt file.
 * ----------------------------------------------------------------------------
**/
#include "scllrfAsynPortDriver.h"

/* Registers */
static const char *Cavity1DetuneErrRString = "CAVITY1_DETUNE_ERR_R";
static const char *Cavity2DetuneErrRString = "CAVITY2_DETUNE_ERR_R";
static const char *Cavity3DetuneErrRString = "CAVITY3_DETUNE_ERR_R";
static const char *Cavity4DetuneErrRString = "CAVITY4_DETUNE_ERR_R";
static const char *Motor1AbsStepsRString = "MOTOR1_ABS_STEPS_R";
static const char *Motor1AccRString = "MOTOR1_ACC_R";
static const char *Motor1AccWString = "MOTOR1_ACC_W";
static const char *Motor1BrdTmpRString = "MOTOR1_BRD_TMP_R";
static const char *Motor1CntlRString = "MOTOR1_CNTL_R";
static const char *Motor1CntlWString = "MOTOR1_CNTL_W";
static const char *Motor1DrviRString = "MOTOR1_DRVI_R";
static const char *Motor1DrviWString = "MOTOR1_DRVI_W";
static const char *Motor1LaccRString = "MOTOR1_LACC_R";
static const char *Motor1LstepsRString = "MOTOR1_LSTEPS_R";
static const char *Motor1LvlctyRString = "MOTOR1_LVLCTY_R";
static const char *Motor1SgnStepsRString = "MOTOR1_SGN_STEPS_R";
static const char *Motor1StatRString = "MOTOR1_STAT_R";
static const char *Motor1StepsRString = "MOTOR1_STEPS_R";
static const char *Motor1StepsWString = "MOTOR1_STEPS_W";
static const char *Motor1StepsActualRString = "MOTOR1_STEPS_ACTUAL_R";
static const char *Motor1SubStpRString = "MOTOR1_SUB_STP_R";
static const char *Motor1SubStpWString = "MOTOR1_SUB_STP_W";
static const char *Motor1VlctyRString = "MOTOR1_VLCTY_R";
static const char *Motor1VlctyWString = "MOTOR1_VLCTY_W";
static const char *Motor2AbsStepsRString = "MOTOR2_ABS_STEPS_R";
static const char *Motor2AccRString = "MOTOR2_ACC_R";
static const char *Motor2AccWString = "MOTOR2_ACC_W";
static const char *Motor2BrdTmpRString = "MOTOR2_BRD_TMP_R";
static const char *Motor2CntlRString = "MOTOR2_CNTL_R";
static const char *Motor2CntlWString = "MOTOR2_CNTL_W";
static const char *Motor2DrviRString = "MOTOR2_DRVI_R";
static const char *Motor2DrviWString = "MOTOR2_DRVI_W";
static const char *Motor2LaccRString = "MOTOR2_LACC_R";
static const char *Motor2LstepsRString = "MOTOR2_LSTEPS_R";
static const char *Motor2LvlctyRString = "MOTOR2_LVLCTY_R";
static const char *Motor2SgnStepsRString = "MOTOR2_SGN_STEPS_R";
static const char *Motor2StatRString = "MOTOR2_STAT_R";
static const char *Motor2StepsRString = "MOTOR2_STEPS_R";
static const char *Motor2StepsWString = "MOTOR2_STEPS_W";
static const char *Motor2StepsActualRString = "MOTOR2_STEPS_ACTUAL_R";
static const char *Motor2SubStpRString = "MOTOR2_SUB_STP_R";
static const char *Motor2SubStpWString = "MOTOR2_SUB_STP_W";
static const char *Motor2VlctyRString = "MOTOR2_VLCTY_R";
static const char *Motor2VlctyWString = "MOTOR2_VLCTY_W";
static const char *Motor3AbsStepsRString = "MOTOR3_ABS_STEPS_R";
static const char *Motor3AccRString = "MOTOR3_ACC_R";
static const char *Motor3AccWString = "MOTOR3_ACC_W";
static const char *Motor3BrdTmpRString = "MOTOR3_BRD_TMP_R";
static const char *Motor3CntlRString = "MOTOR3_CNTL_R";
static const char *Motor3CntlWString = "MOTOR3_CNTL_W";
static const char *Motor3DrviRString = "MOTOR3_DRVI_R";
static const char *Motor3DrviWString = "MOTOR3_DRVI_W";
static const char *Motor3LaccRString = "MOTOR3_LACC_R";
static const char *Motor3LstepsRString = "MOTOR3_LSTEPS_R";
static const char *Motor3LvlctyRString = "MOTOR3_LVLCTY_R";
static const char *Motor3SgnStepsRString = "MOTOR3_SGN_STEPS_R";
static const char *Motor3StatRString = "MOTOR3_STAT_R";
static const char *Motor3StepsRString = "MOTOR3_STEPS_R";
static const char *Motor3StepsWString = "MOTOR3_STEPS_W";
static const char *Motor3StepsActualRString = "MOTOR3_STEPS_ACTUAL_R";
static const char *Motor3SubStpRString = "MOTOR3_SUB_STP_R";
static const char *Motor3SubStpWString = "MOTOR3_SUB_STP_W";
static const char *Motor3VlctyRString = "MOTOR3_VLCTY_R";
static const char *Motor3VlctyWString = "MOTOR3_VLCTY_W";
static const char *Motor4AbsStepsRString = "MOTOR4_ABS_STEPS_R";
static const char *Motor4AccRString = "MOTOR4_ACC_R";
static const char *Motor4AccWString = "MOTOR4_ACC_W";
static const char *Motor4BrdTmpRString = "MOTOR4_BRD_TMP_R";
static const char *Motor4CntlRString = "MOTOR4_CNTL_R";
static const char *Motor4CntlWString = "MOTOR4_CNTL_W";
static const char *Motor4DrviRString = "MOTOR4_DRVI_R";
static const char *Motor4DrviWString = "MOTOR4_DRVI_W";
static const char *Motor4LaccRString = "MOTOR4_LACC_R";
static const char *Motor4LstepsRString = "MOTOR4_LSTEPS_R";
static const char *Motor4LvlctyRString = "MOTOR4_LVLCTY_R";
static const char *Motor4SgnStepsRString = "MOTOR4_SGN_STEPS_R";
static const char *Motor4StatRString = "MOTOR4_STAT_R";
static const char *Motor4StepsRString = "MOTOR4_STEPS_R";
static const char *Motor4StepsWString = "MOTOR4_STEPS_W";
static const char *Motor4StepsActualRString = "MOTOR4_STEPS_ACTUAL_R";
static const char *Motor4SubStpRString = "MOTOR4_SUB_STP_R";
static const char *Motor4SubStpWString = "MOTOR4_SUB_STP_W";
static const char *Motor4VlctyRString = "MOTOR4_VLCTY_R";
static const char *Motor4VlctyWString = "MOTOR4_VLCTY_W";
static const char *Piezo1Adc1RString = "PIEZO1_ADC1_R";
static const char *Piezo1Adc2RString = "PIEZO1_ADC2_R";
static const char *Piezo1BrdSernumRString = "PIEZO1_BRD_SERNUM_R";
static const char *Piezo1BrdTmpRString = "PIEZO1_BRD_TMP_R";
static const char *Piezo1C1InRString = "PIEZO1_C1_IN_R";
static const char *Piezo1C1IpRString = "PIEZO1_C1_IP_R";
static const char *Piezo1C1DriveVRString = "PIEZO1_C1_DRIVE_V_R";
static const char *Piezo1C1OutVRString = "PIEZO1_C1_OUT_V_R";
static const char *Piezo1C2InRString = "PIEZO1_C2_IN_R";
static const char *Piezo1C2IpRString = "PIEZO1_C2_IP_R";
static const char *Piezo1C2DriveVRString = "PIEZO1_C2_DRIVE_V_R";
static const char *Piezo1C2OutVRString = "PIEZO1_C2_OUT_V_R";
static const char *Piezo1CntlRString = "PIEZO1_CNTL_R";
static const char *Piezo1CntlWString = "PIEZO1_CNTL_W";
static const char *Piezo1Dac1RString = "PIEZO1_DAC1_R";
static const char *Piezo1Dac1WString = "PIEZO1_DAC1_W";
static const char *Piezo1Dac2RString = "PIEZO1_DAC2_R";
static const char *Piezo1Dac2WString = "PIEZO1_DAC2_W";
static const char *Piezo1HighWindowRString = "PIEZO1_HIGH_WINDOW_R";
static const char *Piezo1HighWindowWString = "PIEZO1_HIGH_WINDOW_W";
static const char *Piezo1IgainRString = "PIEZO1_IGAIN_R";
static const char *Piezo1IgainWString = "PIEZO1_IGAIN_W";
static const char *Piezo1LowWindowRString = "PIEZO1_LOW_WINDOW_R";
static const char *Piezo1LowWindowWString = "PIEZO1_LOW_WINDOW_W";
static const char *Piezo1StatRString = "PIEZO1_STAT_R";
static const char *Piezo2Adc1RString = "PIEZO2_ADC1_R";
static const char *Piezo2Adc2RString = "PIEZO2_ADC2_R";
static const char *Piezo2BrdSernumRString = "PIEZO2_BRD_SERNUM_R";
static const char *Piezo2BrdTmpRString = "PIEZO2_BRD_TMP_R";
static const char *Piezo2C1InRString = "PIEZO2_C1_IN_R";
static const char *Piezo2C1IpRString = "PIEZO2_C1_IP_R";
static const char *Piezo2C1DriveVRString = "PIEZO2_C1_DRIVE_V_R";
static const char *Piezo2C1OutVRString = "PIEZO2_C1_OUT_V_R";
static const char *Piezo2C2InRString = "PIEZO2_C2_IN_R";
static const char *Piezo2C2IpRString = "PIEZO2_C2_IP_R";
static const char *Piezo2C2DriveVRString = "PIEZO2_C2_DRIVE_V_R";
static const char *Piezo2C2OutVRString = "PIEZO2_C2_OUT_V_R";
static const char *Piezo2CntlRString = "PIEZO2_CNTL_R";
static const char *Piezo2CntlWString = "PIEZO2_CNTL_W";
static const char *Piezo2Dac1RString = "PIEZO2_DAC1_R";
static const char *Piezo2Dac1WString = "PIEZO2_DAC1_W";
static const char *Piezo2Dac2RString = "PIEZO2_DAC2_R";
static const char *Piezo2Dac2WString = "PIEZO2_DAC2_W";
static const char *Piezo2HighWindowRString = "PIEZO2_HIGH_WINDOW_R";
static const char *Piezo2HighWindowWString = "PIEZO2_HIGH_WINDOW_W";
static const char *Piezo2IgainRString = "PIEZO2_IGAIN_R";
static const char *Piezo2IgainWString = "PIEZO2_IGAIN_W";
static const char *Piezo2LowWindowRString = "PIEZO2_LOW_WINDOW_R";
static const char *Piezo2LowWindowWString = "PIEZO2_LOW_WINDOW_W";
static const char *Piezo2StatRString = "PIEZO2_STAT_R";
static const char *Piezo3Adc1RString = "PIEZO3_ADC1_R";
static const char *Piezo3Adc2RString = "PIEZO3_ADC2_R";
static const char *Piezo3BrdSernumRString = "PIEZO3_BRD_SERNUM_R";
static const char *Piezo3BrdTmpRString = "PIEZO3_BRD_TMP_R";
static const char *Piezo3C1InRString = "PIEZO3_C1_IN_R";
static const char *Piezo3C1IpRString = "PIEZO3_C1_IP_R";
static const char *Piezo3C1DriveVRString = "PIEZO3_C1_DRIVE_V_R";
static const char *Piezo3C1OutVRString = "PIEZO3_C1_OUT_V_R";
static const char *Piezo3C2InRString = "PIEZO3_C2_IN_R";
static const char *Piezo3C2IpRString = "PIEZO3_C2_IP_R";
static const char *Piezo3C2DriveVRString = "PIEZO3_C2_DRIVE_V_R";
static const char *Piezo3C2OutVRString = "PIEZO3_C2_OUT_V_R";
static const char *Piezo3CntlRString = "PIEZO3_CNTL_R";
static const char *Piezo3CntlWString = "PIEZO3_CNTL_W";
static const char *Piezo3Dac1RString = "PIEZO3_DAC1_R";
static const char *Piezo3Dac1WString = "PIEZO3_DAC1_W";
static const char *Piezo3Dac2RString = "PIEZO3_DAC2_R";
static const char *Piezo3Dac2WString = "PIEZO3_DAC2_W";
static const char *Piezo3HighWindowRString = "PIEZO3_HIGH_WINDOW_R";
static const char *Piezo3HighWindowWString = "PIEZO3_HIGH_WINDOW_W";
static const char *Piezo3IgainRString = "PIEZO3_IGAIN_R";
static const char *Piezo3IgainWString = "PIEZO3_IGAIN_W";
static const char *Piezo3LowWindowRString = "PIEZO3_LOW_WINDOW_R";
static const char *Piezo3LowWindowWString = "PIEZO3_LOW_WINDOW_W";
static const char *Piezo3StatRString = "PIEZO3_STAT_R";
static const char *Piezo4Adc1RString = "PIEZO4_ADC1_R";
static const char *Piezo4Adc2RString = "PIEZO4_ADC2_R";
static const char *Piezo4BrdSernumRString = "PIEZO4_BRD_SERNUM_R";
static const char *Piezo4BrdTmpRString = "PIEZO4_BRD_TMP_R";
static const char *Piezo4C1InRString = "PIEZO4_C1_IN_R";
static const char *Piezo4C1IpRString = "PIEZO4_C1_IP_R";
static const char *Piezo4C1DriveVRString = "PIEZO4_C1_DRIVE_V_R";
static const char *Piezo4C1OutVRString = "PIEZO4_C1_OUT_V_R";
static const char *Piezo4C2InRString = "PIEZO4_C2_IN_R";
static const char *Piezo4C2IpRString = "PIEZO4_C2_IP_R";
static const char *Piezo4C2DriveVRString = "PIEZO4_C2_DRIVE_V_R";
static const char *Piezo4C2OutVRString = "PIEZO4_C2_OUT_V_R";
static const char *Piezo4CntlRString = "PIEZO4_CNTL_R";
static const char *Piezo4CntlWString = "PIEZO4_CNTL_W";
static const char *Piezo4Dac1RString = "PIEZO4_DAC1_R";
static const char *Piezo4Dac1WString = "PIEZO4_DAC1_W";
static const char *Piezo4Dac2RString = "PIEZO4_DAC2_R";
static const char *Piezo4Dac2WString = "PIEZO4_DAC2_W";
static const char *Piezo4HighWindowRString = "PIEZO4_HIGH_WINDOW_R";
static const char *Piezo4HighWindowWString = "PIEZO4_HIGH_WINDOW_W";
static const char *Piezo4IgainRString = "PIEZO4_IGAIN_R";
static const char *Piezo4IgainWString = "PIEZO4_IGAIN_W";
static const char *Piezo4LowWindowRString = "PIEZO4_LOW_WINDOW_R";
static const char *Piezo4LowWindowWString = "PIEZO4_LOW_WINDOW_W";
static const char *Piezo4StatRString = "PIEZO4_STAT_R";
static const char *PztWaveAvailRString = "PZT_WAVE_AVAIL_R";
static const char *Reserved0RString = "RESERVED0_R";
static const char *Reserved0WString = "RESERVED0_W";
static const char *Reserved1RString = "RESERVED1_R";
static const char *Reserved1WString = "RESERVED1_W";
static const char *Reserved2RString = "RESERVED2_R";
static const char *Reserved2WString = "RESERVED2_W";
static const char *Reserved3RString = "RESERVED3_R";
static const char *Reserved3WString = "RESERVED3_W";
static const char *Reserved4RString = "RESERVED4_R";
static const char *Reserved4WString = "RESERVED4_W";

const unsigned int scllrfRESPolledRegCount = 142;



 /* This class implements the AxiVersion driver. */
class scllrfRESDriver : public scllrfAsynPortDriver {
public:
//    scllrfRESDriver(const char *drvPortName, const char *netPortName);
    scllrfRESDriver(const char *drvPortName, const char *netPortName, int maxAddr, int paramTableAdds);
    virtual ~scllrfRESDriver();



protected:
    virtual asynStatus functionToRegister(const int function, FpgaReg *pToFpga); /**< Translate asyn function number/reason to a register address */
	virtual asynStatus processRegReadback(const FpgaReg *pFromFpga,
			bool &waveIsReady); // parse register data, write to PVs
	virtual asynStatus processRegWriteResponse(const FpgaReg *pFromFpga);



/* Registers */
    int p_Cavity1DetuneErrR;
    #define FIRST_SCLLRFRES_PARAM p_Cavity1DetuneErrR
    int p_Cavity2DetuneErrR;
    int p_Cavity3DetuneErrR;
    int p_Cavity4DetuneErrR;
    int p_Motor1AbsStepsR;
    int p_Motor1AccR;
    int p_Motor1AccW;
    int p_Motor1BrdTmpR;
    int p_Motor1CntlR;
    int p_Motor1CntlW;
    int p_Motor1DrviR;
    int p_Motor1DrviW;
    int p_Motor1LaccR;
    int p_Motor1LstepsR;
    int p_Motor1LvlctyR;
    int p_Motor1SgnStepsR;
    int p_Motor1StatR;
    int p_Motor1StepsR;
    int p_Motor1StepsW;
    int p_Motor1StepsActualR;
    int p_Motor1SubStpR;
    int p_Motor1SubStpW;
    int p_Motor1VlctyR;
    int p_Motor1VlctyW;
    int p_Motor2AbsStepsR;
    int p_Motor2AccR;
    int p_Motor2AccW;
    int p_Motor2BrdTmpR;
    int p_Motor2CntlR;
    int p_Motor2CntlW;
    int p_Motor2DrviR;
    int p_Motor2DrviW;
    int p_Motor2LaccR;
    int p_Motor2LstepsR;
    int p_Motor2LvlctyR;
    int p_Motor2SgnStepsR;
    int p_Motor2StatR;
    int p_Motor2StepsR;
    int p_Motor2StepsW;
    int p_Motor2StepsActualR;
    int p_Motor2SubStpR;
    int p_Motor2SubStpW;
    int p_Motor2VlctyR;
    int p_Motor2VlctyW;
    int p_Motor3AbsStepsR;
    int p_Motor3AccR;
    int p_Motor3AccW;
    int p_Motor3BrdTmpR;
    int p_Motor3CntlR;
    int p_Motor3CntlW;
    int p_Motor3DrviR;
    int p_Motor3DrviW;
    int p_Motor3LaccR;
    int p_Motor3LstepsR;
    int p_Motor3LvlctyR;
    int p_Motor3SgnStepsR;
    int p_Motor3StatR;
    int p_Motor3StepsR;
    int p_Motor3StepsW;
    int p_Motor3StepsActualR;
    int p_Motor3SubStpR;
    int p_Motor3SubStpW;
    int p_Motor3VlctyR;
    int p_Motor3VlctyW;
    int p_Motor4AbsStepsR;
    int p_Motor4AccR;
    int p_Motor4AccW;
    int p_Motor4BrdTmpR;
    int p_Motor4CntlR;
    int p_Motor4CntlW;
    int p_Motor4DrviR;
    int p_Motor4DrviW;
    int p_Motor4LaccR;
    int p_Motor4LstepsR;
    int p_Motor4LvlctyR;
    int p_Motor4SgnStepsR;
    int p_Motor4StatR;
    int p_Motor4StepsR;
    int p_Motor4StepsW;
    int p_Motor4StepsActualR;
    int p_Motor4SubStpR;
    int p_Motor4SubStpW;
    int p_Motor4VlctyR;
    int p_Motor4VlctyW;
    int p_Piezo1Adc1R;
    int p_Piezo1Adc2R;
    int p_Piezo1BrdSernumR;
    int p_Piezo1BrdTmpR;
    int p_Piezo1C1InR;
    int p_Piezo1C1IpR;
    int p_Piezo1C1DriveVR;
    int p_Piezo1C1OutVR;
    int p_Piezo1C2InR;
    int p_Piezo1C2IpR;
    int p_Piezo1C2DriveVR;
    int p_Piezo1C2OutVR;
    int p_Piezo1CntlR;
    int p_Piezo1CntlW;
    int p_Piezo1Dac1R;
    int p_Piezo1Dac1W;
    int p_Piezo1Dac2R;
    int p_Piezo1Dac2W;
    int p_Piezo1HighWindowR;
    int p_Piezo1HighWindowW;
    int p_Piezo1IgainR;
    int p_Piezo1IgainW;
    int p_Piezo1LowWindowR;
    int p_Piezo1LowWindowW;
    int p_Piezo1StatR;
    int p_Piezo2Adc1R;
    int p_Piezo2Adc2R;
    int p_Piezo2BrdSernumR;
    int p_Piezo2BrdTmpR;
    int p_Piezo2C1InR;
    int p_Piezo2C1IpR;
    int p_Piezo2C1DriveVR;
    int p_Piezo2C1OutVR;
    int p_Piezo2C2InR;
    int p_Piezo2C2IpR;
    int p_Piezo2C2DriveVR;
    int p_Piezo2C2OutVR;
    int p_Piezo2CntlR;
    int p_Piezo2CntlW;
    int p_Piezo2Dac1R;
    int p_Piezo2Dac1W;
    int p_Piezo2Dac2R;
    int p_Piezo2Dac2W;
    int p_Piezo2HighWindowR;
    int p_Piezo2HighWindowW;
    int p_Piezo2IgainR;
    int p_Piezo2IgainW;
    int p_Piezo2LowWindowR;
    int p_Piezo2LowWindowW;
    int p_Piezo2StatR;
    int p_Piezo3Adc1R;
    int p_Piezo3Adc2R;
    int p_Piezo3BrdSernumR;
    int p_Piezo3BrdTmpR;
    int p_Piezo3C1InR;
    int p_Piezo3C1IpR;
    int p_Piezo3C1DriveVR;
    int p_Piezo3C1OutVR;
    int p_Piezo3C2InR;
    int p_Piezo3C2IpR;
    int p_Piezo3C2DriveVR;
    int p_Piezo3C2OutVR;
    int p_Piezo3CntlR;
    int p_Piezo3CntlW;
    int p_Piezo3Dac1R;
    int p_Piezo3Dac1W;
    int p_Piezo3Dac2R;
    int p_Piezo3Dac2W;
    int p_Piezo3HighWindowR;
    int p_Piezo3HighWindowW;
    int p_Piezo3IgainR;
    int p_Piezo3IgainW;
    int p_Piezo3LowWindowR;
    int p_Piezo3LowWindowW;
    int p_Piezo3StatR;
    int p_Piezo4Adc1R;
    int p_Piezo4Adc2R;
    int p_Piezo4BrdSernumR;
    int p_Piezo4BrdTmpR;
    int p_Piezo4C1InR;
    int p_Piezo4C1IpR;
    int p_Piezo4C1DriveVR;
    int p_Piezo4C1OutVR;
    int p_Piezo4C2InR;
    int p_Piezo4C2IpR;
    int p_Piezo4C2DriveVR;
    int p_Piezo4C2OutVR;
    int p_Piezo4CntlR;
    int p_Piezo4CntlW;
    int p_Piezo4Dac1R;
    int p_Piezo4Dac1W;
    int p_Piezo4Dac2R;
    int p_Piezo4Dac2W;
    int p_Piezo4HighWindowR;
    int p_Piezo4HighWindowW;
    int p_Piezo4IgainR;
    int p_Piezo4IgainW;
    int p_Piezo4LowWindowR;
    int p_Piezo4LowWindowW;
    int p_Piezo4StatR;
    int p_PztWaveAvailR;
    int p_Reserved0R;
    int p_Reserved0W;
    int p_Reserved1R;
    int p_Reserved1W;
    int p_Reserved2R;
    int p_Reserved2W;
    int p_Reserved3R;
    int p_Reserved3W;
    int p_Reserved4R;
    int p_Reserved4W;
    #define LAST_SCLLRFRES_PARAM p_Reserved4W

#define NUM_SCLLRFRES_PARAMS (&LAST_SCLLRFRES_PARAM - &FIRST_SCLLRFRES_PARAM + NUM_SCLLRF_PARAMS + 1)


    // mapping of register names to addresses
    enum ReadRegAddrs
    {
    	Motor1AccRAdr = 0x00000000,
    	Motor1VlctyRAdr = 0x00000001,
    	Motor1StepsRAdr = 0x00000002,
    	Motor1SgnStepsRAdr = 0x00000003,
    	Motor1AbsStepsRAdr = 0x00000004,
    	Motor1LaccRAdr = 0x00000005,
    	Motor1LvlctyRAdr = 0x00000006,
    	Motor1LstepsRAdr = 0x00000007,
    	Motor1DrviRAdr = 0x00000008,
    	Motor1BrdTmpRAdr = 0x00000009,
    	Motor1CntlRAdr = 0x0000000A,
    	Motor1StatRAdr = 0x0000000B,
    	Piezo1Dac1RAdr = 0x0000000C,
    	Piezo1Dac2RAdr = 0x0000000D,
    	Piezo1Adc1RAdr = 0x0000000E,
    	Piezo1Adc2RAdr = 0x0000000F,
    	Piezo1HighWindowRAdr = 0x00000010,
    	Piezo1LowWindowRAdr = 0x00000011,
    	Piezo1CntlRAdr = 0x00000012,
    	Piezo1StatRAdr = 0x00000013,
    	Piezo1BrdTmpRAdr = 0x00000014,
    	Piezo1BrdSernumRAdr = 0x00000015,
    	Piezo1IgainRAdr = 0x00000016,
    	Cavity1DetuneErrRAdr = 0x00000017,
    	Motor2AccRAdr = 0x00000018,
    	Motor2VlctyRAdr = 0x00000019,
    	Motor2StepsRAdr = 0x0000001A,
    	Motor2SgnStepsRAdr = 0x0000001B,
    	Motor2AbsStepsRAdr = 0x0000001C,
    	Motor2LaccRAdr = 0x0000001D,
    	Motor2LvlctyRAdr = 0x0000001E,
    	Motor2LstepsRAdr = 0x0000001F,
    	Motor2DrviRAdr = 0x00000020,
    	Motor2BrdTmpRAdr = 0x00000021,
    	Motor2CntlRAdr = 0x00000022,
    	Motor2StatRAdr = 0x00000023,
    	Piezo2Dac1RAdr = 0x00000024,
    	Piezo2Dac2RAdr = 0x00000025,
    	Piezo2Adc1RAdr = 0x00000026,
    	Piezo2Adc2RAdr = 0x00000027,
    	Piezo2HighWindowRAdr = 0x00000028,
    	Piezo2LowWindowRAdr = 0x00000029,
    	Piezo2CntlRAdr = 0x0000002A,
    	Piezo2StatRAdr = 0x0000002B,
    	Piezo2BrdTmpRAdr = 0x0000002C,
    	Piezo2BrdSernumRAdr = 0x0000002D,
    	Piezo2IgainRAdr = 0x0000002E,
    	Cavity2DetuneErrRAdr = 0x0000002F,
    	Motor3AccRAdr = 0x00000030,
    	Motor3VlctyRAdr = 0x00000031,
    	Motor3StepsRAdr = 0x00000032,
    	Motor3SgnStepsRAdr = 0x00000033,
    	Motor3AbsStepsRAdr = 0x00000034,
    	Motor3LaccRAdr = 0x00000035,
    	Motor3LvlctyRAdr = 0x00000036,
    	Motor3LstepsRAdr = 0x00000037,
    	Motor3DrviRAdr = 0x00000038,
    	Motor3BrdTmpRAdr = 0x00000039,
    	Motor3CntlRAdr = 0x0000003A,
    	Motor3StatRAdr = 0x0000003B,
    	Piezo3Dac1RAdr = 0x0000003C,
    	Piezo3Dac2RAdr = 0x0000003D,
    	Piezo3Adc1RAdr = 0x0000003E,
    	Piezo3Adc2RAdr = 0x0000003F,
    	Piezo3HighWindowRAdr = 0x00000040,
    	Piezo3LowWindowRAdr = 0x00000041,
    	Piezo3CntlRAdr = 0x00000042,
    	Piezo3StatRAdr = 0x00000043,
    	Piezo3BrdTmpRAdr = 0x00000044,
    	Piezo3BrdSernumRAdr = 0x00000045,
    	Piezo3IgainRAdr = 0x00000046,
    	Cavity3DetuneErrRAdr = 0x00000047,
    	Motor4AccRAdr = 0x00000048,
    	Motor4VlctyRAdr = 0x00000049,
    	Motor4StepsRAdr = 0x0000004A,
    	Motor4SgnStepsRAdr = 0x0000004B,
    	Motor4AbsStepsRAdr = 0x0000004C,
    	Motor4LaccRAdr = 0x0000004D,
    	Motor4LvlctyRAdr = 0x0000004E,
    	Motor4LstepsRAdr = 0x0000004F,
    	Motor4DrviRAdr = 0x00000050,
    	Motor4BrdTmpRAdr = 0x00000051,
    	Motor4CntlRAdr = 0x00000052,
    	Motor4StatRAdr = 0x00000053,
    	Piezo4Dac1RAdr = 0x00000054,
    	Piezo4Dac2RAdr = 0x00000055,
    	Piezo4Adc1RAdr = 0x00000056,
    	Piezo4Adc2RAdr = 0x00000057,
    	Piezo4HighWindowRAdr = 0x00000058,
    	Piezo4LowWindowRAdr = 0x00000059,
    	Piezo4CntlRAdr = 0x0000005A,
    	Piezo4StatRAdr = 0x0000005B,
    	Piezo4BrdTmpRAdr = 0x0000005C,
    	Piezo4BrdSernumRAdr = 0x0000005D,
    	Piezo4IgainRAdr = 0x0000005E,
    	Cavity4DetuneErrRAdr = 0x0000005F,
    	Motor1SubStpRAdr = 0x00000060,
    	Motor2SubStpRAdr = 0x00000061,
    	Motor3SubStpRAdr = 0x00000062,
    	Motor4SubStpRAdr = 0x00000063,
    	Reserved0RAdr = 0x00000064,
    	Reserved1RAdr = 0x00000065,
    	Reserved2RAdr = 0x00000066,
    	Reserved3RAdr = 0x00000067,
    	Reserved4RAdr = 0x00000068,
    	Motor1StepsActualRAdr = 0x00000069,
    	Motor2StepsActualRAdr = 0x0000006A,
    	Motor3StepsActualRAdr = 0x0000006B,
    	Motor4StepsActualRAdr = 0x0000006C,
    	PztWaveAvailRAdr = 0x0000006F,
    	Piezo1C1InRAdr = 0x00000070,
    	Piezo1C1IpRAdr = 0x00000071,
    	Piezo1C1OutVRAdr = 0x00000072,
    	Piezo1C1DriveVRAdr = 0x00000073,
    	Piezo1C2InRAdr = 0x00000074,
    	Piezo1C2IpRAdr = 0x00000075,
    	Piezo1C2OutVRAdr = 0x00000076,
    	Piezo1C2DriveVRAdr = 0x00000077,
    	Piezo2C1InRAdr = 0x00000080,
    	Piezo2C1IpRAdr = 0x00000081,
    	Piezo2C1OutVRAdr = 0x00000082,
    	Piezo2C1DriveVRAdr = 0x00000083,
    	Piezo2C2InRAdr = 0x00000084,
    	Piezo2C2IpRAdr = 0x00000085,
    	Piezo2C2OutVRAdr = 0x00000086,
    	Piezo2C2DriveVRAdr = 0x00000087,
    	Piezo3C1InRAdr = 0x00000090,
    	Piezo3C1IpRAdr = 0x00000091,
    	Piezo3C1OutVRAdr = 0x00000092,
    	Piezo3C1DriveVRAdr = 0x00000093,
    	Piezo3C2InRAdr = 0x00000094,
    	Piezo3C2IpRAdr = 0x00000095,
    	Piezo3C2OutVRAdr = 0x00000096,
    	Piezo3C2DriveVRAdr = 0x00000097,
    	Piezo4C1InRAdr = 0x000000A0,
    	Piezo4C1IpRAdr = 0x000000A1,
    	Piezo4C1OutVRAdr = 0x000000A2,
    	Piezo4C1DriveVRAdr = 0x000000A3,
    	Piezo4C2InRAdr = 0x000000A4,
    	Piezo4C2IpRAdr = 0x000000A5,
    	Piezo4C2OutVRAdr = 0x000000A6,
    	Piezo4C2DriveVRAdr = 0x000000A7,
    };

    // mapping of register names to addresses
    enum RegWriteAddrs
    {
    	Motor1AccWAdr = 0x00000000,
    	Motor1VlctyWAdr = 0x00000001,
    	Motor1StepsWAdr = 0x00000002,
    	Motor1DrviWAdr = 0x00000008,
    	Motor1CntlWAdr = 0x0000000A,
    	Piezo1Dac1WAdr = 0x0000000C,
    	Piezo1Dac2WAdr = 0x0000000D,
    	Piezo1HighWindowWAdr = 0x00000010,
    	Piezo1LowWindowWAdr = 0x00000011,
    	Piezo1CntlWAdr = 0x00000012,
    	Piezo1IgainWAdr = 0x00000016,
    	Motor2AccWAdr = 0x00000018,
    	Motor2VlctyWAdr = 0x00000019,
    	Motor2StepsWAdr = 0x0000001A,
    	Motor2DrviWAdr = 0x00000020,
    	Motor2CntlWAdr = 0x00000022,
    	Piezo2Dac1WAdr = 0x00000024,
    	Piezo2Dac2WAdr = 0x00000025,
    	Piezo2HighWindowWAdr = 0x00000028,
    	Piezo2LowWindowWAdr = 0x00000029,
    	Piezo2CntlWAdr = 0x0000002A,
    	Piezo2IgainWAdr = 0x0000002E,
    	Motor3AccWAdr = 0x00000030,
    	Motor3VlctyWAdr = 0x00000031,
    	Motor3StepsWAdr = 0x00000032,
    	Motor3DrviWAdr = 0x00000038,
    	Motor3CntlWAdr = 0x0000003A,
    	Piezo3Dac1WAdr = 0x0000003C,
    	Piezo3Dac2WAdr = 0x0000003D,
    	Piezo3HighWindowWAdr = 0x00000040,
    	Piezo3LowWindowWAdr = 0x00000041,
    	Piezo3CntlWAdr = 0x00000042,
    	Piezo3IgainWAdr = 0x00000046,
    	Motor4AccWAdr = 0x00000048,
    	Motor4VlctyWAdr = 0x00000049,
    	Motor4StepsWAdr = 0x0000004A,
    	Motor4DrviWAdr = 0x00000050,
    	Motor4CntlWAdr = 0x00000052,
    	Piezo4Dac1WAdr = 0x00000054,
    	Piezo4Dac2WAdr = 0x00000055,
    	Piezo4HighWindowWAdr = 0x00000058,
    	Piezo4LowWindowWAdr = 0x00000059,
    	Piezo4CntlWAdr = 0x0000005A,
    	Piezo4IgainWAdr = 0x0000005E,
    	Motor1SubStpWAdr = 0x00000060,
    	Motor2SubStpWAdr = 0x00000061,
    	Motor3SubStpWAdr = 0x00000062,
    	Motor4SubStpWAdr = 0x00000063,
    	Reserved0WAdr = 0x00000064,
    	Reserved1WAdr = 0x00000065,
    	Reserved2WAdr = 0x00000066,
    	Reserved3WAdr = 0x00000067,
    	Reserved4WAdr = 0x00000068,
    };

    // masks applied to returned register data
    enum RegMasks
    {
    	Motor1CntlMask =  0xFFFFFFFF, // bit fields
    	Motor1StatMask =  0xFFFFFFFF, // bit fields
    	Piezo1CntlMask =  0xFFFFFFFF, // bit fields
    	Piezo1StatMask =  0xFFFFFFFF, // bit fields
    	Motor2CntlMask =  0xFFFFFFFF, // bit fields
    	Motor2StatMask =  0xFFFFFFFF, // bit fields
    	Piezo2CntlMask =  0xFFFFFFFF, // bit fields
    	Piezo2StatMask =  0xFFFFFFFF, // bit fields
    	Motor3CntlMask =  0xFFFFFFFF, // bit fields
    	Motor3StatMask =  0xFFFFFFFF, // bit fields
    	Piezo3CntlMask =  0xFFFFFFFF, // bit fields
    	Piezo3StatMask =  0xFFFFFFFF, // bit fields
    	Motor4CntlMask =  0xFFFFFFFF, // bit fields
    	Motor4StatMask =  0xFFFFFFFF, // bit fields
    	Piezo4CntlMask =  0xFFFFFFFF, // bit fields
    	Piezo4StatMask =  0xFFFFFFFF, // bit fields

    };
};

#endif

