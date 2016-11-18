d = { "name" : "scllrfRES",
     'registers': [
	{
		"name": "motor1_acc",
		"address": 0,
		"access":"r",
		"signed": "unsigned",
		"desc": 'acceleration Hz\/\(Sec\^2\)'
	},
	{
		"name": "motor1_acc",
		"address": 0,
		"access":"w",
		"signed": "unsigned",
		"desc": 'acceleration Hz\/\(Sec\^2\)'
	},
	{
		"name": "motor1_vlcty",
		"address": 1,
		"access":"r",
		"signed": "unsigned",
		"desc": 'velocity Hz\/Sec'
	},
	{
		"name": "motor1_vlcty",
		"address": 1,
		"access":"w",
		"signed": "unsigned",
		"desc": 'velocity Hz\/Sec'
	},
	{
		"name": "motor1_steps",
		"address": 2,
		"access":"r",
		"signed": "unsigned",
		"desc": 'incremental\/relative steps Hz'
	},
	{
		"name": "motor1_steps",
		"address": 2,
		"access":"w",
		"signed": "unsigned",
		"desc": 'incremental\/relative steps Hz'
	},
	{
		"name": "motor1_sgn_steps",
		"address": 3,
		"access":"r",
		"signed": "signed",
		"desc": 'signed steps Hz \(accumulated since power on\)'
	},
	{
		"name": "motor1_abs_steps",
		"address": 4,
		"access":"r",
		"signed": "unsigned",
		"desc": 'absolute steps Hz \(moved distance\)'
	},
	{
		"name": "motor1_drvI",
		"address": 5,
		"access":"r",
		"signed": "unsigned",
		"desc": 'drive current Amps'
	},
	{
		"name": "motor1_drvI",
		"address": 5,
		"access":"w",
		"signed": "unsigned",
		"desc": 'drive current Amps'
	},
	{
		"name": "motor1_brd_tmp",
		"address": 6,
		"access":"r",
		"signed": "unsigned",
		"desc": 'board temp deg C'
	},
	{
		"name": "motor1_cntl",
		"address": 7,
		"access":"r",
		"signed": "unsigned",
		"desc": 'control bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "motor1_cntl",
		"address": 7,
		"access":"w",
		"signed": "unsigned",
		"desc": 'control bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "motor1_stat",
		"address": 8,
		"access":"r",
		"signed": "unsigned",
		"desc": 'status bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "piezo1_dac1",
		"address": 9,
		"access":"r",
		"signed": "signed",
		"desc": 'drive dac1 set point Volts'
	},
	{
		"name": "piezo1_dac1",
		"address": 9,
		"access":"w",
		"signed": "signed",
		"desc": 'drive dac1 set point Volts'
	},
	{
		"name": "piezo1_dac2",
		"address": 10,
		"access":"r",
		"signed": "signed",
		"desc": 'drive dac2 set point Volts'
	},
	{
		"name": "piezo1_dac2",
		"address": 10,
		"access":"w",
		"signed": "signed",
		"desc": 'drive dac2 set point Volts'
	},
	{
		"name": "piezo1_adc1",
		"address": 11,
		"access":"r",
		"signed": "signed",
		"desc": 'position readback adc1 Volts'
	},
	{
		"name": "piezo1_adc2",
		"address": 12,
		"access":"r",
		"signed": "signed",
		"desc": 'position readback adc2 Volts'
	},
	{
		"name": "piezo1_high_window",
		"address": 13,
		"access":"r",
		"signed": "signed",
		"desc": 'position limit high Volts'
	},
	{
		"name": "piezo1_high_window",
		"address": 13,
		"access":"w",
		"signed": "signed",
		"desc": 'position limit high Volts'
	},
	{
		"name": "piezo1_low_window",
		"address": 14,
		"access":"r",
		"signed": "signed",
		"desc": 'position limit low Volts'
	},
	{
		"name": "piezo1_low_window",
		"address": 14,
		"access":"w",
		"signed": "signed",
		"desc": 'position limit low Volts'
	},
	{
		"name": "piezo1_cntl",
		"address": 15,
		"access":"r",
		"signed": "unsigned",
		"desc": 'control bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "piezo1_cntl",
		"address": 15,
		"access":"w",
		"signed": "unsigned",
		"desc": 'control bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "piezo1_stat",
		"address": 16,
		"access":"r",
		"signed": "unsigned",
		"desc": 'status bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "piezo1_brd_tmp",
		"address": 17,
		"access":"r",
		"signed": "unsigned",
		"desc": 'board temp deg C'
	},
	{
		"name": "piezo1_brd_sernum",
		"address": 18,
		"access":"r",
		"signed": "unsigned",
		"desc": 'serial number'
	},
	{
		"name": "piezo1_igain",
		"address": 19,
		"access":"r",
		"signed": "unsigned",
		"desc": 'I controller gain'
	},
	{
		"name": "piezo1_igain",
		"address": 19,
		"access":"w",
		"signed": "unsigned",
		"desc": 'I controller gain'
	},
	{
		"name": "cavity1_detune_err",
		"address": 20,
		"access":"r",
		"signed": "signed",
		"desc": 'detune frequency error Hz'
	},
	{
		"name": "motor2_acc",
		"address": 21,
		"access":"r",
		"signed": "unsigned",
		"desc": 'acceleration Hz\/\(Sec\^2\)'
	},
	{
		"name": "motor2_acc",
		"address": 21,
		"access":"w",
		"signed": "unsigned",
		"desc": 'acceleration Hz\/\(Sec\^2\)'
	},
	{
		"name": "motor2_vlcty",
		"address": 22,
		"access":"r",
		"signed": "unsigned",
		"desc": 'velocity Hz\/Sec'
	},
	{
		"name": "motor2_vlcty",
		"address": 22,
		"access":"w",
		"signed": "unsigned",
		"desc": 'velocity Hz\/Sec'
	},
	{
		"name": "motor2_steps",
		"address": 23,
		"access":"r",
		"signed": "unsigned",
		"desc": 'steps Hz'
	},
	{
		"name": "motor2_steps",
		"address": 23,
		"access":"w",
		"signed": "unsigned",
		"desc": 'steps Hz'
	},
	{
		"name": "motor2_sgn_steps",
		"address": 24,
		"access":"r",
		"signed": "signed",
		"desc": 'signed steps Hz'
	},
	{
		"name": "motor2_abs_steps",
		"address": 25,
		"access":"r",
		"signed": "unsigned",
		"desc": 'absolute steps Hz'
	},
	{
		"name": "motor2_drvI",
		"address": 26,
		"access":"r",
		"signed": "unsigned",
		"desc": 'drive current Amps'
	},
	{
		"name": "motor2_drvI",
		"address": 26,
		"access":"w",
		"signed": "unsigned",
		"desc": 'drive current Amps'
	},
	{
		"name": "motor2_brd_tmp",
		"address": 27,
		"access":"r",
		"signed": "unsigned",
		"desc": 'board temp deg C'
	},
	{
		"name": "motor2_cntl",
		"address": 28,
		"access":"r",
		"signed": "unsigned",
		"desc": 'control bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "motor2_cntl",
		"address": 28,
		"access":"w",
		"signed": "unsigned",
		"desc": 'control bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "motor2_stat",
		"address": 29,
		"access":"r",
		"signed": "unsigned",
		"desc": 'status bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "piezo2_dac1",
		"address": 30,
		"access":"r",
		"signed": "signed",
		"desc": 'drive dac1 set point Volts'
	},
	{
		"name": "piezo2_dac1",
		"address": 30,
		"access":"w",
		"signed": "signed",
		"desc": 'drive dac1 set point Volts'
	},
	{
		"name": "piezo2_dac2",
		"address": 31,
		"access":"r",
		"signed": "signed",
		"desc": 'drive dac2 set point Volts'
	},
	{
		"name": "piezo2_dac2",
		"address": 31,
		"access":"w",
		"signed": "signed",
		"desc": 'drive dac2 set point Volts'
	},
	{
		"name": "piezo2_adc1",
		"address": 32,
		"access":"r",
		"signed": "signed",
		"desc": 'position readback adc1 Volts'
	},
	{
		"name": "piezo2_adc2",
		"address": 33,
		"access":"r",
		"signed": "signed",
		"desc": 'position readback adc2 Volts'
	},
	{
		"name": "piezo2_high_window",
		"address": 34,
		"access":"r",
		"signed": "signed",
		"desc": 'position limit high Volts'
	},
	{
		"name": "piezo2_high_window",
		"address": 34,
		"access":"w",
		"signed": "signed",
		"desc": 'position limit high Volts'
	},
	{
		"name": "piezo2_low_window",
		"address": 35,
		"access":"r",
		"signed": "signed",
		"desc": 'position limit low Volts'
	},
	{
		"name": "piezo2_low_window",
		"address": 35,
		"access":"w",
		"signed": "signed",
		"desc": 'position limit low Volts'
	},
	{
		"name": "piezo2_cntl",
		"address": 36,
		"access":"r",
		"signed": "unsigned",
		"desc": 'control bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "piezo2_cntl",
		"address": 36,
		"access":"w",
		"signed": "unsigned",
		"desc": 'control bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "piezo2_stat",
		"address": 37,
		"access":"r",
		"signed": "unsigned",
		"desc": 'status bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "piezo2_brd_tmp",
		"address": 38,
		"access":"r",
		"signed": "unsigned",
		"desc": 'board temp deg C'
	},
	{
		"name": "piezo2_brd_sernum",
		"address": 39,
		"access":"r",
		"signed": "unsigned",
		"desc": 'serial number'
	},
	{
		"name": "piezo2_igain",
		"address": 40,
		"access":"r",
		"signed": "unsigned",
		"desc": 'I controller gain'
	},
	{
		"name": "piezo2_igain",
		"address": 40,
		"access":"w",
		"signed": "unsigned",
		"desc": 'I controller gain'
	},
	{
		"name": "cavity2_detune_err",
		"address": 41,
		"access":"r",
		"signed": "signed",
		"desc": 'detune frequency error Hz'
	},
	{
		"name": "motor3_acc",
		"address": 42,
		"access":"r",
		"signed": "unsigned",
		"desc": 'acceleration Hz\/\(Sec\^2\)'
	},
	{
		"name": "motor3_acc",
		"address": 42,
		"access":"w",
		"signed": "unsigned",
		"desc": 'acceleration Hz\/\(Sec\^2\)'
	},
	{
		"name": "motor3_vlcty",
		"address": 43,
		"access":"r",
		"signed": "unsigned",
		"desc": 'velocity Hz\/Sec'
	},
	{
		"name": "motor3_vlcty",
		"address": 43,
		"access":"w",
		"signed": "unsigned",
		"desc": 'velocity Hz\/Sec'
	},
	{
		"name": "motor3_steps",
		"address": 44,
		"access":"r",
		"signed": "unsigned",
		"desc": 'steps Hz'
	},
	{
		"name": "motor3_steps",
		"address": 44,
		"access":"w",
		"signed": "unsigned",
		"desc": 'steps Hz'
	},
	{
		"name": "motor3_sgn_steps",
		"address": 45,
		"access":"r",
		"signed": "signed",
		"desc": 'signed steps Hz'
	},
	{
		"name": "motor3_abs_steps",
		"address": 46,
		"access":"r",
		"signed": "unsigned",
		"desc": 'absolute steps Hz'
	},
	{
		"name": "motor3_drvI",
		"address": 47,
		"access":"r",
		"signed": "unsigned",
		"desc": 'drive current Amps'
	},
	{
		"name": "motor3_drvI",
		"address": 47,
		"access":"w",
		"signed": "unsigned",
		"desc": 'drive current Amps'
	},
	{
		"name": "motor3_brd_tmp",
		"address": 48,
		"access":"r",
		"signed": "unsigned",
		"desc": 'board temp deg C'
	},
	{
		"name": "motor3_cntl",
		"address": 49,
		"access":"r",
		"signed": "unsigned",
		"desc": 'control bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "motor3_cntl",
		"address": 49,
		"access":"w",
		"signed": "unsigned",
		"desc": 'control bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "motor3_stat",
		"address": 50,
		"access":"r",
		"signed": "unsigned",
		"desc": 'status bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "piezo3_dac1",
		"address": 51,
		"access":"r",
		"signed": "signed",
		"desc": 'drive dac1 set point Volts'
	},
	{
		"name": "piezo3_dac1",
		"address": 51,
		"access":"w",
		"signed": "signed",
		"desc": 'drive dac1 set point Volts'
	},
	{
		"name": "piezo3_dac2",
		"address": 52,
		"access":"r",
		"signed": "signed",
		"desc": 'drive dac2 set point Volts'
	},
	{
		"name": "piezo3_dac2",
		"address": 52,
		"access":"w",
		"signed": "signed",
		"desc": 'drive dac2 set point Volts'
	},
	{
		"name": "piezo3_adc1",
		"address": 53,
		"access":"r",
		"signed": "signed",
		"desc": 'position readback adc1 Volts'
	},
	{
		"name": "piezo3_adc2",
		"address": 54,
		"access":"r",
		"signed": "signed",
		"desc": 'position readback adc2 Volts'
	},
	{
		"name": "piezo3_high_window",
		"address": 55,
		"access":"r",
		"signed": "signed",
		"desc": 'position limit high Volts'
	},
	{
		"name": "piezo3_high_window",
		"address": 55,
		"access":"w",
		"signed": "signed",
		"desc": 'position limit high Volts'
	},
	{
		"name": "piezo3_low_window",
		"address": 56,
		"access":"r",
		"signed": "signed",
		"desc": 'position limit low Volts'
	},
	{
		"name": "piezo3_low_window",
		"address": 56,
		"access":"w",
		"signed": "signed",
		"desc": 'position limit low Volts'
	},
	{
		"name": "piezo3_cntl",
		"address": 57,
		"access":"r",
		"signed": "unsigned",
		"desc": 'control bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "piezo3_cntl",
		"address": 57,
		"access":"w",
		"signed": "unsigned",
		"desc": 'control bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "piezo3_stat",
		"address": 58,
		"access":"r",
		"signed": "unsigned",
		"desc": 'status bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "piezo3_brd_tmp",
		"address": 59,
		"access":"r",
		"signed": "unsigned",
		"desc": 'board temp deg C'
	},
	{
		"name": "piezo3_brd_sernum",
		"address": 60,
		"access":"r",
		"signed": "unsigned",
		"desc": 'serial number'
	},
	{
		"name": "piezo3_igain",
		"address": 61,
		"access":"r",
		"signed": "unsigned",
		"desc": 'I controller gain'
	},
	{
		"name": "piezo3_igain",
		"address": 61,
		"access":"w",
		"signed": "unsigned",
		"desc": 'I controller gain'
	},
	{
		"name": "cavity3_detune_err",
		"address": 62,
		"access":"r",
		"signed": "signed",
		"desc": 'detune frequency error Hz'
	},
	{
		"name": "motor4_acc",
		"address": 63,
		"access":"r",
		"signed": "unsigned",
		"desc": 'acceleration Hz\/\(Sec\^2\)'
	},
	{
		"name": "motor4_acc",
		"address": 63,
		"access":"w",
		"signed": "unsigned",
		"desc": 'acceleration Hz\/\(Sec\^2\)'
	},
	{
		"name": "motor4_vlcty",
		"address": 64,
		"access":"r",
		"signed": "unsigned",
		"desc": 'velocity Hz\/Sec'
	},
	{
		"name": "motor4_vlcty",
		"address": 64,
		"access":"w",
		"signed": "unsigned",
		"desc": 'velocity Hz\/Sec'
	},
	{
		"name": "motor4_steps",
		"address": 65,
		"access":"r",
		"signed": "unsigned",
		"desc": 'steps Hz'
	},
	{
		"name": "motor4_steps",
		"address": 65,
		"access":"w",
		"signed": "unsigned",
		"desc": 'steps Hz'
	},
	{
		"name": "motor4_sgn_steps",
		"address": 66,
		"access":"r",
		"signed": "signed",
		"desc": 'signed steps Hz'
	},
	{
		"name": "motor4_abs_steps",
		"address": 67,
		"access":"r",
		"signed": "unsigned",
		"desc": 'absolute steps Hz'
	},
	{
		"name": "motor4_drvI",
		"address": 68,
		"access":"r",
		"signed": "unsigned",
		"desc": 'drive current Amps'
	},
	{
		"name": "motor4_drvI",
		"address": 68,
		"access":"w",
		"signed": "unsigned",
		"desc": 'drive current Amps'
	},
	{
		"name": "motor4_brd_tmp",
		"address": 69,
		"access":"r",
		"signed": "unsigned",
		"desc": 'board temp deg C'
	},
	{
		"name": "motor4_cntl",
		"address": 70,
		"access":"r",
		"signed": "unsigned",
		"desc": 'control bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "motor4_cntl",
		"address": 70,
		"access":"w",
		"signed": "unsigned",
		"desc": 'control bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "motor4_stat",
		"address": 71,
		"access":"r",
		"signed": "unsigned",
		"desc": 'status bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "piezo4_dac1",
		"address": 72,
		"access":"r",
		"signed": "signed",
		"desc": 'drive dac1 set point Volts'
	},
	{
		"name": "piezo4_dac1",
		"address": 72,
		"access":"w",
		"signed": "signed",
		"desc": 'drive dac1 set point Volts'
	},
	{
		"name": "piezo4_dac2",
		"address": 73,
		"access":"r",
		"signed": "signed",
		"desc": 'drive dac2 set point Volts'
	},
	{
		"name": "piezo4_dac2",
		"address": 73,
		"access":"w",
		"signed": "signed",
		"desc": 'drive dac2 set point Volts'
	},
	{
		"name": "piezo4_adc1",
		"address": 74,
		"access":"r",
		"signed": "signed",
		"desc": 'position readback adc1 Volts'
	},
	{
		"name": "piezo4_adc2",
		"address": 75,
		"access":"r",
		"signed": "signed",
		"desc": 'position readback adc2 Volts'
	},
	{
		"name": "piezo4_high_window",
		"address": 76,
		"access":"r",
		"signed": "signed",
		"desc": 'position limit high Volts'
	},
	{
		"name": "piezo4_high_window",
		"address": 76,
		"access":"w",
		"signed": "signed",
		"desc": 'position limit high Volts'
	},
	{
		"name": "piezo4_low_window",
		"address": 77,
		"access":"r",
		"signed": "signed",
		"desc": 'position limit low Volts'
	},
	{
		"name": "piezo4_low_window",
		"address": 77,
		"access":"w",
		"signed": "signed",
		"desc": 'position limit low Volts'
	},
	{
		"name": "piezo4_cntl",
		"address": 78,
		"access":"r",
		"signed": "unsigned",
		"desc": 'control bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "piezo4_cntl",
		"address": 78,
		"access":"w",
		"signed": "unsigned",
		"desc": 'control bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "piezo4_stat",
		"address": 79,
		"access":"r",
		"signed": "unsigned",
		"desc": 'status bits',
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		}
	},
	{
		"name": "piezo4_brd_tmp",
		"address": 80,
		"access":"r",
		"signed": "unsigned",
		"desc": 'board temp deg C'
	},
	{
		"name": "piezo4_brd_sernum",
		"address": 81,
		"access":"r",
		"signed": "unsigned",
		"desc": 'serial number'
	},
	{
		"name": "piezo4_igain",
		"address": 82,
		"access":"r",
		"signed": "unsigned",
		"desc": 'I controller gain'
	},
	{
		"name": "piezo4_igain",
		"address": 82,
		"access":"w",
		"signed": "unsigned",
		"desc": 'I controller gain'
	},
	{
		"name": "cavity4_detune_err",
		"address": 83,
		"access":"r",
		"signed": "signed",
		"desc": 'detune frequency error Hz'
	}
	]
}
