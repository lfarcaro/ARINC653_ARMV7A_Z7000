# Gathers variables
variable project_name $env(PROJECT_NAME)
variable blockdesign_name $env(BLOCKDESIGN_NAME)
variable project_vivado_path $env(PROJECT_VIVADO_PATH)

# Opens the project
open_project ${project_vivado_path}/${project_name}.xpr

# Resets run
reset_runs impl_1

# Synthesizes, implements and writes bitstream
launch_runs impl_1 -to_step write_bitstream -jobs 4

# Waits for the process to complete
wait_on_run impl_1

# Closes the project
close_project
