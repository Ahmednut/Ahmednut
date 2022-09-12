#ifndef RTDEX_FIFO_H
#define RTDEX_FIFO_H

#include <linux/ioctl.h>
#define RTDEX_IOC_MAGIC		'N'
#define RTDEX_RESET_ALL		_IO(RTDEX_IOC_MAGIC, 1)
#define RTDEX_SET_BURST_LENGTH	_IO(RTDEX_IOC_MAGIC, 2)
#define RTDEX_SET_BLOCK_SIZE	_IO(RTDEX_IOC_MAGIC, 4)
#define RTDEX_SET_READ_CAST_MODE	_IO(RTDEX_IOC_MAGIC, 5)
#define RTDEX_SET_WRITE_CAST_MODE	_IO(RTDEX_IOC_MAGIC, 6)

#define RTDEX_CAST_MODE_NONE		0x0
#define RTDEX_CAST_MODE_FLOAT2INT	0x1
#define RTDEX_CAST_MODE_CMPLX2SHORT	0x2

#endif


