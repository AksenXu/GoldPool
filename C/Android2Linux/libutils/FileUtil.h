#ifndef __FILE_UTILS_H_
#define __FILE_UTILS_H_

/* reads a file, making sure it is terminated with \n \0 */
void *read_file(const char *fn, unsigned *_sz);

#endif //__FILE_UTILS_H_