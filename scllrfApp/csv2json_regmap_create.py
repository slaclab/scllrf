#!/usr/bin/python
"""Converts data from a CSV file in to a JSON format

Expects to have, at a minimum, columns named:

name, address (hex), access, signed, description

Optional columns are bitindex, ASLO, bit_width, egu, nelm,
range_top, range_bottom, scale_eqn, lsBit

Bitfields are defined by using the bitindex field. Any field with
a value for bitindex will be attached to the previous register that
didn't have a bitindex

name            bitindex
reg0      
bitfield0          0
bitfield1          1
reg1               
reg2                
bitfield0          2
reg3"""

__author__ = "Joshua Einstein"
__version__ = "1.0.0"
__maintainer__ = "Joshua Einstein"
__email__ = "jeinstei@fnal.gov"


import sys, os
import csv
import json

def json_convert(infile, outfile, base=10):
    """Convert CSV file to JSON file"""
    
    print("Generating JSON file...\n")
    
    regs = []
    d = {}
    
    d['registers'] = []
    d['name'] = 'resCtl'

    # Read in file
    f = open(infile)
    rows = csv.DictReader(f);
    
    # Test for required values
    requiredcols = ["name", "address", "access", "signed", "description"]
    
    for field in requiredcols:
        if not (field in rows.fieldnames):
            print("Error: missing {0} column".format(field))
            sys.exit()
    
    # Build loop to build initial register type for JSON
    filefields = []
    
    for name in rows.fieldnames:
        if name != "bitindex" and name != "bits" and not (name in filefields):
            filefields.append(name)

    # Set default index for use in creating registers list
    regnum = -1

    # Test if there is a bitindex column
    if not ("bitindex" in rows.fieldnames):
        hasBitIndex = False
        print("Warning: no bitindex field defined in file")
    else:
        bits = {}
        hasBitIndex = True

    # Loop through data in file and process
    for row in rows:
        register = {}
        
        # Process data differently if there is a bitindex column
        if hasBitIndex:
            if row["bitindex"] != '':
                bits[row["name"]] = int(row["bitindex"])
                
                d["registers"][regnum]["bits"] = bits
            else:
                bits = {}
                
                regnum = regnum+1
                
                for name in filefields:
                    register[name] = row[name]
            
                register["address"] = int(register["address"], base)
            
                d["registers"].append(register)
        else:
            
            for name in filefields:
                register[name] = row[name]
            
            register["address"] = int(register["address"], base)
            
            d["registers"].append(register)
    
    f.close()
    
    f = open(outfile, "wb")
    json.dump(d, f, sort_keys=True, indent=4)
    f.close()
    
    print("Done generating JSON.\n")


def verilog_create(infile, outfile, base=10):
    """Convert CSV file to Verilog lines for a register map"""
    
    print("Generating Verilog...\n")
    
    # Read in file
    f = open(infile)
    rows = csv.DictReader(f);
    
    # Test for required values
    requiredcols = ["description", "address", "access", "verilog"]
    
    for field in requiredcols:
        if not (field in rows.fieldnames):
            print("Error: missing {0} column".format(field))
            sys.exit()

    # Test if there is a bitindex column
    if not ("bitindex" in rows.fieldnames):
        hasBitIndex = False
        print("Warning: no bitindex field defined in file")
    else:
        hasBitIndex = True

    readregs = []
    writeregs = []

    # Loop through data in file and process
    for idx, row in enumerate(rows):
        if row['address'] > 255:
            print("Script only works for <256 addresses")
            sys.exit()
        
        if hasBitIndex:
            if row['bitindex'] == '':
                if 'r' in row['access']:
                    readregs.append({'address' : int(row['address']), 'verilog' : row['verilog'], 'desc' : row['description']})
                
                if 'w' in row['access']:
                    writeregs.append({'address' : int(row['address']), 'verilog' : row['verilog'], 'desc' : row['description']})
                    
                if not( 'r' in row['access']) and not('w' in row['access']):
                    print("Unrecognized access type...Exiting\n")
                    sys.exit()
        else:
            if 'r' in row['access']:
                readregs.append({'address' : int(row['address']), 'verilog' : row['verilog'], 'desc' : row['description']})
                
            if 'w' in row['access']:
                writeregs.append({'address' : int(row['address']), 'verilog' : row['verilog'], 'desc' : row['description']})
                
            if not( 'r' in row['access']) and not('w' in row['access']):
                print("Unrecognized access type...Exiting\n")
                sys.exit()
    
    # Write write registers as verilog
    for reg in writeregs:
        print("always @(posedge lb_clk) if (lb_strobe & ~lb_rd & (lb_addr == 'h{0:x})) {1} <= lb_dout;\n".format(reg['address'], reg['verilog']))
    
    print("\n\n")
    
    # Write write registers as verilog
    print("always @(posedge lb_clk) begin\n\t\tcasex (lb_addr_r)\n")
    for reg in readregs:
        print("24'hxxxx{0:x}: lb_din <= {1};\n".format(reg['address'], reg['verilog']))
    print("\t\tdefault: lb_din <= 32'hfaceface;\n\tendcase\nend\n")
    
    print("Done Generating Verilog.\n")
    

if __name__ == "__main__":
    
    # Default file name
    fname = "Resonance_Control_Register_map_V3b.csv"
    
    # Process input arguments
    if len(sys.argv) == 2:
        fname = sys.argv[1]
    
    # Test function
    json_convert(fname, "Resonance_Control_Register_map.json", base=16)

    # Test Verilog Creation
    verilog_create(fname, "verilog_additions.v")
