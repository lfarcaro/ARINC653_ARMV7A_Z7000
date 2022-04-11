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

# Enables peripherals
set_property -dict [list CONFIG.PCW_UART0_PERIPHERAL_ENABLE {1} CONFIG.PCW_UART0_UART0_IO {MIO 14 .. 15}] [get_bd_cells processing_system7_0]
set_property -dict [list CONFIG.PCW_GPIO_MIO_GPIO_ENABLE {1} CONFIG.PCW_GPIO_MIO_GPIO_IO {MIO} CONFIG.PCW_GPIO_EMIO_GPIO_ENABLE {1}] [get_bd_cells processing_system7_0]

# Sets Bank 0 voltage to 3.3V
set_property -dict [list CONFIG.PCW_PRESET_BANK0_VOLTAGE {LVCMOS 3.3V}] [get_bd_cells processing_system7_0]

# Sets Bank 1 voltage to 3.3V
set_property -dict [list CONFIG.PCW_PRESET_BANK1_VOLTAGE {LVCMOS 3.3V}] [get_bd_cells processing_system7_0]

# Configures the DDR3 memory
set_property -dict [list CONFIG.PCW_UIPARAM_DDR_PARTNO {MT41K256M16 RE-125} CONFIG.PCW_UIPARAM_DDR_BUS_WIDTH {16 Bit}] [get_bd_cells processing_system7_0]

# Configures UART 0
set_property -dict [list CONFIG.PCW_UART0_BAUD_RATE {115200} CONFIG.PCW_MIO_14_PULLUP {enabled} CONFIG.PCW_MIO_15_PULLUP {enabled}] [get_bd_cells processing_system7_0]

# Configures the main peripheral clock to 50 MHz
set_property -dict [list CONFIG.PCW_FPGA0_PERIPHERAL_FREQMHZ {50}] [get_bd_cells processing_system7_0]

# Configures the interrupts
set_property -dict [list CONFIG.PCW_USE_FABRIC_INTERRUPT {1} CONFIG.PCW_IRQ_F2P_INTR {1}] [get_bd_cells processing_system7_0]

# Connects the peripherals clock to the main AXI clock
connect_bd_net [get_bd_pins processing_system7_0/M_AXI_GP0_ACLK] [get_bd_pins processing_system7_0/FCLK_CLK0]

# Creates the IRQ line concatenation IP
create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:2.1 concat_irq
set_property -dict [list CONFIG.NUM_PORTS {16}] [get_bd_cells concat_irq]
connect_bd_net [get_bd_pins concat_irq/dout] [get_bd_pins processing_system7_0/IRQ_F2P]

# Creates the zero constant IP
create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 constant_0
set_property -dict [list CONFIG.CONST_VAL {0}] [get_bd_cells constant_0]

# Connects the zero constant IP to all IRQ lines
connect_bd_net [get_bd_pins constant_0/dout] [get_bd_pins concat_irq/In0]
connect_bd_net [get_bd_pins constant_0/dout] [get_bd_pins concat_irq/In1]
connect_bd_net [get_bd_pins constant_0/dout] [get_bd_pins concat_irq/In2]
connect_bd_net [get_bd_pins constant_0/dout] [get_bd_pins concat_irq/In3]
connect_bd_net [get_bd_pins constant_0/dout] [get_bd_pins concat_irq/In4]
connect_bd_net [get_bd_pins constant_0/dout] [get_bd_pins concat_irq/In5]
connect_bd_net [get_bd_pins constant_0/dout] [get_bd_pins concat_irq/In6]
connect_bd_net [get_bd_pins constant_0/dout] [get_bd_pins concat_irq/In7]
connect_bd_net [get_bd_pins constant_0/dout] [get_bd_pins concat_irq/In8]
connect_bd_net [get_bd_pins constant_0/dout] [get_bd_pins concat_irq/In9]
connect_bd_net [get_bd_pins constant_0/dout] [get_bd_pins concat_irq/In10]
connect_bd_net [get_bd_pins constant_0/dout] [get_bd_pins concat_irq/In11]
connect_bd_net [get_bd_pins constant_0/dout] [get_bd_pins concat_irq/In12]
connect_bd_net [get_bd_pins constant_0/dout] [get_bd_pins concat_irq/In13]
connect_bd_net [get_bd_pins constant_0/dout] [get_bd_pins concat_irq/In14]
connect_bd_net [get_bd_pins constant_0/dout] [get_bd_pins concat_irq/In15]

# OPTIONAL: Creates AXI peripheral for instantiating the AXI subsystem
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_timebase_wdt:3.0 axi_timebase_wdt_0
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/processing_system7_0/FCLK_CLK0 (100 MHz)} Clk_slave {Auto} Clk_xbar {Auto} Master {/processing_system7_0/M_AXI_GP0} Slave {/axi_timebase_wdt_0/S_AXI} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins axi_timebase_wdt_0/S_AXI]

# Renames main General Purpose (GP) AXI Interconnect
set_property name ps7_gp0_axi_periph [get_bd_cells ps7_0_axi_periph]

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