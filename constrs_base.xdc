# UART0
set_property -dict {PACKAGE_PIN Y18 IOSTANDARD LVCMOS33} [get_ports {UART_0_0_txd}] ; ## UART0 TX - GPIO1_4P - JP1.9
set_property -dict {PACKAGE_PIN Y19 IOSTANDARD LVCMOS33} [get_ports {UART_0_0_rxd}] ; ## UART0 RX - GPIO1_4N - JP1.10

# UART1
set_property -dict {PACKAGE_PIN B19 IOSTANDARD LVCMOS33} [get_ports {UART_1_0_txd}] ; ## UART1 TX - GPIO2_4P - JP2.9
set_property -dict {PACKAGE_PIN A20 IOSTANDARD LVCMOS33} [get_ports {UART_1_0_rxd}] ; ## UART1 RX - GPIO2_4N - JP2.10