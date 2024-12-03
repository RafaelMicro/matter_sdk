#
# Copyright 2023 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

import re
import subprocess

if __name__ == '__main__':
    f = open("jlink_setup.cfg", "w")
    res = subprocess.run(['JLinkExe', '-commandfile', 'get_serial_numbers.jlink'], stdout=subprocess.PIPE)
    output = res.stdout.decode('utf-8')
    print(output)
    matches = re.findall(".*J-Link\[\d\].*Serial number: (.*\d), ProductName: (.*)", output, re.MULTILINE)
    for match in matches:
        serial_number = match[0]
        name = match[1]
        f.write(name + "=" + serial_number + '\n')
    f.close()
    f = open("jlink_setup.cfg", "r")
    print("J-Link probes found:")
    print(f.read())
