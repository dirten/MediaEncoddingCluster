% this records save the data that was imported or encoded in the Media Hive
% this record is obsolete, use media instead
-record(file,{id, filename, path, size, containertype,streamcount,duration,bitrate,type,parent, start_time, status=ok}).

% new Record for saving MediaFile Information
-record(media,{id, filename, path, size, containertype,streamcount,duration,bitrate,type,parent, start_time, status}).

% this Record saves the Data for watchfolders
-include("schema_watchfolder.hrl").
%-record(watchfolder,{id,infolder,outfolder,profile,filter}).

%-record(profile,{id,name,ext,vformat,vcodec,vbitrate,vframerate,vwidth,vheight,achannels,acodec,abitrate,asamplerate}).
-include("schema_profile.hrl").
-record(stream,{id,fileid,streamidx,streamtype,codec,codecname,bitrate,rate,num, den, width, height,channels,gop,format, start_time, duration, flags=0}).

%-record(job,{id, infile, outfile, begin_time, complete_time, last_ts}).
-include("schema_job.hrl").
-record(jobdetail,{id, jobid,instream, outstream}).

-record(process_unit,{id, sourcestream, targetstream, startts, framecount, sendtime, sendnode, completetime, sendsize, receivesize, data, log}).

-record(framegroup,{id,framegroup,startts,byte_pos,stream_id,stream_index,frame_count}).

-record(timestamp,{num,den}).

-record(packet,{id,stream_id, is_key,dts ,pts, flags, duration, size, timebase_num, timebase_den, data_size, data}).

-record(scheduler,{id, name, module, func, args, interval, desc, active=true}).

-record(releases,{version,application,desc,status}).

-record(logging,{id, time, level, msg}).

-include("schema_config.hrl").
%-record(streams, {index,packetcount,packets=[]}).
%-record(packetgroup,{id, packets}).