<?php session_start();

echo "<h3>_GET</h3><pre>";
var_dump($_GET);
echo "</pre><hr><h3>_POST</h3><pre>";
var_dump($_POST);
echo "</pre><hr><h3>_REQUEST</h3><pre>";
var_dump($_REQUEST);
echo "</pre><hr><h3>_SERVER</h3><pre>";
var_dump($_SERVER);
echo "</pre><hr><h3>_FILES</h3><pre>";
var_dump($_FILES);
echo "</pre><hr><h3>_ENV</h3><pre>";
var_dump($_ENV);
echo "</pre><hr><h3>_COOKIE</h3><pre>";
var_dump($_COOKIE);
echo "</pre><hr><h3>_SESSION</h3><pre>";
var_dump($_SESSION);
echo "</pre><hr><h3>GLOBALS</h3><pre>";
var_dump($GLOBALS);
echo "</pre>";


?>
