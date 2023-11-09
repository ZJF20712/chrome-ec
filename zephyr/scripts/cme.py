# Copyright 2023 The ChromiumOS Authors
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""Component Manifest Engine"""

import json
import logging
from pathlib import Path
import sys
from typing import List, Optional

import zmake.modules
import zmake.version

from scripts import util


def parse_args(argv: Optional[List[str]] = None):
    """Returns parsed command-line arguments"""
    parser = util.EdtArgumentParser(
        prog="component_manifest_engine",
        description="Zephyr EC component manifest gereration",
    )

    parser.add_argument(
        "--manifest-file",
        type=Path,
        help="Path to the component manifest JSON file in JSON format",
        required=True,
    )

    return parser.parse_args(argv)


class Manifest:
    """Manifest class to operate the component manifest."""

    def __init__(self, ec_version):
        self.manifest = {
            "manifest_version": 1,
            "ec_version": ec_version,
            "component_list": [],
        }

    def insert_component(self, ctype, name, i2c_port, i2c_addr, usbc_port):
        """Insert the component inform to the component manifest.

        Args:
            ctype: String of component type.
            name: String of component name.
            i2c_port: I2C remote port number.
            i2c_address: I2C device address (7-bit).
            usbc_port: USB-C port number.
        """
        component = {
            "component_type": ctype,
            "component_name": name,
            "i2c": {"port": i2c_port, "addr": i2c_addr},
            "usbc": {"port": usbc_port},
        }
        self.manifest["component_list"].append(component)

    def json_dump(self, filepath):
        """Dump the component manifest to a JSON file."""
        with open(filepath, "w", encoding="utf-8") as outfile:
            outfile.write(json.dumps(self.manifest, indent=4))


def find_i2c_portmap(edtlib, edt):
    """Iterate all the I2C ports and find the mapping.

    Args:
        edtlib: Module object for the edtlib library.
        edt: EDT object representation of a devicetree

    Returns:
        A dict of the mapping between I2C name and remote port number.
    """
    try:
        named_i2c_ports = edt.get_node("/named-i2c-ports")
    except edtlib.EDTError:
        # If the named-i2c-ports node doesn't exist, return success.
        logging.error("No named-i2c-ports node found")
        return {}

    # These compats are EC-chip-specific. List all the compats here.
    i2c_compat_list = [
        "nuvoton,npcx-i2c-port",
        "ite,it8xxx2-i2c",
        "ite,enhance-i2c",
        "microchip,xec-i2c-v2",
        "zephyr,i2c-emul-controller",
    ]

    # Append all I2C chip names to a list; its index is the port number.
    i2c_chip_names = []
    for compat in i2c_compat_list:
        for node in edt.compat2okay[compat]:
            i2c_chip_names.append(node.name)

    if not i2c_chip_names:
        logging.error("No I2C port found")
        return {}

    # Find the mapping of the remote_port.
    i2c_portmap = {}
    for label, node in named_i2c_ports.children.items():
        i2c_name = node.props["i2c-port"].val.name
        if "remote-port" in node.props:
            i2c_portmap[i2c_name] = node.props["remote-port"].val
        elif i2c_name in i2c_chip_names:
            i2c_portmap[i2c_name] = i2c_chip_names.index(i2c_name)

        if i2c_name in i2c_portmap:
            logging.debug(
                "i2c remote portmap: %s(%s) -> %d",
                i2c_name,
                label,
                i2c_portmap[i2c_name],
            )
        else:
            logging.warning(
                "Unknown or disabled I2C port: %s(%s)", i2c_name, label
            )

    return i2c_portmap


def compatible_name_parser(ctype, name):
    """Parses the compatible into actual component name.

    Args:
        ctype: type of component
        name: compatible name for component

    Returns:
        Corrected component name
    """
    compatible = name.rsplit("-", 1)
    if len(compatible) > 1:
        if compatible[1] != ctype:
            # it is valid for compatibles to have a hyphen without ctype
            return name

    # TODO: add more cases for other compatibles as need arrises.

    return compatible[0]


def insert_i2c_component(ctype, node, usbc_port, i2c_portmap, manifest):
    """Insert the I2C component to the manifest.

    Args:
        node: Devicetree node object.
        usbc_port: USB-C port number
        i2c_portman: Dict of the mapping from I2C name to remote port number.
        manifest: Manifest object.
    """
    if "compatible" not in node.props:
        logging.error("Compatible not found: %s", node.name)
        return

    if node.parent and node.parent.name not in i2c_portmap:
        logging.info(
            "Component %s(%s) not on I2C bus, skip it",
            node.name,
            node.props["compatible"].val[0],
        )
        return

    # TODO(b/308031064): Add the probe methods if multiple components share the
    # same compatible. These components need to be identified.

    # TODO(b/308031075): Add the SSFC field.

    manifest.insert_component(
        ctype,
        compatible_name_parser(ctype, node.props["compatible"].val[0]),
        i2c_portmap[node.parent.name],
        node.props["reg"].val[0],
        usbc_port,
    )


def iterate_usbc_components(edtlib, edt, i2c_portmap, manifest):
    """Iterate all USB-C components and insert them to the manifest.

    Args:
        edtlib: Module object for the edtlib library.
        edt: EDT object representation of a devicetree
        i2c_portman: Dict of the mapping from I2C name to remote port number.
        manifest: Manifest object.
    """
    try:
        usbc = edt.get_node("/usbc")
    except edtlib.EDTError:
        # If the usbc node doesn't exist, return success.
        logging.error("No usbc node found")
        return

    for node in usbc.children.values():
        # Ignore the child node without a port number.
        if "reg" not in node.props:
            continue
        port = node.props["reg"].val[0]

        for prop in ["chg", "chg_alt"]:
            if prop in node.props:
                chg = node.props[prop].val
                insert_i2c_component(
                    "charger", chg, port, i2c_portmap, manifest
                )

        if "bc12" in node.props:
            bc12 = node.props["bc12"].val
            insert_i2c_component("bc12", bc12, port, i2c_portmap, manifest)

        for prop in ["ppc", "ppc_alt"]:
            if prop in node.props:
                ppc = node.props[prop].val
                insert_i2c_component("ppc", ppc, port, i2c_portmap, manifest)

        if "tcpc" in node.props:
            tcpc = node.props["tcpc"].val
            insert_i2c_component("tcpc", tcpc, port, i2c_portmap, manifest)


def main(argv: Optional[List[str]] = None) -> Optional[int]:
    """The main function.

    Args:
        argv: Optionally, the command-line to parse, not including argv[0].

    Returns:
        Zero upon success, or non-zero upon failure.
    """
    args = parse_args(argv)

    log_format = "%(levelname)s: %(message)s"
    logging.basicConfig(format=log_format, level=args.log_level)

    edtlib, edt, build_dir = util.load_edt(args.zephyr_base, args.edt_pickle)
    if edtlib is None:
        return 0

    logging.info("Running CME, outputting to %s", args.manifest_file)
    i2c_portmap = find_i2c_portmap(edtlib, edt)

    # Compute the version string.
    if util.is_test(args.edt_pickle):
        ec_version_string = "test_build"
    else:
        # TODO(b/308497935): The API no longer work as it requires passing the
        # `version` and `static` arguments that CME doesn't know.
        ec_version_string = zmake.version.get_version_string(
            build_dir.name,
            version=None,
            static=False,
        )
    manifest = Manifest(ec_version_string)

    iterate_usbc_components(edtlib, edt, i2c_portmap, manifest)

    # TODO(b/308028560): Iterate all sensor components.

    manifest.json_dump(args.manifest_file)

    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
