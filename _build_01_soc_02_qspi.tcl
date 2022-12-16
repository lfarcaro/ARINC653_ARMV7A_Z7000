# Gathers variables
variable project_name $env(PROJECT_NAME)
variable blockdesign_name $env(BLOCKDESIGN_NAME)
variable project_vivado_path $env(PROJECT_VIVADO_PATH)

# Opens the project
open_project ${project_vivado_path}/${project_name}.xpr

# Opens the block design
open_bd_design "${project_vivado_path}/${project_name}.srcs/sources_1/bd/${blockdesign_name}/${blockdesign_name}.bd"

# Enables QSPI peripheral
set_property -dict [list CONFIG.PCW_QSPI_PERIPHERAL_ENABLE {1} CONFIG.PCW_QSPI_GRP_SINGLE_SS_ENABLE {1}] [get_bd_cells processing_system7_0]

# Configures MIO pins
set_property -dict [list CONFIG.PCW_SINGLE_QSPI_DATA_MODE {x4} CONFIG.PCW_MIO_1_SLEW {fast} CONFIG.PCW_MIO_2_SLEW {fast} CONFIG.PCW_MIO_3_SLEW {fast} CONFIG.PCW_MIO_4_SLEW {fast} CONFIG.PCW_MIO_5_SLEW {fast} CONFIG.PCW_MIO_6_SLEW {fast}] [get_bd_cells processing_system7_0]

# Copies XDC constraints file
file mkdir "${project_vivado_path}/${project_name}.srcs/constrs_1"
file copy -force "${project_vivado_path}/constrs_qspi.xdc" "${project_vivado_path}/${project_name}.srcs/constrs_1/constrs_qspi.xdc"

# Adds XDC constraints file
add_files -fileset constrs_1 -norecurse "${project_vivado_path}/${project_name}.srcs/constrs_1/constrs_qspi.xdc"

# Saves the design
save_bd_design

# Closes the project
close_project