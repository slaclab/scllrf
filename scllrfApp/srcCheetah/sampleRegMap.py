# The format of a python dictionary and a json file are nearly identical, but
# python allows comments and json doesn't. Put that "d = " at the beginning and
# you have a python dictionary.
d = { "name" : "example",
     'registers': [
	# This  is the least amount of information needed to access a register.
	# Unles specified otherwise, a register is assumed to be read only.
	{"name" : "minimal",
		"address" : 0x000002
	},
	# This includes all fields expected. If it doesn't meet your needs, we can
	# change it.
	{
	# Unique name, limit name to 50 characters of the set a-z A-Z 0-9 _ - : [ ] < > ;
	"name" : "maximal_analog_in",
	# address may be in decimal if you prefer. Use "0x" prefix for hex.
		"address"     : 0x000003,
	# access is generally "r" (read only) or "w" (write only). Should we support rw?
		"access"      : "r",
	# bit_width will be used to limit the range of write values, and sign extend if needed.
		"bit_width"   : 8,
	# The bit position of the least significant bit. So far not used.
		"lsBit"       : 0,
	# For an array, this is the number of elements in the array. Sequential addresses are assumed.
		"nelms"       : 1,
	# egu is the human-readable string showing the units ("Hz", "V", "degrees", etc). 16 char. max.
		"egu"         : "My Units",
	# use "signed" or "unsigned" data. Defaults to signed if not specified.
		"signed"      : "unsigned",
	# range_top will be used to limit the values written to a register or validate/alarm read value.
		"range_top"   : 12,
	# range_bottom will be used to limit the values written to a register or validate/alarm read value.
		"range_bottom": 1,
	# An equation to translate register value to "egu" units for reading, or vice versa for writing.
	# No current plans to accomodate read/write registers here.
		"scale_eqn"   : "x",
	# A descriptive string to remind users what this register does. 40 char. max.
		"desc"        : "Every field",
	# If a register is a bit map or has flags in it, specify the name and mask for each.
 		"bits" :
		{	"wav_ready" : 0x100,
			"bar"       : 0x200
		}
	},
	# This includes all fields expected. If it doesn't meet your needs, we can
	# change it.
	{
	# Unique name, limit name to 50 characters of the set a-z A-Z 0-9 _ - : [ ] < > ;
	"name" : "maximal_analog_out",
	# address may be in decimal if you prefer. Use "0x" prefix for hex.
		"address"     : 0x000004,
	# access is generally "r" (read only) or "w" (write only). Should we support rw?
		"access"      : "w",
	# bit_width will be used to limit the range of write values, and sign extend if needed.
		"bit_width"   : 8,
	# The bit position of the least significant bit. So far not used.
		"lsBit"       : 0,
	# For an array, this is the number of elements in the array. Sequential addresses are assumed.
		"nelms"       : 1,
	# egu is the human-readable string showing the units ("Hz", "V", "degrees", etc). 16 char. max.
		"egu"         : "My Units",
	# use "signed" or "unsigned" data. Defaults to signed if not specified.
		"signed"      : "unsigned",
	# range_top will be used to limit the values written to a register or validate/alarm read value.
		"range_top"   : 12,
	# range_bottom will be used to limit the values written to a register or validate/alarm read value.
		"range_bottom": 1,
	# An equation to translate register value to "egu" units for reading, or vice versa for writing.
	# No current plans to accomodate read/write registers here.
		"scale_eqn"   : "x",
	# A descriptive string to remind users what this register does. 40 char. max.
		"desc"        : "Every field",
	# If a register is a bit map or has flags in it, specify the name and mask for each.
 		"bits" :
		{	"wav_ready" : 0x100,
			"bar"       : 0x200
		}
	},
	# This includes all fields expected. If it doesn't meet your needs, we can
	# change it.
	{
	# Unique name, limit name to 50 characters of the set a-z A-Z 0-9 _ - : [ ] < > ;
	"name" : "maximal_waveform_in",
	# address may be in decimal if you prefer. Use "0x" prefix for hex.
		"address"     : 0x000005,
	# access is generally "r" (read only) or "w" (write only). Should we support rw?
		"access"      : "r",
	# bit_width will be used to limit the range of write values, and sign extend if needed.
		"bit_width"   : 8,
	# The bit position of the least significant bit. So far not used.
		"lsBit"       : 0,
	# For an array, this is the number of elements in the array. Sequential addresses are assumed.
		"nelms"       : 128,
	# egu is the human-readable string showing the units ("Hz", "V", "degrees", etc). 16 char. max.
		"egu"         : "My Units",
	# use "signed" or "unsigned" data. Defaults to signed if not specified.
		"signed"      : "unsigned",
	# range_top will be used to limit the values written to a register or validate/alarm read value.
		"range_top"   : 12,
	# range_bottom will be used to limit the values written to a register or validate/alarm read value.
		"range_bottom": 1,
	# An equation to translate register value to "egu" units for reading, or vice versa for writing.
	# No current plans to accomodate read/write registers here.
		"scale_eqn"   : "x",
	# A descriptive string to remind users what this register does. 40 char. max.
		"desc"        : "Every field",
	# If a register is a bit map or has flags in it, specify the name and mask for each.
 		"bits" :
		{	"wav_ready" : 0x100,
			"bar"       : 0x200
		}
	},
	# This includes all fields expected. If it doesn't meet your needs, we can
	# change it.
	{
	# Unique name, limit name to 50 characters of the set a-z A-Z 0-9 _ - : [ ] < > ;
	"name" : "maximal_waveform_out",
	# address may be in decimal if you prefer. Use "0x" prefix for hex.
		"address"     : 0x000006,
	# access is generally "r" (read only) or "w" (write only). Should we support rw?
		"access"      : "w",
	# bit_width will be used to limit the range of write values, and sign extend if needed.
		"bit_width"   : 8,
	# The bit position of the least significant bit. So far not used.
		"lsBit"       : 0,
	# For an array, this is the number of elements in the array. Sequential addresses are assumed.
		"nelms"       : 128,
	# egu is the human-readable string showing the units ("Hz", "V", "degrees", etc). 16 char. max.
		"egu"         : "My Units",
	# use "signed" or "unsigned" data. Defaults to signed if not specified.
		"signed"      : "unsigned",
	# range_top will be used to limit the values written to a register or validate/alarm read value.
		"range_top"   : 12,
	# range_bottom will be used to limit the values written to a register or validate/alarm read value.
		"range_bottom": 1,
	# An equation to translate register value to "egu" units for reading, or vice versa for writing.
	# No current plans to accomodate read/write registers here.
		"scale_eqn"   : "x",
	# A descriptive string to remind users what this register does. 40 char. max.
		"desc"        : "Every field",
	# If a register is a bit map or has flags in it, specify the name and mask for each.
 		"bits" :
		{	"wav_ready" : 0x100,
			"bar"       : 0x200
		}
	},
	{"name" : "arc_faults",
		"bits":
		{	"cav1" : 0x0001,
			"cav2" : 0x0002,
			"cav3" : 0x0004,
			"cav4" : 0x0008,
			"cav1Mask" : 0x0100,
			"cav2Mask" : 0x0200,
			"cav3Mask" : 0x0400,
			"cav4Mask" : 0x0800
		},
		"address" : 0xabcdef,
		"access"  : "rw",
		"desc"    : "Arc fault bits and mask for half a cryo module"
	},
	{
	# Here's an idea for how you might specify a shift register in this format
		"name" : "shift_reg_signal",
		"address" : 0x000320,
		"access"  : "w",
		"desc"    : "signal FPGA to fill shift register"
	},
	{
		"name" : "shift_reg_first",
		"address" : 0x000321,
		"access"  : "r",
		"desc"    : "data you get on the first read"
	},
	{
		"name" : "shift_reg_second",
		"address" : 0x000321,
		"access"  : "r",
		"desc"    : "data you get on the second read"
	},
	{
		"name" : "shift_reg_third",
		"address" : 0x000321,
		"access"  : "r",
		"desc"    : "data you get on the third read"
	}
	# comments can include things like "continue the above pattern up to the 
	# forty-second register" or "leave at least 0.1 seconds between reads"
	]
}
		