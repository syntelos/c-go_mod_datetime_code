/*
 * go mod datetime code
 * Copyright 2024 John Douglas Pritchard, Syntelos
 */
#include "gd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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
    if (0 == git_revwalk_new(&(walk),gd->repo) &&
	0 == git_revwalk_sorting(walk,GIT_SORT_NONE) &&
	0 == git_revwalk_push_head(walk))
    {
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
char* gd_code_string(gd_repo* gd){
  if (null != gd && null != gd->commit){
    const git_signature *sig = git_commit_author(gd->commit);
    if (null != sig){
      size_t dat_z = (14+1);
      char *dat = calloc(1,dat_z);
      if (dat == gd_sig_time(&(sig->when),dat,dat_z)){
      
	size_t oid_z = (GIT_OID_MAX_HEXSIZE+1);
	char *oid = calloc(1,oid_z);
	if (oid == git_oid_tostr(oid,oid_z,&(gd->oid))){

	  oid[12] = 0;

	  size_t buz = 0x40;
	  char *buf = calloc(1,buz);
	  if (null != buf){

	    if (null != gd->tag){

	      const char *tag = git_tag_name(gd->tag);

	      snprintf(buf,buz,"%s-%14s-%12s",tag,dat,oid);
	    }
	    else {

	      snprintf(buf,buz,"v0.0.0-%14s-%12s",dat,oid);
	    }
	  }
	  
	  free(dat);
	  free(oid);

	  return buf;
	}
      }
    }
  }
  return null;
}
/*
 */
bool_t gd_code_print(gd_repo* gd){
  char *code = gd_code_string(gd);
  if (null != code){

    fprintf(stdout,"%s\n",code);

    free(code);

    return true;
  }
  else {
    return false;
  }
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

  struct tm *utc = gmtime(&(time->time));

  int yyyy = (utc->tm_year+1900);
  int mo = (utc->tm_mon+1);
  int dd = utc->tm_mday;
  int hh = utc->tm_hour;
  int mi = utc->tm_min;
  int ss = utc->tm_sec;

  snprintf(string, size, "%4d%02d%02d%02d%02d%02d",yyyy,mo,dd,hh,mi,ss);

  return string;
}
