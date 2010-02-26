alter IGNORE table profiles add v_aspect_ratio varchar(255) NOT NULL AFTER v_extra;
INSERT INTO version (component,version) VALUES ('database.model','0.0.4');


