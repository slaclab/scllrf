d = { "name" : "scllrfRESwav",
		 'registers': [
 {
   "name": "reserved",
   "address": 0x60,
   "access": "rw",
 },
 {
   "name": "reserved",
   "address": 0x61,
   "access": "rw",
 },
 {
   "name": "reserved",
   "address": 0x62,
   "access": "rw",
 },
 {
   "name": "reserved",
   "address": 0x63,
   "access": "rw",
 },
 {
   "name": "pzt_wave_avail",
   "address": 0x6F,
   "access": "r",
   "signed": "unsigned",
		"bits":
		{	"bit1": 0x0001,
			"bit2": 0x0010
		},
   "desc": "PZT waveform availability: 8 bits per piezo, MSB, pzt 4-1"
 },
 {
   "name": "piezo1_c1_In",
   "address": 0x70,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 1 chan 1 current n"
 },
 {
   "name": "piezo1_c1_Ip",
   "address": 0x71,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 1 chan 1 current p"
 },
 {
   "name": "piezo1_c1_out_v",
   "address": 0x72,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 1 chan 1 out voltage scaled"
 },
 {
   "name": "piezo1_c1_drive_v",
   "address": 0x73,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 1 chan 1 amp drive voltage"
 },
 {
   "name": "piezo1_c2_In",
   "address": 0x74,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 1 chan 2 current n"
 },
 {
   "name": "piezo1_c2_Ip",
   "address": 0x75,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 1 chan 2 current p"
 },
 {
   "name": "piezo1_c2_out_v",
   "address": 0x76,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 1 chan 2 out voltage scaled"
 },
 {
   "name": "piezo1_c2_drive_v",
   "address": 0x77,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 1 chan 2 amp drive voltage"
 },
 {
   "name": "piezo2_c1_In",
   "address": 0x80,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 2 chan 1 current n"
 },
 {
   "name": "piezo2_c1_Ip",
   "address": 0x81,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 2 chan 1 current p"
 },
 {
   "name": "piezo2_c1_out_v",
   "address": 0x82,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 2 chan 1 out voltage scaled"
 },
 {
   "name": "piezo2_c1_drive_v",
   "address": 0x83,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 2 chan 1 amp drive voltage"
 },
 {
   "name": "piezo2_c2_In",
   "address": 0x84,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 2 chan 2 current n"
 },
 {
   "name": "piezo2_c2_Ip",
   "address": 0x85,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 2 chan 2 current p"
 },
 {
   "name": "piezo2_c2_out_v",
   "address": 0x86,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 2 chan 2 out voltage scaled"
 },
 {
   "name": "piezo2_c2_drive_v",
   "address": 0x87,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 2 chan 2 amp drive voltage"
 },
 {
   "name": "piezo3_c1_In",
   "address": 0x90,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 3 chan 1 current n"
 },
 {
   "name": "piezo3_c1_Ip",
   "address": 0x91,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 3 chan 1 current p"
 },
 {
   "name": "piezo3_c1_out_v",
   "address": 0x92,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 3 chan 1 out voltage scaled"
 },
 {
   "name": "piezo3_c1_drive_v",
   "address": 0x93,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 3 chan 1 amp drive voltage"
 },
 {
   "name": "piezo3_c2_In",
   "address": 0x94,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 3 chan 2 current n"
 },
 {
   "name": "piezo3_c2_Ip",
   "address": 0x95,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 3 chan 2 current p"
 },
 {
   "name": "piezo3_c2_out_v",
   "address": 0x96,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 3 chan 2 out voltage scaled"
 },
 {
   "name": "piezo3_c2_drive_v",
   "address": 0x97,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 3 chan 2 amp drive voltage"
 },
 {
   "name": "piezo4_c1_In",
   "address": 0xA0,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 4 chan 1 current n"
 },
 {
   "name": "piezo4_c1_Ip",
   "address": 0xA1,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 4 chan 1 current p"
 },
 {
   "name": "piezo4_c1_out_v",
   "address": 0xA2,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 4 chan 1 out voltage scaled"
 },
 {
   "name": "piezo4_c1_drive_v",
   "address": 0xA3,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 4 chan 1 amp drive voltage"
 },
 {
   "name": "piezo4_c2_In",
   "address": 0xA4,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 4 chan 2 current n"
 },
 {
   "name": "piezo4_c2_Ip",
   "address": 0xA5,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 4 chan 2 current p"
 },
 {
   "name": "piezo4_c2_out_v",
   "address": 0xA6,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 4 chan 2 out voltage scaled"
 },
 {
   "name": "piezo4_c2_drive_v",
   "address": 0xA7,
   "access": "r",
   "signed": "signed",
   "desc": "Piezo 4 chan 2 amp drive voltage"
 }
]
}