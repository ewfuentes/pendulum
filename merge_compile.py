#!/usr/bin/env python3
import json
import sys

if __name__ == '__main__':
    commands = []
    for f in sys.argv[1:]:
        with open(f, 'r') as file_in:
            data = file_in.read().strip()
            if data.endswith(','):
                data = data[:-1]
            commands.append(json.loads(data))
    
    with open('compile_commands.json', 'w') as file_out:
        json.dump(commands, file_out)
    pass
