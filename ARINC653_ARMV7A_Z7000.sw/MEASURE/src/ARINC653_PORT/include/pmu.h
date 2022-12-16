#ifndef PMU_H_
#define PMU_H_

// Includes
#include "xparameters.h"
#include "xil_io.h"

// PMU 0 registers
#define PMU0_REG_PMXEVCNTR0 (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000000)
#define PMU0_REG_PMXEVCNTR1 (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000004)
#define PMU0_REG_PMXEVCNTR2 (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000008)
#define PMU0_REG_PMXEVCNTR3 (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x0000000C)
#define PMU0_REG_PMXEVCNTR4 (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000010)
#define PMU0_REG_PMXEVCNTR5 (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000014)
#define PMU0_REG_PMCCNTR (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x0000007C)
#define PMU0_REG_PMXEVTYPER0 (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000400)
#define PMU0_REG_PMXEVTYPER1 (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000404)
#define PMU0_REG_PMXEVTYPER2 (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000408)
#define PMU0_REG_PMXEVTYPER3 (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x0000040C)
#define PMU0_REG_PMXEVTYPER4 (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000410)
#define PMU0_REG_PMXEVTYPER5 (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000414)
#define PMU0_REG_PMCNTENSET (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000C00)
#define PMU0_REG_PMCNTENCLR (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000C20)
#define PMU0_REG_PMINTENSET (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000C40)
#define PMU0_REG_PMINTENCLR (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000C60)
#define PMU0_REG_PMOVSR (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000C80)
#define PMU0_REG_PMSWINC (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000CA0)
#define PMU0_REG_PMCR (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000E04)
#define PMU0_REG_PMUSERENR (XPAR_PS7_PMU_0_S_AXI_BASEADDR+0x00000E08)

// PMU 1 registers
#define PMU1_REG_PMXEVCNTR0 (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000000)
#define PMU1_REG_PMXEVCNTR1 (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000004)
#define PMU1_REG_PMXEVCNTR2 (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000008)
#define PMU1_REG_PMXEVCNTR3 (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x0000000C)
#define PMU1_REG_PMXEVCNTR4 (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000010)
#define PMU1_REG_PMXEVCNTR5 (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000014)
#define PMU1_REG_PMCCNTR (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x0000007C)
#define PMU1_REG_PMXEVTYPER0 (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000400)
#define PMU1_REG_PMXEVTYPER1 (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000404)
#define PMU1_REG_PMXEVTYPER2 (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000408)
#define PMU1_REG_PMXEVTYPER3 (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x0000040C)
#define PMU1_REG_PMXEVTYPER4 (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000410)
#define PMU1_REG_PMXEVTYPER5 (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000414)
#define PMU1_REG_PMCNTENSET (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000C00)
#define PMU1_REG_PMCNTENCLR (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000C20)
#define PMU1_REG_PMINTENSET (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000C40)
#define PMU1_REG_PMINTENCLR (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000C60)
#define PMU1_REG_PMOVSR (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000C80)
#define PMU1_REG_PMSWINC (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000CA0)
#define PMU1_REG_PMCR (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000E04)
#define PMU1_REG_PMUSERENR (XPAR_PS7_PMU_0_PMU1_S_AXI_BASEADDR+0x00000E08)

// Register read/write methods
unsigned int PMU_READ_PMCCNTR(void);
void PMU_WRITE_PMCCNTR(unsigned int VALUE);
unsigned int PMU_READ_PMCEID0(void);
unsigned int PMU_READ_PMCEID1(void);
unsigned int PMU_READ_PMCNTENCLR(void);
void PMU_WRITE_PMCNTENCLR(unsigned int VALUE);
unsigned int PMU_READ_PMCNTENSET(void);
void PMU_WRITE_PMCNTENSET(unsigned int VALUE);
unsigned int PMU_READ_PMCR(void);
void PMU_WRITE_PMCR(unsigned int VALUE);
unsigned int PMU_READ_PMINTENCLR(void);
void PMU_WRITE_PMINTENCLR(unsigned int VALUE);
unsigned int PMU_READ_PMINTENSET(void);
void PMU_WRITE_PMINTENSET(unsigned int VALUE);
unsigned int PMU_READ_PMOVSR(void);
void PMU_WRITE_PMOVSR(unsigned int VALUE);
unsigned int PMU_READ_PMOVSSET(void);
void PMU_WRITE_PMOVSSET(unsigned int VALUE);
void PMU_WRITE_PMSWINC(unsigned int VALUE);
unsigned int PMU_READ_PMUSERENR(void);
void PMU_WRITE_PMUSERENR(unsigned int VALUE);
unsigned int PMU_READ_PMXEVCNTR(unsigned int PMNX);
void PMU_WRITE_PMXEVCNTR(unsigned int PMNX, unsigned int VALUE);
unsigned int PMU_READ_PMXEVTYPER(unsigned int PMNX);
void PMU_WRITE_PMXEVTYPER(unsigned int PMNX, unsigned int VALUE);

// Startup method
void PMU_STARTUP(void);

#endif