<?php

$url = $_GET["url"];


exec( "yt-dlp --get-url -f worst '$url'" , $output , $return_value  ) ;

echo $output[0];
?>
