# Gathers variables
variable project_name $env(PROJECT_NAME)
variable blockdesign_name $env(BLOCKDESIGN_NAME)
variable project_vivado_path $env(PROJECT_VIVADO_PATH)

# Opens the project
open_project ${project_vivado_path}/${project_name}.xpr

# Opens the block design
open_bd_design "${project_vivado_path}/${project_name}.srcs/sources_1/bd/${blockdesign_name}/${blockdesign_name}.bd"

# Enables SD peripheral
set_property -dict [list CONFIG.PCW_SD0_PERIPHERAL_ENABLE {1}] [get_bd_cells processing_system7_0]

# Configures MIO pins
set_property -dict [list CONFIG.PCW_MIO_40_SLEW {slow} CONFIG.PCW_MIO_41_SLEW {slow} CONFIG.PCW_MIO_42_SLEW {slow} CONFIG.PCW_MIO_43_SLEW {slow} CONFIG.PCW_MIO_44_SLEW {slow} CONFIG.PCW_MIO_45_SLEW {slow}] [get_bd_cells processing_system7_0]

# Copies XDC constraints file
file mkdir "${project_vivado_path}/${project_name}.srcs/constrs_1"
file copy -force "${project_vivado_path}/constrs_sd.xdc" "${project_vivado_path}/${project_name}.srcs/constrs_1/constrs_sd.xdc"

# Adds XDC constraints file
add_files -fileset constrs_1 -norecurse "${project_vivado_path}/${project_name}.srcs/constrs_1/constrs_sd.xdc"

# Saves the design
save_bd_design

# Closes the project
close_project