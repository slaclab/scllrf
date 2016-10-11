d = { "name" : "scllrfRES",
		 'registers': [
	{
		"name": "motor1_acc",
		"address": 0x0,
		"access": "rw",
		"signed": "unsigned",
		"desc": "acceleration Hz/\(Sec^2\)"
	},
	{
		"name": "motor1_vlcty",
		"address": 0x1,
		"access": "rw",
		"signed": "unsigned",
		"desc": "velocity Hz/Sec"
	},
	{
		"name": "motor1_steps",
		"address": 0x2,
		"access": "rw",
		"signed": "unsigned",
		"desc": "steps Hz"
	},
	{
		"name": "motor1_sgn_steps",
		"address": 0x3,
		"access": "r",
		"signed": "signed",
		"desc": "signed steps Hz"
	},
	{
		"name": "motor1_abs_steps",
		"address": 0x4,
		"access": "r",
		"signed": "unsigned",
		"desc": "absolute steps Hz"
	},
	{
		"name": "motor1_lacc",
		"address": 0x5,
		"access": "r",
		"signed": "unsigned",
		"desc": "last acceleration Hz/\(Sec^2\)"
	},
	{
		"name": "motor1_lvlcty",
		"address": 0x6,
		"access": "r",
		"signed": "unsigned",
		"desc": "last velocity Hz/Sec"
	},
	{
		"name": "motor1_lsteps",
		"address": 0x7,
		"access": "r",
		"signed": "unsigned",
		"desc": "last steps Hz"
	},
	{
		"name": "motor1_drvI",
		"address": 0x8,
		"access": "rw",
		"signed": "unsigned",
		"desc": "drive current Amps"
	},
	{
		"name": "motor1_brd_tmp",
		"address": 0x9,
		"access": "r",
		"signed": "unsigned",
		"desc": "board temp deg C"
	},
	{
		"name": "motor1_cntl",
		"address": 0xA,
		"access": "rw",
		"signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
		"desc": "control bits"
	},
	{
		"name": "motor1_stat",
		"address": 0xB,
		"access": "r",
		"signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
		"desc": "status bits"
	},
	{
		"name": "piezo1_dac1",
		"address": 0xC,
		"access": "rw",
		"signed": "signed",
		"desc": "drive dac1 set point Volts"
	},
	{
		"name": "piezo1_dac2",
		"address": 0xD,
		"access": "rw",
		"signed": "signed",
		"desc": "drive dac2 set point Volts"
	},
	{
		"name": "piezo1_adc1",
		"address": 0xE,
		"access": "r",
		"signed": "signed",
		"desc": "position readback adc1 Volts"
	},
	{
		"name": "piezo1_adc2",
		"address": 0xF,
		"access": "r",
		"signed": "signed",
		"desc": "position readback adc2 Volts"
	},
	{
		"name": "piezo1_high_window",
		"address": 0x10,
		"access": "rw",
		"signed": "signed",
		"desc": "position limit high Volts"
	},
	{
		"name": "piezo1_low_window",
		"address": 0x11,
		"access": "rw",
		"signed": "signed",
		"desc": "position limit low Volts"
	},
	{
		"name": "piezo1_cntl",
		"address": 0x12,
		"access": "rw",
		"signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
		"desc": "control bits"
	},
	{
		"name": "piezo1_stat",
		"address": 0x13,
		"access": "r",
		"signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
		"desc": "status bits"
	},
	{
		"name": "piezo1_brd_tmp",
		"address": 0x14,
		"access": "r",
		"signed": "unsigned",
		"desc": "board temp deg C"
	},
	{
		"name": "piezo1_brd_sernum",
		"address": 0x15,
		"access": "r",
		"signed": "unsigned",
		"desc": "serial number"
	},
	{
		"name": "piezo1_igain",
		"address": 0x16,
		"access": "rw",
		"signed": "unsigned",
		"desc": "I controller gain"
	},
	{
		"name": "cavity1_detune_err",
		"address": 0x17,
		"access": "r",
		"signed": "signed",
		"desc": "detune frequency error Hz"
	},
	{
		"name": "motor2_acc",
		"address": 0x18,
		"access": "rw",
		"signed": "unsigned",
		"desc": "acceleration Hz/\(Sec^2\)"
	},
	{
		"name": "motor2_vlcty",
		"address": 0x19,
		"access": "rw",
		"signed": "unsigned",
		"desc": "velocity Hz/Sec"
	},
	{
		"name": "motor2_steps",
		"address": 0x1A,
		"access": "rw",
		"signed": "unsigned",
		"desc": "steps Hz"
	},
	{
		"name": "motor2_sgn_steps",
		"address": 0x1B,
		"access": "r",
		"signed": "signed",
		"desc": "signed steps Hz"
	},
	{
		"name": "motor2_abs_steps",
		"address": 0x1C,
		"access": "r",
		"signed": "unsigned",
		"desc": "absolute steps Hz"
	},
	{
		"name": "motor2_lacc",
		"address": 0x1D,
		"access": "r",
		"signed": "unsigned",
		"desc": "last acceleration Hz/\(Sec^2\)"
	},
	{
		"name": "motor2_lvlcty",
		"address": 0x1E,
		"access": "r",
		"signed": "unsigned",
		"desc": "last velocity Hz/Sec"
	},
	{
		"name": "motor2_lsteps",
		"address": 0x1F,
		"access": "r",
		"signed": "unsigned",
		"desc": "last steps Hz"
	},
	{
		"name": "motor2_drvI",
		"address": 0x20,
		"access": "rw",
		"signed": "unsigned",
		"desc": "motor drive current Amps"
	},
	{
		"name": "motor2_brd_tmp",
		"address": 0x21,
		"access": "r",
		"signed": "unsigned",
		"desc": "board temp deg C"
	},
	{
		"name": "motor2_cntl",
		"address": 0x22,
		"access": "rw",
		"signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
		"desc": "control bits"
	},
	{
		"name": "motor2_stat",
		"address": 0x23,
		"access": "r",
		"signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
		"desc": "status bits"
	},
	{
		"name": "piezo2_dac1",
		"address": 0x24,
		"access": "rw",
		"signed": "signed",
		"desc": "drive dac1 set point Volts"
	},
	{
		"name": "piezo2_dac2",
		"address": 0x25,
		"access": "rw",
		"signed": "signed",
		"desc": "drive dac2 set point Volts"
	},
	{
		"name": "piezo2_adc1",
		"address": 0x26,
		"access": "r",
		"signed": "signed",
		"desc": "position readback adc1 Volts"
	},
	{
		"name": "piezo2_adc2",
		"address": 0x27,
		"access": "r",
		"signed": "signed",
		"desc": "position readback adc2 Volts"
	},
	{
		"name": "piezo2_high_window",
		"address": 0x28,
		"access": "rw",
		"signed": "signed",
		"desc": "position limit high Volts"
	},
	{
		"name": "piezo2_low_window",
		"address": 0x29,
		"access": "rw",
		"signed": "signed",
		"desc": "position limit low Volts"
	},
	{
		"name": "piezo2_cntl",
		"address": 0x2A,
		"access": "rw",
		"signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
		"desc": "control bits"
	},
	{
		"name": "piezo2_stat",
		"address": 0x2B,
		"access": "r",
		"signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
		"desc": "status bits"
	},
	{
		"name": "piezo2_brd_tmp",
		"address": 0x2C,
		"access": "r",
		"signed": "unsigned",
		"desc": "board temp deg C"
	},
	{
		"name": "piezo2_brd_sernum",
		"address": 0x2D,
		"access": "r",
		"signed": "unsigned",
		"desc": "serial number"
	},
	{
		"name": "piezo2_igain",
		"address": 0x2E,
		"access": "rw",
		"signed": "unsigned",
		"desc": "I controller gain"
	},
	{
		"name": "cavity2_detune_err",
		"address": 0x2F,
		"access": "r",
		"signed": "signed",
		"desc": "detune frequency error Hz"
	},
	{
		"name": "motor3_acc",
		"address": 0x30,
		"access": "rw",
		"signed": "unsigned",
		"desc": "acceleration Hz/\(Sec^2\)"
	},
	{
		"name": "motor3_vlcty",
		"address": 0x31,
		"access": "rw",
		"signed": "unsigned",
		"desc": "velocity Hz/Sec"
	},
	{
		"name": "motor3_steps",
		"address": 0x32,
		"access": "rw",
		"signed": "unsigned",
		"desc": "steps Hz"
	},
	{
		"name": "motor3_sgn_steps",
		"address": 0x33,
		"access": "r",
		"signed": "signed",
		"desc": "signed steps Hz"
	},
	{
		"name": "motor3_abs_steps",
		"address": 0x34,
		"access": "r",
		"signed": "unsigned",
		"desc": "absolute steps Hz"
	},
	{
		"name": "motor3_lacc",
		"address": 0x35,
		"access": "r",
		"signed": "unsigned",
		"desc": "last acceleration Hz/\(Sec^2\)"
	},
	{
		"name": "motor3_lvlcty",
		"address": 0x36,
		"access": "r",
		"signed": "unsigned",
		"desc": "last velocity Hz/Sec"
	},
	{
		"name": "motor3_lsteps",
		"address": 0x37,
		"access": "r",
		"signed": "unsigned",
		"desc": "last steps Hz"
	},
	{
		"name": "motor3_drvI",
		"address": 0x38,
		"access": "rw",
		"signed": "unsigned",
		"desc": "drive current Amps"
	},
	{
		"name": "motor3_brd_tmp",
		"address": 0x39,
		"access": "r",
		"signed": "unsigned",
		"desc": "board temp deg C"
	},
	{
		"name": "motor3_cntl",
		"address": 0x3A,
		"access": "rw",
		"signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
		"desc": "control bits"
	},
	{
		"name": "motor3_stat",
		"address": 0x3B,
		"access": "r",
		"signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
		"desc": "status bits"
	},
	{
		"name": "piezo3_dac1",
		"address": 0x3C,
		"access": "rw",
		"signed": "signed",
		"desc": "drive dac1 set point Volts"
	},
	{
		"name": "piezo3_dac2",
		"address": 0x3D,
		"access": "rw",
		"signed": "signed",
		"desc": "drive dac2 set point Volts"
	},
	{
		"name": "piezo3_adc1",
		"address": 0x3E,
		"access": "r",
		"signed": "signed",
		"desc": "position readback adc1 Volts"
	},
	{
		"name": "piezo3_adc2",
		"address": 0x3F,
		"access": "r",
		"signed": "signed",
		"desc": "position readback adc2 Volts"
	},
	{
		"name": "piezo3_high_window",
		"address": 0x40,
		"access": "rw",
		"signed": "signed",
		"desc": "position limit high Volts"
	},
	{
		"name": "piezo3_low_window",
		"address": 0x41,
		"access": "rw",
		"signed": "signed",
		"desc": "position limit low Volts"
	},
	{
		"name": "piezo3_cntl",
		"address": 0x42,
		"access": "rw",
		"signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
		"desc": "control bits"
	},
	{
		"name": "piezo3_stat",
		"address": 0x43,
		"access": "r",
		"signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
		"desc": "status bits"
	},
	{
		"name": "piezo3_brd_tmp",
		"address": 0x44,
		"access": "r",
		"signed": "unsigned",
		"desc": "board temp deg C"
	},
	{
		"name": "piezo3_brd_sernum",
		"address": 0x45,
		"access": "r",
		"signed": "unsigned",
		"desc": "serial number"
	},
	{
		"name": "piezo3_igain",
		"address": 0x46,
		"access": "rw",
		"signed": "unsigned",
		"desc": "I controller gain"
	},
	{
		"name": "cavity3_detune_err",
		"address": 0x47,
		"access": "r",
		"signed": "signed",
		"desc": "detune frequency error Hz"
	},
	{
		"name": "motor4_acc",
		"address": 0x48,
		"access": "rw",
		"signed": "unsigned",
		"desc": "acceleration Hz/\(Sec^2\)"
	},
	{
		"name": "motor4_vlcty",
		"address": 0x49,
		"access": "rw",
		"signed": "unsigned",
		"desc": "velocity Hz/Sec"
	},
	{
		"name": "motor4_steps",
		"address": 0x4A,
		"access": "rw",
		"signed": "unsigned",
		"desc": "steps Hz"
	},
	{
		"name": "motor4_sgn_steps",
		"address": 0x4B,
		"access": "r",
		"signed": "signed",
		"desc": "signed steps Hz"
	},
	{
		"name": "motor4_abs_steps",
		"address": 0x4C,
		"access": "r",
		"signed": "unsigned",
		"desc": "absolute steps Hz"
	},
	{
		"name": "motor4_lacc",
		"address": 0x4D,
		"access": "r",
		"signed": "unsigned",
		"desc": "last acceleration Hz/\(Sec^2\)"
	},
	{
		"name": "motor4_lvlcty",
		"address": 0x4E,
		"access": "r",
		"signed": "unsigned",
		"desc": "last velocity Hz/Sec"
	},
	{
		"name": "motor4_lsteps",
		"address": 0x4F,
		"access": "r",
		"signed": "unsigned",
		"desc": "last steps Hz"
	},
	{
		"name": "motor4_drvI",
		"address": 0x50,
		"access": "rw",
		"signed": "unsigned",
		"desc": "drive current Amps"
	},
	{
		"name": "motor4_brd_tmp",
		"address": 0x51,
		"access": "r",
		"signed": "unsigned",
		"desc": "board temp deg C"
	},
	{
		"name": "motor4_cntl",
		"address": 0x52,
		"access": "rw",
		"signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
		"desc": "control bits"
	},
	{
		"name": "motor4_stat",
		"address": 0x53,
		"access": "r",
		"signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
		"desc": "status bits"
	},
	{
		"name": "piezo4_dac1",
		"address": 0x54,
		"access": "rw",
		"signed": "signed",
		"desc": "drive dac1 set point Volts"
	},
	{
		"name": "piezo4_dac2",
		"address": 0x55,
		"access": "rw",
		"signed": "signed",
		"desc": "drive dac2 set point Volts"
	},
	{
		"name": "piezo4_adc1",
		"address": 0x56,
		"access": "r",
		"signed": "signed",
		"desc": "position readback adc1 Volts"
	},
	{
		"name": "piezo4_adc2",
		"address": 0x57,
		"access": "r",
		"signed": "signed",
		"desc": "position readback adc2 Volts"
	},
	{
		"name": "piezo4_high_window",
		"address": 0x58,
		"access": "rw",
		"signed": "signed",
		"desc": "position limit high Volts"
	},
	{
		"name": "piezo4_low_window",
		"address": 0x59,
		"access": "rw",
		"signed": "signed",
		"desc": "position limit low Volts"
	},
	{
		"name": "piezo4_cntl",
		"address": 0x5A,
		"access": "rw",
		"signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
		"desc": "control bits"
	},
	{
		"name": "piezo4_stat",
		"address": 0x5B,
		"access": "r",
		"signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
		"desc": "status bits"
	},
	{
		"name": "piezo4_brd_tmp",
		"address": 0x5C,
		"access": "r",
		"signed": "unsigned",
		"desc": "board temp deg C"
	},
	{
		"name": "piezo4_brd_sernum",
		"address": 0x5D,
		"access": "r",
		"signed": "unsigned",
		"desc": "serial number"
	},
	{
		"name": "piezo4_igain",
		"address": 0x5E,
		"access": "rw",
		"signed": "unsigned",
		"desc": "I controller gain"
	},
	{
		"name": "cavity4_detune_err",
		"address": 0x5F,
		"access": "r",
		"signed": "signed",
		"desc": "detune frequency error Hz"
	}
]
	}
