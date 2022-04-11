# GPIO
set_property -dict {PACKAGE_PIN P15 IOSTANDARD LVCMOS33} [get_ports {gpio_0_tri_io[0]}] ; ## PL_LED1
set_property -dict {PACKAGE_PIN U12 IOSTANDARD LVCMOS33} [get_ports {gpio_0_tri_io[1]}] ; ## PL_LED2