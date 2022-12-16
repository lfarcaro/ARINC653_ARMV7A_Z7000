# Gathers variables
variable project_name $env(PROJECT_NAME)
variable blockdesign_name $env(BLOCKDESIGN_NAME)
variable project_vivado_path $env(PROJECT_VIVADO_PATH)
variable project_sdk_path $env(PROJECT_SDK_PATH)

# Copies hardware definition into the software directory
file copy -force "${project_vivado_path}/${project_name}.runs/impl_1/${blockdesign_name}_wrapper.sysdef" "${project_sdk_path}/${blockdesign_name}_wrapper.hdf"

# Sets current directory
cd $project_sdk_path

# Sets workspace
setws $project_sdk_path

# Imports the hardware specification
createhw -name ${blockdesign_name}_wrapper_hw_platform_0 -hwspec ${blockdesign_name}_wrapper.hdf

# Creates the FSBL
createapp -name fsbl -app {Zynq FSBL} -hwproject ${blockdesign_name}_wrapper_hw_platform_0 -proc ps7_cortexa9_0 -os standalone

# Imports projects
importprojects ${project_sdk_path}/ARINC653_ARMV7A_Z7000
importprojects ${project_sdk_path}/ARINC653_ARMV7A_Z7000_bsp

# Imports temporary projects
importprojects ${project_sdk_path}/MEASURE
importprojects ${project_sdk_path}/MEASURE_bsp

# Builds the projects
projects -build
