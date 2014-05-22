<?php


require_once('/home/log4php/php/Logger.php');
require_once('mysql.php');
require_once('des.php');

Logger::configure('log4php_config.xml');
$logging = Logger::getLogger('rent_recv');


if(!isset($_REQUEST['data']))
{
	$logging->info("arguments error:");
	echo "arguments error!";
	exit;
} 

// mt 加密前参数以@#分隔 依次为手机号@#指令@#长号码@#linkid@下行内容
$data=$_REQUEST['data'];
//$logging->info("data:".$data);

$key='wraith55';
$Des_Crypt = new Des_Crypt($key);
$data = $Des_Crypt->decrypt($data,$key,true);
$data = urldecode($data);
$logging->info("data:".$data);
$data_array = explode("@#",$data);


$phone_number = $data_array[0];
$cmd = $data_array[1];
$sp_number = $data_array[2]; 
$linkid = $data_array[3];
$mt_message = $data_array[4];

//update mt message
$sql = sprintf("update wraith_rent_record set mt_message='%s' where linkid='%s' limit 1",$mt_message,$linkid);
$logging->info($sql);
mysqli_query($mysqli, "set names utf8");
$res = mysqli_query($mysqli, $sql);
if (!$res) {
	$logging->info("Failed to run query: (" . $mysqli->errno . ") " . $mysqli->error);
	exit;
}


///////get gwid,product_id,area and so on
$sql = sprintf("select gwid,amount,product_id,product_code,province,area from wraith_rent_record where linkid='%s' limit 1",$linkid);
$logging->info($sql);
mysqli_query($mysqli, "set names utf8");
$res = mysqli_query($mysqli, $sql);
if (!$res) {
	$logging->info("Failed to run query: (" . $mysqli->errno . ") " . $mysqli->error);
	exit;
}
$record = $res->fetch_array(MYSQLI_ASSOC);

$logging->info($record);


///////insert into mt table
$sql = sprintf("insert into wraith_mt(gwid,sp_number,phone_number,linkid,amount,product_id,product_code,message,in_time,province,area)
				values ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s',NOW() ,'%s','%s')",
		$record['gwid'],
		$sp_number,
		$phone_number,
		$linkid,
		$record['amount'],
		$record['product_id'],
		$record['product_code'],
		//$record->{'message'}
		$mt_message,
		$record['province'],
		$record['area']
);
$logging->info($sql);
mysqli_query($mysqli, "set names utf8");
$res = mysqli_query($mysqli, $sql);
if (!$res) {
	echo "Failed to run query: (" . $mysqli->errno . ") " . $mysqli->error;
}
