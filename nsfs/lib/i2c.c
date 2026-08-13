#include "i2c.h"

static ret_code_t tw_start(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);

	while (!(TWCR & (1 << TWINT)))
		;

	if (TW_STATUS != TW_START && TW_STATUS != TW_REP_START)
		return TW_STATUS;

	return SUCCESS;
}

static void tw_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

static ret_code_t tw_write_sla(uint8_t sla) {
	TWDR = sla;
	TWCR = (1 << TWINT) | (1 << TWEN);

	while (!(TWCR & (1 << TWINT)))
		;
	if (TW_STATUS != TW_MT_SLA_ACK && TW_STATUS != TW_MR_SLA_ACK)
		return TW_STATUS;

	return SUCCESS;
}

static ret_code_t tw_write(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);

	while (!(TWCR & (1 << TWINT)))
		;
	if (TW_STATUS != TW_MT_DATA_ACK)
		return TW_STATUS;

	return SUCCESS;
}

static uint8_t tw_read(bool read_ack) {
	if (read_ack) {
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
		while (!(TWCR & (1 << TWINT)))
			;
		if (TW_STATUS != TW_MR_DATA_ACK)
			return TW_STATUS;
	} else {
		TWCR = (1 << TWINT) | (1 << TWEN);
		while (!(TWCR & (1 << TWINT)))
			;
		if (TW_STATUS != TW_MR_DATA_NACK)
			return TW_STATUS;
	}
	return TWDR;
}

void tw_init(twi_freq_mode_t twi_freq_mode, bool pullup_en) {
	DDRC |= (1 << TW_SDA_PIN) | (1 << TW_SCL_PIN);
	if (pullup_en) {
		PORTC |= (1 << TW_SDA_PIN) | (1 << TW_SCL_PIN);
	} else {
		PORTC &= ~((1 << TW_SDA_PIN) | (1 << TW_SCL_PIN));
	}
	DDRC &= ~((1 << TW_SDA_PIN) | (1 << TW_SCL_PIN));

	switch (twi_freq_mode) {
	case TW_FREQ_100K:
		TWBR = ((F_CPU / 100000UL) - 16) / 2;
		break;
	case TW_FREQ_250K:
		TWBR = ((F_CPU / 250000UL) - 16) / 2;
		break;
	case TW_FREQ_400K:
		TWBR = ((F_CPU / 400000UL) - 16) / 2;
		break;
	default:
		break;
	}
}

ret_code_t tw_master_transmit(uint8_t slave_addr, uint8_t *p_data, uint8_t len,
			      bool repeat_start) {
	ret_code_t error_code;

	error_code = tw_start();
	if (error_code != SUCCESS) {
		tw_stop();
		return error_code;
	}

	error_code = tw_write_sla(TW_SLA_W(slave_addr));
	if (error_code != SUCCESS) {
		tw_stop();
		return error_code;
	}

	for (int i = 0; i < len; ++i) {
		error_code = tw_write(p_data[i]);
		if (error_code != SUCCESS) {
			tw_stop();
			return error_code;
		}
	}

	if (!repeat_start)
		tw_stop();

	return SUCCESS;
}

ret_code_t tw_master_receive(uint8_t slave_addr, uint8_t *p_data, uint8_t len) {
	ret_code_t error_code;

	if (len == 0)
		return SUCCESS;

	error_code = tw_start();
	if (error_code != SUCCESS) {
		tw_stop();
		return error_code;
	}

	error_code = tw_write_sla(TW_SLA_R(slave_addr));
	if (error_code != SUCCESS) {
		tw_stop();
		return error_code;
	}

	for (int i = 0; i < len - 1; ++i)
		p_data[i] = tw_read(TW_READ_ACK);
	p_data[len - 1] = tw_read(TW_READ_NACK);

	tw_stop();

	return SUCCESS;
}
