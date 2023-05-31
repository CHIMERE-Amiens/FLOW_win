# Copyright (c)  Harris Geospatial Solutions, Inc. All
# rights reserved. This software includes information which is
# proprietary to and a trade secret of Harris Geospatial Solutions, Inc.
# It is not to be disclosed to anyone outside of this organization.
# Reproduction by any means whatsoever is prohibited without express
# written permission.
#

import sys
import os
from subprocess import check_call
from distutils.core import setup
from distutils import sysconfig
import tempfile


print("\nInstalling the IDL Python bridge.")

# Dynamically construct the library name from the Python major.minor version.
vmajor = str(sys.version_info[0])
vminor = str(sys.version_info[1])
if sys.platform == 'win32':
    pythonidllib = "pythonidl" + vmajor + vminor + ".pyd"
    idlpythonlib = "idl_python" + vmajor + vminor + ".dll"
else:
    pythonidllib = "pythonidl" + vmajor + vminor + ".so"
    idlpythonlib = "idl_python" + vmajor + vminor + ".so"


setupPath = os.path.dirname(os.path.realpath(__file__))
idlbinPath = os.path.realpath(setupPath + r'/../../bin')

if sys.platform == 'win32':
    if sys.maxsize == 0x7fffffffffffffff:
        idlbinPath = os.path.join(idlbinPath, 'bin.x86_64')
    else:
        idlbinPath = os.path.join(idlbinPath, 'bin.x86')
elif sys.platform == 'darwin':
    idlbinPath = os.path.join(idlbinPath, 'bin.darwin.x86_64')
else:
    idlbinPath = os.path.join(idlbinPath, 'bin.linux.x86_64')


pythonidllibpath = os.path.join(idlbinPath, pythonidllib)
idlpythonlibpath = os.path.join(idlbinPath, idlpythonlib)

print("Python-to-IDL library location: ")
print("    " + pythonidllibpath)
print("IDL-to-Python library location: ")
print("    " + idlpythonlibpath)

if not os.path.isfile(pythonidllibpath) or not os.path.isfile(idlpythonlibpath):
    print("\n*** Unable to locate the IDL Python bridge libraries.")
    print("Make sure you are running setup.py from the IDL lib/bridges directory.\n")
    exit()


def findfile(name, paths):
    for path in paths:
        for root, dirs, files in os.walk(path):
            if name in files:
                return os.path.join(root, name)


# For Mac we need to call install_name_tool to set the Python library location
# within IDL's libaries
if sys.platform == "darwin":
    pylib = "libpython" + vmajor + "." + vminor
    if int(vmajor) >= 3:
        pylib += 'm'
    pylib += ".dylib"
    pylibpath = findfile(pylib, [sys.prefix + "/lib"])

    try:
        phome = os.environ['PYTHONHOME']
    except Exception as e:
        print("\n*** PYTHONHOME environment variable must be set " +
            "to your Python installation.")
        print("\nSetup failed. Exiting.\n")
        exit()

    print("\nSetting Python library location within IDL libraries")

    if pylibpath != None:
        print("\nPython library location: ")
        print("    " + pylibpath)
    else:
        print("\n*** Unable to locate Python library: " + pylib)
        print("    Try installing a Python distribution such as 'Anaconda'.\n")
        exit()

    try:
        cmd = ["install_name_tool", "-change", pylib, pylibpath, pythonidllibpath]
        print("\n" + " ".join(cmd))
        check_call(cmd)
        cmd = ["install_name_tool", "-change", "@rpath/" + pylib, pylibpath, pythonidllibpath]
        print("\n" + " ".join(cmd))
        check_call(cmd)
        cmd = ["install_name_tool", "-change", pylib, pylibpath, idlpythonlibpath]
        print("\n" + " ".join(cmd))
        check_call(cmd)
        cmd = ["install_name_tool", "-change", "@rpath/" + pylib, pylibpath, idlpythonlibpath]
        print("\n" + " ".join(cmd))
        check_call(cmd)
    except Exception as e:
        print("\n*** install_name_tool failed.")
        print("\n*** Check the paths above, or run the script as 'sudo python setup.py'.")
        exit()


print("Creating idl_python.pth file containing the IDL Python bridge paths")
temppthFilename = os.path.join(tempfile.gettempdir(), "idl_python.pth")
temppthFile = open(temppthFilename, "w")
temppthFile.write(setupPath + '\n')
temppthFile.write(idlbinPath)
temppthFile.close()

site_packages_path = sysconfig.get_python_lib()

setup(name='idl_python',
    version='2.0',
    author='Harris Geospatial Solutions, Inc.',
    description='IDL Python Bridge',
    long_description='IDL Python Bridge',
    data_files=[(site_packages_path, [temppthFilename])]
    )

print("Removing temporary idl_python.pth file")
os.remove(temppthFilename)


print("\nSetup is complete.")

