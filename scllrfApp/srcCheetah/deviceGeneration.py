from __future__ import print_function
import os
import errno
import sys
import argparse
import json
from Cheetah.Template import Template
import re
import time,struct,getopt
from string import replace
import hashlib

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc: # Python >2.5
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else: raise

def makeDirectory(regmap_dict):
    mkdir_p(regmap_dict['name'])
#    mkdir_p(regmap_dict['name']+'/deviceLibrary')
    mkdir_p(regmap_dict['name']+'/src')
    mkdir_p(regmap_dict['name']+'/Db')
    mkdir_p(regmap_dict['name']+'/ui')

#device library
#def makeDeviceLibrary(regmap_dict):
#     f=open(regmap_dict['name']+'/deviceLibrary/'+regmap_dict['name']+'.cc', 'w')
#     print(Template( file='./builder.cc.tmpl', searchList = [regmap_dict] ), file=f)
#     f.close()
#
#     f=open(regmap_dict['name']+'/deviceLibrary/'+regmap_dict['name']+'.h', 'w')
#     print(Template( file='./builder.h.tmpl', searchList = [regmap_dict] ), file=f)
#     f.close()

#asyn driver
def makeAsynDriver(regmap_dict):
    f=open(regmap_dict['name']+'/src/Makefile', 'w')
    print(Template( file='./Makefile.tmpl', searchList = [regmap_dict] ), file=f)
    f.close()

    f=open(regmap_dict['name']+'/src/'+regmap_dict['name']+'.cpp', 'w')
    print(Template( file='./templateScllrfDriver.cpp.tmpl', searchList = [regmap_dict] ), file=f)
    f.close()

    f=open(regmap_dict['name']+'/src/'+regmap_dict['name']+'.h', 'w')
    print(Template( file='./templateScllrfDriver.h.tmpl', searchList = [regmap_dict] ), file=f)
    f.close()

    f=open(regmap_dict['name']+'/src/'+regmap_dict['name']+'DriverSupportInclude.dbd', 'w')
    print(Template( file='./templateDriverSupportInclude.dbd.tmpl', searchList = [regmap_dict] ), file=f)
    f.close()

#database template
def makeDatabase(regmap_dict):
    f=open(regmap_dict['name']+'/Db/'+regmap_dict['name']+'Registers.template', 'w')
    print(Template( file='./database.tmpl', searchList = [regmap_dict] ), file=f)
    f.close()

#gui
def makeGui(regmap_dict):
     f=open(regmap_dict['name']+'/ui/'+regmap_dict['name']+'Registers.edl', 'w')
     print(Template( file='./RegistersScreen.edl.tmpl', searchList = [regmap_dict] ), file=f)
     f.close()
     f=open(regmap_dict['name']+'/ui/'+regmap_dict['name']+'Bits.edl', 'w')
     print(Template( file='./BitsScreen.edl.tmpl', searchList = [regmap_dict] ), file=f)
     f.close()

def fix_prc_names(reg_name):
    old_name = reg_name
    # for names that contain a ',' these are really two values. Split them.
    find_reg = re.compile('([^,]+),([^,]+)')
    find_bits = re.compile('(.*)_([0-9]*):([0-9]*)_')

    if ',' in reg_name:
        split_names = find_reg.match(reg_name)
        reg_name = split_names.group(1) + split_names.group(2)

    if ':' in reg_name:
        reg_name = re.sub(r'(.*)\[([0-9]*):([0-9]*)\]', r'\1_Bits\2to\3', reg_name)

    # get rid of any characters from keys that aren't allowed in C++ variable names
    reg_name = re.sub(r'\W', r'_', reg_name)

    reg_name = replace(reg_name, 'application_top_', '')
    reg_name = replace(reg_name, 'digitizer', 'dig')
    reg_name = replace(reg_name, 'real_sim_mux', 'mux')
    reg_name = replace(reg_name, 'cav4_elec', 'elec')
    reg_name = replace(reg_name, 'cavity_', 'c')
    reg_name = replace(reg_name, 'mode_', 'm')
    reg_name = replace(reg_name, 'dsp_fdbk', 'fdbk')
    reg_name = replace(reg_name, 'drive_couple', 'drive_cpl')
    reg_name = replace(reg_name, 'out_couple', 'out_cpl')
    reg_name = replace(reg_name, '0d0a0d0a', 'd0a0d0a')

    if len(reg_name) > 48:
        print("Long name ", reg_name, " didn't make the 48 character cut")

    if not (old_name == reg_name):
        print("fixed name ", old_name, " -> ", reg_name)

    return reg_name


def usage(progname):
    print("%s:  Make EPICS files for peripheral", progname)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('-i', dest='input_j', help='LBNL json file')
    parser.add_argument('-d', dest='input_d', help='python dictionary register map')
    parser.add_argument('-c', dest='core', help='core name')

    args = parser.parse_args()
    print("input_D", args.input_d)
    # If the input is a python dictionary
    if args.input_d is not None:
        device = __import__(args.input_d.replace('.py', ''))
        regmap_dict = device.d
        sha = hashlib.sha1()
        fv = open(args.input_d).read()
        sha.update(fv)
        print(sha.hexdigest())
        regmap_dict['sha1'] = sha.hexdigest()
        if args.core is not None:
            regmap_dict.name = args.core

    # if the input is a json file
    if args.input_j is not None:
        regmap_dict = {'name': args.core, 'registers': []}
        with open(args.input_j, 'r') as f:
            sha = hashlib.sha1()
            fv = open(args.input_j).read()
            sha.update(fv)
            print(sha.hexdigest())
            regmap_dict['sha1'] = sha.hexdigest()
            json_regmap = json.load(f)
            for k, v in json_regmap.items():
                if type(v) is dict:
                    v['name'] = k
                    v['address'] = (v['base_addr'] if type(v['base_addr']) is int else int(v['base_addr'], 16))
                    v['nelm'] = 2**v['addr_width']
                    if 'buf_trig' in k:
                        v['bits'] = {}
                    if 'trace_reset' in k:
                        v['bits'] = {}
                    if 'llrf_circle_ready' in k:
                        v['bits'] = { "c0_ready": 0, "c1_ready": 1 }
                    if 'circle_buf_flip' in k:
                        v['bits'] = { "flip_c0_buf": 0, "flip_c1_buf": 1 }
                    if 'dsp_chan_keep' in k:
                        v['bits'] = { "Ch0": 0, "Ch1": 1, "Ch2": 2, "Ch3": 3, "Ch4": 4, "Ch5": 5, "Ch6": 6, "Ch7": 7, "Ch8": 8, "Ch9": 9, "Ch10": 10, "Ch11": 11, }
                    if 'trace_keep' in k:
                        v['bits'] = { "Ch0": 0, "Ch1": 1, "Ch2": 2, "Ch3": 3, "Ch4": 4, "Ch5": 5, "Ch6": 6, "Ch7": 7 }
                    if 'circle_buf_stop' in k:
                        v['bits'] = { "stop_c0_buf": 0, "stop_c1_buf": 1 }
                    regmap_dict['registers'].append(v)

    makeDirectory(regmap_dict)
    makeAsynDriver(regmap_dict)
    makeDatabase(regmap_dict)
    makeGui(regmap_dict)
