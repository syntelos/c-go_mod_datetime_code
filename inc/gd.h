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
 * Derive tag and commit from repository.  
 */
bool_t gd_repo_open(gd_repo*,char*);
/*
 * Derive tag and commit from repository.  
 */
bool_t gd_repo_version(gd_repo*);
/*
 * Print GOPL GIT VERSION to stdout.
 */
bool_t gd_repo_print(gd_repo*);
/*
 * GitLib shutdown and malloc cleanup.
 */
void gd_repo_close(gd_repo*);
/*
 * Derive datetime code from GIT TIME.
 */
char* gd_sig_time(const git_time*,char*,size_t);

#endif
