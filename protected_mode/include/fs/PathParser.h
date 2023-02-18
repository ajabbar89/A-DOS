#ifndef __PATHPARSER_H__
#define __PATHPARSER_H__

typedef struct path{
	char *part;
	struct path *next;
}path_t;

typedef struct{
	int driveNumber;
	path_t *first;
}pathRoot_t;

pathRoot_t *pathParse(const char *);
void pathParseFree(pathRoot_t *);

#endif