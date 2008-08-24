-record(packet, {id,stream_id,pts,dts, stream_index, key_frame, frame_group,flags, duration, pos, data}).
-record(units, {id,source_stream,target_stream, start_ts, frame_count, send, complete, priority}).
-record(profiles, {id,name,v_format, v_codec, v_bitrate, v_framerate, v_width, v_height, a_channels, a_codec, a_bitrate, a_samplerate}).
-record(tpacket, {id,data}).

