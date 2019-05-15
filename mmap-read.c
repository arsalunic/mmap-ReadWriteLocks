/* Mapped-memory Reader program
 * Version: February 7, 2017
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mmap.h"

int main (int argc, char* const argv[]) {
	int fd;
	void* file_memory;
	int integer;
	int nNumbers;

	/* open the file  */
	fd = open (argv[1], O_RDWR, S_IRUSR | S_IWUSR);

	/* create the memory-mapping */
	file_memory = mmap (NULL, FILESIZE, PROT_READ | PROT_WRITE,
			MAP_SHARED, fd, 0);
	close (fd);

	/* read and print the integer */

	nNumbers = atoi(argv[2]);
	printf ("file contains: ");

	for(int i = 0 ; i < nNumbers ; i++){
		int jumps = 2;
		sscanf (file_memory, "%d", &integer);
		printf ("%d ", integer);

		if (abs(integer) / 100 >= 1)
			jumps++;
		if (abs(integer) / 10 >= 1)
			jumps++;
		if (integer <= 0)
			jumps++;

		for (int j = 0 ; j< jumps ; j++){
			//(char*)
			file_memory++;
		}
	}

	printf("\n");


	/* release the memory */
	munmap (file_memory, FILESIZE);

	return 0;
}
