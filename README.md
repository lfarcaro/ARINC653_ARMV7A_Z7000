# ARINC653_ARMV7A_Z7000
Bare-metal Real-Time Operating System (RTOS) for Xilinx Zynq-7000 Cortex-A9 (ARMv7-A) multi-core SoCs (ZedBoard, PicoZed, MicroZed and similars) based on the ARINC 653 Part 1 specification

# IDE installation:
- Run Xilinx Unified Installer v2018.3 (or a similar superior version)
- Select "Vivado HL WebPACK" installation
- Ensure "Vivado Design Suite", "Software Development Kit (SDK)" (with the ARM Cortex-A9 Compiler Tool Chain) and "SoCs/Zynq-7000" are marked

# Compilation configuration:
- C/C++ Build / Settings / ARM v7 gcc compiler / Directories / Include Paths += "${workspace_loc:/${ProjName}/src/ARINC653_PORT/include}" (add to both "Debug" and "Release")
- C/C++ Build / Settings / ARM v7 gcc compiler / Directories / Include Paths += "${workspace_loc:/${ProjName}/src/ARINC653/include}" (add to both "Debug" and "Release")
- C/C++ Build / Settings / ARM v7 gcc compiler / Directories / Include Paths += "${workspace_loc:/${ProjName}/src/Test/include}" (add to both "Debug" and "Release")
- C/C++ Build / Settings / ARM v7 gcc linker / Linker Script = "../src/Application/ARMV7A_Z7000.ld" (change for both "Debug" and "Release")

# MicroPhase Z7-Lite JTAG debugging configuration:
- J1 = 1-2 [JTAG]

# Debugging on the target platform:
- Select "Run" -> "Debug Configurations..."
- Double-click "Xilinx C/C++ application (System Debugger)"
- Configure the debug configuration as follows:
	- Target Setup / Debug Type = Standalone Application Debug
	- Target Setup / Connection = Local
	- Target Setup / Reset entire system = TRUE
	- Target Setup / Program FPGA = TRUE
	- Target Setup / Run ps7_init = TRUE
	- Target Setup / Run ps7_post_config = TRUE
	- Application / Stop at 'main' = FALSE
	- Application / Processor "ps7_cortexa9_0":
		- Summary / Download = TRUE
		- Stop at program entry = TRUE
	- Application / Processor "ps7_cortexa9_1":
		- Summary / Download = FALSE
		- Stop at program entry = FALSE
- Click "Apply"
- Click "Debug"

# Debugging on the second core (*after the debug session is started*):
- Select "Run" -> "Debug Configurations..."
- Select debug configuration to be used
- Configure the debug configuration as follows:
	- Symbol files / Debug context "/APU/ARM Cortex-A9 MPCore #1" ():
		- Click "Add..."
		- Select the project's ELF file
		- Click "OK"
	- Click "Close"

# Programming the bitstream into the QSPI memory:
- Build the project
- Open the project in Vivado
- Open "Hardware Manager" -> "Open Target" -> "Auto Connect"
- In window "Hardware", right-click "xc7z010_1" and open "Add Configuration Memory Device..."
- Set "Manufacturer" to "Winbond"
- Set "Type" to "qspi"
- Set "Width" to "x4-single"
- Select the "w25q128fv-qspi-x4-single" part number
- Click "OK"
- Accept programming the device by clicking "OK"
- Set "Configuration file" to the "BOOT.bin" file built
- Set "Zynq FSBL" to the "fsbl/Debug/fsbl.elf" file found in the software project directory
- Click "OK"