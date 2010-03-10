alter IGNORE table profiles add v_deinterlace varchar(255) NOT NULL DEFAULT '0' AFTER v_extra;
alter IGNORE table profiles add v_keep_aspect_ratio varchar(255) NOT NULL DEFAULT '0' AFTER v_deinterlace;
alter IGNORE table streams add deinterlaced int(4) NOT NULL DEFAULT '0' AFTER height;
alter IGNORE table jobs add deinterlace int(4) NOT NULL DEFAULT '0' AFTER outputfile;
alter IGNORE table jobs add keep_display_aspect int(4) NOT NULL DEFAULT '0' AFTER deinterlace;
alter table version drop id;
alter table version add primary key cmp_idx (component);
REPLACE INTO version (component,version) VALUES ('database.model','0.0.5');

