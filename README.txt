go mod datetime code

  Dumps GO MOD version substring from GIT REPO in current
  working directory, or argument directory.

  As in alternative to

    git log -n 1 --abbrev=12 --abbrev-commit --date=format:%Y%m%d%H%M%S --format=format:%cd-%h'

  which fails the requirement for timezone in UTC.


Requires

  libgit2 [GIT]

  Run "apt-get install libgit2-dev".


Build

  Run "make".
 
  Perhaps, "make install" to "/usr/local/bin/gd".


References

  [GIT] https://libgit2.org/

