from __future__ import print_function
import argparse
import json
from Cheetah.Template import Template

def make_asyndriver(d):
    with open('src/'+d['name']+'Driver.cpp', 'w') as f:
        f.write(str(Template(file='./templateScllrfDriver.cpp.tmpl', searchList=[d])))
    with open('src/'+d['name']+'.h', 'w') as f:
        f.write(str(Template(file='./templateScllrfDriver.h.tmpl', searchList=[d])))
    with open('src/'+d['name']+'DriverSupportInclude.dbd', 'w') as f:
        f.write(str(Template(file='./templateDriverSupportInclude.dbd.tmpl', searchList=[d])))

def make_database(d):
    with open('Db/'+d['name']+'Registers.db', 'w') as f:
        f.write(str(Template(file='./database.tmpl', searchList=[d])))

def make_gui(d):
    with open('ui/'+d['name']+'Registers.edl', 'w') as f:
        f.write(str(Template(file='./RegistersScreen.edl.tmpl', searchList=[d])))

def expand_array(array):
    for ix in xrange(1<<array['addr_width']):
        reg['name'] = array['base_addr'] + str(ix)
        reg['address'] = array['base_addr'] + ix


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('-i', dest='input_f', help='LBNL json file', required=True)
    parser.add_argument('-o', dest='output', help='translated file', required=True)
    parser.add_argument('-c', dest='core', help='core name')

    args = parser.parse_args()
    regmap_dict = {'name': args.core, 'registers': []}
    with open(args.input_f, 'r') as f:
        json_regmap = json.load(f)
    regmap = json_regmap['core_llrf']['regmap']
    for k, v in regmap.items():
        if type(v) is dict:
            v['name'] = k
            v['address'] = (v['base_addr'] if type(v['base_addr']) is int else int(v['base_addr'], 16))
            # expand array of length < 32
            if v['addr_width'] == 0:
                regmap_dict['registers'].append(v)
            elif v['addr_width'] <= 10:
                for ix in xrange(50):
                    reg = dict(v)
                    reg['name'] = v['name'] + '_' + str(ix)
                    reg['address'] = v['address'] + ix
                    regmap_dict['registers'].append(reg)
            else:
                print('Warning: Register %s excluded since addr_width > %d.'%(v['name'], v['addr_width']))

    if args.output == 'src':
        make_asyndriver(regmap_dict)
    elif args.output == 'Db':
        make_database(regmap_dict)
    elif args.output == 'ui':
        make_gui(regmap_dict)
