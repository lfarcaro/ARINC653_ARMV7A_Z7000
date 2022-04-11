#ifndef CP15_H
#define CP15_H

// Flags for the CP15ControlFeatureDisable and CP15ControlFeatureEnable functions
#define CP15_CONTROL_MMU (0x00000001)
#define CP15_CONTROL_ALIGN_CHCK (0x00000002)
#define CP15_CONTROL_TEXREMAP (0x10000000)
#define CP15_CONTROL_ACCESSFLAG (0x20000000)

extern unsigned int CP15CoreIDGet(void);
extern void CP15Ttbr0Set(unsigned int ttbr);
extern void CP15Ttbr1Set(unsigned int ttbr);
extern void CP15TlbInvalidate(void);
extern void CP15SetContextID(unsigned int contextid);
extern void CP15MMUDisable(void);
extern void CP15MMUEnable(void);
extern void CP15VectorBaseAddrSet(unsigned int addr);
extern void CP15BranchPredictorInvalidate(void);
extern void CP15BranchPredictionEnable(void);
extern void CP15BranchPredictionDisable(void);
extern void CP15DomainAccessClientSet(void);
extern void CP15ControlFeatureDisable(unsigned int features);
extern void CP15ControlFeatureEnable(unsigned int features);
extern void CP15TtbCtlConfig(unsigned int N);

#endif /* CP15_H */
