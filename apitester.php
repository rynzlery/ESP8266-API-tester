<?php

$url = $_GET["url"];
$url = urldecode($url);

print_r(get_http_response_code($url));

function get_http_response_code($theurl) {
    $headers = get_headers($theurl);
    return substr($headers[0], 9, 3);
}