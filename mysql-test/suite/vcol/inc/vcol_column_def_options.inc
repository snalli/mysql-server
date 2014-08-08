################################################################################
# inc/vcol_column_def_options.inc                                              #
#                                                                              #
# Purpose:                                                                     #
#  Testing different optional parameters specified when defining               #
#  a virtual column.                                                           #
#                                                                              #
#                                                                              #
#------------------------------------------------------------------------------#
# Original Author: Andrey Zhakov                                               #
# Original Date: 2008-09-02                                                    #
# Change Author:                                                               #
# Change Date:                                                                 #
# Change:                                                                      #
################################################################################

--echo #
--echo # Section 1. Wrong column definition options
--echo #            - NULL
--echo #            - DEFAULT <value>
--echo #            - AUTO_INCREMENT

--echo # NOT NULL
create table t1 (a int, b virtual int as (a+1) not null);
--error 1048
insert into t1(a) values(null);
insert into t1(a) values(1);
select * from t1;
drop table t1;
create table t1 (a int, b virtual int as (a+1) stored not null);
--error 1048
insert into t1(a) values(null);
insert into t1(a) values(1);
select * from t1;
drop table t1;
create table t1 (a int);
alter table t1 add column b virtual int as (a+1) not null;
drop table t1;

--echo # NULL  
--error 1064
create table t1 (a int, b virtual int as (a+1) null);
create table t1 (a int);
--error 1064
alter table t1 add column b virtual int as (a+1) null;
drop table t1;

--echo # DEFAULT
--error 1064
create table t1 (a int, b virtual int as (a+1) default 0);
create table t1 (a int);
--error 1064
alter table t1 add column b virtual int as (a+1) default 0;
drop table t1;

--echo # AUTO_INCREMENT
--error 1064
create table t1 (a int, b virtual int as (a+1) AUTO_INCREMENT);
create table t1 (a int);
--error 1064
alter table t1 add column b virtual int as (a+1) AUTO_INCREMENT;
drop table t1;

--echo # [PRIMARY] KEY
--error 1951
create table t1 (a int, b virtual int as (a+1) key);
create table t1 (a int, b virtual int as (a+1) stored key);
insert into t1 (a) values (3),(1),(2);
select * from t1;
drop table t1;
--error 1951
create table t1 (a int, b virtual int as (a+1) primary key);
create table t1 (a int, b virtual int as (a+1) stored primary key);
insert into t1 (a) values (3),(1),(2);
select * from t1;
drop table t1;
create table t1 (a int);
--error 1951
alter table t1 add column b virtual int as (a+1) key;
alter table t1 add column b virtual int as (a+1) stored key;
--error 1951
alter table t1 add column c virtual int as (a+2) primary key;
show create table t1;
--error 1068
alter table t1 add column c virtual int as (a+2) stored primary key;
drop table t1;

--echo # Section 2. Other column definition options
--echo #            - COMMENT
--echo #            - REFERENCES (only syntax testing here)
--echo #            - STORED (only systax testing here)
create table t1 (a int, b virtual int as (a % 2) comment 'my comment');
show create table t1;
describe t1;
drop table t1;
create table t1 (a int, b virtual int as (a % 2));
alter table t1 modify b virtual int as (a % 2) comment 'my comment';
show create table t1;
describe t1;
insert into t1 (a) values (1);
select * from t1;
insert into t1 values (2,default);
select a,b from t1;
create table t2 like t1;
show create table t2;
describe t2;
insert into t2 (a) values (1);
select * from t2;
insert into t2 values (2,default);
select a,b from t2;
drop table t2;
drop table t1;

create table t1 (a int, b virtual int as (a % 2) stored); 
show create table t1;
describe t1;
insert into t1 (a) values (1);
select * from t1;
insert into t1 values (2,default);
select a,b from t1;
drop table t1;


create table t2 (a int);
create table t1 (a int, b virtual int as (a % 2) stored references t2(a));
show create table t1;
drop table t1;
create table t1 (a int, b virtual int as (a % 2));
--error 1064
alter table t1 modify b virtual int as (a % 2) stored references t2(a);
show create table t1;
drop table t1;

--echo Test virtual columns referencing other virtual columns
create table t1 (a int unique, b virtual int as(-a), c virtual int as (b + 1));
insert into t1 (a) values (1), (2);
select * from t1;
insert into t1(a) values (1) on duplicate key update a=3;
select * from t1;
update t1 set a=4 where a=2;
select * from t1;
drop table t1;

--error 1956
create table t1 (a int, b virtual int as(-b), c virtual int as (b + 1));
--error 1956
create table t1 (a int, b virtual int as(-c), c virtual int as (b + 1));
