#ifndef _VITAIDENT_UTILS_H_
#define _VITAIDENT_UTILS_H_


int printStr(bool printStar, char message[250], char *info, ...);
void getSizeString(char *string, uint64_t size);
const char *concat(char* s1, char* s2);

#endif