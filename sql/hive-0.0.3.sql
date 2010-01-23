
SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Datenbank: `hive2`
--

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `codec`
--

CREATE TABLE IF NOT EXISTS `codec` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `codec_id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `long_name` varchar(255) NOT NULL,
  `extra` varchar(1000) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3 ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `config`
--

CREATE TABLE IF NOT EXISTS `config` (
  `config_key` varchar(255) NOT NULL,
  `config_val` varchar(255) NOT NULL,
  PRIMARY KEY (`config_key`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `files`
--

CREATE TABLE IF NOT EXISTS `files` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `filename` varchar(255) DEFAULT NULL,
  `path` varchar(255) NOT NULL,
  `size` int(11) NOT NULL DEFAULT '0',
  `stream_count` int(2) NOT NULL DEFAULT '0',
  `container_type` varchar(255) NOT NULL,
  `title` varchar(255) NOT NULL,
  `author` varchar(255) NOT NULL,
  `copyright` varchar(255) NOT NULL,
  `comment` varchar(255) NOT NULL,
  `album` varchar(255) NOT NULL,
  `year` int(4) NOT NULL DEFAULT '0',
  `track` int(1) NOT NULL DEFAULT '0',
  `genre` varchar(255) NOT NULL,
  `duration` bigint(20) NOT NULL DEFAULT '0',
  `bitrate` int(11) NOT NULL DEFAULT '0',
  `insertdate` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `type` int(1) NOT NULL DEFAULT '0',
  `parent` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=5 ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `frame_groups`
--

CREATE TABLE IF NOT EXISTS `frame_groups` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `jobid` int(11) NOT NULL DEFAULT '0',
  `frame_group` int(11) DEFAULT NULL,
  `startts` double NOT NULL DEFAULT '0',
  `byte_pos` double NOT NULL,
  `stream_id` int(11) DEFAULT NULL,
  `stream_index` int(11) NOT NULL DEFAULT '0',
  `frame_count` int(11) NOT NULL DEFAULT '0',
  `sended` timestamp NULL DEFAULT NULL,
  `complete` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `jobs`
--

CREATE TABLE IF NOT EXISTS `jobs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `inputfile` int(11) DEFAULT NULL,
  `outputfile` int(11) DEFAULT NULL,
  `begin` timestamp NULL DEFAULT NULL,
  `complete` timestamp NULL DEFAULT NULL,
  `profileid` int(11) NOT NULL,
  `status` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3 ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `job_details`
--

CREATE TABLE IF NOT EXISTS `job_details` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `job_id` int(11) DEFAULT NULL,
  `instream` int(11) DEFAULT NULL,
  `outstream` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=5 ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `job_logs`
--

CREATE TABLE IF NOT EXISTS `job_logs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `packet_in` int(11) DEFAULT NULL,
  `packet_out` int(11) DEFAULT NULL,
  `begin` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `complete` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `packets`
--

CREATE TABLE IF NOT EXISTS `packets` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `stream_id` int(11) DEFAULT NULL,
  `pts` bigint(20) DEFAULT NULL,
  `dts` bigint(20) DEFAULT NULL,
  `stream_index` int(11) DEFAULT NULL,
  `key_frame` int(11) DEFAULT NULL,
  `frame_group` int(11) DEFAULT NULL,
  `flags` int(11) DEFAULT NULL,
  `duration` int(11) DEFAULT NULL,
  `pos` int(11) DEFAULT NULL,
  `sort` bigint(20) NOT NULL,
  `data_size` int(11) DEFAULT NULL,
  `data` mediumblob,
  PRIMARY KEY (`id`),
  KEY `packet_group_idx` (`frame_group`),
  KEY `pts` (`pts`),
  KEY `dts` (`dts`),
  KEY `stream_id_2` (`stream_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `process_units`
--

CREATE TABLE IF NOT EXISTS `process_units` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `source_stream` int(11) NOT NULL,
  `target_stream` int(11) NOT NULL,
  `start_ts` bigint(20) NOT NULL,
  `end_ts` bigint(20) NOT NULL,
  `frame_count` int(11) NOT NULL,
  `send` timestamp NULL DEFAULT NULL,
  `complete` timestamp NULL DEFAULT NULL,
  `priority` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=111 ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `profiles`
--

CREATE TABLE IF NOT EXISTS `profiles` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `profile_name` varchar(255) DEFAULT NULL,
  `v_format` varchar(255) DEFAULT NULL,
  `v_format_ext` varchar(10) DEFAULT NULL,
  `v_codec` int(11) DEFAULT NULL,
  `v_bitrate` int(11) DEFAULT NULL,
  `v_framerate` varchar(20) DEFAULT NULL,
  `v_width` int(11) DEFAULT NULL,
  `v_height` int(11) DEFAULT NULL,
  `v_extra` varchar(1000) DEFAULT NULL,
  `a_channels` int(11) DEFAULT NULL,
  `a_codec` int(11) DEFAULT NULL,
  `a_bitrate` int(11) DEFAULT NULL,
  `a_samplerate` int(11) DEFAULT NULL,
  `a_extra` varchar(1000) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=27 ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `statistics_log`
--

CREATE TABLE IF NOT EXISTS `statistics_log` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `stat_key` varchar(255) NOT NULL,
  `stat_val` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `streams`
--

CREATE TABLE IF NOT EXISTS `streams` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `fileid` int(11) NOT NULL DEFAULT '0',
  `stream_index` int(11) NOT NULL DEFAULT '0',
  `stream_type` int(11) NOT NULL DEFAULT '0',
  `codec` int(11) NOT NULL DEFAULT '0',
  `codec_name` varchar(255) DEFAULT NULL,
  `framerate_num` int(11) NOT NULL DEFAULT '0',
  `framerate_den` int(11) NOT NULL,
  `start_time` bigint(20) NOT NULL DEFAULT '0',
  `duration` bigint(20) NOT NULL DEFAULT '0',
  `nb_frames` bigint(20) NOT NULL,
  `time_base_num` int(11) NOT NULL DEFAULT '0',
  `time_base_den` int(11) NOT NULL DEFAULT '0',
  `framecount` int(11) NOT NULL DEFAULT '0',
  `width` int(11) NOT NULL DEFAULT '0',
  `height` int(11) NOT NULL DEFAULT '0',
  `gop_size` int(11) NOT NULL DEFAULT '0',
  `pix_fmt` int(11) NOT NULL DEFAULT '0',
  `bit_rate` int(11) NOT NULL DEFAULT '0',
  `rate_emu` int(11) NOT NULL DEFAULT '0',
  `sample_rate` int(11) NOT NULL DEFAULT '0',
  `channels` int(11) NOT NULL DEFAULT '0',
  `sample_fmt` int(11) NOT NULL DEFAULT '0',
  `bits_per_coded_sample` int(11) NOT NULL,
  `priv_data_size` int(11) DEFAULT '0',
  `priv_data` blob,
  `flags` int(10) unsigned NOT NULL,
  `extra_profile_flags` varchar(1000) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=12 ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `user`
--

CREATE TABLE IF NOT EXISTS `user` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `auth_name` varchar(255) NOT NULL,
  `auth_passwd` varchar(255) NOT NULL,
  `first_name` varchar(255) NOT NULL,
  `last_name` varchar(255) NOT NULL,
  `email` varchar(255) NOT NULL,
  `user_type` int(11) unsigned NOT NULL,
  `created` datetime NOT NULL,
  `updated` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `version`
--

CREATE TABLE IF NOT EXISTS `version` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `component` varchar(255) DEFAULT NULL,
  `version` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `watch_folder`
--

CREATE TABLE IF NOT EXISTS `watch_folder` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `infolder` varchar(255) NOT NULL,
  `profile` int(11) unsigned NOT NULL,
  `outfolder` varchar(255) NOT NULL,
  `extension_filter` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=19 ;


INSERT INTO `config` VALUES('framerates', '1/10#10,1/15#15,1000/23976#23.976,1/24#24,1/25#25,100/2997#29.97,1/30#30');
INSERT INTO `config` VALUES('samplerates', '8000,11025,22050,32000,44100,48000');
INSERT INTO `config` VALUES('audiobitrates', '32000,96000,128000,160000,192000,224000,256000,320000');

INSERT INTO `profiles` VALUES(1, 'MP4 H.264 320x240 MP2 128k 44.1Khz', 'MP4 format', "mp4", 28, 1024000, '24', 320, 240,"", 2, 86016, 128000, 44100,"");
INSERT INTO `profiles` VALUES(2, 'MP4 H.264 15fps 320x240 MP2 128k 44.1Khz', 'MP4 format', "mp4", 28, 1024000, '15', 320, 240,"", 2, 86016, 128000, 44100,"");
INSERT INTO `profiles` VALUES(3, 'MP4 H.264 23.97fps 320x240 MP2 128k 44.1Khz', 'MP4 format', "mp4",28, 1024000, '23.976', 320, 240,"", 2, 86017, 128000, 44100,"");
INSERT INTO `profiles` VALUES(4, 'FLV H.263 320x240 MP3 128k 44.1Khz', 'Flash 9 (AVM2) format', "flv", 22, 1024000, 'same as so', 320, 240,"", 2, 86017, 128000, 44100,"");
INSERT INTO `profiles` VALUES(5, 'AVI MPEG4-2 320x240 MP2 128k 441.Khz', 'AVI format', "avi", 13, 1024000, '15', 320, 240,"", 2, 86016, 128000, 44100,"");
INSERT INTO `profiles` VALUES(6, 'DVD MPEG2 320x240 MP3 128k 44.1Khz', 'MPEG-2 PS format (DVD VOB)', "vob", 2, 1024000, '25', 320, 240,"", 2, 86017, 128000, 44100,"");
INSERT INTO `profiles` VALUES(7, 'DVD MPEG2 320x240 6000kbs ', 'MPEG-2 PS format (DVD VOB)', "vob", 2, 6000000, '25', 320, 240,"", 2, 86017, 128000, 44100,"");
load data infile 'sql/codec.txt' into table codec fields terminated by "#";


