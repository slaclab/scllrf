# Convert the LBL format json file to the py format expected by this code generator
from __future__ import print_function
import os
import errno
import sys
import re
import time,os,struct,json,getopt

def filter_dict(d, filter_string):
    for key, val in d.iteritems():
        if filter_string not in key:
            continue
        yield key, val
        
if __name__ == "__main__":
    with open ("prc_regmap.json") as regmapfile:
        regmaps=json.load(regmapfile)
    write_regmap=regmaps['write_regmap']
    read_regmap=regmaps['read_regmap']
    regmapfile.close()
    
    # for names that contain a ',' these are really two values. Split them.
    find_reg = re.compile('([^,]+),([^,]+)')
    find_bits = re.compile('(.*)\[(.*):(.*)\]')
    #find_second_reg = re.compile('(?<=,)[^,]+')
    expanded_write_regmap = {}
    for reg, val in write_regmap.iteritems():
        if ',' in reg:
            split_names = find_reg.match(reg)
            expanded_write_regmap[split_names.group(1)] = val
            expanded_write_regmap[split_names.group(2)] = val
        elif '[' in reg:
            new_reg = re.sub(r'(.*)\[(.*):(.*)\]', r'\1Bits\2to\3', reg)
            expanded_write_regmap[new_reg] = val
        else:
            expanded_write_regmap[reg] = val
    expanded_read_regmap = {}
    for reg, val in read_regmap.iteritems():
        if ',' in reg:
            split_names = find_reg.match(reg)
            expanded_read_regmap[split_names.group(1)] = val
            expanded_read_regmap[split_names.group(2)] = val
        elif '[' in reg:
            new_reg = re.sub(r'(.*)\[(.*):(.*)\]', r'\1Bits\2to\3', reg)
            expanded_read_regmap[new_reg] = val
        else:
            expanded_read_regmap[reg] = val
    
    # Python didn't like trying to change a dictionary while iterating over it,
    # so I created the expanded_* new dictionaries. If we clear the old ones, we can reuse them.
    write_regmap.clear()
    read_regmap.clear()
    # get rid of any characters from keys that aren't allowed in C++ variable names
    for reg, val in expanded_write_regmap.iteritems():
        write_regmap[re.sub(r'\W', r'_', str(reg))] = expanded_write_regmap[reg]
    for reg, val in expanded_read_regmap.iteritems():
        read_regmap[re.sub(r'\W', r'_', str(reg))] = expanded_read_regmap[reg]
    
    f=open("prc_regmap.py.tmp", 'w')

    f.write('d = { "name" : "scllrfPRC\n",')
    f.write("\t'registers': [\n")

    for reg in sorted(write_regmap.iteritems(), key=lambda x: x[1]):
        f.write('\t\t{ "name" :"' + reg[0] + '_w",\n')
        f.write('\t\t"address"     : ' + repr(reg[1]) + ',\n')
        f.write('\t\t"access"      : "w",\n')
        f.write('\t\t"signed"      : "signed"\n')
        f.write('\t\t},\n')

    for reg in sorted(read_regmap.iteritems(), key=lambda x: x[1]):
        f.write('\t\t{ "name" :"' + reg[0] + '_r",\n')
        f.write('\t\t"address"     : ' + repr(reg[1]) + ',\n')
        f.write('\t\t"access"      : "r",\n')
        f.write('\t\t"signed"      : "signed"\n')
        f.write('\t\t},\n')
        
    f.write('\t]\n')
    f.write('}')
    f.close()