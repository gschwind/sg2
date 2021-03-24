/*
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

#include "sg2_data_handler.hxx"

#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

namespace sg2 {

data_handler::data_handler()
{
	int fd = open("resources/geocentric.data", O_RDONLY);
	if (fd < 0)
		throw std::runtime_error("cannot open file resources/geocentric.data");
	struct stat st;
	fstat(fd, &st);
	data = mmap(0, ((st.st_size-1)/sysconf(_SC_PAGE_SIZE)+1)*sysconf(_SC_PAGE_SIZE), PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
}

data_handler _geocentric_data;

} // namespace sg2

