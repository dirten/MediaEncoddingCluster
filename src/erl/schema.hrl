% this records save the data that was imported or encoded in the Media Hive
-record(file,{id, filename, path, size, containertype,streamcount,duration,bitrate,type,parent}).
% this Record saves the Data for watchfolders
-record(watchfolder,{id,infolder,outfolder,profile,filter}).

-record(profile,{id,name,ext,vformat,vcodec,vbitrate,vframerate,vwidth,vheight,achannels,acodec,abitrate,asamplerate}).
-record(stream,{id,fileid,streamidx,streamtype,codec,codecname,bitrate,rate,num, den, width, height,channels,gop,format}).

-record(job,{id, infile, outfile, begin_time, complete_time, last_ts}).
-record(jobdetail,{id, jobid,instream, outstream}).

-record(process_unit,{id, sourcestream, targetstream, startts, framecount, sendtime, sendnode, completetime, sendsize, receivesize, data}).

-record(framegroup,{id,framegroup,startts,byte_pos,stream_id,stream_index,frame_count}).
-record(packet,{id,pts, dts,data, data_size}).