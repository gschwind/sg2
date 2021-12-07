data.bin is designed to be mmapped or included in binary form in the library,
the file is binary file using little endian and formated as follow:

- first 8 bytes is signed time offset of following data in second since
  1970-01-01 00:00:00
- second 8 bytes is signed time delta of following data in second
- third 8 bytes is signed count of following data, see next item.
- following data is an array of IEEE double of shape [count][3] with columns:
	* sin(L)
	* cos(L)
	* R

