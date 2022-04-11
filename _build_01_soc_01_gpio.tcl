# Gathers variables
variable project_name $env(PROJECT_NAME)
variable blockdesign_name $env(BLOCKDESIGN_NAME)
variable project_vivado_path $env(PROJECT_VIVADO_PATH)

# Opens the project
open_project ${project_vivado_path}/${project_name}.xpr

# Opens the block design
open_bd_design "${project_vivado_path}/${project_name}.srcs/sources_1/bd/${blockdesign_name}/${blockdesign_name}.bd"

# Makes available EMIO GPIO signals
set_property -dict [list CONFIG.PCW_GPIO_EMIO_GPIO_IO {2}] [get_bd_cells processing_system7_0]
make_bd_intf_pins_external -name gpio_0 [get_bd_intf_pins processing_system7_0/GPIO_0]

# Copies XDC constraints file
file mkdir "${project_vivado_path}/${project_name}.srcs/constrs_1"
file copy -force "${project_vivado_path}/constrs_gpio.xdc" "${project_vivado_path}/${project_name}.srcs/constrs_1/constrs_gpio.xdc"

# Adds XDC constraints file
add_files -fileset constrs_1 -norecurse "${project_vivado_path}/${project_name}.srcs/constrs_1/constrs_gpio.xdc"

# Saves the design
save_bd_design

# Closes the project
close_project