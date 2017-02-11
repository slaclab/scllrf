# Convert the LBL format json file to the py format expected by this code generator
from __future__ import print_function
import os
import errno
import sys
import re
import time,os,struct,json,getopt

if __name__ == "__main__":
    with open ("prc_regmap_epics.json") as regmapfile:
        regmap=json.load(regmapfile)
    regmapfile.close()
    
    # for names that contain a ',' these are really two values. Split them.
    find_reg = re.compile('([^,]+),([^,]+)')
    find_bits = re.compile('(.*)_([0-9]*):([0-9]*)_')
    #find_second_reg = re.compile('(?<=,)[^,]+')
    expanded_regmap = {}
    for reg, val in regmap.iteritems():
        if ',' in reg:
            split_names = find_reg.match(reg)
            expanded_regmap[split_names.group(1)] = val
            expanded_regmap[split_names.group(2)] = val
        elif ':' in reg:
            new_reg = re.sub(r'(.*)_([0-9]*):([0-9]*)_', r'\1_Bits\2to\3', reg)
            expanded_regmap[new_reg] = val
        else:
            expanded_regmap[reg] = val
    
    # Python didn't like trying to change a dictionary while iterating over it,
    # so I created the expanded_* new dictionaries. If we clear the old ones, we can reuse them.
    regmap.clear()
    # get rid of any characters from keys that aren't allowed in C++ variable names
    for reg, val in expanded_regmap.iteritems():
        regmap[re.sub(r'\W', r'_', str(reg))] = expanded_regmap[reg]
    
    f=open("prc_regmap_new.py", 'w')

    f.write('d = { "name" : "scllrfPRC",\n')
    f.write("\t'registers': [\n")

    for regName, regAttrs in sorted(regmap.items(), key=lambda k: k[1]['base_addr']):
        f.write('\t{\t"name" :"' + regName + '",\n')
        f.write('\t\t"address"     : ' + repr(regAttrs['base_addr']) + ',\n')
        if regAttrs.has_key('data_width'):
            f.write('\t\t"bit_width"      : ' + repr(regAttrs['data_width']) + ',\n')
        if regAttrs.has_key('addr_width'):
            f.write('\t\t"nelm"      : ' + repr(2**regAttrs['addr_width']) + ',\n')
        if regAttrs.has_key('access'):
            f.write('\t\t"access"      : "' + regAttrs['access'] + '",\n')
        if regAttrs.has_key('sign'):
            f.write('\t\t"signed"      : "' + regAttrs['sign'] + '"\n')
        f.write('\t},\n')
        
        
    f.write('\t]\n')
    f.write('}')
    f.close()