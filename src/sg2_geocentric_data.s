/*
 * Copyright 2021 MINES ParisTech
 * Copyright 2021 Benoit Gschwind <benoit.gschwind@mines-paristech.fr>
 *
 * This file is part of libsg2.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the Lesser GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser GNU General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

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

/* this object does not need executable stack */
.section        .note.GNU-stack,"",@note

