<?php

if($argv[1]=="on"){
	$ips=shell_exec("route.exe print");
	$st1="255.255.255.255  255.255.255.255";
	$st2="1\n";
	$s1=strpos($ips,$st1);
	$ips=substr($ips,$s1,strlen($ips));

	$s1=strpos($ips,$st2);
	$ips=substr($ips,0,$s1);
	$ips=str_replace("  "," ",$ips);
	$ips=str_replace("  "," ",$ips);
	$ips=str_replace("  "," ",$ips);

	$ip = explode(" ", $ips);
	$cmd="route.exe add 0.0.0.0 mask 0.0.0.0 ".$ip[2];
	if($ip[2]!="255.255.255.255")echo shell_exec($cmd);
} else 
if($argv[1]=="off") shell_exec("route.exe -f");
else
if($argv[1]=="?"){
	$ips=shell_exec("route.exe print");
	$st1="255.255.255.255  255.255.255.255";
	$st2="1\n";
	$s1=strpos($ips,$st1);
	$ips=substr($ips,$s1,strlen($ips));

	$s1=strpos($ips,$st2);
	$ips=substr($ips,0,$s1);
	$ips=str_replace("  "," ",$ips);
	$ips=str_replace("  "," ",$ips);
	$ips=str_replace("  "," ",$ips);

	$ip = explode(" ", $ips);
	if($ip[2]!="255.255.255.255")echo "connected";
	else echo "disconnected";
}
?>