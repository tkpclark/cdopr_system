<?php
echo phpinfo();exit;
require_once "./mysql.php";

try{
	$memcache = new Memcache;
	$memcache->connect('localhost','11211') or die ("Could not connect");
	$output   = 400;
	$type     =	$_REQUEST['type'];
	$province = $_REQUEST['province'];
	$city     = $_REQUEST['city'];
	$sql = "SELECT num FROM city_limit WHERE ProvinceName='$province'";
	$result = exsql($sql);
	$row=mysqli_fetch_row($result);
	$province_limit = $row[0];
	$sql = "SELECT num FROM city_limit WHERE ProvinceName='$city'";
	$res = exsql($sql);
	$row=mysqli_fetch_row($res);
	$city_limit = $row[0];


	
	
}catch(Exception $e) {
	$output =  $e->getMessage();
}
//根据接口规范返回输出结果
echo $output;
?>
