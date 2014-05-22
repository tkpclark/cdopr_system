<?php

require_once('/home/log4php/php/Logger.php');
require_once('mysql.php');
require_once('AES_official.php');
require_once('pay_m_lib.php');
require_once('rent_send_1501.php');
require_once('rent_send_msgtunnel.php');

Logger::configure('log4php_config.xml');
$logging = Logger::getLogger('rent_sender');

function update_deal_flag($request,$deal_flag)
{
	global $logging;
	global $mysqli;
	////update db
	mysqli_query($mysqli, "set names utf8");
	$sql = sprintf("update wraith_rent_record set deal_flag=%d,send_time=NOW() where id = '%s'",$deal_flag, $request['id']);
	//$logging->info($sql);
	$res = mysqli_query($mysqli, $sql);
	if (!$res) {
		$logging->info("Failed to run query: (" . $mysqli->errno . ") " . $mysqli->error);
	}
}


function update_one_report($linkid,$report)
{
	global $logging;
	global $mysqli;
	$sql = sprintf("update wraith_rent_record set report='%s' where linkid='%s'", $report, $linkid);
	//$logging->info($sql);
	$res = mysqli_query($mysqli, $sql);
	if (!$res) {
		$logging->info("Failed to run query: (" . $mysqli->errno . ") " . $mysqli->error);
		exit;
	}
}
function check_report($linkid)
{
	global $logging;
	global $mysqli;
	$sql = sprintf("select report from `wraith_mt` where linkid='%s' and report is not null",$linkid);
	//$logging->info($sql);
	$res = mysqli_query($mysqli, $sql);
	if (!$res) {
		$logging->info("Failed to run query: (" . $mysqli->errno . ") " . $mysqli->error);
		exit;
	}
	if(mysqli_num_rows($res))
	{
		$request = $res->fetch_array(MYSQLI_ASSOC);
		$logging->info("report:".$request['report']);
		if($request['report']=='4' or $request['report']=='DELIVRD')
			$report='OK';
		else
			$report='FAIL';
		update_one_report($linkid,$report);
		
	}
}

function update_report()
{
	global $logging;
	global $mysqli;
	$sql = sprintf("select * from `wraith_rent_record` where report is NULL and in_date > NOW()-interval 1 day");
	//$logging->info($sql);
	$res = mysqli_query($mysqli, $sql);
	if (!$res) {
		$logging->info("Failed to run query: (" . $mysqli->errno . ") " . $mysqli->error);
		exit;
	}
	
	while($request = $res->fetch_array(MYSQLI_ASSOC))
	{
		//$logging->info("dealing request id:".$request['id']);
		check_report($request['linkid']);
	}
}

function send_record()
{
	global $logging;
	global $mysqli;
	$sql = sprintf("select * from `wraith_rent_record` where deal_flag in(0,1)  and in_date > NOW()-interval 1 day");
	//$logging->info($sql);
	$res = mysqli_query($mysqli, $sql);
	if (!$res) {
		$logging->info("Failed to run query: (" . $mysqli->errno . ") " . $mysqli->error);
		exit;
	}

	while($request = $res->fetch_array(MYSQLI_ASSOC))
	{
		//$logging->info("dealing request id:".$request['id']);
		$send_func='send_to_destination_'.$request['trs_id'];
		if(function_exists($send_func))
		{
			$send_func($request);
		}
		else
		{
			$logging->info("function ".$send_func." doesn't exist!!!");
			update_deal_flag($request, '11');
		}
	}
}


//================main================


/*
function scan_record()
{
	global $logging;
	global $mysqli;
	$sql = sprintf("select * from `wraith_rent_record` where in_date > NOW()-interval 1 day and deal_flag<>2");
	//$logging->info($sql);
	$res = mysqli_query($mysqli, $sql);
	if (!$res) {
		$logging->info("Failed to run query: (" . $mysqli->errno . ") " . $mysqli->error);
		exit;
	}
	
	while($request = $res->fetch_array(MYSQLI_ASSOC))
	{
		//$logging->info("dealing request id:".$request['id']);
		//=======send mo=======
		if($request['send_mode'] == 1 and $request['deal_flag'] == 0)
		{
			send_mo($request);
		}
		//=======check report========/
		else if(strlen($request['report'])<2)
		{
			check_report($request['linkid']);
		}
		//========send mr========/
		else if($request['send_mode'] == 1 and $request['deal_flag'] == 0 and strlen($request['report'])>2 )
		{
			send_mr($request);
		}
		//========send ma=========/
		else if($request['send_mode'] == 1 and $request['deal_flag'] == 0 and strlen($request['report'])>2 )
		{
			send_ma($request);
		}
				
	}
}
*/


while(1)
{
	update_report();
	send_record();
	sleep(1);

}
