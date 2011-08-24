#!/usr/bin/env php
<?php
require('SOAP/Client.php');
$endpoint = 'http://services.soaplite.com/temper.cgi';
$requester = new SOAP_Client($endpoint);
$method = 'c2f';
$param = '100';
$namespace = "http://www.soaplite.com/Temperatures";
$response = $requester->call($method, $param, $namespace);
printf($response);
echo "\n";
?>
