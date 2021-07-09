#!/usr/bin/env python3

import datetime
import argparse
import json
import random


def flat_rate_rand(min, max):
    return random.randint(min, max)


def create_file(idx, prefix, config):
    logfile = {'id': f'file{idx:03}',
               'filename': f'{prefix}_file{idx:03}.txt',
               'canaries': config['canaries'],
               'devices': []}

    for i in range(1, config['ndevices']+1):
        mote_id = f'ID:{i}'
        rates = []
        for c in config['canaries']:
            cn = len(rates) + 1
            if config['rate_dist'] == 'flat_rate_rand':
                pars = config['rate_dist_params']
                rates.append({'can': c, 'r': f'R{cn}', 'rate':flat_rate_rand(min=pars['min'], max=pars['max'])})
            else:
                print(f'Unknown rate function: {config["rate_dist"]}')
                default = 'flat_rate_rand'
                default_pars = {min: 1, max: 240}
                config['rate_dist'] = default
                config[rate_dist_params] = default_pars
                print(f'Using default {default} with params: {default_params}')
                rates.append({'can': c, 'r': f'R{cn}', 'rate': flat_rate_rand(min=default_pars['min'], max=default_pars['max'])})

        # Create a mote file that can be compiled
        with open(f'{prefix}_{logfile["id"]}_{mote_id}.c', 'w') as f:
            for r in rates:
                f.write(f'#define {r["r"]} {r["rate"]}\n')
            f.write(f'#include "{config["motesource"]}"\n')
        
        logfile['devices'].append({'id': mote_id, 'modrates': rates})
    return logfile


if "__main__" == __name__:
    timestamp = datetime.datetime.now(datetime.timezone.utc)
    parser = argparse.ArgumentParser(description='Create Contiki motes writing 3 files at different rates')
    parser.add_argument('--json', '-j', type=str, nargs=1, default=['moteconfig.json'], help='JSON file with configuration')
    parser.add_argument('--prefix', '-p', type=str, nargs=1, default=[timestamp.isoformat().split('.')[0]], help='Prefix for created nodes and configuration file')
    args = parser.parse_args()

    with open(args.json[0], 'r') as f:
        config = json.load(f)

    if config['type'] != 'moteconfig' or config['version'] != 1:
        print(f'Not a supported mote config file: {args.json[0]}')
        exit(1)

    filesetup = {'type': 'logfilesetup',
                 'version': 1,
                 'date': str(timestamp),
                 'logfiles': []}

    for i in range(config['nfiles']):
        filesetup['logfiles'].append(create_file(i, args.prefix[0], config))

    filesetup['config'] = config
    logfilename = f'{args.prefix[0]}_logfiles.json'
    with open(logfilename, 'w') as f:
        print(f'Writing logfile {logfilename}')
        json.dump(filesetup, f, indent=4)
