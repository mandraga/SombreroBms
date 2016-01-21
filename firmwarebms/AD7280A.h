/*
 * AD7280A Lithium Ion Battery Monitoring System
 *
 * Copyright 2011 Analog Devices Inc.
 *
 * Licensed under the GPL-2.
 */

#ifndef IIO_ADC_AD7280_H_
#define IIO_ADC_AD7280_H_

#define BIT(N) (1 << N)

#define AD7280A_ACQ_TIME_400ns			0
#define AD7280A_ACQ_TIME_800ns			1
#define AD7280A_ACQ_TIME_1200ns			2
#define AD7280A_ACQ_TIME_1600ns			3

#define AD7280A_CONV_AVG_DIS			0
#define AD7280A_CONV_AVG_2			1
#define AD7280A_CONV_AVG_4			2
#define AD7280A_CONV_AVG_8			3

#define AD7280A_ALERT_REMOVE_VIN5		BIT(2)
#define AD7280A_ALERT_REMOVE_VIN4_VIN5		BIT(3)
#define AD7280A_ALERT_REMOVE_AUX5		BIT(0)
#define AD7280A_ALERT_REMOVE_AUX4_AUX5		BIT(1)

/* Registers */
#define AD7280A_CELL_VOLTAGE_1		0x0  /* D11 to D0, Read only */
#define AD7280A_CELL_VOLTAGE_2		0x1  /* D11 to D0, Read only */
#define AD7280A_CELL_VOLTAGE_3		0x2  /* D11 to D0, Read only */
#define AD7280A_CELL_VOLTAGE_4		0x3  /* D11 to D0, Read only */
#define AD7280A_CELL_VOLTAGE_5		0x4  /* D11 to D0, Read only */
#define AD7280A_CELL_VOLTAGE_6		0x5  /* D11 to D0, Read only */
#define AD7280A_AUX_ADC_1		0x6  /* D11 to D0, Read only */
#define AD7280A_AUX_ADC_2		0x7  /* D11 to D0, Read only */
#define AD7280A_AUX_ADC_3		0x8  /* D11 to D0, Read only */
#define AD7280A_AUX_ADC_4		0x9  /* D11 to D0, Read only */
#define AD7280A_AUX_ADC_5		0xA  /* D11 to D0, Read only */
#define AD7280A_AUX_ADC_6		0xB  /* D11 to D0, Read only */
#define AD7280A_SELF_TEST		0xC  /* D11 to D0, Read only */
#define AD7280A_CONTROL_HB		0xD  /* D15 to D8, Read/write */
#define AD7280A_CONTROL_LB		0xE  /* D7 to D0, Read/write */
#define AD7280A_CELL_OVERVOLTAGE	0xF  /* D7 to D0, Read/write */
#define AD7280A_CELL_UNDERVOLTAGE	0x10 /* D7 to D0, Read/write */
#define AD7280A_AUX_ADC_OVERVOLTAGE	0x11 /* D7 to D0, Read/write */
#define AD7280A_AUX_ADC_UNDERVOLTAGE	0x12 /* D7 to D0, Read/write */
#define AD7280A_ALERT			0x13 /* D7 to D0, Read/write */
#define AD7280A_CELL_BALANCE		0x14 /* D7 to D0, Read/write */
#define AD7280A_CB1_TIMER		0x15 /* D7 to D0, Read/write */
#define AD7280A_CB2_TIMER		0x16 /* D7 to D0, Read/write */
#define AD7280A_CB3_TIMER		0x17 /* D7 to D0, Read/write */
#define AD7280A_CB4_TIMER		0x18 /* D7 to D0, Read/write */
#define AD7280A_CB5_TIMER		0x19 /* D7 to D0, Read/write */
#define AD7280A_CB6_TIMER		0x1A /* D7 to D0, Read/write */
#define AD7280A_PD_TIMER		0x1B /* D7 to D0, Read/write */
#define AD7280A_READ			0x1C /* D7 to D0, Read/write */
#define AD7280A_CNVST_CONTROL		0x1D /* D7 to D0, Read/write */

/* Bits and Masks */
#define AD7280A_CTRL_HB_CONV_INPUT_ALL			0
#define AD7280A_CTRL_HB_CONV_INPUT_6CELL_AUX1_3_4	BIT(6)
#define AD7280A_CTRL_HB_CONV_INPUT_6CELL		BIT(7)
#define AD7280A_CTRL_HB_CONV_INPUT_SELF_TEST		(BIT(7) | BIT(6))
#define AD7280A_CTRL_HB_CONV_RES_READ_ALL		0
#define AD7280A_CTRL_HB_CONV_RES_READ_6CELL_AUX1_3_4	BIT(4)
#define AD7280A_CTRL_HB_CONV_RES_READ_6CELL		BIT(5)
#define AD7280A_CTRL_HB_CONV_RES_READ_NO		(BIT(5) | BIT(4))
#define AD7280A_CTRL_HB_CONV_START_CNVST		0
#define AD7280A_CTRL_HB_CONV_START_CS			BIT(3)
#define AD7280A_CTRL_HB_CONV_AVG_DIS			0
#define AD7280A_CTRL_HB_CONV_AVG_2			BIT(1)
#define AD7280A_CTRL_HB_CONV_AVG_4			BIT(2)
#define AD7280A_CTRL_HB_CONV_AVG_8			(BIT(2) | BIT(1))
#define AD7280A_CTRL_HB_CONV_AVG(x)			((x) << 1)
#define AD7280A_CTRL_HB_PWRDN_SW			BIT(0)

#define AD7280A_CTRL_LB_SWRST				BIT(7)
#define AD7280A_CTRL_LB_ACQ_TIME_400ns			0
#define AD7280A_CTRL_LB_ACQ_TIME_800ns			BIT(5)
#define AD7280A_CTRL_LB_ACQ_TIME_1200ns			BIT(6)
#define AD7280A_CTRL_LB_ACQ_TIME_1600ns			(BIT(6) | BIT(5))
#define AD7280A_CTRL_LB_ACQ_TIME(x)			((x) << 5)
#define AD7280A_CTRL_LB_MUST_SET			BIT(4)
#define AD7280A_CTRL_LB_THERMISTOR_EN			BIT(3)
#define AD7280A_CTRL_LB_LOCK_DEV_ADDR			BIT(2)
#define AD7280A_CTRL_LB_INC_DEV_ADDR			BIT(1)
#define AD7280A_CTRL_LB_DAISY_CHAIN_RB_EN		BIT(0)

#define AD7280A_ALERT_GEN_STATIC_HIGH			BIT(6)
#define AD7280A_ALERT_RELAY_SIG_CHAIN_DOWN		(BIT(7) | BIT(6))

#define AD7280A_ALL_CELLS				(0xAD << 16)

#define AD7280A_MAX_SPI_CLK_HZ		700000 /* < 1MHz */
#define AD7280A_MAX_CHAIN		8
#define AD7280A_CELLS_PER_DEV		6
#define AD7280A_BITS			12
#define AD7280A_NUM_CH			(AD7280A_AUX_ADC_6 - \
					AD7280A_CELL_VOLTAGE_1 + 1)

#define AD7280A_DEVADDR_MASTER		0
#define AD7280A_DEVADDR_ALL		0x1F

// 5-bit device address is sent LSB first
#define AD7280A_DEVADDR(addr)	(((addr & 0x1) << 4) | ((addr & 0x2) << 3) | \
				(addr & 0x4) | ((addr & 0x8) >> 3) | \
				((addr & 0x10) >> 4))

/* During a read a valid write is mandatory.
 * So writing to the highest available address (Address 0x1F)
 * and setting the address all parts bit to 0 is recommended
 * So the TXVAL is AD7280A_DEVADDR_ALL + CRC
 */
#define AD7280A_READ_TXVAL	0xF800030A

/*
 * AD7280 CRC
 *
 * P(x) = x^8 + x^5 + x^3 + x^2 + x^1 + x^0 = 0b100101111 => 0x2F
 */
#define POLYNOM		0x2F
#define POLYNOM_ORDER	8
#define HIGHBIT		(1 << (POLYNOM_ORDER - 1))

struct           s_ad7280_state 
{
  unsigned char	 slave_num;       // Device count
  unsigned char	 ctrl_hb;         // 
  unsigned char	 ctrl_lb;
  unsigned char	 cell_threshhigh; // Cell voltage limits
  unsigned char	 cell_threshlow;
  unsigned char	 aux_threshhigh;  // Temperature limits
  unsigned char	 aux_threshlow;
  unsigned char  chan_cnt[CFGAD728AMODULES]; // Channel count
  unsigned char	 cb_mask[CFGAD728AMODULES];  // Cell balance masks
}                t_ad7280_state;

void Configure_AD7280A(void);

#endif /* IIO_ADC_AD7280_H_ */



