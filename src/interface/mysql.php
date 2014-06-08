<?php
     $ip="42.62.78.249";
     $user="tkp";
     $pwd="qepapap";
     $db="cdopr";
      
      
      $mysqli = new mysqli($ip, $user, $pwd, $db);
			if($mysqli->connect_error)
			{
				die('Connect Error (' . $mysqli->connect_errno . ') '
						. $mysqli->connect_error);
			}
      function exsql($sql)
			{
				global $mysqli;
				mysqli_query($mysqli,"set names utf8");
				$result = mysqli_query($mysqli,$sql);
				if(!$result)
				 {
				 		echo "mysql error!\n";
				    echo $sql;
				    die('Connect Error (' . $mysqli->connect_errno . ') '
						. $mysqli->connect_error);
				 }
				 return $result;
			}
			
?>
