// Created by cgo - DO NOT EDIT

package gmf

import "unsafe"

import "os"

import _ "runtime/cgo"

type _ unsafe.Pointer

func _Cerrno(dst *os.Error, x int) { *dst = os.Errno(x) }
type _Ctypedef_AVPacket _Ctype_struct_AVPacket
type _Ctype_struct___3 struct {
	class_name			*_Ctype_char
	item_name			*[0]byte
	option				*[0]byte
	version				_Ctype_int
	log_level_offset_offset		_Ctype_int
	parent_log_context_offset	_Ctype_int
}
type _Ctype_uint uint32
type _Ctypedef_AVCodecContext _Ctype_struct_AVCodecContext
type _Ctypedef_int16_t _Ctype_short
type _Ctype_struct_AVCodec struct {
	name			*_Ctype_char
	_type			int32
	id			uint32
	priv_data_size		_Ctype_int
	init			*[0]byte
	encode			*[0]byte
	close			*[0]byte
	decode			*[0]byte
	capabilities		_Ctype_int
	next			*_Ctype_struct_AVCodec
	flush			*[0]byte
	supported_framerates	*_Ctypedef_AVRational
	pix_fmts		*int32
	long_name		*_Ctype_char
	supported_samplerates	*_Ctype_int
	sample_fmts		*int32
	channel_layouts		*_Ctypedef_int64_t
	max_lowres		_Ctypedef_uint8_t
	_			[3]byte
	priv_class		*_Ctypedef_AVClass
}
type _Ctype_unsignedchar uint8
type _Ctype_struct_AVPanScan struct {
	id		_Ctype_int
	width		_Ctype_int
	height		_Ctype_int
	position	[3][2]_Ctypedef_int16_t
}
type _Ctype_struct_AVInputFormat struct {
	name		*_Ctype_char
	long_name	*_Ctype_char
	priv_data_size	_Ctype_int
	read_probe	*[0]byte
	read_header	*[0]byte
	read_packet	*[0]byte
	read_close	*[0]byte
	read_seek	*[0]byte
	read_timestamp	*[0]byte
	flags		_Ctype_int
	extensions	*_Ctype_char
	value		_Ctype_int
	read_play	*[0]byte
	read_pause	*[0]byte
	codec_tag	**[0]byte
	read_seek2	*[0]byte
	metadata_conv	*[0]byte
	next		*_Ctype_struct_AVInputFormat
}
type _Ctypedef_int8_t _Ctype_schar
type _Ctypedef_AVRational _Ctype_struct_AVRational
type _Ctypedef_AVClass _Ctype_struct___0
type _Ctype_struct_AVCodecParserContext struct {
	priv_data		unsafe.Pointer
	parser			*_Ctype_struct_AVCodecParser
	frame_offset		_Ctypedef_int64_t
	cur_offset		_Ctypedef_int64_t
	next_frame_offset	_Ctypedef_int64_t
	pict_type		_Ctype_int
	repeat_pict		_Ctype_int
	pts			_Ctypedef_int64_t
	dts			_Ctypedef_int64_t
	last_pts		_Ctypedef_int64_t
	last_dts		_Ctypedef_int64_t
	fetch_timestamp		_Ctype_int
	cur_frame_start_index	_Ctype_int
	cur_frame_offset	[4]_Ctypedef_int64_t
	cur_frame_pts		[4]_Ctypedef_int64_t
	cur_frame_dts		[4]_Ctypedef_int64_t
	flags			_Ctype_int
	offset			_Ctypedef_int64_t
	cur_frame_end		[4]_Ctypedef_int64_t
	key_frame		_Ctype_int
	convergence_duration	_Ctypedef_int64_t
	dts_sync_point		_Ctype_int
	dts_ref_dts_delta	_Ctype_int
	pts_dts_delta		_Ctype_int
	cur_frame_pos		[4]_Ctypedef_int64_t
	pos			_Ctypedef_int64_t
	last_pos		_Ctypedef_int64_t
}
type _Ctype_char int8
type _Ctype_struct_AVOutputFormat struct {
	name			*_Ctype_char
	long_name		*_Ctype_char
	mime_type		*_Ctype_char
	extensions		*_Ctype_char
	priv_data_size		_Ctype_int
	audio_codec		uint32
	video_codec		uint32
	write_header		*[0]byte
	write_packet		*[0]byte
	write_trailer		*[0]byte
	flags			_Ctype_int
	set_parameters		*[0]byte
	interleave_packet	*[0]byte
	codec_tag		**[0]byte
	subtitle_codec		uint32
	metadata_conv		*[0]byte
	next			*_Ctype_struct_AVOutputFormat
}
type _Ctypedef_AVProbeData _Ctype_struct_AVProbeData
type _Ctype_struct_AVFormatContext struct {
	av_class				*_Ctypedef_AVClass
	iformat					*_Ctype_struct_AVInputFormat
	oformat					*_Ctype_struct_AVOutputFormat
	priv_data				unsafe.Pointer
	pb					*_Ctypedef_ByteIOContext
	nb_streams				_Ctype_uint
	streams					[20]*_Ctypedef_AVStream
	filename				[1024]_Ctype_char
	timestamp				_Ctypedef_int64_t
	title					[512]_Ctype_char
	author					[512]_Ctype_char
	copyright				[512]_Ctype_char
	comment					[512]_Ctype_char
	album					[512]_Ctype_char
	year					_Ctype_int
	track					_Ctype_int
	genre					[32]_Ctype_char
	ctx_flags				_Ctype_int
	packet_buffer				*_Ctype_struct_AVPacketList
	start_time				_Ctypedef_int64_t
	duration				_Ctypedef_int64_t
	file_size				_Ctypedef_int64_t
	bit_rate				_Ctype_int
	cur_st					*_Ctypedef_AVStream
	cur_ptr_deprecated			*_Ctypedef_uint8_t
	cur_len_deprecated			_Ctype_int
	cur_pkt_deprecated			_Ctypedef_AVPacket
	data_offset				_Ctypedef_int64_t
	index_built				_Ctype_int
	mux_rate				_Ctype_int
	packet_size				_Ctype_uint
	preload					_Ctype_int
	max_delay				_Ctype_int
	loop_output				_Ctype_int
	flags					_Ctype_int
	loop_input				_Ctype_int
	probesize				_Ctype_uint
	max_analyze_duration			_Ctype_int
	key					*_Ctypedef_uint8_t
	keylen					_Ctype_int
	nb_programs				_Ctype_uint
	programs				**_Ctypedef_AVProgram
	video_codec_id				uint32
	audio_codec_id				uint32
	subtitle_codec_id			uint32
	max_index_size				_Ctype_uint
	max_picture_buffer			_Ctype_uint
	nb_chapters				_Ctype_uint
	chapters				**_Ctypedef_AVChapter
	debug					_Ctype_int
	raw_packet_buffer			*_Ctype_struct_AVPacketList
	raw_packet_buffer_end			*_Ctype_struct_AVPacketList
	packet_buffer_end			*_Ctype_struct_AVPacketList
	metadata				*[0]byte
	raw_packet_buffer_remaining_size	_Ctype_int
	start_time_realtime			_Ctypedef_int64_t
}
type _Ctype_struct_AVPicture struct {
	data		[4]*_Ctypedef_uint8_t
	linesize	[4]_Ctype_int
}
type _Ctypedef_AVFifoBuffer _Ctype_struct_AVFifoBuffer
type _Ctype_struct___2 struct {
	last_dts		_Ctypedef_int64_t
	duration_gcd		_Ctypedef_int64_t
	duration_count		_Ctype_int
	duration_error		[725]_Ctype_double
	codec_info_duration	_Ctypedef_int64_t
}
type _Ctype_struct_AVStream struct {
	index				_Ctype_int
	id				_Ctype_int
	codec				*_Ctypedef_AVCodecContext
	r_frame_rate			_Ctypedef_AVRational
	priv_data			unsafe.Pointer
	first_dts			_Ctypedef_int64_t
	pts				_Ctype_struct_AVFrac
	time_base			_Ctypedef_AVRational
	pts_wrap_bits			_Ctype_int
	stream_copy			_Ctype_int
	discard				int32
	quality				_Ctype_float
	start_time			_Ctypedef_int64_t
	duration			_Ctypedef_int64_t
	language			[4]_Ctype_char
	need_parsing			uint32
	parser				*_Ctype_struct_AVCodecParserContext
	cur_dts				_Ctypedef_int64_t
	last_IP_duration		_Ctype_int
	last_IP_pts			_Ctypedef_int64_t
	index_entries			*_Ctypedef_AVIndexEntry
	nb_index_entries		_Ctype_int
	index_entries_allocated_size	_Ctype_uint
	nb_frames			_Ctypedef_int64_t
	unused				[5]_Ctypedef_int64_t
	filename			*_Ctype_char
	disposition			_Ctype_int
	probe_data			_Ctypedef_AVProbeData
	pts_buffer			[17]_Ctypedef_int64_t
	sample_aspect_ratio		_Ctypedef_AVRational
	metadata			*[0]byte
	cur_ptr				*_Ctypedef_uint8_t
	cur_len				_Ctype_int
	cur_pkt				_Ctypedef_AVPacket
	reference_dts			_Ctypedef_int64_t
	probe_packets			_Ctype_int
	last_in_packet_buffer		*_Ctype_struct_AVPacketList
	avg_frame_rate			_Ctypedef_AVRational
	codec_info_nb_frames		_Ctype_int
	info				*_Ctype_struct___2
}
type _Ctype_struct_AVFormatParameters struct {
	time_base	_Ctypedef_AVRational
	sample_rate	_Ctype_int
	channels	_Ctype_int
	width		_Ctype_int
	height		_Ctype_int
	pix_fmt		int32
	channel		_Ctype_int
	standard	*_Ctype_char
	_		[4]byte
	video_codec_id	uint32
	audio_codec_id	uint32
}
type _Ctypedef_AVOption _Ctype_struct_AVOption
type _Ctype_struct_AVChapter struct {
	id		_Ctype_int
	time_base	_Ctypedef_AVRational
	start		_Ctypedef_int64_t
	end		_Ctypedef_int64_t
	title		*_Ctype_char
	metadata	*[0]byte
}
type _Ctype_ushort uint16
type _Ctype_struct_AVIndexEntry struct {
	pos		_Ctypedef_int64_t
	timestamp	_Ctypedef_int64_t
	_		[4]byte
	min_distance	_Ctype_int
}
type _Ctype_struct_AVPacketList struct {
	pkt	_Ctypedef_AVPacket
	next	*_Ctype_struct_AVPacketList
}
type _Ctype_ulonglong uint64
type _Ctype_longlong int64
type _Ctypedef_uint8_t _Ctype_unsignedchar
type _Ctype_struct___5 struct {
	coeff	*_Ctype_double
	length	_Ctype_int
}
type _Ctype_struct_AVProbeData struct {
	filename	*_Ctype_char
	buf		*_Ctype_unsignedchar
	buf_size	_Ctype_int
}
type _Ctypedef_AVOutputFormat _Ctype_struct_AVOutputFormat
type _Ctype_struct_AVFrac struct {
	val	_Ctypedef_int64_t
	num	_Ctypedef_int64_t
	den	_Ctypedef_int64_t
}
type _Ctype_struct_RcOverride struct {
	start_frame	_Ctype_int
	end_frame	_Ctype_int
	qscale		_Ctype_int
	quality_factor	_Ctype_float
}
type _Ctypedef_SwsFilter _Ctype_struct___4
type _Ctype_struct_AVRational struct {
	num	_Ctype_int
	den	_Ctype_int
}
type _Ctype_struct___1 struct {
	buffer		*_Ctype_unsignedchar
	buffer_size	_Ctype_int
	buf_ptr		*_Ctype_unsignedchar
	buf_end		*_Ctype_unsignedchar
	opaque		unsafe.Pointer
	read_packet	*[0]byte
	write_packet	*[0]byte
	seek		*[0]byte
	pos		_Ctypedef_int64_t
	must_flush	_Ctype_int
	eof_reached	_Ctype_int
	write_flag	_Ctype_int
	is_streamed	_Ctype_int
	max_packet_size	_Ctype_int
	checksum	_Ctype_ulong
	checksum_ptr	*_Ctype_unsignedchar
	update_checksum	*[0]byte
	error		_Ctype_int
	read_pause	*[0]byte
	read_seek	*[0]byte
}
type _Ctype_struct_AVHWAccel struct {
	name		*_Ctype_char
	_type		int32
	id		uint32
	pix_fmt		int32
	capabilities	_Ctype_int
	next		*_Ctype_struct_AVHWAccel
	start_frame	*[0]byte
	decode_slice	*[0]byte
	end_frame	*[0]byte
	priv_data_size	_Ctype_int
}
type _Ctypedef_AVStream _Ctype_struct_AVStream
type _Ctypedef_AVPicture _Ctype_struct_AVPicture
type _Ctype_struct_AVProgram struct {
	id			_Ctype_int
	provider_name		*_Ctype_char
	name			*_Ctype_char
	flags			_Ctype_int
	discard			int32
	stream_index		*_Ctype_uint
	nb_stream_indexes	_Ctype_uint
	metadata		*[0]byte
}
type _Ctype_struct_AVPaletteControl struct {
	palette_changed	_Ctype_int
	palette		[256]_Ctype_uint
}
type _Ctypedef_uint16_t _Ctype_ushort
type _Ctypedef_SwsVector _Ctype_struct___5
type _Ctypedef_AVCodec _Ctype_struct_AVCodec
type _Ctype_struct_AVPacket struct {
	pts			_Ctypedef_int64_t
	dts			_Ctypedef_int64_t
	data			*_Ctypedef_uint8_t
	size			_Ctype_int
	stream_index		_Ctype_int
	flags			_Ctype_int
	duration		_Ctype_int
	destruct		*[0]byte
	priv			unsafe.Pointer
	pos			_Ctypedef_int64_t
	convergence_duration	_Ctypedef_int64_t
}
type _Ctypedef_AVFrame _Ctype_struct_AVFrame
type _Ctypedef_AVInputFormat _Ctype_struct_AVInputFormat
type _Ctypedef_AVPanScan _Ctype_struct_AVPanScan
type _Ctypedef_int64_t _Ctype_longlong
type _Ctypedef_AVIndexEntry _Ctype_struct_AVIndexEntry
type _Ctype_struct_AVFrame struct {
	data			[4]*_Ctypedef_uint8_t
	linesize		[4]_Ctype_int
	base			[4]*_Ctypedef_uint8_t
	key_frame		_Ctype_int
	pict_type		_Ctype_int
	pts			_Ctypedef_int64_t
	coded_picture_number	_Ctype_int
	display_picture_number	_Ctype_int
	quality			_Ctype_int
	age			_Ctype_int
	reference		_Ctype_int
	qscale_table		*_Ctypedef_int8_t
	qstride			_Ctype_int
	mbskip_table		*_Ctypedef_uint8_t
	motion_val		[2]*[2]_Ctypedef_int16_t
	mb_type			*_Ctypedef_uint32_t
	motion_subsample_log2	_Ctypedef_uint8_t
	_			[3]byte
	opaque			unsafe.Pointer
	error			[4]_Ctypedef_uint64_t
	_type			_Ctype_int
	repeat_pict		_Ctype_int
	qscale_type		_Ctype_int
	interlaced_frame	_Ctype_int
	top_field_first		_Ctype_int
	pan_scan		*_Ctypedef_AVPanScan
	palette_has_changed	_Ctype_int
	buffer_hints		_Ctype_int
	dct_coeff		*_Ctype_short
	ref_index		[2]*_Ctypedef_int8_t
	reordered_opaque	_Ctypedef_int64_t
	hwaccel_picture_private	unsafe.Pointer
}
type _Ctype_short int16
type _Ctypedef_AVFormatContext _Ctype_struct_AVFormatContext
type _Ctype_struct___4 struct {
	lumH	*_Ctypedef_SwsVector
	lumV	*_Ctypedef_SwsVector
	chrH	*_Ctypedef_SwsVector
	chrV	*_Ctypedef_SwsVector
}
type _Ctype_ulong uint32
type _Ctype_struct___0 struct {
	class_name			*_Ctype_char
	item_name			*[0]byte
	option				*[0]byte
	version				_Ctype_int
	log_level_offset_offset		_Ctype_int
	parent_log_context_offset	_Ctype_int
}
type _Ctype_float float32
type _Ctype_struct_AVFifoBuffer struct {
	buffer	*_Ctypedef_uint8_t
	rptr	*_Ctypedef_uint8_t
	wptr	*_Ctypedef_uint8_t
	end	*_Ctypedef_uint8_t
	rndx	_Ctypedef_uint32_t
	wndx	_Ctypedef_uint32_t
}
type _Ctypedef_AVFormatParameters _Ctype_struct_AVFormatParameters
type _Ctype_struct_AVOption struct {
	name		*_Ctype_char
	help		*_Ctype_char
	offset		_Ctype_int
	_type		uint32
	default_val	_Ctype_double
	min		_Ctype_double
	max		_Ctype_double
	flags		_Ctype_int
	unit		*_Ctype_char
}
type _Ctypedef_uint32_t _Ctype_uint
type _Ctypedef_ByteIOContext _Ctype_struct___1
type _Ctype_struct_AVCodecParser struct {
	codec_ids	[5]_Ctype_int
	priv_data_size	_Ctype_int
	parser_init	*[0]byte
	parser_parse	*[0]byte
	parser_close	*[0]byte
	split		*[0]byte
	next		*_Ctype_struct_AVCodecParser
}
type _Ctypedef_RcOverride _Ctype_struct_RcOverride
type _Ctype_int int32
type _Ctypedef_uint64_t _Ctype_ulonglong
type _Ctype_schar int8
type _Ctypedef_AVChapter _Ctype_struct_AVChapter
type _Ctype_struct_AVCodecContext struct {
	av_class			*_Ctypedef_AVClass
	bit_rate			_Ctype_int
	bit_rate_tolerance		_Ctype_int
	flags				_Ctype_int
	sub_id				_Ctype_int
	me_method			_Ctype_int
	extradata			*_Ctypedef_uint8_t
	extradata_size			_Ctype_int
	time_base			_Ctypedef_AVRational
	width				_Ctype_int
	height				_Ctype_int
	gop_size			_Ctype_int
	pix_fmt				int32
	rate_emu			_Ctype_int
	draw_horiz_band			*[0]byte
	sample_rate			_Ctype_int
	channels			_Ctype_int
	sample_fmt			int32
	frame_size			_Ctype_int
	frame_number			_Ctype_int
	real_pict_num			_Ctype_int
	delay				_Ctype_int
	qcompress			_Ctype_float
	qblur				_Ctype_float
	qmin				_Ctype_int
	qmax				_Ctype_int
	max_qdiff			_Ctype_int
	max_b_frames			_Ctype_int
	b_quant_factor			_Ctype_float
	rc_strategy			_Ctype_int
	b_frame_strategy		_Ctype_int
	hurry_up			_Ctype_int
	codec				*_Ctype_struct_AVCodec
	priv_data			unsafe.Pointer
	rtp_payload_size		_Ctype_int
	rtp_callback			*[0]byte
	mv_bits				_Ctype_int
	header_bits			_Ctype_int
	i_tex_bits			_Ctype_int
	p_tex_bits			_Ctype_int
	i_count				_Ctype_int
	p_count				_Ctype_int
	skip_count			_Ctype_int
	misc_bits			_Ctype_int
	frame_bits			_Ctype_int
	opaque				unsafe.Pointer
	codec_name			[32]_Ctype_char
	codec_type			int32
	codec_id			uint32
	codec_tag			_Ctype_uint
	workaround_bugs			_Ctype_int
	luma_elim_threshold		_Ctype_int
	chroma_elim_threshold		_Ctype_int
	strict_std_compliance		_Ctype_int
	b_quant_offset			_Ctype_float
	error_recognition		_Ctype_int
	get_buffer			*[0]byte
	release_buffer			*[0]byte
	has_b_frames			_Ctype_int
	block_align			_Ctype_int
	parse_only			_Ctype_int
	mpeg_quant			_Ctype_int
	stats_out			*_Ctype_char
	stats_in			*_Ctype_char
	rc_qsquish			_Ctype_float
	rc_qmod_amp			_Ctype_float
	rc_qmod_freq			_Ctype_int
	rc_override			*_Ctypedef_RcOverride
	rc_override_count		_Ctype_int
	rc_eq				*_Ctype_char
	rc_max_rate			_Ctype_int
	rc_min_rate			_Ctype_int
	rc_buffer_size			_Ctype_int
	rc_buffer_aggressivity		_Ctype_float
	i_quant_factor			_Ctype_float
	i_quant_offset			_Ctype_float
	rc_initial_cplx			_Ctype_float
	dct_algo			_Ctype_int
	lumi_masking			_Ctype_float
	temporal_cplx_masking		_Ctype_float
	spatial_cplx_masking		_Ctype_float
	p_masking			_Ctype_float
	dark_masking			_Ctype_float
	idct_algo			_Ctype_int
	slice_count			_Ctype_int
	slice_offset			*_Ctype_int
	error_concealment		_Ctype_int
	dsp_mask			_Ctype_uint
	bits_per_coded_sample		_Ctype_int
	prediction_method		_Ctype_int
	sample_aspect_ratio		_Ctypedef_AVRational
	coded_frame			*_Ctypedef_AVFrame
	debug				_Ctype_int
	debug_mv			_Ctype_int
	error				[4]_Ctypedef_uint64_t
	mb_qmin				_Ctype_int
	mb_qmax				_Ctype_int
	me_cmp				_Ctype_int
	me_sub_cmp			_Ctype_int
	mb_cmp				_Ctype_int
	ildct_cmp			_Ctype_int
	dia_size			_Ctype_int
	last_predictor_count		_Ctype_int
	pre_me				_Ctype_int
	me_pre_cmp			_Ctype_int
	pre_dia_size			_Ctype_int
	me_subpel_quality		_Ctype_int
	get_format			*[0]byte
	dtg_active_format		_Ctype_int
	me_range			_Ctype_int
	intra_quant_bias		_Ctype_int
	inter_quant_bias		_Ctype_int
	color_table_id			_Ctype_int
	internal_buffer_count		_Ctype_int
	internal_buffer			unsafe.Pointer
	global_quality			_Ctype_int
	coder_type			_Ctype_int
	context_model			_Ctype_int
	slice_flags			_Ctype_int
	xvmc_acceleration		_Ctype_int
	mb_decision			_Ctype_int
	intra_matrix			*_Ctypedef_uint16_t
	inter_matrix			*_Ctypedef_uint16_t
	stream_codec_tag		_Ctype_uint
	scenechange_threshold		_Ctype_int
	lmin				_Ctype_int
	lmax				_Ctype_int
	palctrl				*_Ctype_struct_AVPaletteControl
	noise_reduction			_Ctype_int
	reget_buffer			*[0]byte
	rc_initial_buffer_occupancy	_Ctype_int
	inter_threshold			_Ctype_int
	flags2				_Ctype_int
	error_rate			_Ctype_int
	antialias_algo			_Ctype_int
	quantizer_noise_shaping		_Ctype_int
	thread_count			_Ctype_int
	execute				*[0]byte
	thread_opaque			unsafe.Pointer
	me_threshold			_Ctype_int
	mb_threshold			_Ctype_int
	intra_dc_precision		_Ctype_int
	nsse_weight			_Ctype_int
	skip_top			_Ctype_int
	skip_bottom			_Ctype_int
	profile				_Ctype_int
	level				_Ctype_int
	lowres				_Ctype_int
	coded_width			_Ctype_int
	coded_height			_Ctype_int
	frame_skip_threshold		_Ctype_int
	frame_skip_factor		_Ctype_int
	frame_skip_exp			_Ctype_int
	frame_skip_cmp			_Ctype_int
	border_masking			_Ctype_float
	mb_lmin				_Ctype_int
	mb_lmax				_Ctype_int
	me_penalty_compensation		_Ctype_int
	skip_loop_filter		int32
	skip_idct			int32
	skip_frame			int32
	bidir_refine			_Ctype_int
	brd_scale			_Ctype_int
	crf				_Ctype_float
	cqp				_Ctype_int
	keyint_min			_Ctype_int
	refs				_Ctype_int
	chromaoffset			_Ctype_int
	bframebias			_Ctype_int
	trellis				_Ctype_int
	complexityblur			_Ctype_float
	deblockalpha			_Ctype_int
	deblockbeta			_Ctype_int
	partitions			_Ctype_int
	directpred			_Ctype_int
	cutoff				_Ctype_int
	scenechange_factor		_Ctype_int
	mv0_threshold			_Ctype_int
	b_sensitivity			_Ctype_int
	compression_level		_Ctype_int
	use_lpc				_Ctype_int
	lpc_coeff_precision		_Ctype_int
	min_prediction_order		_Ctype_int
	max_prediction_order		_Ctype_int
	prediction_order_method		_Ctype_int
	min_partition_order		_Ctype_int
	max_partition_order		_Ctype_int
	timecode_frame_start		_Ctypedef_int64_t
	request_channels		_Ctype_int
	drc_scale			_Ctype_float
	reordered_opaque		_Ctypedef_int64_t
	bits_per_raw_sample		_Ctype_int
	channel_layout			_Ctypedef_int64_t
	request_channel_layout		_Ctypedef_int64_t
	rc_max_available_vbv_use	_Ctype_float
	rc_min_vbv_overflow_use		_Ctype_float
	hwaccel				*_Ctype_struct_AVHWAccel
	ticks_per_frame			_Ctype_int
	hwaccel_context			unsafe.Pointer
	color_primaries			uint32
	color_trc			uint32
	colorspace			uint32
	color_range			uint32
	chroma_sample_location		uint32
	execute2			*[0]byte
	weighted_p_pred			_Ctype_int
	aq_mode				_Ctype_int
	aq_strength			_Ctype_float
	psy_rd				_Ctype_float
	psy_trellis			_Ctype_float
	rc_lookahead			_Ctype_int
	crf_max				_Ctype_float
	log_level_offset		_Ctype_int
	lpc_type			int32
	lpc_passes			_Ctype_int
	slices				_Ctype_int
	subtitle_header			*_Ctypedef_uint8_t
	subtitle_header_size		_Ctype_int
}
type _Ctypedef_AVProgram _Ctype_struct_AVProgram
type _Ctype_double float64
type _Ctype_void [0]byte
const _Cconst_AVCODEC_MAX_AUDIO_FRAME_SIZE = 192000
const _Cconst_CODEC_TYPE_AUDIO = 1
const _Cconst_CODEC_TYPE_VIDEO = 0
const _Cconst_URL_WRONLY = 1

func _Cfunc_av_close_input_file(*_Ctypedef_AVFormatContext)
func _Cfunc_GoString(*_Ctype_char) string
func _Cfunc_av_fifo_alloc(_Ctype_uint) *_Ctypedef_AVFifoBuffer
func _Cfunc_avformat_alloc_context() *_Ctypedef_AVFormatContext
func _Cfunc_av_write_header(*_Ctypedef_AVFormatContext) _Ctype_int
func _Cfunc_avcodec_close(*_Ctypedef_AVCodecContext) _Ctype_int
func _Cfunc_av_dup_packet(*_Ctypedef_AVPacket) _Ctype_int
func _Cfunc_avpicture_get_size(int32, _Ctype_int, _Ctype_int) _Ctype_int
func _Cfunc_CString(string) *_Ctype_char
func _Cfunc_sws_getContext(_Ctype_int, _Ctype_int, int32, _Ctype_int, _Ctype_int, int32, _Ctype_int, *_Ctypedef_SwsFilter, *_Ctypedef_SwsFilter, *_Ctype_double) *[0]byte
func _Cfunc_audio_resample(*[0]byte, *_Ctype_short, *_Ctype_short, _Ctype_int) _Ctype_int
func _Cfunc_url_fclose(*_Ctypedef_ByteIOContext) _Ctype_int
func _Cfunc_av_free(unsafe.Pointer)
func _Cfunc_av_set_parameters(*_Ctypedef_AVFormatContext, *_Ctypedef_AVFormatParameters) _Ctype_int
func _Cfunc_gmf_resample_compensate(*[0]byte, _Ctype_int, _Ctype_int)
func _Cfunc_av_get_string(unsafe.Pointer, *_Ctype_char, **_Ctypedef_AVOption, *_Ctype_char, _Ctype_int) *_Ctype_char
func _Cfunc_avcodec_register_all()
func _Cfunc_avcodec_decode_video2(*_Ctypedef_AVCodecContext, *_Ctypedef_AVFrame, *_Ctype_int, *_Ctypedef_AVPacket) _Ctype_int
func _Cfunc_avcodec_decode_audio3(*_Ctypedef_AVCodecContext, *_Ctypedef_int16_t, *_Ctype_int, *_Ctypedef_AVPacket) _Ctype_int
func _Cfunc_audio_resample_close(*[0]byte)
func _Cfunc_sws_freeContext(*[0]byte)
func _Cfunc_avcodec_encode_video(*_Ctypedef_AVCodecContext, *_Ctypedef_uint8_t, _Ctype_int, *_Ctypedef_AVFrame) _Ctype_int
func _Cfunc_av_fifo_free(*_Ctypedef_AVFifoBuffer)
func _Cfunc_av_set_string3(unsafe.Pointer, *_Ctype_char, *_Ctype_char, _Ctype_int, **_Ctypedef_AVOption) _Ctype_int
func _Cfunc_av_init_packet(*_Ctypedef_AVPacket)
func _Cfunc_av_interleaved_write_frame(*_Ctypedef_AVFormatContext, *_Ctypedef_AVPacket) _Ctype_int
func _Cfunc_av_free_packet(*_Ctypedef_AVPacket)
func _Cfunc_av_compare_ts(_Ctypedef_int64_t, _Ctypedef_AVRational, _Ctypedef_int64_t, _Ctypedef_AVRational) _Ctype_int
func _Cfunc_avcodec_open(*_Ctypedef_AVCodecContext, *_Ctypedef_AVCodec) _Ctype_int
func _Cfunc_avpicture_deinterlace(*_Ctypedef_AVPicture, *_Ctypedef_AVPicture, int32, _Ctype_int, _Ctype_int) _Ctype_int
func _Cfunc_url_fopen(**_Ctypedef_ByteIOContext, *_Ctype_char, _Ctype_int) _Ctype_int
func _Cfunc_av_next_option(unsafe.Pointer, *_Ctypedef_AVOption) *_Ctypedef_AVOption
func _Cfunc_av_fifo_space(*_Ctypedef_AVFifoBuffer) _Ctype_int
func _Cfunc_av_guess_format(*_Ctype_char, *_Ctype_char, *_Ctype_char) *_Ctypedef_AVOutputFormat
func _Cfunc_av_fifo_generic_write(*_Ctypedef_AVFifoBuffer, unsafe.Pointer, _Ctype_int, *[0]byte) _Ctype_int
func _Cfunc_av_get_bits_per_sample_fmt(int32) _Ctype_int
func _Cfunc_free(unsafe.Pointer)
func _Cfunc_avpicture_alloc(*_Ctypedef_AVPicture, int32, _Ctype_int, _Ctype_int) _Ctype_int
func _Cfunc_avcodec_alloc_context() *_Ctypedef_AVCodecContext
func _Cfunc_avcodec_get_frame_defaults(*_Ctypedef_AVFrame)
func _Cfunc_av_fifo_generic_read(*_Ctypedef_AVFifoBuffer, unsafe.Pointer, _Ctype_int, *[0]byte) _Ctype_int
func _Cfunc_avcodec_get_context_defaults2(*_Ctypedef_AVCodecContext, int32)
func _Cfunc_avcodec_find_decoder_by_name(*_Ctype_char) *_Ctypedef_AVCodec
func _Cfunc_av_rescale_q(_Ctypedef_int64_t, _Ctypedef_AVRational, _Ctypedef_AVRational) _Ctypedef_int64_t
func _Cfunc_dump_format(*_Ctypedef_AVFormatContext, _Ctype_int, *_Ctype_char, _Ctype_int)
func _Cfunc_avcodec_encode_audio(*_Ctypedef_AVCodecContext, *_Ctypedef_uint8_t, _Ctype_int, *_Ctype_short) _Ctype_int
func _Cfunc_avpicture_fill(*_Ctypedef_AVPicture, *_Ctypedef_uint8_t, int32, _Ctype_int, _Ctype_int) _Ctype_int
func _Cfunc_av_new_stream(*_Ctypedef_AVFormatContext, _Ctype_int) *_Ctypedef_AVStream
func _Cfunc_av_register_all()
func _Cfunc_av_write_trailer(*_Ctypedef_AVFormatContext) _Ctype_int
func _Cfunc_avcodec_find_decoder(uint32) *_Ctypedef_AVCodec
func _Cfunc_avcodec_find_encoder_by_name(*_Ctype_char) *_Ctypedef_AVCodec
func _Cfunc_av_fifo_realloc2(*_Ctypedef_AVFifoBuffer, _Ctype_uint) _Ctype_int
func _Cfunc_avcodec_find_encoder(uint32) *_Ctypedef_AVCodec
func _Cfunc_av_read_frame(*_Ctypedef_AVFormatContext, *_Ctypedef_AVPacket) _Ctype_int
func _Cfunc_av_find_stream_info(*_Ctypedef_AVFormatContext) _Ctype_int
func _Cfunc_sws_scale(*[0]byte, **_Ctypedef_uint8_t, *_Ctype_int, _Ctype_int, _Ctype_int, **_Ctypedef_uint8_t, *_Ctype_int) _Ctype_int
func _Cfunc_av_open_input_file(**_Ctypedef_AVFormatContext, *_Ctype_char, *_Ctypedef_AVInputFormat, _Ctype_int, *_Ctypedef_AVFormatParameters) _Ctype_int
func _Cfunc_av_audio_resample_init(_Ctype_int, _Ctype_int, _Ctype_int, _Ctype_int, int32, int32, _Ctype_int, _Ctype_int, _Ctype_int, _Ctype_double) *[0]byte
func _Cfunc_av_fifo_size(*_Ctypedef_AVFifoBuffer) _Ctype_int
func _Cfunc_av_cmp_q(_Ctypedef_AVRational, _Ctypedef_AVRational) _Ctype_int
