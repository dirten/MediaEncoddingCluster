% this records save the data that was imported or encoded in the Media Hive
-record(file,{id, filename, path, size, containertype,streamcount,duration,bitrate,type,parent}).
% this Record saves the Data for watchfolders
-record(watchfolder,{id,infolder,outfolder,profile,filter}).

-record(profile,{id,name,ext,vformat,vcodec,vbitrate,vframerate,vwidth,vheight,achannels,acodec,abitrate,asamplerate}).
-record(stream,{id,fileid,streamidx,streamtype,codec,codecname,rate,num, den, width, height,channels,gop,format}).

-record(job,{id, infile, outfile, begin_time, complete_time}).
-record(jobdetail,{id, jobid,instream, outstream}).

-record(framegroup,{id,framegroup,startts,byte_pos,stream_id,stream_index,frame_count}).
-record(packet,{id,pts,data, data_size}).
