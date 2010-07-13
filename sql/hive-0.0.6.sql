CREATE TABLE IF NOT EXISTS  project (
                id int(11) NOT NULL AUTO_INCREMENT,
                created timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
                name VARCHAR(1000) NOT NULL,
                PRIMARY KEY (id)
);


