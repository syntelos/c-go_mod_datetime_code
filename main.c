/*
 * go mod datetime code
 * Copyright 2024 John Douglas Pritchard, Syntelos
 */
#include "gd.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
/*
 */
int main(int argc, char **argv){

  char *prog = argv[0];
  /*
   * Local filesystem directory containing '.git'.
   */
  char *path;
  if (1 < argc) {
    path = argv[1];

    struct stat sb;
    if (0 != stat(path,&sb) || S_IFDIR != (sb.st_mode & S_IFDIR)){

      fprintf(stderr,"Synopsis\n\n\t%s [dir]\n\nDescription\n\n\tPrint git repo version in the current\n\tworking directory, or the optional\n\targument directory.\n\n",prog);

      return 1;
    }
  } else {
    path = ".";
  }
  /*
   * Local filesystem repository.
   */
  gd_repo *state = calloc(1,sizeof(gd_repo));

  if (gd_repo_open(state,path)){

    if (gd_repo_version(state)){

      if (gd_repo_print(state)){

	gd_repo_close(state);

	return 0;
      }
      else {
	gd_repo_close(state);

	fprintf(stderr,"%s error: repository version print '%s'.\n",prog,path);

	return 1;
      }
    }
    else {
      gd_repo_close(state);

      fprintf(stderr,"%s error: repository version not found '%s'.\n",prog,path);

      return 1;
    }
  }
  else {
    gd_repo_close(state);

    fprintf(stderr,"%s error: repository not found '%s'.\n",prog,path);

    return 1;
  }
}
