/* Mapped-memory Writer program
 * Version: February 7, 2017
 */
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "mmap.h"

/* return a uniformly generated random number in the range [low,high]  
 */
int random_range (unsigned const low, unsigned const high) {
	unsigned const range = high - low + 1;
	return low + (int) (((double) range) * rand () / (RAND_MAX + 1.0));
}

/* Create and write to a shared file for communication with another process.
 *
 * argv[1] = file name
 *
 * Note: Error processing is omitted
 */
int main (int argc, char* const argv[]) {
	int fd;
	int nNumbers;
	void* file_memory;

	/* seed the random number generator */
	srand (time (NULL));

	/* open or create a file to hold an unsigned integer  */
	fd = open (argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

	/* write FILESIZE spaces */
	for (int i=0; i<FILESIZE; i++) write (fd, " ", 1);
	write (fd, "", 1); /* write a NULL at EOF */

	/* create the memory-mapping
     1st param=start addr of mapping into memory, NULL means chosen by OS
     2nd param=length of map (bytes)
     3rd param=protection
     4th param=options, MAP_SHARED used for interprocess communications
     5th param=file descriptor of mapped file
     6th param=offset from start of file where mapping starts
	 */
	file_memory = mmap (NULL, FILESIZE, PROT_WRITE, MAP_SHARED, fd, 0);
	close (fd);

	nNumbers = atoi(argv[2]);
	/* write nNumbers random integer to memory-mapped area  */
	for(int i = 0 ; i < nNumbers ; i++){
		int jumps = 2;
		int randomlyGenerated = random_range (-100, 100);
		//printf("This time the generated value is %d.\n",randomlyGenerated);
		sprintf((char*) file_memory, "%d ",randomlyGenerated);
		if (abs(randomlyGenerated) / 100 >= 1)
			jumps++;
		if (abs(randomlyGenerated) / 10 >= 1)
			jumps++;
		if (randomlyGenerated <= 0)
			jumps++;
		for (int j = 0 ; j< jumps ; j++){
			//(char*)
			file_memory++;
		}
	}

	/* release the memory  */
	munmap (file_memory, FILESIZE);
}
