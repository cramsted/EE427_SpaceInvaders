##############################################################################
## Filename:          C:\Users\superman\Desktop\cramshall\EE427_SpaceInvaders\SpaceInvaders/drivers/capacitive_buttons_v1_00_a/data/capacitive_buttons_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Thu Nov 17 20:12:37 2016 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "capacitive_buttons" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
