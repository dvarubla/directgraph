import os
import re

files = [
    'directgraph_stdint.h', 'export_header.h',
    'directgraph_pack.h', 'directgraph_constants.h',
    'directgraph_api.h'
]

dir = 'directgraph'

with open('directgraph.h', 'w') as outf:
    outf.write('/*This file was generated automatically with ' + os.path.basename(__file__) + '*/\n')
    for fname in files:
        with open(os.path.join(dir, fname), 'r') as inf:
            text = inf.read()
            text = re.sub(r'^#ifndef.+?#define[^\r\n]+[\r\n]+(.+)#endif[^\r\n]*\s*$', '\\1', text, flags=re.DOTALL)
            text = re.sub(r'^#include "(?:directgraph_|export_header.h).+$', '', text, flags=re.MULTILINE)
            outf.write('/* File: ' + fname + ' */\n')
            outf.write(text)
            outf.write('\n')