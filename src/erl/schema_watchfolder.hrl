% this Record saves the Data for watchfolders
-record(
  watchfolder,
  {
    id,
    infolder,
    outfolder,
    profile,
    filter,
    recursive=false,
    status=active
  }
).

