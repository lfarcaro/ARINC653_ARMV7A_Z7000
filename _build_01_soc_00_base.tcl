# Gathers variables
variable project_name $env(PROJECT_NAME)
variable blockdesign_name $env(BLOCKDESIGN_NAME)
variable project_vivado_path $env(PROJECT_VIVADO_PATH)

# Creates the project
create_project ${project_name} ${project_vivado_path} -force -part xc7z010clg400-1
set_property target_language VHDL [current_project]

# Creates the block design
create_bd_design ${blockdesign_name}

# Sets block design as current
current_bd_design ${blockdesign_name}

# Creates Zynq7 processing system block and loads the default preset
create_bd_cell -type ip -vlnv xilinx.com:ip:processing_system7:5.5 processing_system7_0
set_property -dict [list CONFIG.preset {Default}] [get_bd_cells processing_system7_0]
apply_bd_automation -rule xilinx.com:bd_rule:processing_system7 -config {make_external "FIXED_IO, DDR" Master "Disable" Slave "Disable" }  [get_bd_cells processing_system7_0]
set_property -dict [list CONFIG.PCW_USE_M_AXI_GP0 {0}] [get_bd_cells processing_system7_0]
set_property -dict [list CONFIG.PCW_EN_CLK0_PORT {0}] [get_bd_cells processing_system7_0]
set_property -dict [list CONFIG.PCW_EN_RST0_PORT {0}] [get_bd_cells processing_system7_0]

# Enables peripherals
set_property -dict [list CONFIG.PCW_UART0_PERIPHERAL_ENABLE {1} CONFIG.PCW_UART0_UART0_IO {EMIO}] [get_bd_cells processing_system7_0]
set_property -dict [list CONFIG.PCW_UART1_PERIPHERAL_ENABLE {1} CONFIG.PCW_UART1_UART1_IO {EMIO}] [get_bd_cells processing_system7_0]
set_property -dict [list CONFIG.PCW_GPIO_MIO_GPIO_ENABLE {1} CONFIG.PCW_GPIO_MIO_GPIO_IO {MIO} CONFIG.PCW_GPIO_EMIO_GPIO_ENABLE {1}] [get_bd_cells processing_system7_0]

# Sets Bank 0 voltage to 3.3V
set_property -dict [list CONFIG.PCW_PRESET_BANK0_VOLTAGE {LVCMOS 3.3V}] [get_bd_cells processing_system7_0]

# Sets Bank 1 voltage to 3.3V
set_property -dict [list CONFIG.PCW_PRESET_BANK1_VOLTAGE {LVCMOS 3.3V}] [get_bd_cells processing_system7_0]

# Configures the DDR3 memory
set_property -dict [list CONFIG.PCW_UIPARAM_DDR_PARTNO {MT41K256M16 RE-125} CONFIG.PCW_UIPARAM_DDR_BUS_WIDTH {16 Bit}] [get_bd_cells processing_system7_0]

# Configures UART 0
set_property -dict [list CONFIG.PCW_UART0_BAUD_RATE {115200}] [get_bd_cells processing_system7_0]
make_bd_intf_pins_external [get_bd_intf_pins processing_system7_0/UART_0]

# Configures UART 1
set_property -dict [list CONFIG.PCW_UART1_BAUD_RATE {115200}] [get_bd_cells processing_system7_0]
make_bd_intf_pins_external [get_bd_intf_pins processing_system7_0/UART_1]

# Copies XDC constraints file
file mkdir "${project_vivado_path}/${project_name}.srcs/constrs_1"
file copy -force "${project_vivado_path}/constrs_base.xdc" "${project_vivado_path}/${project_name}.srcs/constrs_1/constrs_base.xdc"

# Adds XDC constraints file
add_files -fileset constrs_1 -norecurse "${project_vivado_path}/${project_name}.srcs/constrs_1/constrs_base.xdc"

# Validates the design
validate_bd_design

# Regenerates the layout
regenerate_bd_layout

# Creates a top level wrapper and adds it to the project
make_wrapper -files [get_files ${project_vivado_path}/${project_name}.srcs/sources_1/bd/${blockdesign_name}/${blockdesign_name}.bd] -top
add_files -norecurse "${project_vivado_path}/${project_name}.srcs/sources_1/bd/${blockdesign_name}/hdl/${blockdesign_name}_wrapper.vhd"

# Sets IP repository path
set_property ip_repo_paths "${project_vivado_path}/ips" [current_project]
update_ip_catalog

# Saves the design
save_bd_design

# Closes the project
close_project