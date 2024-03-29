CREATE TABLE IF NOT EXISTS `config` (
  `config_key` varchar(255) NOT NULL,
  `config_val` varchar(255) NOT NULL,
  PRIMARY KEY  (`config_key`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS `files` (
  `id` int(11) NOT NULL auto_increment,
  `filename` varchar(255) default NULL,
  `path` varchar(255) NOT NULL,
  `size` int(11) NOT NULL default '0',
  `stream_count` int(2) NOT NULL default '0',
  `container_type` varchar(255) NOT NULL,
  `title` varchar(255) NOT NULL,
  `author` varchar(255) NOT NULL,
  `copyright` varchar(255) NOT NULL,
  `comment` varchar(255) NOT NULL,
  `album` varchar(255) NOT NULL,
  `year` int(4) NOT NULL default '0',
  `track` int(1) NOT NULL default '0',
  `genre` varchar(255) NOT NULL,
  `duration` int(11) NOT NULL default '0',
  `bitrate` int(11) NOT NULL default '0',
  `insertdate` timestamp NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `type` int(1) NOT NULL default '0',
  `parent` int(11) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS `frame_groups` (
  `id` int(11) NOT NULL auto_increment,
  `jobid` int(11) NOT NULL default '0',
  `frame_group` int(11) default NULL,
  `startts` double NOT NULL default '0',
  `byte_pos` double NOT NULL,
  `stream_id` int(11) default NULL,
  `stream_index` int(11) NOT NULL default '0',
  `frame_count` int(11) NOT NULL default '0',
  `sended` timestamp NULL default NULL,
  `complete` timestamp NULL default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS `jobs` (
  `id` int(11) NOT NULL auto_increment,
  `inputfile` int(11) default NULL,
  `outputfile` int(11) default NULL,
  `begin` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `complete` timestamp NULL default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS `job_details` (
  `id` int(11) NOT NULL auto_increment,
  `job_id` int(11) default NULL,
  `instream` int(11) default NULL,
  `outstream` int(11) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS `job_logs` (
  `id` int(11) NOT NULL auto_increment,
  `packet_in` int(11) default NULL,
  `packet_out` int(11) default NULL,
  `begin` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `complete` timestamp NOT NULL default '0000-00-00 00:00:00',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS `packets` (
  `id` int(11) NOT NULL auto_increment,
  `stream_id` int(11) default NULL,
  `pts` double default NULL,
  `dts` double default NULL,
  `stream_index` int(11) default NULL,
  `key_frame` int(11) default NULL,
  `frame_group` int(11) default NULL,
  `flags` int(11) default NULL,
  `duration` int(11) default NULL,
  `pos` int(11) default NULL,
  `data_size` int(11) default NULL,
  `data` mediumblob,
  PRIMARY KEY  (`id`),
  KEY `packet_pts_idx` (`pts`),
  KEY `packet_dts_idx` (`pts`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS `process_units` (
  `id` int(11) NOT NULL auto_increment,
  `source_stream` int(11) NOT NULL,
  `target_stream` int(11) NOT NULL,
  `start_ts` double NOT NULL,
  `frame_count` int(11) NOT NULL,
  `send` timestamp NULL default NULL,
  `complete` timestamp NULL default NULL,
  `priority` tinyint(1) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS `profiles` (
  `id` int(11) NOT NULL auto_increment,
  `profile_name` varchar(255) default NULL,
  `v_format` varchar(255) default NULL,
  `v_codec` int(11) default NULL,
  `v_bitrate` int(11) default NULL,
  `v_framerate` int(11) default NULL,
  `v_width` int(11) default NULL,
  `v_height` int(11) default NULL,
  `a_channels` int(11) default NULL,
  `a_codec` varchar(255) default NULL,
  `a_bitrate` int(11) default NULL,
  `a_samplerate` int(11) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS `statistics_log` (
  `id` bigint(20) NOT NULL auto_increment,
  `stat_key` varchar(255) NOT NULL,
  `stat_val` varchar(255) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS `streams` (
  `id` int(11) NOT NULL auto_increment,
  `fileid` int(11) NOT NULL default '0',
  `stream_index` int(11) NOT NULL default '0',
  `stream_type` int(11) NOT NULL default '0',
  `codec` int(11) NOT NULL default '0',
  `codec_name` varchar(255) default NULL,
  `framerate` int(11) NOT NULL default '0',
  `start_time` double NOT NULL default '0',
  `duration` int(11) NOT NULL default '0',
  `time_base_num` int(11) NOT NULL default '0',
  `time_base_den` int(11) NOT NULL default '0',
  `framecount` int(11) NOT NULL default '0',
  `width` int(11) NOT NULL default '0',
  `height` int(11) NOT NULL default '0',
  `gop_size` int(11) NOT NULL default '0',
  `pix_fmt` int(11) NOT NULL default '0',
  `bit_rate` int(11) NOT NULL default '0',
  `rate_emu` int(11) NOT NULL default '0',
  `sample_rate` int(11) NOT NULL default '0',
  `channels` int(11) NOT NULL default '0',
  `sample_fmt` int(11) NOT NULL default '0',
  `priv_data_size` int(11) default '0',
  `priv_data` blob,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS `user` (
  `id` int(11) unsigned NOT NULL auto_increment,
  `auth_name` varchar(255) NOT NULL,
  `auth_passwd` varchar(255) NOT NULL,
  `first_name` varchar(255) NOT NULL,
  `last_name` varchar(255) NOT NULL,
  `email` varchar(255) NOT NULL,
  `user_type` int(11) unsigned NOT NULL,
  `created` datetime NOT NULL,
  `updated` datetime NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS `version` (
  `id` int(11) NOT NULL auto_increment,
  `component` varchar(255) default NULL,
  `version` varchar(255) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;
CREATE TABLE IF NOT EXISTS `watch_folder` (
  `id` int(11) unsigned NOT NULL auto_increment,
  `folder` varchar(255) NOT NULL,
  `profile` int(11) unsigned NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;
INSERT INTO `config` (`config_key`, `config_val`) VALUES ('web.start','true');
INSERT INTO `version` (`id`, `component`, `version`) VALUES(1, 'database.model', '0.0.1');