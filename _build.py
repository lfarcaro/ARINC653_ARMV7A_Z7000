import sys
import argparse
import os
import shutil
import glob
import subprocess
#import re
#import platform

# Run using Python 3.10 installed with the following specific packages:
# cmd
# python -m pip install --upgrade pip

# ------------------------- ENVIRONMENT CONFIGURATION -------------------------

# Environment dictionary
oEnvironmentDictionary = {}

# Reads a key/value text file describing the environment as a dictionary
def getEnvironmentDictionary(sEnvironmentFilePath):
    # Open environment file
    oEnvironmentFile = open(sEnvironmentFilePath, 'r')
    # Create empty dictionary
    oEnvironmentDictionary = {}
    # Key list
    lKeys = []
    # Iterate environment file lines
    for sLine in oEnvironmentFile:
        # Split sLine into pair
        lLinePair = sLine.split("=")
        # Check pair
        if len(lLinePair) < 2:
            continue
        # Strip key and value
        sLineKey = lLinePair[0].strip()
        sLineValue = lLinePair[1].strip()
        # Add key to list
        if not sLineKey in lKeys:
            lKeys.append(sLineKey)
        # Add to dictionary
        if sLineKey in oEnvironmentDictionary:
            # Store as array if defined multiple times
            if isinstance(oEnvironmentDictionary[sLineKey], list):
                oEnvironmentDictionary[sLineKey].append(sLineValue)
            else:
                oEnvironmentDictionary[sLineKey] = [oEnvironmentDictionary[sLineKey], sLineValue]
        else:
            oEnvironmentDictionary[sLineKey] = sLineValue
    # Resolve references
    for sKey1 in lKeys:
        for sKey2 in lKeys:
            if sKey1 == sKey2:
                break
            oEnvironmentDictionary[sKey1] = oEnvironmentDictionary[sKey1].replace('$' + sKey2, oEnvironmentDictionary[sKey2])
    # Close environment file
    oEnvironmentFile.close()
    return oEnvironmentDictionary

# Retrieves a scalar value from the environment dictionary
def getEnvironmentScalar(sEntry):
    global oEnvironmentDictionary
    sResult = None
    if sEntry in oEnvironmentDictionary:
        sResult = oEnvironmentDictionary[sEntry]
        if isinstance(sResult, list):
            sResult = None
    return sResult

# Retrieves an array value from the environment dictionary
def getEnvironmentArray(sEntry):
    global oEnvironmentDictionary
    lResult = []
    if sEntry in oEnvironmentDictionary:
        lResult = oEnvironmentDictionary[sEntry]
        if not isinstance(lResult, list):
            lResult = [lResult]
    return lResult

# ------------------------- FILESYSTEM MANIPULATION -------------------------

# Deletes directory by name
def cmdDeleteDirectory(sName):
    try:
        if os.path.exists(sName):
            shutil.rmtree(sName)
    except:
        print('ERROR: Directory {0} could not be deleted'.format(sName))

# Create directory by name
def cmdCreateDirectory(sName):
    try:
        os.makedirs(sName, exist_ok=True)
    except:
        print('ERROR: Directory {0} could not be created'.format(sName))

# Copies a file by name
def cmdCopyFile(sFrom, sTo):
    try:
        shutil.copyfile(sFrom, sTo)
    except:
        print('ERROR: File {0} could not be copied to {1}'.format(sFrom, sTo))

# Deletes files by pattern
def cmdDeleteFiles(sPattern):
    lFiles = glob.glob(sPattern)
    for sFilePath in lFiles:
        try:
            os.remove(sFilePath)
        except:
            print('ERROR: File {0} could not be deleted'.format(sFilePath))

# Executes a command
def cmdExecuteCommand(lCommand):
    try:
        mEnvironment = os.environ.copy()
        for sKey in oEnvironmentDictionary:
            mEnvironment[sKey] = oEnvironmentDictionary[sKey]
        subprocess.run(lCommand, env=mEnvironment)
    except:
        raise Exception('ERROR: Error running command {0}'.format(' '.join(lCommand)))

# ------------------------- CLEAN -------------------------

# Cleaning method
def clean():
    sProjectName = getEnvironmentScalar("PROJECT_NAME")
    sBlockDesignName = getEnvironmentScalar("BLOCKDESIGN_NAME")
    sProjectVivadoPath = getEnvironmentScalar("PROJECT_VIVADO_PATH")
    sProjectSDKPath = getEnvironmentScalar("PROJECT_SDK_PATH")

    print('Cleaning...')
    cmdDeleteDirectory("{0}/.Xil".format(sProjectVivadoPath))
    cmdDeleteDirectory("{0}/{1}.cache".format(sProjectVivadoPath, sProjectName))
    cmdDeleteDirectory("{0}/{1}.hw".format(sProjectVivadoPath, sProjectName))
    cmdDeleteDirectory("{0}/{1}.ip_user_files".format(sProjectVivadoPath, sProjectName))
    cmdDeleteDirectory("{0}/{1}.runs".format(sProjectVivadoPath, sProjectName))
    cmdDeleteDirectory("{0}/{1}.sim".format(sProjectVivadoPath, sProjectName))
    cmdDeleteDirectory("{0}/{1}.srcs".format(sProjectVivadoPath, sProjectName))
    cmdDeleteDirectory("{0}/Packages".format(sProjectVivadoPath))
    cmdDeleteFiles("{0}/{1}.xpr".format(sProjectVivadoPath, sProjectName))
    cmdDeleteFiles("{0}/*.bif".format(sProjectVivadoPath))
    cmdDeleteFiles("{0}/*.jou".format(sProjectVivadoPath))
    cmdDeleteFiles("{0}/*.log".format(sProjectVivadoPath))

    cmdDeleteDirectory("{0}/{1}.sw/.metadata".format(sProjectVivadoPath, sProjectName))
    cmdDeleteDirectory("{0}/{1}.sw/.sdk".format(sProjectVivadoPath, sProjectName))
    cmdDeleteDirectory("{0}/{1}.sw/.Xil".format(sProjectVivadoPath, sProjectName))
    cmdDeleteDirectory("{0}/{1}.sw/ARINC653_ARMV7A_Z7000/Debug".format(sProjectVivadoPath, sProjectName))
    cmdDeleteDirectory("{0}/{1}.sw/ARINC653_ARMV7A_Z7000/Release".format(sProjectVivadoPath, sProjectName))
    cmdDeleteDirectory("{0}/{1}.sw/fsbl".format(sProjectVivadoPath, sProjectName))
    cmdDeleteDirectory("{0}/{1}.sw/fsbl_bsp".format(sProjectVivadoPath, sProjectName))
    cmdDeleteDirectory("{0}/{1}.sw/MEASURE/Debug".format(sProjectVivadoPath, sProjectName))
    cmdDeleteDirectory("{0}/{1}.sw/MEASURE/Release".format(sProjectVivadoPath, sProjectName))
    cmdDeleteDirectory("{0}/{1}.sw/{2}_wrapper_hw_platform_0".format(sProjectVivadoPath, sProjectName, sBlockDesignName))
    cmdDeleteDirectory("{0}/{1}.sw/RemoteSystemsTempFiles".format(sProjectVivadoPath, sProjectName))
    cmdDeleteDirectory("{0}/{1}.sw/webtalk".format(sProjectVivadoPath, sProjectName))
    cmdDeleteFiles("{0}/{1}.sw/*.log".format(sProjectVivadoPath, sProjectName))
    cmdDeleteFiles("{0}/{1}.sw/{2}_wrapper.hdf".format(sProjectVivadoPath, sProjectName, sBlockDesignName))

# ------------------------- BUILD -------------------------

# Building method
def build():
    sProjectName = getEnvironmentScalar("PROJECT_NAME")
    sBlockDesignName = getEnvironmentScalar("BLOCKDESIGN_NAME")
    sProjectVivadoPath = getEnvironmentScalar("PROJECT_VIVADO_PATH")
    sProjectSDKPath = getEnvironmentScalar("PROJECT_SDK_PATH")
    sXilinxVivadoPath = getEnvironmentScalar("XILINX_VIVADO_PATH")
    sXilinxSDKPath = getEnvironmentScalar("XILINX_SDK_PATH")
    sProjectFSBLPath = getEnvironmentScalar("PROJECT_FSBL_PATH")
    sProjectBitstreamPath = getEnvironmentScalar("PROJECT_BITSTREAM_PATH")
    sProjectBinaryPath = getEnvironmentScalar("PROJECT_BINARY_PATH")

    print('Building...')
    cmdExecuteCommand(['{0}/bin/vivado.bat'.format(sXilinxVivadoPath), '-mode', 'batch', '-source', '_build_01_soc_00_base.tcl'])
    cmdExecuteCommand(['{0}/bin/vivado.bat'.format(sXilinxVivadoPath), '-mode', 'batch', '-source', '_build_01_soc_01_gpio.tcl'])
    cmdExecuteCommand(['{0}/bin/vivado.bat'.format(sXilinxVivadoPath), '-mode', 'batch', '-source', '_build_01_soc_02_qspi.tcl'])
    cmdExecuteCommand(['{0}/bin/vivado.bat'.format(sXilinxVivadoPath), '-mode', 'batch', '-source', '_build_01_soc_03_sd.tcl'])
    cmdExecuteCommand(['{0}/bin/vivado.bat'.format(sXilinxVivadoPath), '-mode', 'batch', '-source', '_build_02_impl.tcl'])
    cmdExecuteCommand(['{0}/bin/xsct.bat'.format(sXilinxSDKPath), '_build_03_sw.tcl'])

    print("Generating boot image...")
    fBootgen = open("{0}/bootgen.bif".format(sProjectVivadoPath), "w")
    fBootgen.write("//arch = zynq; split = false; format = BIN\n")
    fBootgen.write("the_ROM_image:\n")
    fBootgen.write("{\n")
    fBootgen.write("[bootloader]{0}\n".format(sProjectFSBLPath))
    fBootgen.write("{0}\n".format(sProjectBitstreamPath))
    fBootgen.write("{0}\n".format(sProjectBinaryPath))
    fBootgen.write("}")
    fBootgen.close()
    cmdExecuteCommand(["{0}/bin/bootgen.bat".format(sXilinxSDKPath), "-image", "{0}/bootgen.bif".format(sProjectVivadoPath), "-arch", "zynq", "-o", "{0}/BOOT.bin".format(sProjectVivadoPath), "-w", "on"])

# ------------------------- MAIN -------------------------

# Main method
def main(argv):
    global oEnvironmentDictionary
    # Parse arguments
    parser = argparse.ArgumentParser(description="This is a script for building the project")
    parser.add_argument('--environment', '-e', default=".\\_build.env", help="Text file defining environmental information")
    parser.add_argument('--clean', '-c', default=False, action='store_true', help="Clean the project")
    parser.add_argument('--build', '-b', default=False, action='store_true', help="Build the project")
    args = parser.parse_args()
    # Check if environment file exists and it is not empty
    sEnvironmentFilePath = args.environment
    if os.path.exists(sEnvironmentFilePath):
        if os.stat(sEnvironmentFilePath).st_size == 0:
            print('ERROR: Environment file {0} is empty'.format(sEnvironmentFilePath))
            return
    else:
        print('ERROR: Environment file {0} does not exist'.format(sEnvironmentFilePath))
        return
    # Read environment information
    oEnvironmentDictionary = getEnvironmentDictionary(sEnvironmentFilePath)
    print('INFO: Environment:')
    for sEntry in oEnvironmentDictionary:
        print('INFO:     ' + sEntry + ' = ' + str(oEnvironmentDictionary[sEntry]))
    # Handle "clean" command
    if args.clean == True:
        clean()
    # Handle "build" command
    if args.build == True:
        build()
    return

# Entry point
if __name__ == "__main__":
    # Run main method
    main(sys.argv)