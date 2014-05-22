<?php
	$mysqli = new mysqli('202.85.209.109', 'wraith', 'tengyewudi2012@)!@', 'wraith');
	if($mysqli->connect_error)
	{
		die('Connect Error (' . $mysqli->connect_errno . ') '
				. $mysqli->connect_error);
	}	
?>