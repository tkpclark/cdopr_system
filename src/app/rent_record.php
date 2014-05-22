<?php

require_once('/home/log4php/php/Logger.php');
require_once('mysql.php');

Logger::configure('log4php_config.xml');
$logging = Logger::getLogger('rent_record');
/******recv $_GET arguments:*********
*
*
* mt_mode
* == 1 : wraith send mt
* == 2 : send mo to partner and they supply mt message
*
*/

if(!isset($_REQUEST['record']) ||!isset($_REQUEST['trs_id']) || !isset($_REQUEST['mt_mode']))
{
	echo "arguments error!";
	exit;
}

$record = json_decode($_REQUEST['record']);
$trs_id = $_REQUEST['trs_id'];
$mt_mode = $_REQUEST['mt_mode'];
//$logging->info($record);
//====================




//$status=strpos($record->{'allow_province'},$record->{'province'})===false?2:0;
$status=0;//it's useless
$sql = sprintf("insert into wraith_rent_record(in_date,phone_number,linkid,message,sp_number,province,area,deal_flag,trs_id,mt_mode,gwid,amount,product_id,product_code,mt_message,product_seq) values(NOW(),'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
		$record->{'phone_number'},
		$record->{'linkid'},
		$record->{'message'},
		$record->{'sp_number'},
		$record->{'province'},
		$record->{'area'},
		$status,
		$trs_id,
		$mt_mode,
		$record->{'gwid'},
		$record->{'amount'},
		$record->{'product_id'},
		$record->{'product_code'},
		$record->{'default_msg'},
		$record->{'product_seq'}
		
	);
$logging->info($sql);
mysqli_query($mysqli, "set names utf8");
$res = mysqli_query($mysqli, $sql);
if (!$res) {
	$logging->info("Failed to run query: (" . $mysqli->errno . ") " . $mysqli->error);
}

//====================

if($mt_mode==1)//给用户回信息
{
	$sql = sprintf("insert into wraith_mt(gwid,sp_number,phone_number,linkid,amount,product_id,product_code,message,in_time,province,area) values ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s',NOW() ,'%s','%s')",
			$record->{'gwid'},
			$record->{'sp_number'},
			$record->{'phone_number'},
			$record->{'linkid'},
			$record->{'amount'},
			$record->{'product_id'},
			$record->{'product_code'},
			//$record->{'message'}
			$record->{'default_msg'},
			$record->{'province'},
			$record->{'area'}
	);
	$logging->info($sql);
	mysqli_query($mysqli, "set names utf8");
	$res = mysqli_query($mysqli, $sql);
	if (!$res) {
		echo "Failed to run query: (" . $mysqli->errno . ") " . $mysqli->error;
	}
}