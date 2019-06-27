/*
 * registers.h
 *
 *  Created on: Jun 17, 2016
 *      Author: gwbrown
 */

#ifndef scllrfAPP_SRC_REGISTERS_H_
#define scllrfAPP_SRC_REGISTERS_H_


/* These are the drvInfo strings that are used to identify the parameters.
 * They are used by asyn clients, including standard asyn device support */
#define P_MagicString            "MAGIC"            /* asynInt32,    r/o */

enum RegAddresses
{
	MAGIC = 0x0000,
	DSP_FLAVOR = 0x0001,
	YEAR = 0x0002,
	MONTH = 0x0003,
	DAY = 0x0004
};

#endif /* scllrfAPP_SRC_REGISTERS_H_ */
