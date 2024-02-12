/*
 * go mod datetime code
 * Copyright 2024 John Douglas Pritchard, Syntelos
 */
#include "gd.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
/*
 */
bool_t gd_repo_open(gd_repo* gd, char *path){
  git_libgit2_init();

  return (0 == git_repository_open(&(gd->repo),path));
}
/*
 */
bool_t gd_repo_version(gd_repo* gd){
  git_strarray tag_names = {0};
  if (0 == git_tag_list(&tag_names,gd->repo)){

    bool_t tag = (0 < tag_names.count);
    git_strarray_free(&tag_names);

    git_revwalk *walk = null;
    if (0 == git_revwalk_new(&(walk),gd->repo)){
      /*
       * Walk commits with or without tags.
       */
      for (; 0 == git_revwalk_next(&(gd->oid),walk); git_commit_free(gd->commit)){

	if (0 == git_commit_lookup(&(gd->commit),gd->repo,&(gd->oid))){

	  if (tag){

	    if (0 == git_tag_lookup(&(gd->tag),gd->repo,&(gd->oid))){
	      /*
	       * Found a tag.
	       */
	      git_revwalk_free(walk);

	      return true;
	    }
	  } else {
	    /*
	     * Not looking for a tag.
	     */
	    git_revwalk_free(walk);

	    return true;
	  }
	} else {
	  /*
	   * Unable to retrieve commit.
	   */
	  break;
	}
      }
      /*
       * Loop tail case.
       */
      git_revwalk_free(walk);
    }
  }
  return false;
}
/*
 */
bool_t gd_repo_print(gd_repo* gd){
  if (null != gd && null != gd->commit){
    const git_signature *sig = git_commit_author(gd->commit);
    if (null != sig){
      size_t dat_z = (14+1);
      char *dat = calloc(1,dat_z);
      if (dat == gd_sig_time(&(sig->when),dat,dat_z)){
      
	size_t oid_z = (GIT_OID_MAX_HEXSIZE+1);
	char *oid = calloc(1,oid_z);
	if (oid == git_oid_tostr(oid,oid_z,&(gd->oid))){

	  if (null != gd->tag){
	    const char *tag = git_tag_name(gd->tag);

	    fprintf(stdout,"%s-%14s-%12s\n",tag,dat,oid);

	    return true;
	  }
	  else {

	    fprintf(stdout,"v0.0.0-%14s-%12s\n",dat,oid);

	    return true;
	  }
	}
      }
    }
  }
  return false;
}
/*
 */
void gd_repo_close(gd_repo* gd){

  if (null != gd){

    if (null != gd->repo){

      git_repository_free(gd->repo);

      if (null != gd->tag){

	git_tag_free(gd->tag);
      }

      if (null != gd->commit){

	git_commit_free(gd->commit);
      }
    }
    memset(gd,0,sizeof(gd_repo));

    free(gd);
  }
  git_libgit2_shutdown();
}
/*
 */
char* gd_sig_time(const git_time *time, char *string, size_t size){

  time_t t = (time_t)time->time + (time->offset * 60);

  struct tm *intm = gmtime(&t);

  strftime(string, size, "%Y%m%d%H%M%S", intm);

  return string;
}
