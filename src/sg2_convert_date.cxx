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

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <sg2.hxx>

int main(int argc, char ** argv)
{
	if (argc < 2) {
		printf("invalid arguments\n");
		return 1;
	}

	sg2::julian jd;
	sg2::date d;
	sg2::ymdh ymdh;
	sg2::ymdhmsn ymdhmsn;
	sg2::ydoyh ydoyh;

	if (strcmp("--jd", argv[1]) == 0) {
		if (argc != 3) {
			printf("invalid arguments\n");
			return 1;
		}

		jd = sg2::julian{atof(argv[2])};
		d = sg2::date{jd.value};
		ymdh = sg2::ymdh{jd.value};
		ymdhmsn = sg2::ymdhmsn{d};
		ydoyh = sg2::ydoyh{ymdh};
	}

	if (strcmp("--nsec", argv[1]) == 0) {
		if (argc != 3) {
			printf("invalid arguments\n");
			return 1;
		}

		d = sg2::date{static_cast<int64_t>(atoll(argv[2]))};
		jd = sg2::julian{d};
		ymdh = sg2::ymdh{jd.value};
		ymdhmsn = sg2::ymdhmsn{d};
		ydoyh = sg2::ydoyh{ymdh};
	}

	if (strcmp("--ymdh", argv[1]) == 0) {
		if (argc != 6) {
			printf("invalid arguments\n");
			return 1;
		}

		ymdh = sg2::ymdh(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atof(argv[5]));
		d = sg2::date{ymdh};
		jd = sg2::julian{ymdh};
		ymdhmsn = sg2::ymdhmsn{d};
		ydoyh = sg2::ydoyh{ymdh};
	}

	printf("jd ............ %f\n", jd.value);
	printf("msec .......... %ld\n", d.msec);
	printf("ymdh .......... %04d-%02d-%02d %f\n", ymdh.year, ymdh.month, ymdh.day_of_month, ymdh.hour);
	printf("ymdhmsn ....... %04d-%02d-%02dT%02d:%02d:%02d.%09d\n", ymdhmsn.year, ymdhmsn.month, ymdhmsn.day_of_month, ymdhmsn.hour, ymdhmsn.min, ymdhmsn.sec, ymdhmsn.msec);
	printf("ymdh .......... %04d-%03d %f\n", ydoyh.year, ydoyh.day_of_year, ydoyh.hour);

	return 0;
}


