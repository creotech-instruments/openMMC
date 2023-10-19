/*
 * Si5341 Rev D Configuration Register Export Header File
 *
 * This file represents a series of Skyworks Si5341 Rev D 
 * register writes that can be performed to load a single configuration 
 * on a device. It was created by a Skyworks ClockBuilder Pro
 * export tool.
 *
 * Part:		                                       Si5341 Rev D
 * Design ID:                                          
 * Includes Pre/Post Download Control Register Writes: Yes
 * Created By:                                         ClockBuilder Pro v4.9 [2023-04-11]
 * Timestamp:                                          2023-10-06 17:03:51 GMT+02:00
 *
 * A complete design report corresponding to this export is included at the end 
 * of this header file.
 *
 */

#ifndef SI5341_REVD_REG_CONFIG_HEADER
#define SI5341_REVD_REG_CONFIG_HEADER

#define SI5341_REVD_REG_CONFIG_NUM_REGS				384

typedef struct
{
	unsigned int address; /* 16-bit register address */
	unsigned char value; /* 8-bit register data */

} si5341_revd_register_t;

si5341_revd_register_t const si5341_revd_registers[SI5341_REVD_REG_CONFIG_NUM_REGS] =
{

	/* Start configuration preamble */
	{ 0x0B24, 0xD8 },
	{ 0x0B25, 0x00 },
	/* Rev B stuck divider fix */
	{ 0x0502, 0x01 },
	{ 0x0505, 0x03 },
	{ 0x0957, 0x1F },
	{ 0x0B4E, 0x1A },
	/* End configuration preamble */

	/* Delay 300 msec */
	/*    Delay is worst case time for device to complete any calibration */
	/*    that is running due to device state change previous to this script */
	/*    being processed. */

	/* Start configuration registers */
	{ 0x000B, 0x74 },
	{ 0x0017, 0xFA },
	{ 0x0018, 0xFF },
	{ 0x0021, 0x0F },
	{ 0x0022, 0x00 },
	{ 0x002B, 0x02 },
	{ 0x002C, 0x27 },
	{ 0x002D, 0x15 },
	{ 0x002E, 0x93 },
	{ 0x002F, 0x00 },
	{ 0x0030, 0x93 },
	{ 0x0031, 0x00 },
	{ 0x0032, 0x93 },
	{ 0x0033, 0x00 },
	{ 0x0034, 0x00 },
	{ 0x0035, 0x00 },
	{ 0x0036, 0x93 },
	{ 0x0037, 0x00 },
	{ 0x0038, 0x93 },
	{ 0x0039, 0x00 },
	{ 0x003A, 0x93 },
	{ 0x003B, 0x00 },
	{ 0x003C, 0x00 },
	{ 0x003D, 0x00 },
	{ 0x0041, 0x05 },
	{ 0x0042, 0x05 },
	{ 0x0043, 0x05 },
	{ 0x0044, 0x00 },
	{ 0x009E, 0x00 },
	{ 0x0102, 0x01 },
	{ 0x0108, 0x06 },
	{ 0x0109, 0x09 },
	{ 0x010A, 0x3E },
	{ 0x010B, 0x19 },
	{ 0x010D, 0x06 },
	{ 0x010E, 0x09 },
	{ 0x010F, 0x3E },
	{ 0x0110, 0x19 },
	{ 0x0112, 0x01 },
	{ 0x0113, 0x09 },
	{ 0x0114, 0x3B },
	{ 0x0115, 0x28 },
	{ 0x0117, 0x01 },
	{ 0x0118, 0x09 },
	{ 0x0119, 0x3B },
	{ 0x011A, 0x28 },
	{ 0x011C, 0x01 },
	{ 0x011D, 0x09 },
	{ 0x011E, 0x3E },
	{ 0x011F, 0x18 },
	{ 0x0121, 0x01 },
	{ 0x0122, 0x09 },
	{ 0x0123, 0x3E },
	{ 0x0124, 0x18 },
	{ 0x0126, 0x06 },
	{ 0x0127, 0x09 },
	{ 0x0128, 0x3E },
	{ 0x0129, 0x18 },
	{ 0x012B, 0x02 },
	{ 0x012C, 0x09 },
	{ 0x012D, 0x3B },
	{ 0x012E, 0x29 },
	{ 0x0130, 0x01 },
	{ 0x0131, 0x09 },
	{ 0x0132, 0x3B },
	{ 0x0133, 0x28 },
	{ 0x013A, 0x01 },
	{ 0x013B, 0x09 },
	{ 0x013C, 0x3B },
	{ 0x013D, 0x28 },
	{ 0x013F, 0x00 },
	{ 0x0140, 0x00 },
	{ 0x0141, 0x40 },
	{ 0x0206, 0x00 },
	{ 0x0208, 0x01 },
	{ 0x0209, 0x00 },
	{ 0x020A, 0x00 },
	{ 0x020B, 0x00 },
	{ 0x020C, 0x00 },
	{ 0x020D, 0x00 },
	{ 0x020E, 0x01 },
	{ 0x020F, 0x00 },
	{ 0x0210, 0x00 },
	{ 0x0211, 0x00 },
	{ 0x0212, 0x01 },
	{ 0x0213, 0x00 },
	{ 0x0214, 0x00 },
	{ 0x0215, 0x00 },
	{ 0x0216, 0x00 },
	{ 0x0217, 0x00 },
	{ 0x0218, 0x01 },
	{ 0x0219, 0x00 },
	{ 0x021A, 0x00 },
	{ 0x021B, 0x00 },
	{ 0x021C, 0x01 },
	{ 0x021D, 0x00 },
	{ 0x021E, 0x00 },
	{ 0x021F, 0x00 },
	{ 0x0220, 0x00 },
	{ 0x0221, 0x00 },
	{ 0x0222, 0x01 },
	{ 0x0223, 0x00 },
	{ 0x0224, 0x00 },
	{ 0x0225, 0x00 },
	{ 0x0226, 0x00 },
	{ 0x0227, 0x00 },
	{ 0x0228, 0x00 },
	{ 0x0229, 0x00 },
	{ 0x022A, 0x00 },
	{ 0x022B, 0x00 },
	{ 0x022C, 0x00 },
	{ 0x022D, 0x00 },
	{ 0x022E, 0x00 },
	{ 0x022F, 0x00 },
	{ 0x0235, 0x00 },
	{ 0x0236, 0x00 },
	{ 0x0237, 0x00 },
	{ 0x0238, 0xD8 },
	{ 0x0239, 0xD6 },
	{ 0x023A, 0x00 },
	{ 0x023B, 0x00 },
	{ 0x023C, 0x00 },
	{ 0x023D, 0x00 },
	{ 0x023E, 0xC0 },
	{ 0x024A, 0x00 },
	{ 0x024B, 0x00 },
	{ 0x024C, 0x00 },
	{ 0x024D, 0x00 },
	{ 0x024E, 0x00 },
	{ 0x024F, 0x00 },
	{ 0x0250, 0x00 },
	{ 0x0251, 0x00 },
	{ 0x0252, 0x00 },
	{ 0x0253, 0x00 },
	{ 0x0254, 0x00 },
	{ 0x0255, 0x00 },
	{ 0x0256, 0x00 },
	{ 0x0257, 0x00 },
	{ 0x0258, 0x00 },
	{ 0x0259, 0x00 },
	{ 0x025A, 0x00 },
	{ 0x025B, 0x00 },
	{ 0x025C, 0x00 },
	{ 0x025D, 0x00 },
	{ 0x025E, 0x00 },
	{ 0x025F, 0x13 },
	{ 0x0260, 0x00 },
	{ 0x0261, 0x00 },
	{ 0x0262, 0x00 },
	{ 0x0263, 0x00 },
	{ 0x0264, 0x00 },
	{ 0x0268, 0x00 },
	{ 0x0269, 0x00 },
	{ 0x026A, 0x00 },
	{ 0x026B, 0x00 },
	{ 0x026C, 0x00 },
	{ 0x026D, 0x00 },
	{ 0x026E, 0x00 },
	{ 0x026F, 0x00 },
	{ 0x0270, 0x00 },
	{ 0x0271, 0x00 },
	{ 0x0272, 0x00 },
	{ 0x0302, 0x00 },
	{ 0x0303, 0x00 },
	{ 0x0304, 0x00 },
	{ 0x0305, 0x00 },
	{ 0x0306, 0x16 },
	{ 0x0307, 0x00 },
	{ 0x0308, 0x00 },
	{ 0x0309, 0x00 },
	{ 0x030A, 0x00 },
	{ 0x030B, 0x80 },
	{ 0x030C, 0x00 },
	{ 0x030D, 0x00 },
	{ 0x030E, 0x00 },
	{ 0x030F, 0x00 },
	{ 0x0310, 0x30 },
	{ 0x0311, 0x11 },
	{ 0x0312, 0x00 },
	{ 0x0313, 0x00 },
	{ 0x0314, 0x00 },
	{ 0x0315, 0x00 },
	{ 0x0316, 0x80 },
	{ 0x0317, 0x00 },
	{ 0x0318, 0x00 },
	{ 0x0319, 0x00 },
	{ 0x031A, 0x00 },
	{ 0x031B, 0x00 },
	{ 0x031C, 0x00 },
	{ 0x031D, 0x00 },
	{ 0x031E, 0x00 },
	{ 0x031F, 0x00 },
	{ 0x0320, 0x00 },
	{ 0x0321, 0x00 },
	{ 0x0322, 0x00 },
	{ 0x0323, 0x00 },
	{ 0x0324, 0x00 },
	{ 0x0325, 0x00 },
	{ 0x0326, 0x00 },
	{ 0x0327, 0x00 },
	{ 0x0328, 0x00 },
	{ 0x0329, 0x00 },
	{ 0x032A, 0x00 },
	{ 0x032B, 0x00 },
	{ 0x032C, 0x00 },
	{ 0x032D, 0x00 },
	{ 0x032E, 0x00 },
	{ 0x032F, 0x00 },
	{ 0x0330, 0x00 },
	{ 0x0331, 0x00 },
	{ 0x0332, 0x00 },
	{ 0x0333, 0x00 },
	{ 0x0334, 0x00 },
	{ 0x0335, 0x00 },
	{ 0x0336, 0x00 },
	{ 0x0337, 0x00 },
	{ 0x0338, 0x00 },
	{ 0x0339, 0x1F },
	{ 0x033B, 0x00 },
	{ 0x033C, 0x00 },
	{ 0x033D, 0x00 },
	{ 0x033E, 0x00 },
	{ 0x033F, 0x00 },
	{ 0x0340, 0x00 },
	{ 0x0341, 0x00 },
	{ 0x0342, 0x00 },
	{ 0x0343, 0x00 },
	{ 0x0344, 0x00 },
	{ 0x0345, 0x00 },
	{ 0x0346, 0x00 },
	{ 0x0347, 0x00 },
	{ 0x0348, 0x00 },
	{ 0x0349, 0x00 },
	{ 0x034A, 0x00 },
	{ 0x034B, 0x00 },
	{ 0x034C, 0x00 },
	{ 0x034D, 0x00 },
	{ 0x034E, 0x00 },
	{ 0x034F, 0x00 },
	{ 0x0350, 0x00 },
	{ 0x0351, 0x00 },
	{ 0x0352, 0x00 },
	{ 0x0353, 0x00 },
	{ 0x0354, 0x00 },
	{ 0x0355, 0x00 },
	{ 0x0356, 0x00 },
	{ 0x0357, 0x00 },
	{ 0x0358, 0x00 },
	{ 0x0359, 0x00 },
	{ 0x035A, 0x00 },
	{ 0x035B, 0x00 },
	{ 0x035C, 0x00 },
	{ 0x035D, 0x00 },
	{ 0x035E, 0x00 },
	{ 0x035F, 0x00 },
	{ 0x0360, 0x00 },
	{ 0x0361, 0x00 },
	{ 0x0362, 0x00 },
	{ 0x0802, 0x00 },
	{ 0x0803, 0x00 },
	{ 0x0804, 0x00 },
	{ 0x0805, 0x00 },
	{ 0x0806, 0x00 },
	{ 0x0807, 0x00 },
	{ 0x0808, 0x00 },
	{ 0x0809, 0x00 },
	{ 0x080A, 0x00 },
	{ 0x080B, 0x00 },
	{ 0x080C, 0x00 },
	{ 0x080D, 0x00 },
	{ 0x080E, 0x00 },
	{ 0x080F, 0x00 },
	{ 0x0810, 0x00 },
	{ 0x0811, 0x00 },
	{ 0x0812, 0x00 },
	{ 0x0813, 0x00 },
	{ 0x0814, 0x00 },
	{ 0x0815, 0x00 },
	{ 0x0816, 0x00 },
	{ 0x0817, 0x00 },
	{ 0x0818, 0x00 },
	{ 0x0819, 0x00 },
	{ 0x081A, 0x00 },
	{ 0x081B, 0x00 },
	{ 0x081C, 0x00 },
	{ 0x081D, 0x00 },
	{ 0x081E, 0x00 },
	{ 0x081F, 0x00 },
	{ 0x0820, 0x00 },
	{ 0x0821, 0x00 },
	{ 0x0822, 0x00 },
	{ 0x0823, 0x00 },
	{ 0x0824, 0x00 },
	{ 0x0825, 0x00 },
	{ 0x0826, 0x00 },
	{ 0x0827, 0x00 },
	{ 0x0828, 0x00 },
	{ 0x0829, 0x00 },
	{ 0x082A, 0x00 },
	{ 0x082B, 0x00 },
	{ 0x082C, 0x00 },
	{ 0x082D, 0x00 },
	{ 0x082E, 0x00 },
	{ 0x082F, 0x00 },
	{ 0x0830, 0x00 },
	{ 0x0831, 0x00 },
	{ 0x0832, 0x00 },
	{ 0x0833, 0x00 },
	{ 0x0834, 0x00 },
	{ 0x0835, 0x00 },
	{ 0x0836, 0x00 },
	{ 0x0837, 0x00 },
	{ 0x0838, 0x00 },
	{ 0x0839, 0x00 },
	{ 0x083A, 0x00 },
	{ 0x083B, 0x00 },
	{ 0x083C, 0x00 },
	{ 0x083D, 0x00 },
	{ 0x083E, 0x00 },
	{ 0x083F, 0x00 },
	{ 0x0840, 0x00 },
	{ 0x0841, 0x00 },
	{ 0x0842, 0x00 },
	{ 0x0843, 0x00 },
	{ 0x0844, 0x00 },
	{ 0x0845, 0x00 },
	{ 0x0846, 0x00 },
	{ 0x0847, 0x00 },
	{ 0x0848, 0x00 },
	{ 0x0849, 0x00 },
	{ 0x084A, 0x00 },
	{ 0x084B, 0x00 },
	{ 0x084C, 0x00 },
	{ 0x084D, 0x00 },
	{ 0x084E, 0x00 },
	{ 0x084F, 0x00 },
	{ 0x0850, 0x00 },
	{ 0x0851, 0x00 },
	{ 0x0852, 0x00 },
	{ 0x0853, 0x00 },
	{ 0x0854, 0x00 },
	{ 0x0855, 0x00 },
	{ 0x0856, 0x00 },
	{ 0x0857, 0x00 },
	{ 0x0858, 0x00 },
	{ 0x0859, 0x00 },
	{ 0x085A, 0x00 },
	{ 0x085B, 0x00 },
	{ 0x085C, 0x00 },
	{ 0x085D, 0x00 },
	{ 0x085E, 0x00 },
	{ 0x085F, 0x00 },
	{ 0x0860, 0x00 },
	{ 0x0861, 0x00 },
	{ 0x090E, 0x02 },
	{ 0x091C, 0x04 },
	{ 0x0943, 0x01 },
	{ 0x0949, 0x07 },
	{ 0x094A, 0x70 },
	{ 0x094E, 0x49 },
	{ 0x094F, 0x02 },
	{ 0x095E, 0x00 },
	{ 0x0A02, 0x00 },
	{ 0x0A03, 0x03 },
	{ 0x0A04, 0x01 },
	{ 0x0A05, 0x03 },
	{ 0x0A14, 0x00 },
	{ 0x0A1A, 0x00 },
	{ 0x0A20, 0x00 },
	{ 0x0A26, 0x00 },
	{ 0x0A2C, 0x00 },
	{ 0x0B44, 0x0F },
	{ 0x0B4A, 0x1C },
	{ 0x0B57, 0x0E },
	{ 0x0B58, 0x01 },
	/* End configuration registers */

	/* Start configuration postamble */
	{ 0x001C, 0x01 },
	{ 0x0B24, 0xDB },
	{ 0x0B25, 0x02 },
	/* End configuration postamble */

};

/*
 * Design Report
 *
 * Overview
 * ========
 * 
 * Part:               Si5341AB Rev D
 * Project File:       C:\Users\mbiernat\OneDrive - Creotech Instruments S.A\Dokumenty\PROJEKTY\[AFCZ]\Si5341 ClockBuilder\Si5341-RevD-Project_Marcin.slabtimeproj
 * Design ID:          <none>
 * Created By:         ClockBuilder Pro v4.9 [2023-04-11]
 * Timestamp:          2023-10-06 17:03:51 GMT+02:00
 * 
 * Design Rule Check
 * =================
 * 
 * Errors:
 * - No errors
 * 
 * Warnings:
 * - No warnings
 * 
 * Device Grade
 * ============
 * Maximum Output Frequency: 200 MHz
 * Frequency Synthesis Mode: Fractional
 * Frequency Plan Grade:     B
 * Minimum Base OPN:         Si5341B*
 * 
 * Base       Output Clock         Supported Frequency Synthesis Modes
 * OPN Grade  Frequency Range      (Typical Jitter)
 * ---------  -------------------  --------------------------------------------
 * Si5341A    100 Hz to 1.028 GHz  Integer (< 100 fs) and fractional (< 150 fs)
 * Si5341B*   100 Hz to 350 MHz    "
 * Si5341C    100 Hz to 1.028 GHz  Integer only (< 100 fs)
 * Si5341D    100 Hz to 350 MHz    "
 * 
 * * Based on your calculated frequency plan, a Si5341B grade device is
 * sufficient for your design. For more in-system configuration flexibility
 * (higher frequencies and/or to enable fractional synthesis), consider
 * selecting device grade Si5341A when specifying an ordering part number (OPN)
 * for your application. See the datasheet Ordering Guide for more information.
 * 
 * Design
 * ======
 * Host Interface:
 *    I/O Power Supply: VDDA (3.3V)
 *    SPI Mode: 4-Wire
 *    I2C Address Range: 116d to 119d / 0x74 to 0x77 (selected via A0/A1 pins)
 * 
 * Inputs:
 *    XAXB: 48 MHz
 *          Crystal Mode
 *     IN0: 48 MHz
 *          Standard
 *     IN1: 48 MHz
 *          Standard
 *     IN2: 48 MHz
 *          Standard
 *   FB_IN: Unused
 * 
 * Outputs:
 *    OUT0: 200 MHz
 *          Enabled, LVDS 1.8 V
 *    OUT1: 200 MHz
 *          Enabled, LVDS 1.8 V
 *    OUT2: Unused
 *    OUT3: Unused
 *    OUT4: Unused
 *    OUT5: Unused
 *    OUT6: 156.25 MHz
 *          Enabled, LVDS 1.8 V
 *    OUT7: 10 MHz
 *          Enabled, LVDS 2.5 V
 *    OUT8: Unused
 *    OUT9: Unused
 * 
 * Frequency Plan
 * ==============
 * 
 * Priority: OUT6 is lowest jitter output
 * 
 * Fpfd = 48 MHz
 * Fvco = 13.75 GHz
 * Fms0 = 312.5 MHz
 * Fms1 = 400 MHz
 * 
 * P dividers:
 *    P0  = 1
 *    P1  = 1
 *    P2  = 1
 *    P3  = Unused
 *    Pxaxb = 1
 * 
 * M = 286.4583333333333333... [ 286 + 11/24 ]
 * N dividers:
 *    N0:
 *       Value: 44
 *       OUT6: 156.25 MHz
 *    N1:
 *       Value: 34.375
 *       OUT0: 200 MHz
 *       OUT1: 200 MHz
 *       OUT7: 10 MHz
 *    N2:
 *       Unused
 *    N3:
 *       Unused
 *    N4:
 *       Unused
 * 
 * R dividers:
 *    R0 = 2
 *    R1 = 2
 *    R2 = Unused
 *    R3 = Unused
 *    R4 = Unused
 *    R5 = Unused
 *    R6 = 2
 *    R7 = 40
 *    R8 = Unused
 *    R9 = Unused
 * 
 * Dividers listed above show effective values. These values are translated to register settings by ClockBuilder Pro. For the actual register values, see below. Refer to the Family Reference Manual for information on registers related to frequency plan.
 * 
 * Digitally Controlled Oscillator (DCO)
 * =====================================
 * Mode: Register Direct Write
 * 
 * N0: DCO Disabled
 * 
 * N1: DCO Disabled
 * 
 * N2: DCO Disabled
 * 
 * N3: DCO Disabled
 * 
 * N4: DCO Disabled
 * 
 * Estimated Power & Junction Temperature
 * ======================================
 * Assumptions:
 * 
 * VDD:      1.8 V
 * Ta:       25 �C
 * Theta-JA: 18.30 �C/W (JEDEC Board with 2 m/s airflow)
 * 
 *                               Overall  On Chip
 * Condition                     Power    Power    Ta    Tj
 * ----------------------------  -------  -------  ----  ----
 * Typical Ta, Voltage, Current  742 mW   718 mW   25 C  38 C
 * 
 *                                    -----------------------
 *                                            Typical        
 *                                    -----------------------
 *                                    Voltage  Current  Power
 *        Output  Frequency   Format    (V)     (mA)    (mW) 
 *        ------  ----------  ------  -------  -------  -----
 * VDD                                   1.80      132    237
 * VDDA                                  3.30      116    382
 * VDDO0  OUT0       200 MHz    LVDS     1.80       16     29
 * VDDO1  OUT1       200 MHz    LVDS     1.80       16     29
 * VDDO2  OUT2        Unused        
 * VDDO3  OUT3        Unused        
 * VDDO4  OUT4        Unused        
 * VDDO5  OUT5        Unused        
 * VDDO6  OUT6    156.25 MHz    LVDS     1.80       16     28
 * VDDO7  OUT7        10 MHz    LVDS     2.50       15     38
 * VDDO8  OUT8        Unused        
 * VDDO9  OUT9        Unused        
 *                                    -------  -------  -----
 *                                       1.80      179    323
 *                                       2.50       15     38
 *                                       3.30      116    382
 *                                    -------  -------  -----
 *                                      Total             742
 *                                    -------  -------  -----
 * 
 * Note:
 * 
 * - Tj is junction temperature. Tj must be less than 125 �C (on Si5341 Revision
 *   D) for device to comply with datasheet specifications. Tj = Ta +
 *   Theta_JA*On_Chip_Power.
 * - Overall power includes on-chip power dissipation and adds differential load
 *   power dissipation to estimate total power requirements.
 * - Above are estimates only: power and temperature should be measured on your
 *   PCB.
 * - Selection of appropriate Theta-JA is required for most accurate estimate.
 *   Ideally, select 'User Specified Theta-JA' and enter a Theta-JA value based
 *   on the thermal properties of your PCB.
 * 
 * Settings
 * ========
 * 
 * Location      Setting Name         Decimal Value      Hex Value        
 * ------------  -------------------  -----------------  -----------------
 * 0x0006[23:0]  TOOL_VERSION         0                  0x000000         
 * 0x000B[6:0]   I2C_ADDR             116                0x74             
 * 0x0017[0]     SYSINCAL_INTR_MSK    0                  0x0              
 * 0x0017[1]     LOSXAXB_INTR_MSK     1                  0x1              
 * 0x0017[2]     LOSREF_INTR_MSK      0                  0x0              
 * 0x0017[3]     LOL_INTR_MSK         1                  0x1              
 * 0x0017[5]     SMB_TMOUT_INTR_MSK   1                  0x1              
 * 0x0018[3:0]   LOSIN_INTR_MSK       15                 0xF              
 * 0x0021[0]     IN_SEL_REGCTRL       1                  0x1              
 * 0x0021[2:1]   IN_SEL               3                  0x3              
 * 0x0022[1]     OE                   0                  0x0              
 * 0x002B[3]     SPI_3WIRE            0                  0x0              
 * 0x002B[5]     AUTO_NDIV_UPDATE     0                  0x0              
 * 0x002C[3:0]   LOS_EN               7                  0x7              
 * 0x002C[4]     LOSXAXB_DIS          0                  0x0              
 * 0x002D[1:0]   LOS0_VAL_TIME        1                  0x1              
 * 0x002D[3:2]   LOS1_VAL_TIME        1                  0x1              
 * 0x002D[5:4]   LOS2_VAL_TIME        1                  0x1              
 * 0x002D[7:6]   LOS3_VAL_TIME        0                  0x0              
 * 0x002E[15:0]  LOS0_TRG_THR         147                0x0093           
 * 0x0030[15:0]  LOS1_TRG_THR         147                0x0093           
 * 0x0032[15:0]  LOS2_TRG_THR         147                0x0093           
 * 0x0034[15:0]  LOS3_TRG_THR         0                  0x0000           
 * 0x0036[15:0]  LOS0_CLR_THR         147                0x0093           
 * 0x0038[15:0]  LOS1_CLR_THR         147                0x0093           
 * 0x003A[15:0]  LOS2_CLR_THR         147                0x0093           
 * 0x003C[15:0]  LOS3_CLR_THR         0                  0x0000           
 * 0x0041[4:0]   LOS0_DIV_SEL         5                  0x05             
 * 0x0042[4:0]   LOS1_DIV_SEL         5                  0x05             
 * 0x0043[4:0]   LOS2_DIV_SEL         5                  0x05             
 * 0x0044[4:0]   LOS3_DIV_SEL         0                  0x00             
 * 0x009E[7:4]   LOL_SET_THR          0                  0x0              
 * 0x0102[0]     OUTALL_DISABLE_LOW   1                  0x1              
 * 0x0108[0]     OUT0_PDN             0                  0x0              
 * 0x0108[1]     OUT0_OE              1                  0x1              
 * 0x0108[2]     OUT0_RDIV_FORCE2     1                  0x1              
 * 0x0109[2:0]   OUT0_FORMAT          1                  0x1              
 * 0x0109[3]     OUT0_SYNC_EN         1                  0x1              
 * 0x0109[5:4]   OUT0_DIS_STATE       0                  0x0              
 * 0x0109[7:6]   OUT0_CMOS_DRV        0                  0x0              
 * 0x010A[3:0]   OUT0_CM              14                 0xE              
 * 0x010A[6:4]   OUT0_AMPL            3                  0x3              
 * 0x010B[2:0]   OUT0_MUX_SEL         1                  0x1              
 * 0x010B[5:4]   OUT0_VDD_SEL         1                  0x1              
 * 0x010B[3]     OUT0_VDD_SEL_EN      1                  0x1              
 * 0x010B[7:6]   OUT0_INV             0                  0x0              
 * 0x010D[0]     OUT1_PDN             0                  0x0              
 * 0x010D[1]     OUT1_OE              1                  0x1              
 * 0x010D[2]     OUT1_RDIV_FORCE2     1                  0x1              
 * 0x010E[2:0]   OUT1_FORMAT          1                  0x1              
 * 0x010E[3]     OUT1_SYNC_EN         1                  0x1              
 * 0x010E[5:4]   OUT1_DIS_STATE       0                  0x0              
 * 0x010E[7:6]   OUT1_CMOS_DRV        0                  0x0              
 * 0x010F[3:0]   OUT1_CM              14                 0xE              
 * 0x010F[6:4]   OUT1_AMPL            3                  0x3              
 * 0x0110[2:0]   OUT1_MUX_SEL         1                  0x1              
 * 0x0110[5:4]   OUT1_VDD_SEL         1                  0x1              
 * 0x0110[3]     OUT1_VDD_SEL_EN      1                  0x1              
 * 0x0110[7:6]   OUT1_INV             0                  0x0              
 * 0x0112[0]     OUT2_PDN             1                  0x1              
 * 0x0112[1]     OUT2_OE              0                  0x0              
 * 0x0112[2]     OUT2_RDIV_FORCE2     0                  0x0              
 * 0x0113[2:0]   OUT2_FORMAT          1                  0x1              
 * 0x0113[3]     OUT2_SYNC_EN         1                  0x1              
 * 0x0113[5:4]   OUT2_DIS_STATE       0                  0x0              
 * 0x0113[7:6]   OUT2_CMOS_DRV        0                  0x0              
 * 0x0114[3:0]   OUT2_CM              11                 0xB              
 * 0x0114[6:4]   OUT2_AMPL            3                  0x3              
 * 0x0115[2:0]   OUT2_MUX_SEL         0                  0x0              
 * 0x0115[5:4]   OUT2_VDD_SEL         2                  0x2              
 * 0x0115[3]     OUT2_VDD_SEL_EN      1                  0x1              
 * 0x0115[7:6]   OUT2_INV             0                  0x0              
 * 0x0117[0]     OUT3_PDN             1                  0x1              
 * 0x0117[1]     OUT3_OE              0                  0x0              
 * 0x0117[2]     OUT3_RDIV_FORCE2     0                  0x0              
 * 0x0118[2:0]   OUT3_FORMAT          1                  0x1              
 * 0x0118[3]     OUT3_SYNC_EN         1                  0x1              
 * 0x0118[5:4]   OUT3_DIS_STATE       0                  0x0              
 * 0x0118[7:6]   OUT3_CMOS_DRV        0                  0x0              
 * 0x0119[3:0]   OUT3_CM              11                 0xB              
 * 0x0119[6:4]   OUT3_AMPL            3                  0x3              
 * 0x011A[2:0]   OUT3_MUX_SEL         0                  0x0              
 * 0x011A[5:4]   OUT3_VDD_SEL         2                  0x2              
 * 0x011A[3]     OUT3_VDD_SEL_EN      1                  0x1              
 * 0x011A[7:6]   OUT3_INV             0                  0x0              
 * 0x011C[0]     OUT4_PDN             1                  0x1              
 * 0x011C[1]     OUT4_OE              0                  0x0              
 * 0x011C[2]     OUT4_RDIV_FORCE2     0                  0x0              
 * 0x011D[2:0]   OUT4_FORMAT          1                  0x1              
 * 0x011D[3]     OUT4_SYNC_EN         1                  0x1              
 * 0x011D[5:4]   OUT4_DIS_STATE       0                  0x0              
 * 0x011D[7:6]   OUT4_CMOS_DRV        0                  0x0              
 * 0x011E[3:0]   OUT4_CM              14                 0xE              
 * 0x011E[6:4]   OUT4_AMPL            3                  0x3              
 * 0x011F[2:0]   OUT4_MUX_SEL         0                  0x0              
 * 0x011F[5:4]   OUT4_VDD_SEL         1                  0x1              
 * 0x011F[3]     OUT4_VDD_SEL_EN      1                  0x1              
 * 0x011F[7:6]   OUT4_INV             0                  0x0              
 * 0x0121[0]     OUT5_PDN             1                  0x1              
 * 0x0121[1]     OUT5_OE              0                  0x0              
 * 0x0121[2]     OUT5_RDIV_FORCE2     0                  0x0              
 * 0x0122[2:0]   OUT5_FORMAT          1                  0x1              
 * 0x0122[3]     OUT5_SYNC_EN         1                  0x1              
 * 0x0122[5:4]   OUT5_DIS_STATE       0                  0x0              
 * 0x0122[7:6]   OUT5_CMOS_DRV        0                  0x0              
 * 0x0123[3:0]   OUT5_CM              14                 0xE              
 * 0x0123[6:4]   OUT5_AMPL            3                  0x3              
 * 0x0124[2:0]   OUT5_MUX_SEL         0                  0x0              
 * 0x0124[5:4]   OUT5_VDD_SEL         1                  0x1              
 * 0x0124[3]     OUT5_VDD_SEL_EN      1                  0x1              
 * 0x0124[7:6]   OUT5_INV             0                  0x0              
 * 0x0126[0]     OUT6_PDN             0                  0x0              
 * 0x0126[1]     OUT6_OE              1                  0x1              
 * 0x0126[2]     OUT6_RDIV_FORCE2     1                  0x1              
 * 0x0127[2:0]   OUT6_FORMAT          1                  0x1              
 * 0x0127[3]     OUT6_SYNC_EN         1                  0x1              
 * 0x0127[5:4]   OUT6_DIS_STATE       0                  0x0              
 * 0x0127[7:6]   OUT6_CMOS_DRV        0                  0x0              
 * 0x0128[3:0]   OUT6_CM              14                 0xE              
 * 0x0128[6:4]   OUT6_AMPL            3                  0x3              
 * 0x0129[2:0]   OUT6_MUX_SEL         0                  0x0              
 * 0x0129[5:4]   OUT6_VDD_SEL         1                  0x1              
 * 0x0129[3]     OUT6_VDD_SEL_EN      1                  0x1              
 * 0x0129[7:6]   OUT6_INV             0                  0x0              
 * 0x012B[0]     OUT7_PDN             0                  0x0              
 * 0x012B[1]     OUT7_OE              1                  0x1              
 * 0x012B[2]     OUT7_RDIV_FORCE2     0                  0x0              
 * 0x012C[2:0]   OUT7_FORMAT          1           { 0x0B24, 0xD8 },
 */

#endif
