import os
import re
import sys

BOARD_HEADERS = {
    "RT584H": {
        "mac":   "prg_rt584h_mpa_asic_pci_fw.h",
        "ble":   "prg_rt584h_mpa_asic_ble_fw.h",
        "multi": "prg_rt584h_mpa_asic_multi_fw.h",
    },
    "RT584HA4": {
        "mac":   "prg_rt584h_mpa_asic_pci_fw.h",
        "ble":   "prg_rt584h_mpa_asic_ble_fw.h",
        "multi": "prg_rt584h_mpa_asic_multi_fw.h",
    },
    "RT584L": {
        "mac":   "prg_rt584l_mpa_asic_pci_fw.h",
        "ble":   "prg_rt584l_mpa_asic_ble_fw.h",
        "multi": "prg_rt584l_mpa_asic_multi_fw.h",
    },
}

VERSION_DEFINES = {
    "mac":   ("FW_MAC_VERSION_NO",   "BUILD_MAC_FW_INFO"),
    "ble":   ("FW_BLE_VERSION_NO",   "BUILD_BLE_FW_INFO"),
    "multi": ("FW_MULTI_VERSION_NO", "BUILD_MULTI_FW_INFO"),
}

def extract_version(header_path, define_name):
    pattern = re.compile(r'#define\s+' + re.escape(define_name) + r'\s+\((\d+)UL\)')
    try:
        with open(header_path, "r") as f:
            for line in f:
                m = pattern.search(line)
                if m:
                    return m.group(1)
    except OSError:
        pass
    return None

def main():
    if len(sys.argv) < 2:
        sys.exit("Usage: get_fw_version.py <rafael_board>")

    board = sys.argv[1]

    if board not in BOARD_HEADERS:
        print("BUILD_MAC_FW_INFO=0")
        print("BUILD_BLE_FW_INFO=0")
        print("BUILD_MULTI_FW_INFO=0")
        return

    include_dir = os.path.join(
        os.path.dirname(os.path.abspath(__file__)),
        "repo", "components", "network", "rt569-fw", "rt584", "include",
    )
    for key in ("mac", "ble", "multi"):
        header = os.path.join(include_dir, BOARD_HEADERS[board][key])
        define_name, build_define = VERSION_DEFINES[key]
        ver = extract_version(header, define_name)
        if ver is None:
            sys.exit("Could not find {} in {}".format(define_name, header))
        print("{}={}".format(build_define, ver))

main()
