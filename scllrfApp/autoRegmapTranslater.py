# Convert the LBL format json file to the py format expected by this code generator
from __future__ import print_function
import os
import errno
import sys
import re
import time,os,struct,json,getopt

if __name__ == "__main__":
    with open ("regmap_application_top.json") as regmapfile:
        regmap=json.load(regmapfile)
    regmapfile.close()
    
    f=open("regmap_application_top.py.tmp", 'w')

    f.write('d = { "name" : "scllrfPRC",\n')
    f.write("\t'registers': [\n")

    for regName, regAttrs in regmap.iteritems():
        f.write('\t{\t"name" :"' + regName + '",\n')
        f.write('\t\t"address"     : ' + repr(regAttrs['base_addr']) + ',\n')
        if regAttrs.has_key('data_width'):
            f.write('\t\t"bit_width"      : "' + repr(regAttrs['data_width']) + '",\n')
        if regAttrs.has_key('addr_width'):
            f.write('\t\t"nelm"      : "' + repr(2**regAttrs['addr_width']) + '",\n')
        f.write('\t\t"access"      : "w",\n')
        if regAttrs.has_key('sign'):
            f.write('\t\t"signed"      : ' + regAttrs['sign'] + '"\n')
        f.write('\t},\n')
        
        
    f.write('\t]\n')
    f.write('}')
    f.close()