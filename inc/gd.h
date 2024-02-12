/*
 * go mod datetime code
 * Copyright 2024 John Douglas Pritchard, Syntelos
 */
#ifndef _gd_h_
#define _gd_h_

#include "syntelos.h"
#include <git2.h>
/*
 * Collected most recent information from repository.
 */
typedef struct {
  git_repository *repo;
  git_commit *commit;
  git_oid oid;
  git_tag *tag;

} gd_repo;
/*
 * Open git repository found at path.
 */
bool_t gd_repo_open(gd_repo*,char*);
/*
 * Derive commit, oid, and tag from repository.  
 */
bool_t gd_repo_version(gd_repo*);
/*
 * Derive code string from commit, oid, and tag.  Returns a
 * dynamically allocated bufffer.
 */
char* gd_code_string(gd_repo*);
/*
 * Print code string to stdout.
 */
bool_t gd_code_print(gd_repo*);
/*
 * GitLib shutdown and malloc cleanup.
 */
void gd_repo_close(gd_repo*);

#endif
