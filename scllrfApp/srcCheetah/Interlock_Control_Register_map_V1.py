from ctypes.test.test_bitfields import BITS
d = { "name" : "scllrfINT",
    'registers': [
  {
    "name": "C1WAV",
    "address": 0x0,
    "access": "r",
    "signed": "unsigned",
    "description": "cavity1 arc detector voltage readback"
  },
  {
    "name": "C2WAV",
    "address": 0x1,
    "access": "r",
    "signed": "unsigned",
    "description": "cavity2 arc detector voltage readback"
  },
  {
    "name": "C3WAV",
    "address": 0x2,
    "access": "r",
    "signed": "unsigned",
    "description": "cavity3 arc detector voltage readback"
  },
  {
    "name": "C4WAV",
    "address": 0x3,
    "access": "r",
    "signed": "unsigned",
    "description": "cavity4 arc detector voltage readback"
  },
  {
    "name": "C1WAPS",
    "address": 0x4,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 1 arc detector power supply set voltage"
  },
  {
    "name": "C2WAPS",
    "address": 0x5,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 2 arc detector power supply set voltage"
  },
  {
    "name": "C3WAPS",
    "address": 0x6,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 3 arc detector power supply set voltage"
  },
  {
    "name": "C4WAPS",
    "address": 0x7,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 4 arc detector power supply set voltage"
  },
  {
    "name": "C1WAL",
    "address": 0x8,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 1 arc detector limit volts"
  },
  {
    "name": "C2WAL",
    "address": 0x9,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 2 arc detector limit volts"
  },
  {
    "name": "C3WAL",
    "address": 0xA,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 3 arc detector limit volts"
  },
  {
    "name": "C4WAL",
    "address": 0xB,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 4 arc detector limit volts"
  },
  {
    "name": "C1WAP",
    "address": 0xC,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 1 arc detector period usec"
  },
  {
    "name": "C2WAP",
    "address": 0xD,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 2 arc detector period usec"
  },
  {
    "name": "C3WAP",
    "address": 0xE,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 3 arc detector period usec"
  },
  {
    "name": "C4WAP",
    "address": 0xF,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 4 arc detector period usec"
  },
  {
    "name": "XCWAD",
    "address": 0x10,
    "access": "r",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "arc faults latched and instantaneous"
  },
  {
    "name": "XCWAM",
    "address": 0x11,
    "access": "rw",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "arc fault mask"
  },
  {
    "name": "XCWAT",
    "address": 0x12,
    "access": "rw",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "arc detector led test"
  },
  {
    "name": "XATFLT",
    "address": 0x13,
    "access": "r",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "arc detector led test fault status"
  },
  {
    "name": "XCWAFC",
    "address": 0x14,
    "access": "rw",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "arc fault clear"
  },
  {
    "name": "C1WWT",
    "address": 0x15,
    "access": "r",
    "signed": "signed",
    "description": "cavity 1 window voltage readback"
  },
  {
    "name": "C2WWT",
    "address": 0x16,
    "access": "r",
    "signed": "signed",
    "description": "cavity 2 window voltage readback"
  },
  {
    "name": "C3WWT",
    "address": 0x17,
    "access": "r",
    "signed": "signed",
    "description": "cavity 3 window voltage readback"
  },
  {
    "name": "C4WWT",
    "address": 0x18,
    "access": "r",
    "signed": "signed",
    "description": "cavity 4 window voltage readback"
  },
  {
    "name": "C1WWL",
    "address": 0x19,
    "access": "rw",
    "signed": "signed",
    "description": "cavity 1 window limit volts"
  },
  {
    "name": "C2WWL",
    "address": 0x1A,
    "access": "rw",
    "signed": "signed",
    "description": "cavity 2 window limit volts"
  },
  {
    "name": "C3WWL",
    "address": 0x1B,
    "access": "rw",
    "signed": "signed",
    "description": "cavity 3 window limit volts"
  },
  {
    "name": "C4WWL",
    "address": 0x1C,
    "access": "rw",
    "signed": "signed",
    "description": "cavity 4 window limit volts"
  },
  {
    "name": "XCWWD",
    "address": 0x1D,
    "access": "r",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "window faults latched and instantaneous"
  },
  {
    "name": "XCWWTS",
    "address": 0x1E,
    "access": "rw",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "window ir sensor test"
  },
  {
    "name": "XCWWM",
    "address": 0x1F,
    "access": "r",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "window fault mask"
  },
  {
    "name": "XCWWFC",
    "address": 0x20,
    "access": "r",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "window fault clear"
  },
  {
    "name": "C1STMPI",
    "address": 0x21,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 1 stepper motor PT100 excitaion current mA"
  },
  {
    "name": "C2STMPI",
    "address": 0x22,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 2 stepper motor PT100 excitaion current mA"
  },
  {
    "name": "C3STMPI",
    "address": 0x23,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 3 stepper motor PT100 excitaion current mA"
  },
  {
    "name": "C4STMPI",
    "address": 0x24,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 4 stepper motor PT100 excitaion current mA"
  },
  {
    "name": "C1STMPV",
    "address": 0x25,
    "access": "r",
    "signed": "unsigned",
    "description": "cavity 1 stepper motor PT100 voltage readback"
  },
  {
    "name": "C2STMPV",
    "address": 0x26,
    "access": "r",
    "signed": "unsigned",
    "description": "cavity 2 stepper motor PT100 voltage readback"
  },
  {
    "name": "C3STMPV",
    "address": 0x27,
    "access": "r",
    "signed": "unsigned",
    "description": "cavity 3 stepper motor PT100 voltage readback"
  },
  {
    "name": "C4STMPV",
    "address": 0x28,
    "access": "r",
    "signed": "unsigned",
    "description": "cavity 4 stepper motor PT100 voltage readback"
  },
  {
    "name": "C1STMPVL",
    "address": 0x29,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 1 stepper motor PT100 voltage limit"
  },
  {
    "name": "C2STMPVL",
    "address": 0x2A,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 2 stepper motor PT100 voltage limit"
  },
  {
    "name": "C3STMPVL",
    "address": 0x2B,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 3 stepper motor PT100 voltage limit"
  },
  {
    "name": "C4STMPVL",
    "address": 0x2C,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 4 stepper motor PT100 voltage limit"
  },
  {
    "name": "XSTMPLS",
    "address": 0x2D,
    "access": "r",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "stepper motor temperature limit status"
  },
  {
    "name": "C1PLTMP1I",
    "address": 0x2E,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 1 coupler PT100 1 excitation current mA"
  },
  {
    "name": "C1PLTMP2I",
    "address": 0x2F,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 1 coupler PT100 2 excitation current mA"
  },
  {
    "name": "C2PLTMP1I",
    "address": 0x30,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 2 coupler PT100 1 excitation current mA"
  },
  {
    "name": "C2PLTMP2I",
    "address": 0x31,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 2 coupler PT100 2 excitation current mA"
  },
  {
    "name": "C3PLTMP1I",
    "address": 0x32,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 3 coupler PT100 1 excitation current mA"
  },
  {
    "name": "C3PLTMP2I",
    "address": 0x33,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 3 coupler PT100 2 excitation current mA"
  },
  {
    "name": "C4PLTMP1I",
    "address": 0x34,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 4 coupler PT100 1 excitation current mA"
  },
  {
    "name": "C4PLTMP2I",
    "address": 0x35,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 4 coupler PT100 2 excitation current mA"
  },
  {
    "name": "C1PLTMP1V",
    "address": 0x36,
    "access": "r",
    "signed": "unsigned",
    "description": "cavity 1 coupler PT100 1 voltage readback"
  },
  {
    "name": "C1PLTMP2V",
    "address": 0x37,
    "access": "r",
    "signed": "unsigned",
    "description": "cavity 1 coupler PT100 2 voltage readback"
  },
  {
    "name": "C2PLTMP1V",
    "address": 0x38,
    "access": "r",
    "signed": "unsigned",
    "description": "cavity 2 coupler PT100 1 voltage readback"
  },
  {
    "name": "C2PLTMP2V",
    "address": 0x39,
    "access": "r",
    "signed": "unsigned",
    "description": "cavity 2 coupler PT100 2 voltage readback"
  },
  {
    "name": "C3PLTMP1V",
    "address": 0x3A,
    "access": "r",
    "signed": "unsigned",
    "description": "cavity 3 coupler PT100 1 voltage readback"
  },
  {
    "name": "C3PLTMP2V",
    "address": 0x3B,
    "access": "r",
    "signed": "unsigned",
    "description": "cavity 3 coupler PT100 2 voltage readback"
  },
  {
    "name": "C4PLTMP1V",
    "address": 0x3C,
    "access": "r",
    "signed": "unsigned",
    "description": "cavity 4 coupler PT100 1 voltage readback"
  },
  {
    "name": "C4PLTMP2V",
    "address": 0x3D,
    "access": "r",
    "signed": "unsigned",
    "description": "cavity 4 coupler PT100 2 voltage readback"
  },
  {
    "name": "C1PLTMP1VL",
    "address": 0x3E,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 1 coupler PT100 1 voltage limit"
  },
  {
    "name": "C1PLTMP2VL",
    "address": 0x3F,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 1 coupler PT100 2 voltage limit"
  },
  {
    "name": "C2PLTMP1VL",
    "address": 0x40,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 2 coupler PT100 1 voltage limit"
  },
  {
    "name": "C2PLTMP2VL",
    "address": 0x41,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 2 coupler PT100 2 voltage limit"
  },
  {
    "name": "C3PLTMP1VL",
    "address": 0x42,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 3 coupler PT100 1 voltage limit"
  },
  {
    "name": "C3PLTMP2VL",
    "address": 0x43,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 3 coupler PT100 2 voltage limit"
  },
  {
    "name": "C4PLTMP1VL",
    "address": 0x44,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 4 coupler PT100 1 voltage limit"
  },
  {
    "name": "C4PLTMP2VL",
    "address": 0x45,
    "access": "rw",
    "signed": "unsigned",
    "description": "cavity 4 coupler PT100 2 voltage limit"
  },
  {
    "name": "XCPLTMPLS",
    "address": 0x46,
    "access": "r",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "coupler temperature limit status"
  },
  {
    "name": "C1PLFEP1I",
    "address": 0x47,
    "access": "r",
    "signed": "signed",
    "description": "coupler 1 electron probe 1 current readback uA"
  },
  {
    "name": "C1PLFEP2I",
    "address": 0x48,
    "access": "r",
    "signed": "signed",
    "description": "coupler 1 electron probe 2 current readback uA"
  },
  {
    "name": "C1PLFEP3I",
    "address": 0x49,
    "access": "r",
    "signed": "signed",
    "description": "coupler 1 electron probe 3 current readback uA"
  },
  {
    "name": "C2PLFEP1I",
    "address": 0x4A,
    "access": "r",
    "signed": "signed",
    "description": "coupler 2 electron probe 1 current readback uA"
  },
  {
    "name": "C2PLFEP2I",
    "address": 0x4B,
    "access": "r",
    "signed": "signed",
    "description": "coupler 2 electron probe 2 current readback uA"
  },
  {
    "name": "C2PLFEP3I",
    "address": 0x4C,
    "access": "r",
    "signed": "signed",
    "description": "coupler 2 electron probe 3 current readback uA"
  },
  {
    "name": "C3PLFEP1I",
    "address": 0x4D,
    "access": "r",
    "signed": "signed",
    "description": "coupler 3 electron probe 1 current readback uA"
  },
  {
    "name": "C3PLFEP2I",
    "address": 0x4E,
    "access": "r",
    "signed": "signed",
    "description": "coupler 3 electron probe 2 current readback uA"
  },
  {
    "name": "C3PLFEP3I",
    "address": 0x4F,
    "access": "r",
    "signed": "signed",
    "description": "coupler 3 electron probe 3 current readback uA"
  },
  {
    "name": "C4PLFEP1I",
    "address": 0x50,
    "access": "r",
    "signed": "signed",
    "description": "coupler 4 electron probe 1 current readback uA"
  },
  {
    "name": "C4PLFEP2I",
    "address": 0x51,
    "access": "r",
    "signed": "signed",
    "description": "coupler 4 electron probe 2 current readback uA"
  },
  {
    "name": "C4PLFEP3I",
    "address": 0x52,
    "access": "r",
    "signed": "signed",
    "description": "coupler 4 electron probe 3 current readback uA"
  },
  {
    "name": "C1PLFEP1ILH",
    "address": 0x53,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 1 electron probe 1 current limit high"
  },
  {
    "name": "C1PLFEP2ILH",
    "address": 0x54,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 1 electron probe 2 current limit high"
  },
  {
    "name": "C1PLFEP3ILH",
    "address": 0x55,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 1 electron probe 3 current limit high"
  },
  {
    "name": "C2PLFEP1ILH",
    "address": 0x56,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 2 electron probe 1 current limit high"
  },
  {
    "name": "C2PLFEP2ILH",
    "address": 0x57,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 2 electron probe 2 current limit high"
  },
  {
    "name": "C2PLFEP3ILH",
    "address": 0x58,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 2 electron probe 3 current limit high"
  },
  {
    "name": "C3PLFEP1ILH",
    "address": 0x59,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 3 electron probe 1 current limit high"
  },
  {
    "name": "C3PLFEP2ILH",
    "address": 0x5A,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 3 electron probe 2 current limit high"
  },
  {
    "name": "C3PLFEP3ILH",
    "address": 0x5B,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 3 electron probe 3 current limit high"
  },
  {
    "name": "C4PLFEP1ILH",
    "address": 0x5C,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 4 electron probe 1 current limit high"
  },
  {
    "name": "C4PLFEP2ILH",
    "address": 0x5D,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 4 electron probe 2 current limit high"
  },
  {
    "name": "C4PLFEP3ILH",
    "address": 0x5E,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 4 electron probe 3 current limit high"
  },
  {
    "name": "C1PLFEP1ILL",
    "address": 0x5F,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 1 electron probe 1 current limit low"
  },
  {
    "name": "C1PLFEP2ILL",
    "address": 0x60,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 1 electron probe 2 current limit low"
  },
  {
    "name": "C1PLFEP3ILL",
    "address": 0x61,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 1 electron probe 3 current limit low"
  },
  {
    "name": "C2PLFEP1ILL",
    "address": 0x62,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 2 electron probe 1 current limit low"
  },
  {
    "name": "C2PLFEP2ILL",
    "address": 0x63,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 2 electron probe 2 current limit low"
  },
  {
    "name": "C2PLFEP3ILL",
    "address": 0x64,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 2 electron probe 3 current limit low"
  },
  {
    "name": "C3PLFEP1ILL",
    "address": 0x65,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 3 electron probe 1 current limit low"
  },
  {
    "name": "C3PLFEP2ILL",
    "address": 0x66,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 3 electron probe 2 current limit low"
  },
  {
    "name": "C3PLFEP3ILL",
    "address": 0x67,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 3 electron probe 3 current limit low"
  },
  {
    "name": "C4PLFEP1ILL",
    "address": 0x68,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 4 electron probe 1 current limit low"
  },
  {
    "name": "C4PLFEP2ILL",
    "address": 0x69,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 4 electron probe 2 current limit low"
  },
  {
    "name": "C4PLFEP3ILL",
    "address": 0x6A,
    "access": "rw",
    "signed": "signed",
    "description": "coupler 4 electron probe 3 current limit low"
  },
  {
    "name": "XCPLFEPLS",
    "address": 0x6B,
    "access": "r",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "coupler electron probe limit status"
  },
  {
    "name": "XCPLFEPFC",
    "address": 0x6C,
    "access": "r",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "coupler electro probe fault clear"
  },
  {
    "name": "XCVF",
    "address": 0x6D,
    "access": "r",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "vacuum fault status"
  },
  {
    "name": "XCVFM",
    "address": 0x6E,
    "access": "rw",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "vacuum fault mask"
  },
  {
    "name": "XCVFC",
    "address": 0x6F,
    "access": "rw",
    "signed": "unsigned",
    "bits" :
    {
        "bit0": 0x01,
    },
    "description": "vacuum fault clear"
  },
  {
    "name": "XCIEN",
    "address": 0x70,
    "access": "r",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "interlock enable latch and present from FCC"
  },
  {
    "name": "XCIENM",
    "address": 0x71,
    "access": "rw",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "interlock enable latch and present from FCC mask"
  },
  {
    "name": "XCIENC",
    "address": 0x72,
    "access": "rw",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "interlock enable clear"
  },
  {
    "name": "XFFSD",
    "address": 0x73,
    "access": "r",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "FSD to field control latch and present"
  },
  {
    "name": "XFFSDM",
    "address": 0x74,
    "access": "rw",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "FSD to field control latch and present mask"
  },
  {
    "name": "XFFSDC",
    "address": 0x75,
    "access": "rw",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "FSD to field control clear"
  },
  {
    "name": "XISTAT",
    "address": 0x76,
    "access": "r",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "miscellaneous status bits"
  },
  {
    "name": "XICTL",
    "address": 0x77,
    "access": "rw",
    "signed": "unsigned",
    "bits" : 
    {
        "bit0": 0x01,
    },
    "description": "miscellaneous control bits"
  },
  {
    "name": "XIVER",
    "address": 0x78,
    "access": "r",
    "signed": "unsigned",
    "description": "firmware version"
  }
]
}