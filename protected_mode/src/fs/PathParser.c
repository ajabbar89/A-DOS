#include <fs/PathParser.h>
#include <memory/memory.h>
#include <config/config.h>
#include <kernel/kernel.h>


bool isValidPath(const char *path) {
	int len = strnlen(path,MAX_FILENAME_LENGTH);
	return (len >=3 && isDigit(path[0]) && (memcmp((void *)&path[1],":/",2) == 0) && len < MAX_FILENAME_LENGTH);
}


int getDriveNumber(const char *path) {
	return charToDigit(path[0]);
}

path_t *getPartPath(const char **path) {
	path_t *partPath = NULL;
	if(**path != '\0') {
		int i = 0;
		partPath = (path_t *)kmalloc(sizeof(path_t));
		partPath->part = (char *)kmalloc(sizeof(MAX_FILENAME_LENGTH));
		while(**path != '\0' && **path != '/') {
			*(partPath->part) = **path;
			partPath->part++;
			*path += 1;
			i++;
		}
		if(**path == '/')
			*path += 1;
		partPath->part -= i;
		partPath->next = NULL;
	}
	return partPath;
}

pathRoot_t *pathParse(const char *path) {
	pathRoot_t *root = NULL;
	if(!isValidPath(path)) {
		return root;
	}
	root = (pathRoot_t *)kmalloc(sizeof(pathRoot_t));
	root->driveNumber = getDriveNumber(path);
	root->first = NULL;

	path +=3;
	root->first = getPartPath(&path);

	path_t *temp,*last;
	root->first->next = NULL;
	last = root->first;
	bool parsingDone = false;
	
	while(!parsingDone) {
		temp = getPartPath(&path);
		if(temp == NULL) {
			parsingDone = true;
			break;
		}
		last->next = temp;
		last = last->next;
	}

	return root;
}


void pathParseFree(pathRoot_t *root) {
	path_t *temp = root->first;
	while(temp != NULL) {
		path_t *tmp = temp;
		temp = temp->next;
		kfree(tmp); 
	}
	kfree(root);
}