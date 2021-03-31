
.section .rodata
.global _sg2_geocentric_data
.type _sg2_geocentric_data, STT_OBJECT
.align 8

_sg2_geocentric_data:
.incbin "../resources/geocentric.data"

.global _sg2_geocentric_data_sz
.type _sg2_geocentric_data_sz, STT_OBJECT
.align 8
_sg2_geocentric_data_sz:
.int _sg2_geocentric_data_sz - _sg2_geocentric_data

